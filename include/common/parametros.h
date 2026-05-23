#pragma once
#include <stddef.h>

typedef struct {
    int input;
    int m, n, l;
} Parametros;

void print_parametros(Parametros p);
int  leer_params(const char *dir, Parametros *p);
int  crear_outdir(int input, char *outdir, size_t size);
