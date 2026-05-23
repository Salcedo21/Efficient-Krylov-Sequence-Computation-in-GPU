#include "gpu/kernels/kernel_coalesced.h"
// Leer primero kernel_naive.cu para mayor contexto.
// Esta optimizacion busca aprovechar como la localidad de memoria
// Usaremos el hecho de que la memoria posee un controlador que agrupa llamados
// de memoria a direcciones contiguas en un solo llamado.  
// Asi es posible acceder a un range de direcciones consecutivas por cada llamado
// Cambiaremos el mapeo de los threads para que accedan a memoria contigua con respecto a la naive.
__global__ void coalesced_mat_mul_kernel(
    const float *d_A, const float *d_Zin, float *d_Zout, int m, int n) {
    
    // Ahora cada hilo accede a la misma fila de A y a columnas cosecutivas de Z
    // Asi threads en el mismo warp acceden a elementos consecutivos de Z
    const int col = blockDim.x * blockIdx.x + threadIdx.x;  
    const int row = blockDim.y * blockIdx.y + threadIdx.y;

    if (row >= m || col >= n) return;

    float value = 0.0f;
    for (int k = 0; k < m; k++)
        value += d_A[row * m + k] * d_Zin[k * n + col];

    d_Zout[row * n + col] = 1 * value + 0 * d_Zout[row * n + col];
    // El resto no se ve afectado con respecto a la naive
        }

void coalesced_xgemm(const float *d_A, const float *d_Zin, float *d_Zout, int m, int n) {
    // Threads blocks 32x32
    dim3 dim_block(32, 32, 1);
    // Grid 2D 32x32 thread blocks
    dim3 dim_grid(ceil(n / (float)32), ceil(m / (float)32), 1);
    coalesced_mat_mul_kernel<<<dim_grid, dim_block>>>(d_A, d_Zin, d_Zout, m, n);
}
