#include <stdio.h>
#include "parametros.h"

/* Imprime en pantalla los parámetros de configuración del benchmark. */
void print_parametros(Parametros p) {

    // Imprimir encabezado de la sección
    printf("\n================================================\n");
    printf("            PARAMETROS DE ENTRADA\n");
    printf("================================================\n");

    // Imprimir el exponente ingresado por el usuario
    printf("  Exponente (input)  : %d\n",      p.input);

    // Imprimir la dimensión de la matriz cuadrada A
    printf("  Dimension A        : %d x %d\n", p.m, p.m);

    // Imprimir el número de columnas de Z
    printf("  Columnas Z (n)     : %d\n",      p.n);

    // Imprimir el número de iteraciones del benchmark
    printf("  Iteraciones (l)    : %d\n",      p.l);

    printf("================================================\n");
}
