#include "MatrixMultiplyUnit.h"
#include "Control.h"

std::vector<Cell> Cells;

int32_t Accumulator[ACCUMULATOR_SIZE][MATRIX_SIZE];
int8_t double_buffer[MATRIX_SIZE][MATRIX_SIZE]; // written in Control.cpp

//Control logic
int32_t buffer_accumulator[MATRIX_SIZE]; // MMU���� Accumulator�� �Ѿ�� ���� ��� ����ϴ� ����
int accumulator_index[ACCUMULATOR_SIZE]; //Bx256�϶� �� entry�ȿ� �ִ� 256���� 32bit ���� ��ġ�� Ʈ��ŷ�ϱ� ���� �ε��� �����
int ibuf_index = 0; // index bit to implement the InputBuffer to be like a queue
//int input_matrix_col = -1;
int count_idle_cell = 0;


Cell::Cell():
	weight(0), input(0), partial_sum(0), tmp_in(0), tmp_psum(0), rightval(NULL), downval(NULL) {};

void Cell::setWeight(int8_t w) { this->weight = w; }

void Cell::mac() { // this operation is done in one cycle
	//�ϴ� weight�� preload�Ǿ��ְ� input�� �� ���޹޾Ҿ���Ѵ�.
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

void Cell::interconnect(int8_t* rval, int32_t* dval) {
	this->rightval = rval;
	this->downval = dval;
}


//----------------------------------------------------------------------------------------------------------------------


void MMU_initialize() {
 
	//Step3: MMU initialization (loading weights & interconnecting cells)
	for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) { Cells.push_back(Cell()); }
	//transposed the weight matrix!!!

	for (int i = 0; i < MATRIX_SIZE - 1; i++) {
		for (int j = 0; j < MATRIX_SIZE - 1; j++) {
			Cells[i * MATRIX_SIZE + j].interconnect(&Cells[i * MATRIX_SIZE + (j + 1)].tmp_in, &Cells[(i + 1) * MATRIX_SIZE + j].tmp_psum);
			// (__int16* rval, int32_t* dval)
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
	for (int i = 0; i < MATRIX_SIZE; i++) {
		Cells[i * MATRIX_SIZE].input = UnifiedBuffer[i][ibuf_index];
	}
	ibuf_index++;

	//Control_run1(input_row_len, input_col_len, weight_row_len, weight_col_len);
	int dd1= Cells[0].input;

	//[���� ����] MMU���� ū ����� ���, �̰� ��ĳ������ ������ �غ����Ѵ�.

	//new code
	count_idle_cell = 0; // resetting the variable for the new cycle
	for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) { Cells[i].mac(); }
	for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) { Cells[i].propagate(); }
	//void setWeight(int8_t w); �̰� pipeline�� �� �����ϰ� ���δ�!
	//the place where you can reload the weight (possible due to the double buffering -> used in large matrix & CNN)

	for (int i = 0; i < MATRIX_SIZE; i++) {
		if (buffer_accumulator[i] != 0) {
			Accumulator[i][accumulator_index[i]] = buffer_accumulator[i];
			accumulator_index[i]++;
		}
	}

	Analysis_incrementIdleRate(count_idle_cell);
	Analysis_sendCycleDone();

	for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) {
		if (Cells[i].partial_sum != 0) { break; }
		if (i == MATRIX_SIZE * MATRIX_SIZE - 1) { return COMPLETE; }
	}
	
	return IN_PROGRESS;
}

void MMU_reset() {
	count_idle_cell = 0;
}