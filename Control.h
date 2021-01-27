#pragma once
#include "UnifiedBuffer.h"
#include "WeightFIFO.h"
#include "MatrixMultiplyUnit.h"
#include <inttypes.h>

#define WEIGHT_LOAD_ALL 1
#define WEIGHT_LOAD_COL 2

//�ش� ������Ʈ��ũ�� ��� ���̾ �ִ��� �����ؾ��Ѵ�

extern int current_weight_tile_index;
extern int current_input_tile_index;

//void Control_run(int input_row_len, int input_col_len, int weight_row_len, int weight_col_len);
//void Control_run1(int input_row_len, int input_col_len, int weight_row_len, int weight_col_len);
void Control_run2(int input_row_len, int input_col_len, int weight_row_len, int weight_col_len);
void Control_setTiledInput(int idx);
void Control_setTiledWeight(int idx);
void Control_doublebufferSetUp();
//void Control_weightSetup(int mode, int chosen_col);
//bool Control_detectInputTileDone(int input_row_len, int input_col_len);