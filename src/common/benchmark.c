#include <stdio.h>
#include <stdlib.h>
#include "common/benchmark.h"
#include "common/matrices.h"
#include "common/metricas.h"
#include "common/parametros.h"
#if defined(USE_CUDA)
    #include "gpu/matmul_gpu.h"
#else
    #include "cpu/matmul_cpu.h"
#endif

#if defined(USE_CUDA)
    #if defined(GPU_KERNEL_TILED)
        #define KERNEL_SELECCIONADO GPU_KERNEL_TILED
    #elif defined(GPU_KERNEL_COALESCED)
        #define KERNEL_SELECCIONADO GPU_KERNEL_COALESCED
    #else
        #define KERNEL_SELECCIONADO GPU_KERNEL_NAIVE
    #endif
#endif

#if !defined(USE_CUDA) // Usar CPU

void ejecutar_bucle_cpu(float **A, float **Z, Parametros p, const char *outdir, Metricas *met) {

    // Iniciar el doble buffer Z_cur es entrada, Z_nxt es salida, se intercambian cada iteración.
    float **Z_alt = crear_matriz(p.m, p.n);
    float **Z_cur = Z;
    float **Z_nxt = Z_alt;

    printf("\n=== Ejecucion CPU (%d iter.) ===\n", p.l);

    for (int iter = 0; iter < p.l; iter++) {

        double t0 = tiempo_actual_ms();
        matmul_cpu(A, p.m, p.n, Z_cur, Z_nxt);
        double dt = tiempo_actual_ms() - t0;

        metricas_registrar(met, dt);
        guardar_snapshot(Z_nxt, p.n, iter, outdir);

        float **tmp = Z_cur;
        Z_cur = Z_nxt;
        Z_nxt = tmp;

        printf("  iter %4d  %7.2f ms\n", iter, dt);
    }

    liberar_matriz(Z_alt, p.m);
}

#else

void ejecutar_bucle_gpu(float **A, float **Z, Parametros p, const char *outdir, Metricas *met) {

    float **Z_alt = crear_matriz(p.m, p.n);
    float **Z_cur = Z;
    float **Z_nxt = Z_alt;

    GpuBuffers buf = gpu_alloc(p.m, p.n);
    gpu_carga_matriz(A, buf.d_A, p.m, p.m);

    printf("\n=== Ejecucion GPU (%d iter.) ===\n", p.l);

    for (int iter = 0; iter < p.l; iter++) {

        gpu_carga_matriz(Z_cur, buf.d_Zin, p.m, p.n);

        double t0 = tiempo_actual_ms();
        matmul_gpu(buf.d_A, p.m, p.n, buf.d_Zin, buf.d_Zout, KERNEL_SELECCIONADO);
        cudaDeviceSynchronize();
        double dt = tiempo_actual_ms() - t0;

        gpu_descarga_matriz(buf.d_Zout, Z_nxt, p.m, p.n);

        metricas_registrar(met, dt);
        guardar_snapshot(Z_nxt, p.n, iter, outdir);

        float **tmp = Z_cur;
        Z_cur = Z_nxt;
        Z_nxt = tmp;

        printf("  iter %4d  %7.2f ms\n", iter, dt);
    }

    gpu_free(&buf);
    liberar_matriz(Z_alt, p.m);
}

#endif

void benchmark(Parametros p, const char *matrices_dir, const char *outdir) {
    
    //Cargar las matrices A y Z desde los archivos de /data.
    float **A, **Z;
    if (cargar_matrices(matrices_dir, p.m, p.n, &A, &Z) != 0) {
        fprintf(stderr, "Error cargando matrices desde '%s'\n", matrices_dir);
        return;
    }

    Metricas met;
    metricas_init(&met, p.l, p.m, p.n);

    double t0 = tiempo_actual_ms();

    #if defined(USE_CUDA)
        ejecutar_bucle_gpu(A, Z, p, outdir, &met);
        liberar_matriz(A, p.m);
        liberar_matriz(Z, p.m);
    #else
        ejecutar_bucle_cpu(A, Z, p, outdir, &met);
        liberar_matriz(A, p.m);
        liberar_matriz(Z, p.m);
    #endif

    double total = tiempo_actual_ms() - t0;

    metricas_imprimir(&met);
    printf("  Tiempo total benchmark: %8.3f ms\n", total);
    metricas_guardar(&met, outdir, total);
    metricas_free(&met);
}