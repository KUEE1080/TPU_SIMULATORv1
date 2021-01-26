//[ASSUMPTION!]: the memory situations are ideal, so most of the hardware constraints are not considered for simplicity

#include "WeightFIFO.h"

std::vector<__int8> node_weight;
std::vector<std::vector<__int8>> tiled_weight; // in reality, weightFIFO are only 4 tiles deep

int tiled_row, tiled_col;

void WeightFIFO_fetchWeight(int weight_row_len, int weight_col_len) { // not considering the Weight Memory since it is ideal
	srand(time(0));
	for (int i = 0; i < weight_row_len; i++) {
		for (int j = 0; j < weight_col_len; j++) {
			/*__int8 tmp = (rand() % 256) - (256 / 2);
			if (tmp != 0) { node_weight.push_back(tmp); }
			else { node_weight.push_back(1); }*/
			node_weight.push_back(1 + i * weight_row_len + j); //-- testing
		}
	}
}

void WeightFIFO_tileWeight(int weight_row_len, int weight_col_len) { // 
	tiled_row = weight_row_len / MATRIX_SIZE;
	tiled_col = weight_col_len / MATRIX_SIZE;

	for (int i = 0; i < tiled_row; i++) {
		for (int j = 0; j < tiled_col; j++) {
			std::vector<__int8> fragment_weight;
			for (int k = 0; k < MATRIX_SIZE; k++) {
				for (int l = 0; l < MATRIX_SIZE; l++) {
					fragment_weight.push_back(node_weight[ ( i * (weight_col_len * MATRIX_SIZE) + j * MATRIX_SIZE ) + (k * weight_col_len + l) ]);
				}
			}
			tiled_weight.push_back(fragment_weight);
		}
	}
}