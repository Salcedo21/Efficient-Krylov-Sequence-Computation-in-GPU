#include "gpu/kernels/kernel_cublas.h"
#include <cublas_v2.h>

// Esta es llamada a la función de cuBLAS nativa de CUDApara multiplicar matrices y da miedo lo optimizada que esta.
// cuBLAS implementa C= alpha * A * B + beta * C
// En nuestro caso A es d_A, B es d_Zin, C es d_Z
// alpha es 1 y beta es 0, es decir C= A*B
void cublas_xgemm(const float *d_A, const float *d_Zin, float *d_Zout, int m, int n) {

    // El handle es el ctx interno de cuBLAS 
    cublasHandle_t handle;
    // Inicializamos el handle para usar cuBLAS
    cublasCreate(&handle);

    const float alpha = 1.0f;
    const float beta  = 0.0f;
    



    cublasSgemm(handle,
        CUBLAS_OP_N, // No transponer Zin
        CUBLAS_OP_N, // No transponer A
        n, // Columnas del resultado, pero esto es column-major
        m, // Filas del resultado, como col-mayor esta trocada
        m, // Dimension compartida de A y Zin  
        &alpha,
        d_Zin, n, // Primer operando     
        d_A,   m, // Segundo operando    
        &beta,
        d_Zout, n); // Matriz resultado    

    // Liberar el handle de cuBLAS
    cublasDestroy(handle);
}