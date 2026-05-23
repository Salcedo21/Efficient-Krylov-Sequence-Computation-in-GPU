#pragma once

typedef struct {
    double tiempo_ms;
    double gflops;
} Muestra;

typedef struct {
    Muestra *muestras;
    int      n, cap;
    int      m, n_cols;
} Metricas;

double tiempo_actual_ms(void);

void metricas_init    (Metricas *met, int capacidad, int m, int n_cols);
void metricas_registrar(Metricas *met, double tiempo_ms);
void metricas_free    (Metricas *met);
void metricas_imprimir(const Metricas *met);
void metricas_guardar (const Metricas *met, const char *outdir, double benchmark_total_ms);
