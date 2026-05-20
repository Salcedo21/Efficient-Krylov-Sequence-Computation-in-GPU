#include "gpu/kernels/kernel_tiled.h"

__global__ void tiled_mat_mul_kernel(
    const float *d_A, const float *d_Zin, float *d_Zout, int m, int n)
{
    assert(TILE_WIDTH == blockDim.x);
    assert(TILE_WIDTH == blockDim.y);

    const int by = blockIdx.y,  bx = blockIdx.x;
    const int ty = threadIdx.y, tx = threadIdx.x;

    const int row = TILE_WIDTH * by + ty;
    const int col = TILE_WIDTH * bx + tx;

    __shared__ float sh_A[TILE_WIDTH][TILE_WIDTH];
    __shared__ float sh_B[TILE_WIDTH][TILE_WIDTH];

    const int phases = (int)ceil((float)m / TILE_WIDTH);

    float value = 0.0f;
    for (int phase = 0; phase < phases; phase++) {

        sh_A[ty][tx] = (row < m && (phase * TILE_WIDTH + tx) < m)
                       ? d_A[row * m + phase * TILE_WIDTH + tx]
                       : 0.0f;

        sh_B[ty][tx] = ((phase * TILE_WIDTH + ty) < m && col < n)
                       ? d_Zin[(phase * TILE_WIDTH + ty) * n + col]
                       : 0.0f;

        __syncthreads();

        for (int k = 0; k < TILE_WIDTH; k++)
            value += sh_A[ty][k] * sh_B[k][tx];

        __syncthreads();
    }

    if (row < m && col < n)
        d_Zout[row * n + col] = 1 * value + 0 * d_Zout[row * n + col];
}

void tiled_xgemm(const float *d_A, const float *d_Zin, float *d_Zout, int m, int n) {
    dim3 dim_block(TILE_WIDTH, TILE_WIDTH, 1);
    dim3 dim_grid(ceil(n / (float)TILE_WIDTH), ceil(m / (float)TILE_WIDTH), 1);
    tiled_mat_mul_kernel<<<dim_grid, dim_block>>>(d_A, d_Zin, d_Zout, m, n);
}