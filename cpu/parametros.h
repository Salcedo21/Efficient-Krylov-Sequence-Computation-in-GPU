#ifndef PARAMETROS_H
#define PARAMETROS_H

/* Parámetros de configuración derivados del exponente ingresado por el usuario. */
typedef struct {
    int input;  /* Exponente ingresado por el usuario.       */
    int m;      /* Dimensión de A (m×m) y filas de Z (m×n).  */
    int n;      /* Columnas de Z (fijo en 128).              */
    int l;      /* Número de iteraciones = (2·m)/n.          */
} Parametros;

/* Imprime en pantalla los parámetros de configuración del benchmark. */
void print_parametros(Parametros p);

#endif
