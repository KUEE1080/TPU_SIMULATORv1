#include "Analysis.h"

int number_of_idle_cell = 0; //needs to be set to zero if the cycle is done!!
bool cycle_done = false;
std::vector<int> utilization_per_cycle;
//std::vector<__int8> assigned_weight; //values only used in analysis! (same as the weights in MMU), expected size: MATRIX_SIZE x MATRIX_SIZE
//std::vector<__int8> transposed_weight(MATRIX_SIZE * MATRIX_SIZE);

//[���߻���] ���� mac�� ���������� �̷��� �������� Ȱ���� �����Ǹ� (dataflow�� �ϴ� ��� X), utilize �ȰŶ�� �� �� �ִ�.
void Analysis_incrementIdleRate(int cnt) {
	number_of_idle_cell = cnt;
}

void Analysis_sendCycleDone() {
	utilization_per_cycle.push_back((MATRIX_SIZE * MATRIX_SIZE) - number_of_idle_cell);
	number_of_idle_cell = 0;
}

//���⼭ ������ FC layer ���, �� ��� ����� ������ ������ systolic array result�� ���ϴ� ������ ���ļ� true �Ǵ� false�� ����Ѵ�.
//bool compareResult(int weight_row_len, int weight_col_len, int input_row_len, int input_col_len) {
//	std::vector<__int32> ground_truth(weight_row_len * input_col_len);
//
//	//weight matrix: input_row_len x input_row_len
//	//input matrix: input_row_len x input_col_len
//	
//	//Step1: weight transpose
//	//weight_row_len = weight_col_len
//	for (int i = 0; i < weight_row_len * weight_col_len; i++) {
//		int a = i / weight_col_len;
//		int b = i % weight_col_len;
//		transposed_weight[i] = node_weight[weight_row_len * b + a];
//	}
//
//	//[���� ���� ������!!]
//	//Step2: matrix multiplication (generating ground truth) .. assigned_weight is already transposed
//	/*for (int i = 0; i < MATRIX_SIZE; i++) {
//		for (int j = 0; j < _ipt_size; j++) {
//			for (int k = 0; k < MATRIX_SIZE; k++) {
//				ground_truth[i * _ipt_size + j] += transposed_weight[i * MATRIX_SIZE + k] * node_input[k * _ipt_size + j];
//			}
//		}
//	}*/
//
//	//int aa = ground_truth[0] + Accumulator[0][0];
//
//	////Step3: Compare with ground truth and the accumulator
//	//for (int i = 0; i < MATRIX_SIZE; i++) {
//	//	for (int j = 0; j < _ipt_size; j++) {
//	//		if (ground_truth[_ipt_size * i + j] != Accumulator[i][j])
//	//			return false;
//	//	}
//	//}
//
//	return true;
//}

void runAnalysis(int num_of_layer, int weight_row_len, int weight_col_len, int input_row_len, int input_col_len, int _cycle) {
	long double utilization_rate = 0;
	/*
	[���߻���] �� �Լ��� �ùķ��̼� ����� �м��ϰ� ������ִ� ����� �Ѵ�.
	1. �� ����Ŭ �ҿ� �ð�
	2. �� cell�� utilization rate�� ���
	3. ���� ��� �˰���� systolic array�� ������ ���� ��ġ�ϴ��� ���ؼ� true false ���
	*/

	//overflow issue exists
	long double denom = (long double)_cycle;
	long double num = (long double)(std::accumulate(utilization_per_cycle.begin(), utilization_per_cycle.end(), 0));
	utilization_rate = (num / denom);
	utilization_rate /= MATRIX_SIZE;
	utilization_rate /= MATRIX_SIZE;
	utilization_rate *= 100;
	//utilization_rate = ((long double)(std::accumulate(utilization_per_cycle.begin(), utilization_per_cycle.end(), 0)) / (_cycle * MATRIX_SIZE * MATRIX_SIZE)) * 100;

	std::cout << std::endl; std::cout << std::endl;
	std::cout << "------------- Layer " << num_of_layer << " simulation result -------------" << std::endl;
	std::cout << "0. Input size: " << input_row_len << "x" << input_col_len <<std::endl;
	std::cout << "1. Total cycle time: " << _cycle << " cycles" << std::endl;
	std::cout << "2. Average utilization rate of cells: " << utilization_rate << "%" << std::endl;

	/*if (compareResult(weight_row_len, weight_col_len, input_row_len, input_col_len)) {
		std::cout << "3. Result Comparision: Systolic Array result is SAME as the ground truth." << std::endl; }
	else { std::cout << "3. Result Comparision: Systolic Array result is NOT SAME as the ground truth." << std::endl; }*/
}
