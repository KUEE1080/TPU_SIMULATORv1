#pragma once

#include <vector>
#include <inttypes.h>
#include "UnifiedBuffer.h"
#include "WeightFIFO.h"
#include "Analysis.h"

#define MATRIX_SIZE 256
#define ACCUMULATOR_SIZE 4096

#define IN_PROGRESS 0
#define COMPLETE 1

class Cell {
public:
	Cell();
	
	int8_t input;
	int8_t weight;
	int32_t partial_sum;
	int8_t tmp_in;
	int32_t tmp_psum;
	

	//Cell* rightCell;
	//Cell* downCell;

	//the value will be passed with the pointer variable below! Cell pointer variable is obsolete!
	//Cell�� �ƴ� int32_t�� ���� ������, Cell�� �صθ� accumulator�� ������ �� ����� ��Ȥ���������� �׳� general �ϰ� int32_t�� �״�.
	int8_t* rightval;
	int32_t* downval;

	//void interconnect(Cell* R_Cell, Cell* D_Cell);
	void interconnect(int8_t* rval, int32_t* dval);
	void setWeight(int8_t w);
	void mac();
	void propagate();

};

extern std::vector<Cell> Cells;
extern int32_t Accumulator[ACCUMULATOR_SIZE][MATRIX_SIZE];
extern int8_t double_buffer[MATRIX_SIZE][MATRIX_SIZE];
extern int accumulator_index[ACCUMULATOR_SIZE];
extern int ibuf_index;

void MMU_initialize();
//void MMU_assignWeight(int weight_size);
int MMU_run(int input_row_len, int input_col_len, int weight_row_len, int weight_col_len);
void MMU_reset();