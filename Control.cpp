#include "Control.h"

int current_weight_tile_index = 0;
int current_input_tile_index = 0;
int pipeline_col_index = 0;

int x = 0, y = 0, z = 0;

void Control_run2(int input_row_len, int input_col_len, int weight_row_len, int weight_col_len) {
	int weight_tiled_row = weight_row_len / MATRIX_SIZE;
	int weight_tiled_col = weight_col_len / MATRIX_SIZE;

	int input_tiled_row = input_row_len / MATRIX_SIZE;
	int input_tiled_col = input_col_len / MATRIX_SIZE;

	if (weight_row_len % MATRIX_SIZE != 0) { weight_tiled_row++; }
	if (weight_col_len % MATRIX_SIZE != 0) { weight_tiled_col++; }

	if (input_row_len % MATRIX_SIZE != 0) { input_tiled_row++; }
	if (input_col_len % MATRIX_SIZE != 0) { input_tiled_col++; }

	if (UnifiedBuffer[MATRIX_SIZE - 1][ibuf_index] == 10) {
		if (y < weight_tiled_col - 1) { // weight_tiled_col = input_tiled_row
			y++;
		}
		else {
			y = 0;
			if (z < input_tiled_col - 1) {
				z++;
			}
			else {
				z = 0;
				if (x < weight_tiled_row - 1) {
					x++;
				}
				else {
					//done!!!
					ibuf_index = 60000;
					x = 0;
					y = 0;
					z = 0;
					return;
				}
			}
		}
		Control_setTiledWeight(x * weight_tiled_col + y);
		Control_setTiledInput(y * input_tiled_col + z);
	}
}

bool Control_detectInputTileDone(int input_row_len, int input_col_len) { // ��� ���� �Է� ��� ũ�Ⱑ Bx256, �� 256�� ������ �ɷ� �����ϰ� ®��. ������ �ʿ��ϸ� �� ����
	//static int count = 1; // ������ systolic array�� ���� ���� ����Ŭ�� ����ؾ��Ѵ�.
	//
	//if (count == MATRIX_SIZE - 2 + MATRIX_SIZE) { count = 1; return true; }
	//else { count++; return false; }
	if (UnifiedBuffer[MATRIX_SIZE - 1][ibuf_index] == 10) { return true; } //�ε����� 0���� �����ϴϱ�
	else { return false; }
}

void Control_setTiledInput(int idx) {
	//[���� ���� ������]
	//tiling�� ����ؾ��Ѵ�
	if (current_input_tile_index < tiled_input.size()) {

	}
	ibuf_index = 0;
	for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = 0; j < MATRIX_SIZE; j++) {
			int a = current_input_tile_index;
			UnifiedBuffer[i][i + j] = tiled_input[idx][i * MATRIX_SIZE + j];
		}
	}
}

void Control_setTiledWeight(int idx) {
	for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = 0; j < MATRIX_SIZE; j++) {
			Cells[i * MATRIX_SIZE + j].setWeight(tiled_weight[idx][i * MATRIX_SIZE + j]);
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
	//tiling�� ����ؾ��Ѵ�
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