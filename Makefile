# ── Makefile ──────────────────────────────────────────────────────────────────
# Uso:
#   make cpu                        → compila modo CPU
#   make gpu                        → compila GPU con kernel naive  (por defecto)
#   make gpu GPU_KERNEL=COALESCED   → compila GPU con kernel coalesced
#   make gpu GPU_KERNEL=TILED       → compila GPU con kernel tiled
#   make clean                      → elimina binarios

CC     = gcc
NVCC   = nvcc
CFLAGS = -O2 -Wall -I include
NFLAGS = -O2 -I include

# Fuentes comunes (C puro)
SRC_COMMON = src/common/benchmark.c  \
             src/common/main.c       \
             src/common/matrices.c   \
             src/common/metricas.c   \
             src/common/parametros.c

SRC_CPU = src/cpu/matmul_cpu.c

SRC_GPU = src/gpu/matmul_gpu.cu             \
          src/gpu/gpu_memory.cu             \
          src/gpu/kernels/kernel_naive.cu   \
          src/gpu/kernels/kernel_coalesced.cu \
          src/gpu/kernels/kernel_tiled.cu

# ── Kernel GPU seleccionable ───────────────────────────────────────────────────
GPU_KERNEL ?= NAIVE
KERNEL_FLAG = -DGPU_KERNEL_$(GPU_KERNEL)

# ── Targets ───────────────────────────────────────────────────────────────────
.PHONY: cpu gpu clean

cpu:
	$(CC) $(CFLAGS) $(SRC_COMMON) $(SRC_CPU) -o benchmark_cpu

gpu:
	$(NVCC) $(NFLAGS) -DUSE_CUDA $(KERNEL_FLAG) \
	    $(SRC_COMMON) $(SRC_GPU) -o benchmark_gpu

clean:
	del /Q benchmark_cpu.exe benchmark_gpu.exe 2>nul || \
	rm -f benchmark_cpu benchmark_gpu
