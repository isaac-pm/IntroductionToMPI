#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <mpi.h>

#include "array.h"
#include "multiply.h"

static unsigned int const seed = 1234;
static int const dimensions[] = {128*1, 128*2, 128*4, 128*8};
static int const n_dimensions = sizeof(dimensions)/sizeof(int);
static double const epsilon = 1e-10;

typedef void (*GEMM)(
    int const m, int const k, int const n,
    double const* const A, double const* const B, double* const C
);

static void populate_compatible_random_matrix_pairs(
    int const m, int const k, int const n,
    int const seed,
    double* const A, double* const B)
{
    set_initilize_rand_seed(seed);

    initialize_2d_double_blocked_rand(A, m, k);
    initialize_2d_double_blocked_rand(B, k, n);
}

static void initialize_problem_matrices(
    int const m, int const k, int const n,
    double** const A, double** const B, double** const C)
{
    *A = allocate_2d_double_blocked(m, k);
    *B = allocate_2d_double_blocked(k, n);
    *C = allocate_2d_double_blocked(m, n);
}

static void destroy_problem_matrices(double** const A, double** const B, double** const C)
{
    *A = free_2d_double_blocked(*A);
    *C = free_2d_double_blocked(*C);
    *C = free_2d_double_blocked(*C);
}

static bool test_muptiply(int const m, int const k, int const n, GEMM gemm, double const epsilon, unsigned int const seed)
{
    double* A = NULL;
    double* B = NULL;
    double* C = NULL;
    initialize_problem_matrices(m, k, n, &A, &B, &C);
    populate_compatible_random_matrix_pairs(m, k, n, seed, A, B);

    gemm(m, k, n, A, B, C);
    bool result_is_correct = is_product(m, k, n, A, B, C, epsilon);

    destroy_problem_matrices(&A, &B, &C);

    return result_is_correct;
}

// Implement a function "parallel_gemm" of type GEMM, that implements the
// matrix multiplication operation.
//
void parallel_gemm(
  int const m, int const k, int const n,
  double const* const A, double const* const B, double* const C)
{
    MPI_Comm comm = MPI_COMM_WORLD;
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    /* Assume strict divisibility for simplicity */
    if (m % size != 0 || n % size != 0) {
        if (rank == 0) fprintf(stderr, "Error: m and n must be divisible by size.\n");
        MPI_Abort(comm, 1);
    }

    int m_local = m / size; // Rows of A per rank
    int n_local = n / size; // Cols of B and C per rank
    
    /* Create derived datatype for sending block rows of A (strided in col-major) */
    MPI_Datatype block_row_type;
    MPI_Type_vector(k, m_local, m, MPI_DOUBLE, &block_row_type);
    MPI_Type_commit(&block_row_type);
    
    /* Allocate Local Buffers */
    double *A_local = malloc(m_local * k * sizeof(double));
    double *B_local = malloc(k * n_local * sizeof(double));
    double *C_local = malloc(m * n_local * sizeof(double)); 
    double *temp_block = malloc(m_local * n_local * sizeof(double));

    /* Distribution Phase */
    // Scatter B (Columns) - Standard Scatter as columns are contiguous
    MPI_Scatter(B, k * n_local, MPI_DOUBLE, 
                B_local, k * n_local, MPI_DOUBLE, 
                0, comm);

    // Scatter A (Rows) - Rank 0 distributes block rows
    if (rank == 0) {
        MPI_Request *reqs = malloc(sizeof(MPI_Request) * (size_t)size);
        
        // Send blocks to other ranks
        for (int i = 1; i < size; ++i) {
            MPI_Isend(A + i * m_local, 1, block_row_type, i, 0, comm, &reqs[i]);
        }
        
        // Copy own block (Row 0)
        for(int j = 0; j < k; ++j) {
            for(int i = 0; i < m_local; ++i) {
                A_local[i + j * m_local] = A[i + j * m];
            }
        }
        
        for(int i = 1; i < size; ++i) MPI_Wait(&reqs[i], MPI_STATUS_IGNORE);
        free(reqs);
    } else {
        // Recv A_local (contiguous)
        MPI_Recv(A_local, m_local * k, MPI_DOUBLE, 0, 0, comm, MPI_STATUS_IGNORE);
    }

    /* Initialize C_local */
    for(int i = 0; i < m * n_local; ++i) C_local[i] = 0.0;

    /* Computation Loop (Ring Algorithm) */
    int next_rank = (rank + 1) % size;
    int prev_rank = (rank - 1 + size) % size;
    double *A_recv_buf = malloc(m_local * k * sizeof(double));
    int current_A_row_block_idx = rank; // Tracks which global row block we hold

    for (int step = 0; step < size; ++step) {
        /* Compute: temp = A_local * B_local */
        multiply_matrices(m_local, k, n_local, A_local, B_local, temp_block);

        /* Accumulate into C_local */
        int row_start = current_A_row_block_idx * m_local;
        for (int j = 0; j < n_local; ++j) {
            for (int i = 0; i < m_local; ++i) {
                C_local[(row_start + i) + j * m] += temp_block[i + j * m_local];
            }
        }

        /* Shift A to next processor */
        if (step < size - 1) {
            MPI_Request req;
            MPI_Isend(A_local, m_local * k, MPI_DOUBLE, next_rank, 1, comm, &req);
            MPI_Recv(A_recv_buf, m_local * k, MPI_DOUBLE, prev_rank, 1, comm, MPI_STATUS_IGNORE);
            MPI_Wait(&req, MPI_STATUS_IGNORE);
            
            // Swap buffers
            double *tmp = A_local;
            A_local = A_recv_buf;
            A_recv_buf = tmp;
            
            // Update block index
            current_A_row_block_idx = (current_A_row_block_idx - 1 + size) % size;
        }
    }

    /* Gather Result C */
    MPI_Gather(C_local, m * n_local, MPI_DOUBLE,
               C, m * n_local, MPI_DOUBLE,
               0, comm);

    MPI_Bcast(C, m * n, MPI_DOUBLE, 0, comm);

    MPI_Type_free(&block_row_type);
    free(A_local);
    free(B_local);
    free(C_local);
    free(A_recv_buf);
    free(temp_block);
}

// Then set "tested_gemm" to the address of your funtion
// GEMM const tested_gemm = &multiply_matrices;
GEMM const tested_gemm = &parallel_gemm;

static bool generate_square_matrix_dimension(int* const m, int* const k, int* const n)
{
    int const max_dim = n_dimensions;
    static int dim = 0;

    if (dim >= max_dim) {
        return false;
    }

    *m = dimensions[dim];
    *k = dimensions[dim];
    *n = dimensions[dim];
    
    dim++;

    return true;
}

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    bool all_test_pass = true;

    int m = 0;
    int k = 0;
    int n = 0;

    while (generate_square_matrix_dimension(&m, &k, &n)) {
        bool const test_pass = test_muptiply(m, k, n, tested_gemm, epsilon, seed);
        if (!test_pass) {
            printf("Multiplication failed for: m=%d, k=%d, n=%d\n", m, k, n);
            all_test_pass = false;
        } else {
            printf("Multiplication passed for: m=%d, k=%d, n=%d\n", m, k, n);
        }
    }

    MPI_Finalize();

    if (!all_test_pass) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
