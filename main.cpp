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
#include <string.h>
#include <inttypes.h>
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

int number_of_layers;
std::vector<int> NN_input_row_size;
std::vector<int> NN_input_col_size;
std::vector<int> NN_weight_row_size;
std::vector<int> NN_weight_col_size;

int type_of_NN; // FC or CONV

//only used for CONV layer
int filter_size;


int main(void) {
	int cycle = 1;

	std::cout<<std::endl; std::cout << std::endl; std::cout << std::endl;
	std::cout << "------------- < TENSOR PROCESSING UNIT SIMULATOR v1> -------------" << std::endl;
	std::cout << "Author : Lee Dong Jae" << std::endl;
	std::cout << "Description : TPU simulation focused on the systolic array, assuming the memory conditions are ideal." << std::endl;
	std::cout << "Input : Given Deep Neural Network" << std::endl;
	std::cout << "Output:" << std::endl;
	std::cout << "1. shows the input, weights, and the output of the matrix" << std::endl;
	std::cout << "2. the utilization of the processing elements inside the matrix multiply unit" << std::endl;
	std::cout << "----------------------------------------------------" << std::endl;
	std::cout << std::endl; std::cout << std::endl; std::cout << std::endl;

	//initialize initial values and the settings of the TPU

	std::cout << "Number of layers" << std::endl;
	std::cin >> number_of_layers;

	for (int i = 0; i < number_of_layers; i++) {
		std::cout << std::endl; std::cout << std::endl;
		std::cout << "------------- Layer number " << i << " -------------" << std::endl;

		//FC
		std::cout << "The size of the Weight matrix (row col)." << std::endl;
		std::cin >> weight_row_size >> weight_col_size;

		std::cout << "The size of the Input matrix (row col)." << std::endl;
		std::cin >> input_row_size >> input_col_size;

		NN_input_row_size.push_back(input_row_size);
		NN_input_col_size.push_back(input_col_size);

		NN_weight_row_size.push_back(weight_row_size);
		NN_weight_col_size.push_back(weight_col_size);
	}

	for (int i = 0; i < number_of_layers; i++) {
		//type_of_NN = FULLY_CONNECTED;

		//input_row_size = MATRIX_SIZE;
		//input_col_size = MATRIX_SIZE;
		////weight�� ��� ���� ������ ���ƾ� �Ѵ�!!! ISA�� ����~ only focusing on dense matrix
		//weight_row_size = MATRIX_SIZE;
		//weight_col_size = MATRIX_SIZE;

		//code operation order: memory related initialization done first, and then operate the MMU initialization

		/*
		[Matrix Multiplication Operation Mode]
		1. MATRIX_MATRIX_MULT => Input: Matrix, Weight: Matrix
		2. Convolve
		*/

		//Step1: Input data & weight initialization
		UnifiedBuffer_fetchInput(NN_input_row_size[i], NN_input_col_size[i]); // input setup
		UnifiedBuffer_tileInput(NN_input_row_size[i], NN_input_col_size[i]);

		WeightFIFO_fetchWeight(NN_weight_row_size[i], NN_weight_col_size[i]);
		WeightFIFO_tileWeight(NN_weight_row_size[i], NN_weight_col_size[i]);

		//Step2: Interconnecting the MMU
		MMU_initialize();

		Control_setTiledInput(0);
		Control_setTiledWeight(0);

		while (MMU_run(NN_input_row_size[i], NN_input_col_size[i], NN_weight_row_size[i], NN_weight_col_size[i]) == IN_PROGRESS) {
			Control_run2(NN_input_row_size[i], NN_input_col_size[i], NN_weight_row_size[i], NN_weight_col_size[i]);
			cycle++;
		}
		runAnalysis(i, NN_weight_row_size[i], NN_weight_col_size[i], NN_input_row_size[i], NN_input_col_size[i], cycle); //displaying the result of the TPU simulation

		//resetting the states to calculate the next layer
		ibuf_index = 0;
		cycle = 1;
		Cells.clear();
		node_input.clear();
		node_weight.clear();
		tiled_input.clear();
		tiled_weight.clear();
		utilization_per_cycle.clear();
		MMU_reset();
		Analysis_incrementIdleRate(0);

		for (int i = 0; i < MATRIX_SIZE; i++) { memset(UnifiedBuffer[i], 0, sizeof(int8_t) * UNIFIED_BUFFER_LENGTH); }
		for (int i = 0; i < ACCUMULATOR_SIZE; i++) { memset(Accumulator[i], 0, sizeof(int32_t) * MATRIX_SIZE); }
		memset(accumulator_index, 0, sizeof(int) * ACCUMULATOR_SIZE);
	}

	
	
	return 0;
}
