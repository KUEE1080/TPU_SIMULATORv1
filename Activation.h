#pragma once
#include <vector>
#include <string>
#include <math.h>
#include <inttypes.h>
#include "MatrixMultiplyUnit.h"

#define ReLU 0
#define Sigmoid 1
#define tanh 2

extern std::vector<int32_t> activation_node;
void Activation_run(int mode);