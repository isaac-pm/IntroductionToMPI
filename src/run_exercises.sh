#!/bin/bash
set -e

module load toolchain/foss

echo "Compiling helper objects..."
gcc -c -O2 array.c -o array.o
gcc -c -O2 multiply.c -o multiply.o

# BLAS library configuration
BLAS_FLAGS=""
BLAS_LIBS="-lblas"

if command -v pkg-config &> /dev/null; then
    if pkg-config --exists flexiblas; then
        BLAS_FLAGS=$(pkg-config --cflags flexiblas)
        BLAS_LIBS=$(pkg-config --libs flexiblas)
    elif pkg-config --exists openblas; then
        BLAS_FLAGS=$(pkg-config --cflags openblas)
        BLAS_LIBS=$(pkg-config --libs openblas)
    fi
fi

echo "Using BLAS options: $BLAS_FLAGS $BLAS_LIBS"

echo "Compiling Exercises..."

# Compile Serial/BLAS exercises
for exe in exercise_1_1 exercise_1_2 exercise_1_3; do
    echo "Compiling $exe..."
    gcc -O2 ${exe}.c array.o -o $exe $BLAS_FLAGS $BLAS_LIBS -lm
done

# Compile MPI exercises
for exe in exercise_2_1 exercise_2_2; do
    echo "Compiling $exe..."
    mpicc -O2 ${exe}.c array.o multiply.o -o $exe -lm
done

echo "Submitting jobs to Slurm..."

submit_job() {
    EXE=$1
    TASKS=$2
    OUTPUT="${EXE}.out"
    
    echo "Submitting $EXE with $TASKS tasks..."

    # Create a temporary submission script
    cat <<EOT > submit_${EXE}.sh
#!/bin/bash
#SBATCH --job-name=${EXE}
#SBATCH --output=${OUTPUT}
#SBATCH --ntasks=${TASKS}
#SBATCH --time=00:05:00
#SBATCH --mem=1G

# Load modules
module load toolchain/foss

echo "Running ${EXE}..."
if [ "${TASKS}" -eq "1" ]; then
    ./${EXE}
else
    # Use srun for MPI jobs in Slurm
    srun ./${EXE}
fi
EOT

    sbatch submit_${EXE}.sh
    rm submit_${EXE}.sh
}

# Submit Serial jobs
submit_job exercise_1_1 1
submit_job exercise_1_2 1
submit_job exercise_1_3 1

# Submit MPI jobs (using 4 tasks)
submit_job exercise_2_1 4
submit_job exercise_2_2 4

echo "All jobs submitted. Check *.out files for results."
