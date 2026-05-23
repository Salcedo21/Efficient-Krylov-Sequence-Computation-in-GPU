#include <stdio.h>
#include <stdlib.h>
#include "gpu/gpu_memory.h"

/// Reserva de buffers.
///
/// Reserva tres buffers en GPU: d_A (m×m), d_Zin (m×n) y d_Zout (m×n).
///
/// Argumentos:
/// - m, n: Dimensiones de A y Z.
/// 
GpuBuffers gpu_alloc(int m, int n) {
    GpuBuffers buf;
    buf.bytes_A = (size_t)m * m * sizeof(float);
    buf.bytes_Z = (size_t)m * n * sizeof(float);
    cudaMalloc(&buf.d_A, buf.bytes_A);
    cudaMalloc(&buf.d_Zin, buf.bytes_Z);
    cudaMalloc(&buf.d_Zout, buf.bytes_Z);
    return buf;
}

/// Liberación de buffers.
///
/// Libera los tres buffers reservados en GPU: d_A, d_Zin y d_Zout.
///
/// Argumentos:
/// - buf: puntero a GpuBuffers con los tres buffers a liberar.
///
void gpu_free(GpuBuffers *buf) {
    cudaFree(buf->d_A);
    cudaFree(buf->d_Zin);
    cudaFree(buf->d_Zout);
}

/// Carga de RAM (host) → VRAM (device)
///
/// Aplana la matriz y lo copia a memoria GPU.
///
/// Argumentos:
/// - src_device: origen en CPU.  
/// - dst: destino en GPU.  
/// - filas, cols: dimensiones de la matriz.
///
void gpu_carga_matriz(float **src, float *dst_device, int filas, int cols) {
    size_t bytes = (size_t)filas * cols * sizeof(float);
    float *tmp = (float *)malloc(bytes);

    for (int i = 0; i < filas; i++)
        for (int j = 0; j < cols; j++)
            tmp[i * cols + j] = src[i][j];

    cudaMemcpy(dst_device, tmp, bytes, cudaMemcpyHostToDevice);
    free(tmp);
}

/// Descarga de VRAM (device) → RAM (host)
///
/// Copia la matriz lineal de GPU y lo "desaplana".
///
/// Argumentos:
/// - src_device: origen en GPU.  
/// - dst: destino en CPU.  
/// - filas, cols: dimensiones de la matriz.
///
void gpu_descarga_matriz(float *src_device, float **dst, int filas, int cols) {
    size_t bytes = (size_t)filas * cols * sizeof(float);
    float *tmp = (float *)malloc(bytes);

    cudaMemcpy(tmp, src_device, bytes, cudaMemcpyDeviceToHost);

    for (int i = 0; i < filas; i++)
        for (int j = 0; j < cols; j++)
            dst[i][j] = tmp[i * cols + j];

    free(tmp);
}
