#pragma once
#include <cuda_runtime.h>

__global__ void coalesced_mat_mul_kernel(const float *d_A, const float *d_Zin,
                                         float *d_Zout, int m, int n);

void coalesced_xgemm(const float *d_A, const float *d_Zin,
                     float *d_Zout, int m, int n);
