#include "Control.h"

int inference_status;
int current_weight_tile_index = 0;
int current_input_tile_index = 0;
int pipeline_col_index = 0;

void Control_run(int input_row_len, int input_col_len, int weight_row_len, int weight_col_len) {
	// input_col_len = weight_row_len
	int max_weight_index =  (weight_col_len * weight_row_len) / (MATRIX_SIZE * MATRIX_SIZE);
	int max_input_index = (input_col_len / MATRIX_SIZE); // 얘도 수정되어야할듯 싶다

	if (Control_detectInputTileDone(input_row_len, input_col_len)) {
		//tiled_input, tiled_weight 이건 다 애초에 1차원 배열로 저장을 하긴 한다. (쓰일때는 2차원 배열로)
		pipeline_col_index = 0;
		
		/*if (current_input_tile_index < max_input_index) {  }
		else { current_input_tile_index = 0; }*/
		current_input_tile_index++;
		current_input_tile_index %= max_input_index;

		//if (current_weight_tile_index < max_weight_index) { current_weight_tile_index++; }
		//else { current_weight_tile_index = 0; }
		
		int k = tiled_input[0][0];
		if (current_weight_tile_index <= max_weight_index) {
			
			Control_inputDataSetup(input_row_len, input_col_len); //loading new input values to the inputbuffer
			current_weight_tile_index++;
			//Control_doublebufferSetUp();
			//Control_weightSetup(WEIGHT_LOAD_COL, pipeline_col_index); //loading new weight values to the MMU
			//pipeline_col_index++;
		}
		else { // no more input data left
			ibuf_index = 60000; // 걍 다 0만 출력하면 된다
		}
	}
	else { // implementation of pipelining
		//if (pipeline_col_index < MATRIX_SIZE) {
		//	Control_weightSetup(WEIGHT_LOAD_COL, pipeline_col_index);
		//	pipeline_col_index++;
		//}
		//else { // 더이상 들어갓 인풋이 없고 이제 남은 인풋들이 systolic array 안에서 맴도는 상황
		//	//nops

		//}
	}

	//if (weight_row_len == weight_col_len) { // Case1: Square Matrix
	//	

	//}
	//else if (weight_row_len > weight_col_len) { // Case2: Rectangular Matrix (eg. 4 x 2)

	//}
	//else { // Case3: Rectangular Matrix (eg. 2 x 4)

	//}

}

void Control_run1(int input_row_len, int input_col_len, int weight_row_len, int weight_col_len) { //used in MMU_run()
	int max_weight_index = (weight_col_len * weight_row_len) / (MATRIX_SIZE * MATRIX_SIZE);
	int max_input_index = (input_col_len * input_row_len) / (MATRIX_SIZE * MATRIX_SIZE);

	if (UnifiedBuffer[MATRIX_SIZE - 1][ibuf_index] == 10) {
		if (current_weight_tile_index == tiled_weight.size() - 1) { // no more input data left
			ibuf_index = 60000; // 걍 다 0만 출력하면 된다
		}
		else {
			current_weight_tile_index++;
			
			current_input_tile_index++;
			current_input_tile_index %= max_input_index;
			Control_inputDataSetup(input_row_len, input_col_len);
		}
	}
}

bool Control_detectInputTileDone(int input_row_len, int input_col_len) { // 얘는 지금 입력 행렬 크기가 Bx256, 즉 256이 고정된 걸로 간주하고 짰다. 변경이 필요하면 얘 수정
	//static int count = 1; // 실제로 systolic array에 들어가기 직전 사이클도 고려해야한다.
	//
	//if (count == MATRIX_SIZE - 2 + MATRIX_SIZE) { count = 1; return true; }
	//else { count++; return false; }
	if (UnifiedBuffer[MATRIX_SIZE - 1][ibuf_index] == 10) { return true; } //인덱스가 0부터 시작하니까
	else { return false; }
}

void Control_inputDataSetup(int input_row_len, int input_col_len) {
	//[현재 개발 진행중]
	//tiling도 고려해야한다
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
	//[현재 개발 진행중]
	//tiling도 고려해야한다
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