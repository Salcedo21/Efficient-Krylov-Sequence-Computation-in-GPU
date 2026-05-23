# Cómputo Eficiente de Secuencias de Krylov en GPU

> Benchmarking de multiplicación iterativa de matrices para generación de subespacios de Krylov, comparativa entre CPU base y GPU con CUDA.

![C](https://img.shields.io/badge/C-00599C?style=flat&logo=c&logoColor=white)
![CUDA](https://img.shields.io/badge/CUDA-76B900?style=flat&logo=nvidia&logoColor=white)
![Plataforma](https://img.shields.io/badge/plataforma-Windows-blue)

---

## Tabla de Contenidos

1. [Subespacios de Krylov](#subespacios-de-krylov)
2. [Algoritmo](#algoritmo)
3. [Estructura del Proyecto](#estructura-del-proyecto)
4. [Formato de Matrices](#formato-de-matrices)
5. [Documentación](#documentación)
6. [Inicio Rápido](#inicio-rápido)

---

## Subespacios de Krylov

Un **subespacio de Krylov** de orden *r* generado por una matriz *m×m* *A* y un vector *z* es el subespacio vectorial generado por los productos matriz-vector sucesivos:

$$\mathcal{K}_l(A, z) = \text{span}\{z,\ Az,\ \ldots,\ A^{l-1}z\}$$

Los subespacios de Krylov son la base de los solvers iterativos para sistemas lineales grandes y problemas de autovalores (GMRES, Lanczos, Arnoldi). Su cómputo eficiente es crítico cuando *m* es grande, lo que los convierte en un objetivo ideal para su optimización.

Para saber más, revisar la investigación que se hizo para el proyecto en <a href="docs/research.md">Ver investigación</a>

([volver arriba](#cómputo-eficiente-de-secuencias-de-krylov-en-gpu))

---

## Algoritmo

El benchmark computa una **secuencia de Krylov por bloques** que reemplaza el vector inicial por una matriz *Z*.

### Parámetros

| Símbolo | Descripción | Valor |
| ------- | ----------- | ----- |
| `m` | Tamaño del problema (dimensión de la matriz) | 2¹⁰ ≤ m ≤ 2²⁰ |
| `n` | Tamaño del bloque (constante) | 128 |
| `A` | Matriz cuadrada (*m × m*, float) | aleatoria row-estocástica |
| `Z` | Matriz de bloque (*m × n*, float) | aleatoria |
| `l` | Número de iteraciones | `l = 2m/n` |

### Bucle

Para `i = 0, 1, ..., l − 1`:

1. Calcular `A · Z` (multiplicación de matrices, *m×m* × *m×n*)
2. Guardar las primeras `n` filas del resultado como snapshot de la iteración `i`
3. Actualizar `Z ← A · Z` para la siguiente iteración

El resultado es una secuencia de *l* snapshots de tamaño *n×n*.

([volver arriba](#cómputo-eficiente-de-secuencias-de-krylov-en-gpu))

---

## Estructura del Proyecto

WIP

([volver arriba](#cómputo-eficiente-de-secuencias-de-krylov-en-gpu))

---

## Documentación

WIP

---

## Inicio Rápido

### Prerrequisitos

#### CPU

- MinGW-w64 (Windows)
- GNU Make

#### GPU *(cuando esté disponible)*

- GPU NVIDIA con CUDA Toolkit ≥ 11

### Clonar

```bash
git clone https://github.com/<your-username>/efficient-krylov-sequence-computation-in-gpu.git
cd efficient-krylov-sequence-computation-in-gpu
```

### Ejecutar benchmark CPU

WIP

### Ejecutar benchmark GPU

WIP

([volver arriba](#cómputo-eficiente-de-secuencias-de-krylov-en-gpu))

---

*Proyecto hecho con trasnochos y sufrimiento*