#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifdef _WIN32
  #include <direct.h>
  #define MKDIR(p) _mkdir(p)
#else
  #include <sys/stat.h>
  #define MKDIR(p) mkdir(p, 0755)
#endif
#include "gen.h"

/* Punto de entrada: lee el exponente, calcula parámetros, guarda params.txt y genera matrices. */
int main(void) {

    // Leer el exponente ingresado por el usuario
    int input;
    printf("Ingrese el exponente: ");
    if (scanf("%d", &input) != 1) {
        fprintf(stderr, "Error: entrada invalida\n");
        return 1;
    }

    // Calcular dimensiones a partir del exponente
    int m = (int)pow(2, input);
    int n = 128;
    int l = (2 * m) / n;

    // Calcular tamaños en MB
    float mb_A = (float)(m * m * sizeof(float)) / (1024 * 1024);
    float mb_Z = (float)(m * n * sizeof(float)) / (1024 * 1024);

    // Crear el directorio de salida de matrices
    const char *dir = "data";
    MKDIR(dir);

    // Guardar los parámetros en params.txt para que cpu los pueda leer
    char ruta[256];
    snprintf(ruta, sizeof(ruta), "%s/params.txt", dir);
    FILE *f = fopen(ruta, "w");
    if (!f) { fprintf(stderr, "Error: no se pudo abrir %s\n", ruta); return 1; }
    fprintf(f, "input %d\n"
               "m %d\n"
               "n %d\n"
               "l %d\n"
               "A: %d x %d floats = %.1f MB\n"
               "Z: %d x %d floats = %.1f MB\n",
               input, m, n, l,
               m, m, mb_A,
               m, n, mb_Z);
    fclose(f);
    printf("Params guardados en %s\n", ruta);

    // Generar y guardar las matrices A y Z en disco
    generar_matrices(dir, m, n);

    printf("\nMatrices listas en '%s/'\n", dir);
    return 0;
}