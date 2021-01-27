// Assuming the unified buffer is ideal. So the code structure does not consider the hardware constraints
#include "UnifiedBuffer.h"

std::vector<__int8> node_input; //assuming this is the process of fetching the data from the host memory
std::vector<std::vector<__int8>> tiled_input;
__int8 UnifiedBuffer[MATRIX_SIZE][UNIFIED_BUFFER_LENGTH]; //total storage of the unified buffer, Approximately 16MiB

void UnifiedBuffer_fetchInput(int input_row_len, int input_col_len) {
	//srand(time(0));
	//needs to be in 2d array
	int input_filtered_row_size;
	int input_filtered_col_size;

	if (input_row_len % MATRIX_SIZE == 0) { input_filtered_row_size = input_row_len; }
	else { input_filtered_row_size = ((input_row_len / MATRIX_SIZE) + 1) * MATRIX_SIZE; } //making it to the multiple of MATRIX_SIZE

	if (input_col_len % MATRIX_SIZE == 0) { input_filtered_col_size = input_col_len; }
	else { input_filtered_col_size = ((input_col_len / MATRIX_SIZE) + 1) * MATRIX_SIZE; } //making it to the multiple of MATRIX_SIZE

	for (int i = 0; i < input_filtered_row_size; i++) {
		for (int j = 0; j < input_filtered_col_size; j++) {
			node_input.push_back(0);
		}
	}

	for (int i = 0; i < input_row_len; i++) {
		for (int j = 0; j < input_col_len; j++) {
			node_input[i * input_filtered_col_size + j] = 5;
			//node_input[i * input_filtered_col_size + j] = cc; // testing [시발!!!!]
			//cc++;
		}
	}

	int aaaaa = node_input[0];

	//for (int i = 0; i < input_row_len * input_col_len; i++) {
	//	//__int8 tmp = (rand() % 256) - (256 / 2);
	//	//if (tmp != 0) { node_input.push_back(tmp); } // related to __int8 range, not the matrix size
	//	//else { node_input.push_back(1); } // not allowing zero values for the input for simplicity
	//	//node_input.push_back(i + 1); //-- testing
	//	node_input.push_back(5); //5: in progress data
	//}
	

	//Step5: Systolic Data Setup
	
	//Case1 (Fully Connected): weight size: 256x256, input size: Bx256 (1<=B<=256)
	//Case2 (Fully Connected): weight size: AxA, input size: BxA (A is a multiple of 256, 1<=B<=A)
	//implemented by using sequences of Case1 and gets result by getting the partial sum)
	//Case3 (CONV): weight size: 256x256, filter size: CxC, input size: Bx256 (1<=B<=256)
	//=> new_weight: (256 - C + 1)xB, input size: Bx256
	//Case4 (CONV): weight size: AxA, filter size: CxC, input size: BxA (A is a multiple of 256, 1<=B<=A)
	//=> new_weight: (A - C + 1)xB, input size: BxA
	//[caution] case3 and case 4 weight matrix is rectangular matrix!!!
	
	/*
	<pseudo code>
	일단 case1하고 case2는 삼중포문으로 구현하면 될듯 싶다.

	*/

	//[현재 개발 진행중]
	/*for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = 0; j < input_size; j++) {
			UnifiedBuffer[i][i + j] = node_input[i * input_size + j];
		}
	}*/
}

//void UnifiedBuffer_tileInput(int input_row_len, int input_col_len) { //obsolete
//	int tiled_row = input_row_len / MATRIX_SIZE;
//
//	if (input_row_len % MATRIX_SIZE != 0) { tiled_row++; }
//
//	for (int i = 0; i < tiled_row; i++) {
//		std::vector<__int8> fragment_input;
//		for (int k = 0; k < MATRIX_SIZE; k++) {
//			for (int l = 0; l < MATRIX_SIZE; l++) { // --- loop1
//				fragment_input.push_back(node_input[i * (MATRIX_SIZE * MATRIX_SIZE) + (k * MATRIX_SIZE + l)]);
//			}
//		}
//		fragment_input[0] = 1; // 1: start of the data
//		fragment_input[(MATRIX_SIZE * MATRIX_SIZE) - 1] = 10; // 10: end of the data
//		tiled_input.push_back(fragment_input);
//	}
//
//	for (int i = 0; i < tiled_row; i++) {
//
//	}
//
//	for (int i = 0; i < MATRIX_SIZE; i++) {
//		for (int j = 0; j < MATRIX_SIZE; j++) {
//
//			//tiled_input[tiled_input.size() - 1].[]
//		}
//	}
//
//	int asd = tiled_input[0][0];
//}

void UnifiedBuffer_tileInput(int input_row_len, int input_col_len) {
	int tiled_row = input_row_len / MATRIX_SIZE;
	int tiled_col = input_col_len / MATRIX_SIZE;

	int input_filtered_row_size;
	int input_filtered_col_size;

	if (input_row_len % MATRIX_SIZE == 0) { input_filtered_row_size = input_row_len; }
	else { input_filtered_row_size = ((input_row_len / MATRIX_SIZE) + 1) * MATRIX_SIZE; } //making it to the multiple of MATRIX_SIZE

	if (input_col_len % MATRIX_SIZE == 0) { input_filtered_col_size = input_col_len; }
	else { input_filtered_col_size = ((input_col_len / MATRIX_SIZE) + 1) * MATRIX_SIZE; } //making it to the multiple of MATRIX_SIZE
	
	if (input_row_len % MATRIX_SIZE != 0) { tiled_row++; }
	if (input_col_len % MATRIX_SIZE != 0) { tiled_col++; }

	for (int i = 0; i < tiled_row; i++) {
		for (int j = 0; j < tiled_col; j++) {
			std::vector<__int8> fragment_input;
			for (int k = 0; k < MATRIX_SIZE; k++) {
				for (int l = 0; l < MATRIX_SIZE; l++) {
					fragment_input.push_back(node_input[( input_filtered_col_size * MATRIX_SIZE * i) + 
						( MATRIX_SIZE * j ) + ( k * input_filtered_col_size + l ) ]);
				}
			}
			tiled_input.push_back(fragment_input);
		}
	}

	for (int i = 0; i < tiled_row; i++) { //inserting the start and the end of the input matrix 
		for (int j = 0; j < tiled_col; j++) {
			int end_row = MATRIX_SIZE, end_col = MATRIX_SIZE;
			for (int c = 0; c < MATRIX_SIZE; c++) {
				if (tiled_input[i * tiled_col + j][c] == 0) { end_col = c; break; }
			}
			for (int r = 0; r < MATRIX_SIZE; r++) {
				if (tiled_input[i * tiled_col + j][r * MATRIX_SIZE] == 0) { end_row = r; break; }
			}
			tiled_input[i * tiled_col + j][0] = 1; //implying the start
			tiled_input[i * tiled_col + j][MATRIX_SIZE * (end_row - 1) + end_col - 1] = 10; //implying the end
		}
	}
	int asd = tiled_input[0][0];
}