#pragma once
#include <cublas_v2.h>

void cublas_xgemm(const float *d_A, const float *d_Zin, float *d_Zout, int m, int n);