#pragma once
#include <cuda_runtime.h>
#include <assert.h>

#define TILE_WIDTH 32

__global__ void tiled_mat_mul_kernel(const float *d_A, const float *d_Zin,float *d_Zout, int m, int n);

void tiled_xgemm(const float *d_A, const float *d_Zin,
                 float *d_Zout, int m, int n);
