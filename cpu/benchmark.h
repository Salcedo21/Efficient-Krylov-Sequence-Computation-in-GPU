#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "parametros.h"
#include "metricas.h"

void ejecutar_iteracion(float **A, float **Z_actual, Parametros p, float ***out_Z_nueva, float ***out_snap);
void ejecutar_bucle(float **A, float **Z, Parametros p, float ***resultado,
                    const char *outdir, Metricas *m);

void benchmark(Parametros p, const char *outdir);

#endif
