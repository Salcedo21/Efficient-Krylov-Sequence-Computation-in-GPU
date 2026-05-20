#pragma once
#include <cuda_runtime.h>
#include <stddef.h>

/// Buffers de GPU para un ciclo matmul: d_A, d_Zin, d_Zout.
typedef struct {
    float  *d_A;
    float  *d_Zin;
    float  *d_Zout;
    size_t  bytes_A;
    size_t  bytes_Z;
} GpuBuffers;

GpuBuffers gpu_alloc        (int m, int n);
void       gpu_free         (GpuBuffers *buf);
void       gpu_subir_matriz (float **src,    float *dst_device, int filas, int cols);
void       gpu_bajar_matriz (float *src_device, float **dst,   int filas, int cols);
