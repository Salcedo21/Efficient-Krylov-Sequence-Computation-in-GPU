#include "gpu/kernels/kernel_naive.h"

__global__ void naive_mat_mul_kernel(
    const float *d_A, const float *d_Zin, float *d_Zout, int m, int n)
{
    const int row = blockDim.x * blockIdx.x + threadIdx.x;
    const int col = blockDim.y * blockIdx.y + threadIdx.y;

    if (row >= m || col >= n) return;

    float value = 0.0f;
    for (int k = 0; k < m; k++)
        value += d_A[row * m + k] * d_Zin[k * n + col];

    d_Zout[row * n + col] = 1 * value + 0 * d_Zout[row * n + col];
}

void naive_xgemm(const float *d_A, const float *d_Zin, float *d_Zout, int m, int n) {
    dim3 dim_block(32, 32, 1);
    dim3 dim_grid(ceil(m / (float)32), ceil(n / (float)32), 1);
    naive_mat_mul_kernel<<<dim_grid, dim_block>>>(d_A, d_Zin, d_Zout, m, n);
}
