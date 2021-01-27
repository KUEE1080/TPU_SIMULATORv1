#pragma once

#include <iostream> 
#include <vector>
#include <algorithm>
#include <numeric>

#include "MatrixMultiplyUnit.h"

extern std::vector<int> utilization_per_cycle;
//extern std::vector<__int8> assigned_weight;

//used in MMU_run
void Analysis_incrementIdleRate(int cnt);
void Analysis_sendCycleDone();

//used in main.cpp
void runAnalysis(int num_of_layer, int weight_row_len, int weight_col_len, int input_row_len, int input_col_len, int _cycle);