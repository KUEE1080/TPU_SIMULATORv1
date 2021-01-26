// Title: TPU Simulator v1
// Author : Lee Dong Jae
// Description : TPU simulation focused on the systolic array, assuming the memory conditions are ideal.
// Input : Given Deep Neural Network
//
// Output:
// 1. shows the input, weights, and the output of the matrix
// 2. the utilization of the processing elements inside the matrix multiply unit

//############# Code segment #################

#include <iostream> 
#include "MatrixMultiplyUnit.h"
#include "UnifiedBuffer.h"
#include "WeightFIFO.h"
#include "Activation.h"
#include "Control.h"

#define FULLY_CONNECTED 1
#define CONV 2

int input_size;
int input_row_size;
int input_col_size;
int weight_row_size;
int weight_col_size;

int type_of_NN; // FC or CONV

//only used for CONV layer
int filter_size;


int main(void) {
	int cycle = 1;

	std::cout<<std::endl; std::cout << std::endl; std::cout << std::endl;
	std::cout << "■■■■■■■■ < TENSOR PROCESSING UNIT SIMULATOR v1> ■■■■■■■■" << std::endl;
	std::cout << "Author : Lee Dong Jae" << std::endl;
	std::cout << "Description : TPU simulation focused on the systolic array, assuming the memory conditions are ideal." << std::endl;
	std::cout << "Input : clock speed & input matrix size (the values will be all randomized)" << std::endl;
	std::cout << "Output:" << std::endl;
	std::cout << "1. shows the input, weights, and the output of the matrix" << std::endl;
	std::cout << "2. the utilization of the processing elements inside the matrix multiply unit" << std::endl;
	std::cout << "■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■" << std::endl;
	std::cout << std::endl; std::cout << std::endl; std::cout << std::endl;

	//initialize initial values and the settings of the TPU

	//std::cout << "The type of the Neural Network (1: FC, 2:CONV)." << std::endl;
	//std::cin >> type_of_NN;
	//
	//if (type_of_NN == FULLY_CONNECTED) {
	//	//FC
	//	// Reason of 2048x2048 size limitation: takes too long
	//	std::cout << "The size of the Input matrix (row x col). The row & size should be multiple of 256, maxsize is 2048" << std::endl;
	//	std::cin >> input_row_size >> input_col_size;
	//}
	//else if (type_of_NN == CONV){
	//	//CONV -- not implemented yet!!
	//	std::cout << "The size of the Input matrix (row x col). The row & size should be multiple of 256, maxsize is 2048" << std::endl;
	//	std::cin >> input_row_size >> input_col_size;
	//	std::cout << "The size of the filter (assuming stride is always 1)" << std::endl;
	//	std::cin >> filter_size;
	//}
	
	//DEBUGGING -- MATRIX_SIZE = 4라고 가정하고 디버깅하는게 제일 편하다.
	type_of_NN = FULLY_CONNECTED;
	input_row_size = MATRIX_SIZE;
	input_col_size = MATRIX_SIZE;
	//weight의 행과 열은 무조건 같아야 한다!!! ISA에 따라~ only focusing on dense matrix
	weight_row_size = MATRIX_SIZE;
	weight_col_size = MATRIX_SIZE;

	//code operation order: memory related initialization done first, and then operate the MMU initialization

	/*
	[Matrix Multiplication Operation Mode]
	1. MATRIX_MATRIX_MULT => Input: Matrix, Weight: Matrix
	2. Convolve
	*/

	//Step1: Input data & weight initialization
	UnifiedBuffer_fetchInput(input_row_size, input_col_size); // input setup
	UnifiedBuffer_tileInput(input_row_size, input_col_size);

	if (type_of_NN == FULLY_CONNECTED) {
		//weight_row_size = input_row_size;
		//weight_col_size = weight_row_size;
		WeightFIFO_fetchWeight(weight_row_size, MATRIX_SIZE);
		WeightFIFO_tileWeight(weight_row_size, MATRIX_SIZE);
	} // weight setup
	else if (type_of_NN == CONV) { //콘볼루션은 일단 개발 보류.
		weight_row_size = input_row_size - filter_size + 1;
		weight_col_size = input_row_size - filter_size + 1;

		WeightFIFO_fetchWeight(weight_row_size * MATRIX_SIZE, input_row_size); //im2col convolution
		WeightFIFO_tileWeight(weight_row_size, MATRIX_SIZE);
	}

	//Step2: Interconnecting the MMU
	MMU_initialize(input_row_size, input_col_size, weight_row_size, MATRIX_SIZE);

	Control_inputDataSetup(input_row_size, input_col_size);
	Control_doublebufferSetUp();
	Control_weightSetup(WEIGHT_LOAD_ALL, -1);

	int a = Accumulator[0][0];

	while (MMU_run() == IN_PROGRESS) {
		//int aaaa = ibuf_index;
		Control_run(input_row_size, input_col_size, weight_row_size, MATRIX_SIZE);
		
		//해당 레이어가 끝났는지 감지해주는 코드도 필요하다. 이건 accumulator가 다 찼는지로 확인하면 될듯
		
		cycle++;
	}
	//int d = utilization_per_cycle[0];
	//Activation_run(ReLU);
	//int d1 = activation_node[0];

	runAnalysis(weight_row_size, weight_col_size, input_row_size, input_col_size, cycle); //displaying the result of the TPU simulation
	return 0;
}
