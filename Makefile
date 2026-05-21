CC     = gcc
NVCC   = nvcc
CFLAGS = -O0 -Wall -I include
NFLAGS = -O0 -Xptxas -O0 -Xcompiler /Od -I include

SRC_COMMON = src/common/benchmark.c  \
             src/common/main.c       \
             src/common/matrices.c   \
             src/common/metricas.c   \
             src/common/parametros.c

SRC_GEN      = src/common/gen_main.c \
               src/common/gen.c
SRC_GEN_DEPS = src/common/matrices.c \
               src/common/parametros.c

SRC_CPU = src/cpu/matmul_cpu.c

SRC_GPU = src/gpu/matmul_gpu.cu                \
          src/gpu/gpu_memory.cu                \
          src/gpu/kernels/kernel_naive.cu      \
          src/gpu/kernels/kernel_coalesced.cu  \
          src/gpu/kernels/kernel_tiled.cu      \
          src/gpu/kernels/kernel_cublas.cu

GPU_KERNEL ?= NAIVE
EXP        ?= 8
BUILD       = build

$(BUILD):
	mkdir -p $(BUILD)

cpu: $(BUILD)
	$(CC) $(CFLAGS) $(SRC_COMMON) $(SRC_CPU) -o $(BUILD)/benchmark_cpu
	./$(BUILD)/benchmark_cpu

gpu: $(BUILD)
	$(NVCC) $(NFLAGS) -DUSE_CUDA -DGPU_KERNEL_$(GPU_KERNEL) \
	    $(SRC_COMMON) $(SRC_GPU) -lcublas -o $(BUILD)/benchmark_gpu
	./$(BUILD)/benchmark_gpu

gen: $(BUILD)
	$(CC) $(CFLAGS) $(SRC_GEN) $(SRC_GEN_DEPS) -o $(BUILD)/gen_matrices
	./$(BUILD)/gen_matrices $(EXP)

clean:
	rm -rf $(BUILD)

EXPS ?= 10 11 12 13 14

benchmark: $(BUILD)
	@echo "=== Compilando CPU ==="
	$(CC) $(CFLAGS) $(SRC_COMMON) $(SRC_CPU) -o $(BUILD)/benchmark_cpu

	@echo "=== Compilando GPU (todos los kernels) ==="
	$(NVCC) $(NFLAGS) -DUSE_CUDA -DGPU_KERNEL_NAIVE \
	    $(SRC_COMMON) $(SRC_GPU) -lcublas -o $(BUILD)/benchmark_gpu_naive
	$(NVCC) $(NFLAGS) -DUSE_CUDA -DGPU_KERNEL_COALESCED \
	    $(SRC_COMMON) $(SRC_GPU) -lcublas -o $(BUILD)/benchmark_gpu_coalesced
	$(NVCC) $(NFLAGS) -DUSE_CUDA -DGPU_KERNEL_TILED \
	    $(SRC_COMMON) $(SRC_GPU) -lcublas -o $(BUILD)/benchmark_gpu_tiled
	$(NVCC) $(NFLAGS) -DUSE_CUDA -DGPU_KERNEL_CUBLAS \
	    $(SRC_COMMON) $(SRC_GPU) -lcublas -o $(BUILD)/benchmark_gpu_cublas

	@for exp in $(EXPS); do \
	    echo ""; \
	    echo "============================================"; \
	    echo "  Exponente: $$exp  (N = 2^$$exp)"; \
	    echo "============================================"; \
	    echo "--- CPU ---"; \
	    ./$(BUILD)/benchmark_cpu $$exp; \
	    echo "--- GPU NAIVE ---"; \
	    ./$(BUILD)/benchmark_gpu_naive $$exp; \
	    echo "--- GPU COALESCED ---"; \
	    ./$(BUILD)/benchmark_gpu_coalesced $$exp; \
	    echo "--- GPU TILED ---"; \
	    ./$(BUILD)/benchmark_gpu_tiled $$exp; \
	    echo "--- GPU CUBLAS ---"; \
	    ./$(BUILD)/benchmark_gpu_cublas $$exp; \
	done