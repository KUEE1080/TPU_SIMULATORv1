#pragma once
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "Analysis.h"

extern std::vector<__int8> node_weight;
extern std::vector<std::vector<__int8>> tiled_weight;

void WeightFIFO_fetchWeight(int weight_row_len, int weight_col_len);
void WeightFIFO_tileWeight(int weight_row_len, int weight_col_len);