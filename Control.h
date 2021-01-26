#pragma once
#include "UnifiedBuffer.h"
#include "WeightFIFO.h"
#include "MatrixMultiplyUnit.h"

#define WEIGHT_LOAD_ALL 1
#define WEIGHT_LOAD_COL 2

//해당 뉴럴네트워크에 몇개의 레이어가 있는지 저장해야한다

extern int current_weight_tile_index;
extern int current_input_tile_index;

void Control_run(int input_row_len, int input_col_len, int weight_row_len, int weight_col_len);
void Control_run1(int input_row_len, int input_col_len, int weight_row_len, int weight_col_len);
void Control_inputDataSetup(int input_row_len, int input_col_len);
void Control_doublebufferSetUp();
void Control_weightSetup(int mode, int chosen_col);
bool Control_detectInputTileDone(int input_row_len, int input_col_len);