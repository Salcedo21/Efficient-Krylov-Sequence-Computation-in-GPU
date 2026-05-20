#include "gpu/kernels/kernel_naive.h"
// Inputs: un puntero de A (tamaño mxm) en device, Z_in (tamaño mxn)
// Se ejecuta la multiplicacion en paralelo de estas dos y se guarda en Z_out en device. 
// Con __global__ nos aseguramos que esta funcion solo puede ser ejecutada por GPU.
// Al estar trabajando en SIMT y SIMD cada operacion es ejecutada en distintos hilos con distinta data .
// Esta multiplicacion corresponde a la implementacion mas sencilla en GPU que se nos ocurrio.
__global__ void naive_mat_mul_kernel(
    const float *d_A, const float *d_Zin, float *d_Zout, int m, int n)
{
    // Mapeo Global de cada hilo de ejecucion correspondiente a una entrada final de la multiplicacion
    // asi dZ_out[row][col]
    const int row = blockDim.x * blockIdx.x + threadIdx.x;
    const int col = blockDim.y * blockIdx.y + threadIdx.y;

    if (row >= m || col >= n) return;

    // valor resultante tras la combinacion lineal fila x columna en cada thread
    float value = 0.0f;
    for (int k = 0; k < m; k++)
        // Recordar que la matriz A esta "Linealizada" es decir se guarda como un array de numeros
        value += d_A[row * m + k] * d_Zin[k * n + col];

    // Guardamos el resultado en cada entrada de d_Zout 
    // Esta en esa notacion para comparar contra cuBLAS
    d_Zout[row * n + col] = 1 * value + 0 * d_Zout[row * n + col];
}
// API para multiplicar 2 matrices en modo naive
// LLama a naive_mat_mul_kernel que hace la multiplicacion
void naive_xgemm(const float *d_A, const float *d_Zin, float *d_Zout, int m, int n) {
    // Ajustamos las dimensiones de la Grid y los threads blocks para una ejecucion optima
    // Los threads blocks son 2D de tamaño 32x32 
    dim3 dim_block(32, 32, 1);
    // La Grid poseera dimensiones de techo(m/32) x techo(n/32) esa es la cantidad de thread blocks
    // techo(m/32) x techo(n/32) x 32 x32 es el numero de threads, es decir por cada entrada de
    // d_Zout hay un thread correspondiente
    dim3 dim_grid(ceil(m / (float)32), ceil(n / (float)32), 1);
    naive_mat_mul_kernel<<<dim_grid, dim_block>>>(d_A, d_Zin, d_Zout, m, n);
}
