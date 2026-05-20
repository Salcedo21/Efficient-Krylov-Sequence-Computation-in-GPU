#include <cuda_runtime.h>
#include "gpu/matmul_gpu.h"
#include "gpu/gpu_memory.h"
#include "gpu/kernels/kernel_naive.h"
#include "gpu/kernels/kernel_coalesced.h"
#include "gpu/kernels/kernel_tiled.h"

void matmul_gpu(float *d_A, int m, int n,float *d_Zin, float *d_Zout, GpuKernel kernel) {
    
    switch (kernel) {
        case GPU_KERNEL_NAIVE:
            naive_xgemm(d_A, d_Zin, d_Zout, m, n);
            break;
        case GPU_KERNEL_COALESCED:
            coalesced_xgemm(d_A, d_Zin, d_Zout, m, n);
            break;
        case GPU_KERNEL_TILED:
            tiled_xgemm(d_A, d_Zin, d_Zout, m, n);
            break;
    }
}