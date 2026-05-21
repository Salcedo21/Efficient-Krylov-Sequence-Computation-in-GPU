//! main.c - Lectura de parámetros, creación de directorio de salida y ejecución del benchmark.
//!
//! Cómo funciona:
//! 1. Lee los parámetros desde "data/params.txt" y los muestra por consola.
//! 2. Crea un directorio de salida basado en el input y la fecha/hora actual.
//! 3. Llama a benchmark() con los parámetros leídos.
//!
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "common/parametros.h"
#include "common/benchmark.h"

int main(int argc, char *argv[]) {

    const char *matrices_dir = "data";
    Parametros p;

    if (argc >= 2) {
        int input = atoi(argv[1]);
        if (input <= 0) {
            fprintf(stderr, "Error: EXP debe ser un entero positivo\n");
            return 1;
        }
        p.input = input;
        p.m     = (int)pow(2, input);
        p.n     = 128;
        p.l     = (2 * p.m) / p.n;
    } else {
        if (leer_params(matrices_dir, &p) != 0) return 1;
    }

    #if defined(USE_CUDA)
        printf("\n[Modo: GPU]\n");
    #else
        printf("\n[Modo: CPU]\n");
    #endif

    print_parametros(p);

    char outdir[80];  
    if (crear_outdir(p.input, outdir, sizeof(outdir)) != 0) {
        return 1;
    }

    benchmark(p, matrices_dir, outdir);
    return 0;
}
