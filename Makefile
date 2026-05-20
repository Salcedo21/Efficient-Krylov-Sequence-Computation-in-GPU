CC     = gcc
NVCC   = nvcc
CFLAGS = -O0 -Wall -I include
NFLAGS = -O0 -Xptxas -O0 -Xcompiler /Od -I include

SRC_COMMON = src/common/benchmark.c  \
             src/common/main.c       \
             src/common/matrices.c   \
             src/common/metricas.c   \
             src/common/parametros.c

SRC_GEN = src/common/gen_main.c \
          src/common/gen.c

SRC_CPU = src/cpu/matmul_cpu.c

SRC_GPU = src/gpu/matmul_gpu.cu                \
          src/gpu/gpu_memory.cu                \
          src/gpu/kernels/kernel_naive.cu      \
          src/gpu/kernels/kernel_coalesced.cu  \
          src/gpu/kernels/kernel_tiled.cu      \
          src/gpu/kernels/kernel_cublas.cu

GPU_KERNEL ?= NAIVE
EXP        ?= 8

cpu:
	$(CC) $(CFLAGS) $(SRC_COMMON) $(SRC_CPU) -o benchmark_cpu

gpu:
	$(NVCC) $(NFLAGS) -DUSE_CUDA -DGPU_KERNEL_$(GPU_KERNEL) \
	    $(SRC_COMMON) $(SRC_GPU) -lcublas -o benchmark_gpu

gen:
	$(CC) $(CFLAGS) $(SRC_GEN) src/common/matrices.c src/common/parametros.c -o gen_matrices
	./gen_matrices $(EXP)

run-cpu: cpu
	./benchmark_cpu

run-gpu: gpu
	./benchmark_gpu

clean:
	rm -f benchmark_cpu benchmark_gpu gen_matrices