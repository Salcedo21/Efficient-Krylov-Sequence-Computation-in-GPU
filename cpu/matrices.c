#include <stdio.h>
#include <stdlib.h>
#include "matrices.h"

float **crear_matriz(int filas, int cols) {

    // Reservar arreglo de punteros a filas
    float **matriz = malloc(filas * sizeof(float *));

    for (int i = 0; i < filas; i++) {
        // Reservar memoria para cada fila
        matriz[i] = malloc(cols * sizeof(float));

        for (int j = 0; j < cols; j++)
            // Asignar valor aleatorio entre 0 y 1
            matriz[i][j] = (float)rand() / RAND_MAX;
    }
    // Retornar la matriz inicializada
    return matriz;
}

void print_tamano(const char *nombre, int filas, int cols) {

    // Calcular tamaño en bytes de la matriz
    long bytes = (long)filas * cols * sizeof(float);

    // Imprimir nombre y dimensiones de la matriz
    printf("  Matriz %s [%d x %d]: ", nombre, filas, cols);

    // Imprimir tamaño en TB si es mayor a 1 TB
    if (bytes >= 1024L * 1024 * 1024 * 1024)
        printf("%ld bytes (%.2f TB)\n", bytes, bytes / (1024.0 * 1024.0 * 1024.0 * 1024.0));

    // Imprimir tamaño en GB si es mayor a 1 GB
    else if (bytes >= 1024L * 1024 * 1024)
        printf("%ld bytes (%.2f GB)\n", bytes, bytes / (1024.0 * 1024.0 * 1024.0));

    // Imprimir tamaño en MB si es mayor a 1 MB
    else if (bytes >= 1024 * 1024)
        printf("%ld bytes (%.2f MB)\n", bytes, bytes / (1024.0 * 1024.0));

    // Imprimir tamaño en KB si es mayor a 1 KB
    else if (bytes >= 1024)
        printf("%ld bytes (%.2f KB)\n", bytes, bytes / 1024.0);

    // Imprimir tamaño en bytes si es menor a 1 KB
    else
        printf("%ld bytes\n", bytes);
}

float inicializar_matrices(Parametros p, float ***out_A, float ***out_Z) {

    printf("\n=== Inicializacion ===\n");

    *out_A = crear_matriz(p.m, p.m);
    *out_Z = crear_matriz(p.m, p.n);

    print_tamano("A", p.m, p.m);
    print_tamano("Z", p.m, p.n);
}

void liberar_matriz(float **matriz, int filas) {

    // Liberar memoria de cada fila
    for (int i = 0; i < filas; i++) {free(matriz[i]);}
 
    // Liberar arreglo de punteros a filas
    free(matriz);
}

float **copiar_snapshot(float **src, int n) {

    // Reservar arreglo de punteros a filas
    float **snap = malloc(n * sizeof(float *));

    for (int i = 0; i < n; i++) {
        // Reservar memoria para cada fila
        snap[i] = malloc(n * sizeof(float));

        for (int j = 0; j < n; j++)
            // Copiar elemento por elemento desde src
            snap[i][j] = src[i][j];
    }

    // Retornar la copia de la matriz
    return snap;
}

float **multiplicar_matrices(float **A, int filas_A, int cols_A, float **B, int cols_B) {

    // Reservar arreglo de punteros a filas de la matriz resultado
    float **C = malloc(filas_A * sizeof(float *));

    for (int i = 0; i < filas_A; i++) {

        // Reservar memoria para cada fila
        C[i] = malloc(cols_B * sizeof(float));

        for (int j = 0; j < cols_B; j++) {

            // Acumulador para la suma de productos
            float suma = 0.0f;

            for (int k = 0; k < cols_A; k++)

                // Multiplicar fila de A por columna de B
                suma += A[i][k] * B[k][j];

            // Asignar resultado al elemento C[i][j]
            C[i][j] = suma;
        }
    }
    
    // Retornar la matriz resultado
    return C;
}

void guardar_resultado_txt(float **mat, int filas, int cols, int iter, const char *outdir) {
    char nombre[256];
    snprintf(nombre, sizeof(nombre), "%s/resultado_%d.txt", outdir, iter);

    FILE *f = fopen(nombre, "w");
    if (!f) {
        fprintf(stderr, "Error: no se pudo abrir %s\n", nombre);
        return;
    }

    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < cols; j++) {
            fprintf(f, j < cols - 1 ? "%f " : "%f", mat[i][j]);
        }
        fprintf(f, "\n");
    }

    fclose(f);
    printf("  Guardado: %s\n", nombre);
}

void liberar_resultado(float ***resultado, int l, int n) {
    for (int i = 0; i < l; i++)
        liberar_matriz(resultado[i], n);
    free(resultado);
}