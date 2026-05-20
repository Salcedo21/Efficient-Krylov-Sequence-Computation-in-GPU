//! main.c - Lectura de parámetros, creación de directorio de salida y ejecución del benchmark.
//!
//! Cómo funciona:
//! 1. Lee los parámetros desde "data/params.txt" y los muestra por consola.
//! 2. Crea un directorio de salida basado en el input y la fecha/hora actual.
//! 3. Llama a benchmark() con los parámetros leídos.
//!
#include <stdio.h>
#include <stdlib.h>
#include "common/parametros.h"
#include "common/benchmark.h"

int main(void) {

    const char *matrices_dir = "data";
    Parametros p;

    if (leer_params(matrices_dir, &p) != 0) {
        return 1;
    }

    #if defined(USE_CUDA)
        printf("\n[Modo: GPU / CUDA]\n");
    #else
        printf("\n[Modo: CPU]\n");
    #endif

    print_parametros(p);

    char outdir[64];
    if (crear_outdir(p.input, outdir, sizeof(outdir)) != 0) {
        return 1;
    }

    benchmark(p, matrices_dir, outdir);
    return 0;
}
