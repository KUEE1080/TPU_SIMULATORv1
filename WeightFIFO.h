#pragma once
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <inttypes.h>
#include "Analysis.h"

extern std::vector<int8_t> node_weight;
extern std::vector<std::vector<int8_t>> tiled_weight;

void WeightFIFO_fetchWeight(int weight_row_len, int weight_col_len);
void WeightFIFO_tileWeight(int weight_row_len, int weight_col_len);