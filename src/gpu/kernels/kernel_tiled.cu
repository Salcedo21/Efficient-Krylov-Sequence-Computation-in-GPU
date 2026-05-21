#include "gpu/kernels/kernel_tiled.h"

//Leer kernel_coalesced.cu para mayor contexto.

//Esta optimizacion modifica coalesced para reducir los accesos a memoria global
//Para ello se partira cada matriz en Fragmentos que entren en la memoria compartida de los SMs
//Se cargara en Shared memory para que cada threads en el warp puedan acceder al mismo fragmento de A y Z
//Shared memory es signficativamente mas rapido que global memory.
//Esto reutiliza los datos llamados desde global memory.
// Cada bloque procesa un tile de la matriz resultante
__global__ void tiled_mat_mul_kernel(
    const float *d_A, const float *d_Zin, float *d_Zout, int m, int n)
{   // TILE_WIDTH=32
    // Verificamos que el bloque sea cuadrado y coincida con el tamaño del tile
    assert(TILE_WIDTH == blockDim.x);
    assert(TILE_WIDTH == blockDim.y);

    
    const int by = blockIdx.y,  bx = blockIdx.x; //  Indices del bloque dentro de la grilla
    const int ty = threadIdx.y, tx = threadIdx.x; // Indices locales del thread dentro del thread block

    // Coordenadas globales del elemento que el thread calcula dentro de la matriz resultante
    const int row = TILE_WIDTH * by + ty;
    const int col = TILE_WIDTH * bx + tx;

    //Tiles almacenados en shared memory
    // sh_A almacena un bloque de A
    // sh_B almacena un bloque de Zin
    __shared__ float sh_A[TILE_WIDTH][TILE_WIDTH];
    __shared__ float sh_B[TILE_WIDTH][TILE_WIDTH];

    // Numero de fases necesarias para recorrer la dimension compartida de la multiplicacion
    // Cada fase carga un tile distinto de A y Zin
    const int phases = (int)ceil((float)m / TILE_WIDTH);

    float value = 0.0f;
    for (int phase = 0; phase < phases; phase++) {
        //Cada thread carga un elemento de A y un elemento de Zin en shared memory
        //El acceso esta organizado para acceder a posiciones contiguas, permitiendo coalescing

        sh_A[ty][tx] = (row < m && (phase * TILE_WIDTH + tx) < m)
                       ? d_A[row * m + phase * TILE_WIDTH + tx]
                       : 0.0f; // Si el indice esta fuera de los limites de A, se carga 0 para no afectar la suma
        
        // Para Zin, se accede a la columna correspondiente al bloque actual (col) 
        // y a la fila correspondiente a la fase actual (phase * TILE_WIDTH + ty)
        sh_B[ty][tx] = ((phase * TILE_WIDTH + ty) < m && col < n)
                       ? d_Zin[(phase * TILE_WIDTH + ty) * n + col]
                       : 0.0f; // Si el indice esta fuera de los limites de Zin, se carga 0 para no afectar la suma
        
        // Esperamos a que todos los threads del bloque terminen de cargar sus datos antes de utilizarlos  de nuevo
        // Esto garantiza correctitud.
        __syncthreads();

        // Cada thread calcula su parte del producto punto  utilizando los tiles cargados en shared memory.
        for (int k = 0; k < TILE_WIDTH; k++)
            value += sh_A[ty][k] * sh_B[k][tx];
        
        // Esperamos antes de sobrescrbir en shared memory.
        __syncthreads();
    }
    // Escritura del resultado final en global memory.
    if (row < m && col < n)
        d_Zout[row * n + col] = 1 * value + 0 * d_Zout[row * n + col];
}
// Función de host para lanzar el kernel de multiplicación de matrices con tiling
void tiled_xgemm(const float *d_A, const float *d_Zin, float *d_Zout, int m, int n) {
    // Cada thread block es 32x32 pues Tile_WIDTH=32 
    dim3 dim_block(TILE_WIDTH, TILE_WIDTH, 1);
    // La Grid es 2D y Se calcula igual que las anteriores pues Tile_WIDTH=32 jaja.
    dim3 dim_grid(ceil(n / (float)TILE_WIDTH), ceil(m / (float)TILE_WIDTH), 1);
    tiled_mat_mul_kernel<<<dim_grid, dim_block>>>(d_A, d_Zin, d_Zout, m, n);
}