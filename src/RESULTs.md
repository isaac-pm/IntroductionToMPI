# Final Exercise Results

## Summary

All implementations for Exercises 1.1, 1.2, 1.3, 2.1, and 2.2 have successfully passed their verification tests.

## Exercise 1.1

Status: PASSED

- DAXPY verified for all sizes (n=128 to 1024).
- DDOT verified for all sizes (n=128 to 1024).

## Exercise 1.2: Matrix Computation (Sequential) - Running Times

The following durations were recorded for column-wise vs row-wise access:

```
Duration of case m=32, n=32: columnwise=0.000001, rowwise=0.000001
Duration of case m=256, n=32: columnwise=0.000012, rowwise=0.000004
Duration of case m=2048, n=32: columnwise=0.125267, rowwise=0.000088
Duration of case m=16384, n=32: columnwise=0.125911, rowwise=0.000544
Duration of case m=32, n=256: columnwise=0.000009, rowwise=0.000009
Duration of case m=256, n=256: columnwise=0.000184, rowwise=0.000091
Duration of case m=2048, n=256: columnwise=0.378388, rowwise=0.000567
Duration of case m=16384, n=256: columnwise=0.887817, rowwise=0.630451
Duration of case m=32, n=2048: columnwise=0.000127, rowwise=0.000052
Duration of case m=256, n=2048: columnwise=0.126351, rowwise=0.119456
Duration of case m=2048, n=2048: columnwise=0.921047, rowwise=0.631806
Duration of case m=16384, n=2048: columnwise=1.781868, rowwise=0.881660
Duration of case m=32, n=16384: columnwise=0.123780, rowwise=0.000569
Duration of case m=256, n=16384: columnwise=0.861554, rowwise=0.612727
Duration of case m=2048, n=16384: columnwise=1.699856, rowwise=0.813696
Duration of case m=16384, n=16384: columnwise=28.687107, rowwise=0.98591
```

## Exercise 1.3: Matrix Multiplication (Sequential) - Running Times

The following durations were recorded:

