#include "Control.h"

int inference_status;
int current_weight_tile_index = 0;
int current_input_tile_index = 0;
int pipeline_col_index = 0;

void Control_run(int input_row_len, int input_col_len, int weight_row_len, int weight_col_len) {
	// input_col_len = weight_row_len
	int max_weight_index = (weight_col_len / MATRIX_SIZE);
	int max_input_index = (input_col_len / MATRIX_SIZE);

	if (Control_detectInputTileDone()) {
		//tiled_input, tiled_weight �̰� �� ���ʿ� 1���� �迭�� ������ �ϱ� �Ѵ�. (���϶��� 2���� �迭��)
		pipeline_col_index = 0;
		
		if (current_input_tile_index < max_input_index) { current_input_tile_index++; }
		else { current_input_tile_index = 0; }

		if (current_weight_tile_index < max_weight_index) { current_weight_tile_index++; }
		else { current_weight_tile_index = 0; }

		if (weight) {
			Control_inputDataSetup(input_row_len, input_col_len); //loading new input values to the inputbuffer
		}
		else {

		}

		

		Control_doublebufferSetUp();
		Control_weightSetup(WEIGHT_LOAD_COL, pipeline_col_index); //loading new weight values to the MMU
		pipeline_col_index++;
		
	}
	else { // implementation of pipelining
		if (pipeline_col_index < MATRIX_SIZE) {
			Control_weightSetup(WEIGHT_LOAD_COL, pipeline_col_index);
			pipeline_col_index++;
		}
		else { // ���̻� �� ��ǲ�� ���� ���� ���� ��ǲ���� systolic array �ȿ��� �ɵ��� ��Ȳ
			//nops

		}
	}

	//if (weight_row_len == weight_col_len) { // Case1: Square Matrix
	//	

	//}
	//else if (weight_row_len > weight_col_len) { // Case2: Rectangular Matrix (eg. 4 x 2)

	//}
	//else { // Case3: Rectangular Matrix (eg. 2 x 4)

	//}

}

bool Control_detectInputTileDone() {
	//static int count = 1; // ������ systolic array�� ���� ���� ����Ŭ�� �����ؾ��Ѵ�.
	//
	//if (count == MATRIX_SIZE - 2 + MATRIX_SIZE) { count = 1; return true; }
	//else { count++; return false; }
	if (ibuf_index == 2 * MATRIX_SIZE - 2) { return true; } //�ε����� 0���� �����ϴϱ�
	else { return false; }
}

void Control_inputDataSetup(int input_row_len, int input_col_len) {
	//[���� ���� ������]
	//tiling�� �����ؾ��Ѵ�
	if (current_input_tile_index < tiled_input.size()) {

	}
	ibuf_index = 0;
	for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = 0; j < input_col_len; j++) {
			UnifiedBuffer[i][i + j] = tiled_input[current_input_tile_index][i * input_col_len + j];
		}
	}
}

void Control_doublebufferSetUp() {
	for (int i = 0; i < MATRIX_SIZE; i++) { // initialization of double buffering
		for (int j = 0; j < MATRIX_SIZE; j++) {
			double_buffer[i][j] = tiled_weight[current_weight_tile_index][i * MATRIX_SIZE + j];
		}
	}
}

void Control_weightSetup(int mode, int chosen_col) {
	//[���� ���� ������]
	//tiling�� �����ؾ��Ѵ�
	//case1: when inferencing the new layer

	switch (mode)
	{
	case WEIGHT_LOAD_ALL:
		for (int i = 0; i < MATRIX_SIZE; i++) {
			for (int j = 0; j < MATRIX_SIZE; j++) {
				Cells[i * MATRIX_SIZE + j].setWeight(double_buffer[i][j]);
			}
		}
		break;
	case WEIGHT_LOAD_COL:
		for (int i = 0; i < MATRIX_SIZE; i++) {
			Cells[i * MATRIX_SIZE + chosen_col].setWeight(double_buffer[i][chosen_col]);
		}
		break;
	}
	
	//case2: when pipelining the layer to increase the throughput
}