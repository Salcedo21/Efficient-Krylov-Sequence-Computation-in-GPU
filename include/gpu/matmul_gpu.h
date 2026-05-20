#pragma once

typedef enum {
    GPU_KERNEL_NAIVE,
    GPU_KERNEL_COALESCED,
    GPU_KERNEL_TILED,
    GPU_KERNEL_CUBLAS
} GpuKernel;

void matmul_gpus(float *d_A, int m, int n,float *d_Zin, float *d_Zout, GpuKernel kernel);