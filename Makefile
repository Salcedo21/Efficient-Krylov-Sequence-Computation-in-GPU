CC   = gcc
SRCS = cpu/main.c cpu/benchmark.c cpu/matrices.c cpu/parametros.c \
       cpu/metricas.c cpu/perfil.c
EXP ?= 8

ifeq ($(OS),Windows_NT)
    LIBS = -lm -lpsapi
    BIN  = bench_O0.exe
else
    LIBS = -lm
    BIN  = bench_O0
endif

.PHONY: run clean

$(BIN): $(SRCS)
	$(CC) -O0 -o $@ $^ $(LIBS)

run: $(BIN)
	echo "$(EXP)" | ./$(BIN)

clean:
	rm -f bench_O0 bench_O0.exe gmon.out .last_outdir
