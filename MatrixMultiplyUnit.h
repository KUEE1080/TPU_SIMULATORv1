#pragma once

#include <vector>
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
	
	__int8 input;
	__int8 weight;
	__int32 partial_sum;
	__int8 tmp_in;
	__int32 tmp_psum;
	

	//Cell* rightCell;
	//Cell* downCell;

	//the value will be passed with the pointer variable below! Cell pointer variable is obsolete!
	//Cell이 아닌 __int32를 쓰는 이유는, Cell로 해두면 accumulator랑 연결할 때 상당히 곤혹스러워져서 그냥 general 하게 __int32로 뒀다.
	__int8* rightval;
	__int32* downval;

	//void interconnect(Cell* R_Cell, Cell* D_Cell);
	void interconnect(__int8* rval, __int32* dval);
	void setWeight(__int8 w);
	void mac();
	void propagate();

};

extern std::vector<Cell> Cells;
extern __int32 Accumulator[ACCUMULATOR_SIZE][MATRIX_SIZE];
extern __int8 double_buffer[MATRIX_SIZE][MATRIX_SIZE];
extern int ibuf_index;

void MMU_initialize();
//void MMU_assignWeight(int weight_size);
int MMU_run(int input_row_len, int input_col_len, int weight_row_len, int weight_col_len);