#include "gpu/kernels/kernel_cublas.h"
#include <cublas_v2.h>

void cublas_xgemm(const float *d_A, const float *d_Zin, float *d_Zout, int m, int n) {

    cublasHandle_t handle;
    cublasCreate(&handle);

    const float alpha = 1.0f;
    const float beta  = 0.0f;

    // cuBLAS asume column-major, por eso se invierten A y Zin
    cublasSgemm(handle,
        CUBLAS_OP_N, CUBLAS_OP_N,
        n, m, m,        // cols C, rows C, cols A / rows B
        &alpha,
        d_Zin, n,       // B (Zin) en column-major
        d_A,   m,       // A en column-major
        &beta,
        d_Zout, n);     // C (Zout)

    cublasDestroy(handle);
}