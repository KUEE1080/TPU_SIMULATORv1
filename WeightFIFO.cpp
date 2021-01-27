//[ASSUMPTION!]: the memory situations are ideal, so most of the hardware constraints are not considered for simplicity

#include "WeightFIFO.h"

std::vector<__int8> node_weight;
std::vector<std::vector<__int8>> tiled_weight; // in reality, weightFIFO are only 4 tiles deep

int tiled_row, tiled_col;

void WeightFIFO_fetchWeight(int weight_row_len, int weight_col_len) { // not considering the Weight Memory since it is ideal
	//srand(time(0));
	
	int weight_filtered_row_size;
	int weight_filtered_col_size;

	if (weight_row_len % MATRIX_SIZE == 0) { weight_filtered_row_size = weight_row_len; } 
	else { weight_filtered_row_size = ((weight_row_len / MATRIX_SIZE) + 1)* MATRIX_SIZE; } //making it to the multiple of MATRIX_SIZE

	if (weight_col_len % MATRIX_SIZE == 0) { weight_filtered_col_size = weight_col_len; }
	else { weight_filtered_col_size = ((weight_col_len / MATRIX_SIZE) + 1) * MATRIX_SIZE; } //making it to the multiple of MATRIX_SIZE

	for (int i = 0; i < weight_filtered_row_size; i++) {
		for (int j = 0; j < weight_filtered_col_size; j++) {
			node_weight.push_back(0);
		}
	}

	for (int i = 0; i < weight_row_len; i++) {
		for (int j = 0; j < weight_col_len; j++) {
			node_weight[i * weight_filtered_col_size + j] = 2;
		}
	}
	int af = node_weight[0];
}

void WeightFIFO_tileWeight(int weight_row_len, int weight_col_len) { // 
	/*tiled_row = weight_row_len / MATRIX_SIZE;
	tiled_col = weight_col_len / MATRIX_SIZE;

	if (weight_row_len % MATRIX_SIZE != 0) { tiled_row++; }
	if (weight_col_len % MATRIX_SIZE != 0) { tiled_col++; }

	for (int i = 0; i < tiled_row; i++) {
		for (int j = 0; j < tiled_col; j++) {
			std::vector<__int8> fragment_weight;
			for (int k = 0; k < MATRIX_SIZE; k++) {
				for (int l = 0; l < MATRIX_SIZE; l++) {
					fragment_weight.push_back(node_weight[ ( i * (MATRIX_SIZE * MATRIX_SIZE) + j * MATRIX_SIZE ) + (k * MATRIX_SIZE + l) ]);
				}
			}
			tiled_weight.push_back(fragment_weight);
		}
	}*/

	int tiled_row = weight_row_len / MATRIX_SIZE;
	int tiled_col = weight_col_len / MATRIX_SIZE;

	int weight_filtered_row_size;
	int weight_filtered_col_size;

	if (weight_row_len % MATRIX_SIZE == 0) { weight_filtered_row_size = weight_row_len; }
	else { weight_filtered_row_size = ((weight_row_len / MATRIX_SIZE) + 1) * MATRIX_SIZE; } //making it to the multiple of MATRIX_SIZE

	if (weight_col_len % MATRIX_SIZE == 0) { weight_filtered_col_size = weight_col_len; }
	else { weight_filtered_col_size = ((weight_col_len / MATRIX_SIZE) + 1) * MATRIX_SIZE; } //making it to the multiple of MATRIX_SIZE

	if (weight_row_len % MATRIX_SIZE != 0) { tiled_row++; }
	if (weight_col_len % MATRIX_SIZE != 0) { tiled_col++; }

	for (int i = 0; i < tiled_row; i++) {
		for (int j = 0; j < tiled_col; j++) {
			std::vector<__int8> fragment_weight;
			for (int k = 0; k < MATRIX_SIZE; k++) {
				for (int l = 0; l < MATRIX_SIZE; l++) {
					fragment_weight.push_back(node_weight[(weight_filtered_col_size * MATRIX_SIZE * i) +
						(MATRIX_SIZE * j) + (k * weight_filtered_col_size + l)]);
				}
			}
			tiled_weight.push_back(fragment_weight);
		}
	}
	int asd = tiled_weight[0][0];
}