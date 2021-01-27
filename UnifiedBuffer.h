#pragma once

#include <stdlib.h>
#include <time.h>
#include <string>
#include <vector>
//#include "MatrixMultiplyUnit.h"

#define UNIFIED_BUFFER_LENGTH 65536 // trying to match the actual size of the 
#define MATRIX_SIZE 256

extern std::vector<__int8> node_input; //assuming this is the process of fetching the data from the host memory
extern std::vector<std::vector<__int8>> tiled_input;
extern __int8 UnifiedBuffer[MATRIX_SIZE][UNIFIED_BUFFER_LENGTH]; //total storage of the unified buffer

void UnifiedBuffer_fetchInput(int input_row_len, int input_col_len);
void UnifiedBuffer_tileInput(int input_row_len, int input_col_len);
