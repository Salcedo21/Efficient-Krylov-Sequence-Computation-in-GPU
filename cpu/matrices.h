#ifndef MATRICES_H
#define MATRICES_H

#include "parametros.h"

float **crear_matriz(int filas, int cols);
float inicializar_matrices(Parametros p, float ***out_A, float ***out_Z);
void print_tamano(const char *nombre, int filas, int cols);
void liberar_matriz(float **matriz, int filas);
float **copiar_snapshot(float **src, int n);
float **multiplicar_matrices(float **A, int filas_A, int cols_A, float **B, int cols_B);
void guardar_resultado_txt(float **mat, int filas, int cols, int iter, const char *outdir);
void liberar_resultado(float ***resultado, int l, int n);

#endif
