#pragma once
#include <cuda_runtime.h>

typedef enum {
    GPU_KERNEL_NAIVE,
    GPU_KERNEL_COALESCED,
    GPU_KERNEL_TILED,
    GPU_KERNEL_CUBLAS
} GpuKernel;

void matmul_gpu_device(float *d_A, int m, int n,float *d_Zin, float *d_Zout, GpuKernel kernel);