```
Duration of case m=32, n=32, k=32: columnwise=0.000021, rowwise=0.000024
Duration of case m=128, n=32, k=32: columnwise=0.000092, rowwise=0.000088
Duration of case m=512, n=32, k=32: columnwise=0.115557, rowwise=0.000380
Duration of case m=1024, n=32, k=32: columnwise=0.342707, rowwise=0.103562
Duration of case m=32, n=32, k=128: columnwise=0.000109, rowwise=0.000089
Duration of case m=128, n=32, k=128: columnwise=0.000672, rowwise=0.000378
Duration of case m=512, n=32, k=128: columnwise=0.823950, rowwise=0.125422
Duration of case m=1024, n=32, k=128: columnwise=0.831202, rowwise=0.377310
Duration of case m=32, n=32, k=512: columnwise=0.000493, rowwise=0.000354
Duration of case m=128, n=32, k=512: columnwise=0.379002, rowwise=0.125328
Duration of case m=512, n=32, k=512: columnwise=0.837615, rowwise=0.625804
Duration of case m=1024, n=32, k=512: columnwise=0.949008, rowwise=0.789633
Duration of case m=32, n=32, k=1024: columnwise=0.125582, rowwise=0.036471
Duration of case m=128, n=32, k=1024: columnwise=0.789803, rowwise=0.376391
Duration of case m=512, n=32, k=1024: columnwise=0.899483, rowwise=0.875951
Duration of case m=1024, n=32, k=1024: columnwise=1.036107, rowwise=0.858586
Duration of case m=32, n=128, k=32: columnwise=0.000084, rowwise=0.000094
Duration of case m=128, n=128, k=32: columnwise=0.000395, rowwise=0.000409
Duration of case m=512, n=128, k=32: columnwise=0.686004, rowwise=0.119430
Duration of case m=1024, n=128, k=32: columnwise=0.781498, rowwise=0.348717
Duration of case m=32, n=128, k=128: columnwise=0.000465, rowwise=0.000355
Duration of case m=128, n=128, k=128: columnwise=0.252520, rowwise=0.248204
Duration of case m=512, n=128, k=128: columnwise=0.882070, rowwise=0.624332
Duration of case m=1024, n=128, k=128: columnwise=0.894947, rowwise=0.791665
Duration of case m=32, n=128, k=512: columnwise=0.250622, rowwise=0.186869
Duration of case m=128, n=128, k=512: columnwise=0.811228, rowwise=0.635317
Duration of case m=512, n=128, k=512: columnwise=1.032357, rowwise=0.859411
Duration of case m=1024, n=128, k=512: columnwise=1.302676, rowwise=0.868021
Duration of case m=32, n=128, k=1024: columnwise=0.502429, rowwise=0.250249
Duration of case m=128, n=128, k=1024: columnwise=0.861135, rowwise=0.848213
Duration of case m=512, n=128, k=1024: columnwise=1.309576, rowwise=0.843449
Duration of case m=1024, n=128, k=1024: columnwise=1.719366, rowwise=0.925866
Duration of case m=32, n=512, k=32: columnwise=0.112040, rowwise=0.105693
Duration of case m=128, n=512, k=32: columnwise=0.121905, rowwise=0.218503
Duration of case m=512, n=512, k=32: columnwise=0.834212, rowwise=0.732735
Duration of case m=1024, n=512, k=32: columnwise=0.812161, rowwise=0.836748
Duration of case m=32, n=512, k=128: columnwise=0.242170, rowwise=0.161906
Duration of case m=128, n=512, k=128: columnwise=0.813639, rowwise=0.628481
Duration of case m=512, n=512, k=128: columnwise=1.043165, rowwise=0.808696
Duration of case m=1024, n=512, k=128: columnwise=1.164001, rowwise=0.884535
Duration of case m=32, n=512, k=512: columnwise=0.862292, rowwise=0.729607
Duration of case m=128, n=512, k=512: columnwise=0.813667, rowwise=0.794897
Duration of case m=512, n=512, k=512: columnwise=1.697186, rowwise=0.853939
Duration of case m=1024, n=512, k=512: columnwise=2.679872, rowwise=0.991580
Duration of case m=32, n=512, k=1024: columnwise=0.860522, rowwise=0.853317
Duration of case m=128, n=512, k=1024: columnwise=0.960843, rowwise=0.905779
Duration of case m=512, n=512, k=1024: columnwise=2.709563, rowwise=0.974420
Duration of case m=1024, n=512, k=1024: columnwise=4.631709, rowwise=1.167776
Duration of case m=32, n=1024, k=32: columnwise=0.016944, rowwise=0.064968
Duration of case m=128, n=1024, k=32: columnwise=0.362585, rowwise=0.475071
Duration of case m=512, n=1024, k=32: columnwise=0.863763, rowwise=0.788316
Duration of case m=1024, n=1024, k=32: columnwise=0.875500, rowwise=0.832504
Duration of case m=32, n=1024, k=128: columnwise=0.379430, rowwise=0.377071
Duration of case m=128, n=1024, k=128: columnwise=0.800287, rowwise=0.813741
Duration of case m=512, n=1024, k=128: columnwise=1.146082, rowwise=0.871665
Duration of case m=1024, n=1024, k=128: columnwise=1.562188, rowwise=0.925870
Duration of case m=32, n=1024, k=512: columnwise=0.842014, rowwise=0.828993
Duration of case m=128, n=1024, k=512: columnwise=0.925620, rowwise=0.879531
Duration of case m=512, n=1024, k=512: columnwise=2.662156, rowwise=0.974547
Duration of case m=1024, n=1024, k=512: columnwise=4.543788, rowwise=1.137855
Duration of case m=32, n=1024, k=1024: columnwise=0.832969, rowwise=0.830408
Duration of case m=128, n=1024, k=1024: columnwise=0.980682, rowwise=0.898830
Duration of case m=512, n=1024, k=1024: columnwise=4.619620, rowwise=1.153900
Duration of case m=1024, n=1024, k=1024: columnwise=8.285864, rowwise=1.500710
```

## Exercise 2.1

Status: PASSED

- Matrix multiplication verified for all dimensions (m, n, k up to 1024).

## Exercise 2.2

Status: PASSED

- Matrix multiplication verified for all dimensions (m, n, k up to 1024).
