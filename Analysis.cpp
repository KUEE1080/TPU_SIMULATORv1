#include "Analysis.h"

int number_of_idle_cell = 0; //needs to be set to zero if the cycle is done!!
bool cycle_done = false;
std::vector<int> utilization_per_cycle;
//std::vector<__int8> assigned_weight; //values only used in analysis! (same as the weights in MMU), expected size: MATRIX_SIZE x MATRIX_SIZE
//std::vector<__int8> transposed_weight(MATRIX_SIZE * MATRIX_SIZE);

//[개발사항] 만약 mac이 정상적으로 이렇게 곱해지는 활동이 감지되면 (dataflow만 하는 경우 X), utilize 된거라고 볼 수 있다.
void Analysis_incrementIdleRate(int cnt) {
	number_of_idle_cell = cnt;
}

void Analysis_sendCycleDone() {
	utilization_per_cycle.push_back((MATRIX_SIZE * MATRIX_SIZE) - number_of_idle_cell);
	number_of_idle_cell = 0;
}

//여기서 실제로 FC layer 계산, 즉 행렬 계산을 진행한 다음에 systolic array result랑 비교하는 과정을 거쳐서 true 또는 false를 출력한다.
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
//	//[현재 개발 진행중!!]
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
	[개발사항] 이 함수는 시뮬레이션 결과를 분석하고 출력해주는 기능을 한다.
	1. 총 사이클 소요 시간
	2. 각 cell들 utilization rate의 평균
	3. 실제 행렬 알고리즘과 systolic array로 구현한 값이 일치하는지 비교해서 true false 출력
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
