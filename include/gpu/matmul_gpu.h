#pragma once
#include <cuda_runtime.h>
#include "gpu/gpu_memory.h"

typedef enum {
    GPU_KERNEL_NAIVE,
    GPU_KERNEL_COALESCED,
    GPU_KERNEL_TILED,
    GPU_KERNEL_CUBLAS
} GpuKernel;


#ifdef __cplusplus
extern "C" {
#endif

void matmul_gpu_device(float *d_A, int m, int n,float *d_Zin, float *d_Zout, GpuKernel kernel);

#ifdef __cplusplus
}
#endif