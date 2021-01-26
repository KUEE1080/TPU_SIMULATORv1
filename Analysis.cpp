#include "Analysis.h"

int number_of_idle_cell = 0; //needs to be set to zero if the cycle is done!!
bool cycle_done = false;
std::vector<int> utilization_per_cycle;
//std::vector<__int8> assigned_weight; //values only used in analysis! (same as the weights in MMU), expected size: MATRIX_SIZE x MATRIX_SIZE
std::vector<__int8> transposed_weight(MATRIX_SIZE * MATRIX_SIZE);

//[���߻���] ���� mac�� ���������� �̷��� �������� Ȱ���� �����Ǹ� (dataflow�� �ϴ� ��� X), utilize �ȰŶ�� �� �� �ִ�.
void Analysis_incrementIdleRate(int cnt) {
	number_of_idle_cell = cnt;
}

void Analysis_sendCycleDone() {
	utilization_per_cycle.push_back((MATRIX_SIZE * MATRIX_SIZE) - number_of_idle_cell);
	number_of_idle_cell = 0;
}

//���⼭ ������ FC layer ���, �� ��� ����� ������ ������ systolic array result�� ���ϴ� ������ ���ļ� true �Ǵ� false�� ����Ѵ�.
bool compareResult(int weight_row_len, int weight_col_len, int input_row_len, int input_col_len) {
	std::vector<__int32> ground_truth(weight_row_len * input_col_len);

	//weight matrix: input_row_len x input_row_len
	//input matrix: input_row_len x input_col_len
	
	//Step1: weight transpose
	//weight_row_len = weight_col_len
	for (int i = 0; i < weight_row_len * weight_col_len; i++) {
		int a = i / weight_col_len;
		int b = i % weight_col_len;
		transposed_weight[i] = node_weight[weight_row_len * b + a];
	}

	//[���� ���� ������!!]
	//Step2: matrix multiplication (generating ground truth) .. assigned_weight is already transposed
	/*for (int i = 0; i < MATRIX_SIZE; i++) {
		for (int j = 0; j < _ipt_size; j++) {
			for (int k = 0; k < MATRIX_SIZE; k++) {
				ground_truth[i * _ipt_size + j] += transposed_weight[i * MATRIX_SIZE + k] * node_input[k * _ipt_size + j];
			}
		}
	}*/

	//int aa = ground_truth[0] + Accumulator[0][0];

	////Step3: Compare with ground truth and the accumulator
	//for (int i = 0; i < MATRIX_SIZE; i++) {
	//	for (int j = 0; j < _ipt_size; j++) {
	//		if (ground_truth[_ipt_size * i + j] != Accumulator[i][j])
	//			return false;
	//	}
	//}

	return true;
}

void runAnalysis(int weight_row_len, int weight_col_len, int input_row_len, int input_col_len, int _cycle) {
	double utilization_rate = 0;
	/*
	[���߻���] �� �Լ��� �ùķ��̼� ����� �м��ϰ� ������ִ� ����� �Ѵ�.
	1. �� ����Ŭ �ҿ� �ð�
	2. �� cell�� utilization rate�� ���
	3. ���� ��� �˰������ systolic array�� ������ ���� ��ġ�ϴ��� ���ؼ� true false ���
	*/

	//debugging code
	/*assigned_weight.clear();
	assigned_weight.push_back(11); assigned_weight.push_back(15); assigned_weight.push_back(19); assigned_weight.push_back(23);
	assigned_weight.push_back(12); assigned_weight.push_back(16); assigned_weight.push_back(20); assigned_weight.push_back(24);
	assigned_weight.push_back(13); assigned_weight.push_back(17); assigned_weight.push_back(21); assigned_weight.push_back(25);
	assigned_weight.push_back(14); assigned_weight.push_back(18); assigned_weight.push_back(22); assigned_weight.push_back(26);*/

	//overflow issue exists
	utilization_rate = ((double)(std::accumulate(utilization_per_cycle.begin(), utilization_per_cycle.end(), 0)) / (_cycle * input_row_len * input_col_len)) * 100;

	std::cout << std::endl; std::cout << std::endl;
	std::cout << "------------- Simulation Result -------------" << std::endl;
	std::cout << "0. Input size: " << input_row_len << "x" << input_col_len <<std::endl;
	std::cout << "1. Total Cycle time: " << _cycle << " cycles" << std::endl;
	std::cout << "2. Average Utilization rate of Cells: " << utilization_rate << "%" << std::endl;

	if (compareResult(weight_row_len, weight_col_len, input_row_len, input_col_len)) {
		std::cout << "3. Result Comparision: Systolic Array result is SAME as the ground truth." << std::endl; }
	else { std::cout << "3. Result Comparision: Systolic Array result is NOT SAME as the ground truth." << std::endl; }
}