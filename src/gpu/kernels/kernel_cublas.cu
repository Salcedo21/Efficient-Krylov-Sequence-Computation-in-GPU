#include "gpu/kernels/kernel_cublas.h"
#include <cublas_v2.h>

void cublas_xgemm(const float *d_A, const float *d_Zin, float *d_Zout, int m, int n) {

    cublasHandle_t handle;
    cublasCreate(&handle);

    const float alpha = 1.0f;
    const float beta  = 0.0f;

    cublasSgemm(handle,
        CUBLAS_OP_N, CUBLAS_OP_N,
        n, m, m,    
        &alpha,
        d_Zin, n,     
        d_A,   m,     
        &beta,
        d_Zout, n);    

    cublasDestroy(handle);
}