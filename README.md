# Cómputo Eficiente de Secuencias de Krylov en GPU

> Benchmarking de multiplicación iterativa de matrices para generación de subespacios de Krylov, comparativa entre CPU base y GPU con CUDA.

![C](https://img.shields.io/badge/C-00599C?style=flat&logo=c&logoColor=white)
![CUDA](https://img.shields.io/badge/CUDA-76B900?style=flat&logo=nvidia&logoColor=white)
![Plataforma](https://img.shields.io/badge/plataforma-Windows-blue)

---

## Tabla de Contenidos

1. [Subespacios de Krylov](#subespacios-de-krylov)
2. [Algoritmo](#algoritmo)
3. [Implementación GPU](#implementación-gpu)
4. [Estructura del Proyecto](#estructura-del-proyecto)
5. [Formato de Matrices](#formato-de-matrices)
6. [Documentación](#documentación)
7. [Inicio Rápido](#inicio-rápido)

---

## Subespacios de Krylov

Un **subespacio de Krylov** de orden *r* generado por una matriz *m×m* *A* y un vector *z* es el subespacio vectorial generado por los productos matriz-vector sucesivos:

$$\mathcal{K}_l(A, z) = \text{span}\{z,\ Az,\ \ldots,\ A^{l-1}z\}$$

Los subespacios de Krylov son la base de los solvers iterativos para sistemas lineales grandes y problemas de autovalores (GMRES, Lanczos, Arnoldi). Su cómputo eficiente es crítico cuando *m* es grande, lo que los convierte en un objetivo ideal para su optimización.

([volver arriba](#cómputo-eficiente-de-secuencias-de-krylov-en-gpu))

---

## Algoritmo

El benchmark computa una **secuencia de Krylov por bloques** que reemplaza el vector inicial por una matriz *Z*.

### Parámetros

| Símbolo | Descripción | Valor |
| ------- | ----------- | ----- |
| `m` | Variable | 2¹² ≤ m ≤ 2¹⁶ |
| `n` | Constante | 128 |
| `A` | Matriz cuadrada (*m × m*, float) | aleatoria row-estocástica |
| `Z` | Matriz de bloque (*m × n*, float) | aleatoria |
| `l` | Número de iteraciones | `l = 2m/n` |

### Bucle

Para `i = 0, 1, ..., l − 1`:

1. Calcular `A · Z`
2. Guardar las primeras `n` filas del resultado como snapshot de la iteración `i`
3. Actualizar `Z ← A · Z` para la siguiente iteración

El resultado es una secuencia de *l* snapshots de tamaño *n×n*.

([volver arriba](#cómputo-eficiente-de-secuencias-de-krylov-en-gpu))

---

## Estructura del Proyecto

```
efficient-krylov/
├── src/
│   ├── common/         # fuentes compartidas (main, benchmark, matrices, métricas, gen)
│   ├── cpu/            # multiplicación CPU (matmul_cpu.c)
│   └── gpu/            # multiplicación GPU (matmul_gpu.cu)
├── include/
│   ├── common/         # cabeceras compartidas
│   ├── cpu/            # matmul_cpu.h
│   └── gpu/            # matmul_gpu.h
├── data/               # matrices generadas por make gen (ignorado por git)
├── build/              # binarios compilados (ignorado por git)
└── Makefile
```

([volver arriba](#cómputo-eficiente-de-secuencias-de-krylov-en-gpu))

---

## Documentación

La documentación detallada de cada componente está en `docs/`:

| Documento | Descripción |
| --------- | ----------- |
| <a href="docs/generador.md">generador.md</a> | Generación e inicialización de matrices |
| <a href="docs/cpu.md">cpu.md</a> | Implementación CPU |
| <a href="docs/gpu.md">gpu.md</a> | Implementación GPU (CUDA) |
| <a href="docs/info_hardware.md">info_hardware.md</a> | Especificaciones de los equipos usados para el benchmark |

([volver arriba](#cómputo-eficiente-de-secuencias-de-krylov-en-gpu))

---

## Inicio Rápido

### Prerrequisitos

#### CPU

- MSYS2
- MinGW-w64 / GCC

#### GPU

- GPU NVIDIA con soporte CUDA 
- CUDA Toolkit ≥ 11
- `nvcc` en el PATH

### Clonar

```bash
git clone https://github.com/JeroHoyos/Efficient-Krylov-Sequence-Computation-in-GPU
cd cd Efficient-Krylov-Sequence-Computation-in-GPU
```

### Ejecutar benchmark CPU

```bash
make gen EXP=10   # genera matrices de entrada (m=2^EXP)
make run          # compila y ejecuta el benchmark
```

### Ejecutar benchmark GPU

```bash
make GPU=1        # compila con nvcc
make gen EXP=12   # genera matrices de entrada (m=2^EXP)
make run GPU=1    # ejecuta el benchmark GPU
```

### Benchmark completo (EXP 12 a 16)

Genera automáticamente las matrices y ejecuta el benchmark para cada tamaño sin intervención manual:

```bash
make bench-all        # CPU: EXP=12,13,14,15,16
make bench-all GPU=1  # GPU: EXP=12,13,14,15,16
```

### Referencia de targets

| Comando | Descripción |
| ------- | ----------- |
| `make` | Compila benchmark CPU + generador |
| `make GPU=1` | Compila benchmark GPU (nvcc) + generador |
| `make gen EXP=N` | Genera matrices para m = 2^N |
| `make run` | Ejecuta el benchmark con los datos actuales en `data/` |
| `make run GPU=1` | Ídem en GPU |
| `make bench-all` | Genera y ejecuta para EXP=12..16 (CPU) |
| `make bench-all GPU=1` | Genera y ejecuta para EXP=12..16 (GPU) |
| `make clean` | Elimina el directorio `build/` |
| `make help` | Muestra la referencia de comandos |

([volver arriba](#cómputo-eficiente-de-secuencias-de-krylov-en-gpu))

---

*Proyecto hecho con trasnochos y sufrimiento*
