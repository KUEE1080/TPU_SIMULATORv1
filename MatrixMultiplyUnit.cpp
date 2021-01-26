#include "MatrixMultiplyUnit.h"
#include "Control.h"

std::vector<Cell> Cells;

__int32 Accumulator[ACCUMULATOR_SIZE][MATRIX_SIZE];
__int8 double_buffer[MATRIX_SIZE][MATRIX_SIZE]; // written in Control.cpp

//Control logic
__int32 buffer_accumulator[MATRIX_SIZE]; // MMU에서 Accumulator로 넘어가기 전에 잠깐 대기하는 값들
int accumulator_index[ACCUMULATOR_SIZE]; //Bx256일때 각 entry안에 있는 256개의 32bit 저장 위치를 트래킹하기 위한 인덱스 저장소
int ibuf_index = 0; // index bit to implement the InputBuffer to be like a queue
//int input_matrix_col = -1;
int count_idle_cell = 0;


Cell::Cell():
	weight(0), input(0), partial_sum(0), tmp_in(0), tmp_psum(0), rightval(NULL), downval(NULL) {};

void Cell::setWeight(__int8 w) { this->weight = w; }

void Cell::mac() { // this operation is done in one cycle
	//일단 weight은 preload되어있고 input은 다 전달받았어야한다.
	if (this->input == 0 || this->weight == 0) { count_idle_cell++; } // used for simulation analysis
	this->partial_sum += (this->input) * (this->weight);
}

void Cell::propagate() {

	if (this->downval != NULL) {
		*downval = this->partial_sum;
		this->partial_sum = this->tmp_psum;
	}
	else { // end of the MMU
		this->partial_sum = this->tmp_psum;
	}

	if (this->rightval != NULL) {
		*rightval = this->input;
		this->input = this->tmp_in;
	}
	else { // end of the MMU
		this->input = this->tmp_in;
	}
}

void Cell::interconnect(__int8* rval, __int32* dval) {
	this->rightval = rval;
	this->downval = dval;
}


//----------------------------------------------------------------------------------------------------------------------


void MMU_initialize() {
 
	//Step3: MMU initialization (loading weights & interconnecting cells)
	for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) { Cells.push_back(Cell()); }
	//transposed the weight matrix!!!

	/*Cells.push_back(11); Cells.push_back(15); Cells.push_back(19); Cells.push_back(23);
	Cells.push_back(12); Cells.push_back(16); Cells.push_back(20); Cells.push_back(24);
	Cells.push_back(13); Cells.push_back(17); Cells.push_back(21); Cells.push_back(25);
	Cells.push_back(14); Cells.push_back(18); Cells.push_back(22); Cells.push_back(26);*/

	for (int i = 0; i < MATRIX_SIZE - 1; i++) {
		for (int j = 0; j < MATRIX_SIZE - 1; j++) {
			Cells[i * MATRIX_SIZE + j].interconnect(&Cells[i * MATRIX_SIZE + (j + 1)].tmp_in, &Cells[(i + 1) * MATRIX_SIZE + j].tmp_psum);
			// (__int16* rval, __int32* dval)
		}
	}

	for (int i = 0; i < MATRIX_SIZE - 1; i++) {
		Cells[MATRIX_SIZE * (MATRIX_SIZE - 1) + i].interconnect(&Cells[MATRIX_SIZE * (MATRIX_SIZE - 1) + i + 1].tmp_in, 
			&buffer_accumulator[i]);
	}
	Cells[MATRIX_SIZE * MATRIX_SIZE - 1].interconnect(NULL, &buffer_accumulator[MATRIX_SIZE - 1]);

	for (int i = 0; i < MATRIX_SIZE - 1; i++) {
		Cells[MATRIX_SIZE * i + (MATRIX_SIZE - 1)].interconnect(NULL, &Cells[MATRIX_SIZE * (i + 1) + (MATRIX_SIZE - 1)].tmp_psum);
	}
}

int MMU_run(int input_row_len, int input_col_len, int weight_row_len, int weight_col_len) { // operation matrix multiply unit done in ONE CYCLE
	if (count_idle_cell == MATRIX_SIZE * MATRIX_SIZE) { return COMPLETE; }
	else { //propagating inputbuffer to MMU
		for (int i = 0; i < MATRIX_SIZE; i++) {
			Cells[i * MATRIX_SIZE].input = UnifiedBuffer[i][ibuf_index];
		}
		ibuf_index++;
	}
	Control_run1(input_row_len, input_col_len, weight_row_len, weight_col_len);
	int dd1= Cells[0].input;

	//[개발 사항] MMU보다 큰 행렬일 경우, 이걸 어캐할지는 생각을 해봐야한다.

	//new code
	count_idle_cell = 0; // resetting the variable for the new cycle
	for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) { Cells[i].mac(); }
	for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) { Cells[i].propagate(); }
	//void setWeight(__int8 w); 이거 pipeline할 때 유용하게 쓰인다!
	//the place where you can reload the weight (possible due to the double buffering -> used in large matrix & CNN)

	for (int i = 0; i < MATRIX_SIZE; i++) {
		if (buffer_accumulator[i] != 0) {
			Accumulator[i][accumulator_index[i]] = buffer_accumulator[i];
			accumulator_index[i]++;
		}
	}
	int d1 = Accumulator[0][0];

	Analysis_incrementIdleRate(count_idle_cell);
	Analysis_sendCycleDone();

	int dd = ibuf_index;
	
	return IN_PROGRESS;
}