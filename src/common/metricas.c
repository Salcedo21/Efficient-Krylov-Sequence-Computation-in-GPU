#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "common/metricas.h"

double tiempo_actual_ms(void) {
    static LARGE_INTEGER freq;
    static int initialized = 0;
    LARGE_INTEGER now;

    if (!initialized) {
        QueryPerformanceFrequency(&freq);
        initialized = 1;
    }

    QueryPerformanceCounter(&now);
    return (double)now.QuadPart * 1000.0 / (double)freq.QuadPart;
}

void metricas_init(Metricas *met, int capacidad, int m, int n_cols) {
    met->muestras = malloc(capacidad * sizeof(Muestra));
    met->n        = 0;
    met->cap      = capacidad;
    met->m        = m;
    met->n_cols   = n_cols;
}

void metricas_registrar(Metricas *met, double tiempo_ms) {
    if (met->n >= met->cap) {
        met->cap   *= 2;
        met->muestras = realloc(met->muestras, met->cap * sizeof(Muestra));
    }

    long long flops = 2LL * met->m * met->m * met->n_cols;
    Muestra *s   = &met->muestras[met->n++];
    s->tiempo_ms = tiempo_ms;
    s->gflops    = (tiempo_ms > 0.0)
                   ? (double)flops / (tiempo_ms / 1000.0) / 1e9
                   : 0.0;
}

void metricas_free(Metricas *met) {
    free(met->muestras);
    met->muestras = NULL;
    met->n = met->cap = 0;
}

void metricas_imprimir(const Metricas *met) {
    if (met->n == 0) { printf("  (sin muestras)\n"); return; }

    double suma_t = 0, suma_g = 0;
    double min_t = met->muestras[0].tiempo_ms;
    double max_t = min_t;

    for (int i = 0; i < met->n; i++) {
        double t = met->muestras[i].tiempo_ms;
        suma_t  += t;
        suma_g  += met->muestras[i].gflops;
        if (t < min_t) {min_t = t;}
        if (t > max_t) {max_t = t;}
    }

    printf("\n=== Metricas (%d iter.) ===\n", met->n);
    printf("  Tiempo promedio : %8.3f ms\n", suma_t / met->n);
    printf("  Tiempo min      : %8.3f ms\n", min_t);
    printf("  Tiempo max      : %8.3f ms\n", max_t);
    printf("  GFLOPs totales  : %8.3f\n",    suma_g);  
    printf("  GFLOPs promedio : %8.3f\n",    suma_g / met->n);        
}

void metricas_guardar(const Metricas *met, const char *outdir, double benchmark_total_ms) {

    char ruta[256];
    snprintf(ruta, sizeof(ruta), "%s/benchmark_info.txt", outdir);
    FILE *fi = fopen(ruta, "w");
    if (!fi) {
        fprintf(stderr, "Error: no se pudo crear %s\n", ruta);
    } else {
        fprintf(fi, "=== Benchmark Info ===\n");
        fprintf(fi, "Dimensiones     : A(%d x %d) x Z(%d x %d)\n",
                met->m, met->m, met->m, met->n_cols);
        fprintf(fi, "Iteraciones     : %d\n",    met->n);
        fprintf(fi, "Tiempo total    : %.3f ms\n", benchmark_total_ms);

        if (met->n > 0) {
            double suma_t = 0, suma_g = 0;
            double min_t = met->muestras[0].tiempo_ms, max_t = min_t;
            for (int i = 0; i < met->n; i++) {
                double t = met->muestras[i].tiempo_ms;
                suma_t  += t; suma_g += met->muestras[i].gflops;
                if (t < min_t) min_t = t;
                if (t > max_t) max_t = t;
            }
            fprintf(fi, "Tiempo promedio : %.3f ms\n", suma_t / met->n);
            fprintf(fi, "Tiempo min      : %.3f ms\n", min_t);
            fprintf(fi, "Tiempo max      : %.3f ms\n", max_t);
            fprintf(fi, "GFLOPs totales  : %.3f\n",    suma_g);
            fprintf(fi, "GFLOPs promedio : %.3f\n",    suma_g / met->n);      
        }
        fclose(fi);
        printf("  Info: %s\n", ruta);
    }

    snprintf(ruta, sizeof(ruta), "%s/benchmark.csv", outdir);
    FILE *fc = fopen(ruta, "w");
    if (!fc) { fprintf(stderr, "Error: no se pudo crear %s\n", ruta); return; }

    fprintf(fc, "iter,tiempo_ms,gflops\n");
    for (int i = 0; i < met->n; i++)
        fprintf(fc, "%d,%.6f,%.6f\n", i,
                met->muestras[i].tiempo_ms, met->muestras[i].gflops);

    fclose(fc);
    printf("  CSV:  %s\n", ruta);
}