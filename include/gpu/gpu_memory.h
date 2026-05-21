#pragma once
#include <cuda_runtime.h>
#include <stddef.h>

typedef struct {
    float  *d_A;
    float  *d_Zin;
    float  *d_Zout;
    size_t  bytes_A;
    size_t  bytes_Z;
} GpuBuffers;


#ifdef __cplusplus
extern "C" {
#endif

GpuBuffers gpu_alloc           (int m, int n);
void       gpu_free            (GpuBuffers *buf);
void       gpu_carga_matriz    (float **src,       float *dst_device, int filas, int cols);
void       gpu_descarga_matriz (float *src_device, float **dst,       int filas, int cols);

#ifdef __cplusplus
}
#endif