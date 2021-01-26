// Assuming the unified buffer is ideal. So the code structure does not consider the hardware constraints
#include "UnifiedBuffer.h"

std::vector<__int8> node_input; //assuming this is the process of fetching the data from the host memory
std::vector<std::vector<__int8>> tiled_input;
__int8 UnifiedBuffer[MATRIX_SIZE][UNIFIED_BUFFER_LENGTH]; //total storage of the unified buffer, Approximately 16MiB

void UnifiedBuffer_fetchInput(int input_row_len, int input_col_len) {
	srand(time(0));
	//needs to be in 2d array
	for (int i = 0; i < input_row_len * input_col_len; i++) {
		//__int8 tmp = (rand() % 256) - (256 / 2);
		//if (tmp != 0) { node_input.push_back(tmp); } // related to __int8 range, not the matrix size
		//else { node_input.push_back(1); } // not allowing zero values for the input for simplicity
		node_input.push_back(i + 1); //-- testing
	}

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
	�ϴ� case1�ϰ� case2�� ������������ �����ϸ� �ɵ� �ʹ�.

	*/

	//[���� ���� ������]
	/*for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = 0; j < input_size; j++) {
			UnifiedBuffer[i][i + j] = node_input[i * input_size + j];
		}
	}*/
}

void UnifiedBuffer_tileInput(int input_row_len, int input_col_len) {
	int tiled_row = input_row_len / MATRIX_SIZE;

	//[���� �� ��]!
	/*
	������ �Է� ����� ũ��� Bx256�̰� �ִ� B�� ũ��� 256�ε�, ������ loop1�� ���� ���´� �Ͼ�� ���� ���̴�.
	*/
	for (int i = 0; i < tiled_row; i++) {
		std::vector<__int8> fragment_input;
		for (int k = 0; k < MATRIX_SIZE; k++) {
			for (int l = 0; l < input_col_len; l++) { // --- loop1
				fragment_input.push_back(node_input[i * (input_col_len * MATRIX_SIZE) + (k * input_col_len + l)]);
			}
		}
		tiled_input.push_back(fragment_input);
	}
}