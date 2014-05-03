#include <iostream>
#include <fstream>
#include<CL/cl.hpp>
#include<memory>
#include<cmath>
#include "../include/simplematrix.hpp"
#include "../include/matrixImportExport.h"

using namespace std;

int main(){

	const int n_rows = 6;
	const int n_columns = 5;
	
	vector<cl_float> data = {
  	-50.972997,  7.267182,-80.362807, 33.435276, 89.826729, 85.247810,
   	52.026942,-70.416841, 41.437409,-86.200249, 93.689581, 90.498121,
   	58.237788, 62.407472,-89.736342, 91.276346, 32.108500,-98.458315,
  	-56.182364, 43.596687,-80.273997,-90.728443,-81.887168, 26.229231,
   	89.249611, 34.373032,-24.401737, 63.499165, 15.006422, 47.089691
	};
	
	SimpleMatrix<cl_float> matrix = SimpleMatrix<cl_float>(data, n_rows, n_columns);
	
	save(matrix, "testMatrixBackup.txt");
	cout << "The matrix shoud have been saved." << endl;
	
	SimpleMatrix<cl_float>* testMatrix = load("testMatrixBackup.txt");
	
	bool isEqual = 1;
	int i = 0, j = 0;
	
	isEqual = ((testMatrix->getHeight() == (int) (matrix.getHeight()) ? 1 : 0) * (testMatrix->getWidth() == (int) (matrix.getWidth()) ? 1 : 0));
	if (isEqual != 1){
		cout << "The matrix created have not the right size! "<< testMatrix->getHeight() << "x" << testMatrix->getWidth() << endl;
		return -1;
	}else
		cout << "All the values were loaded in a Matrix." << endl;
	
	while ((j < (int) matrix.getWidth()) && (isEqual == 1)){
		while ((i < (int) matrix.getHeight()) && (isEqual == 1)){
			isEqual = ( abs(testMatrix->at(i,j) - matrix.at(i,j)) < 0.000001 ? 1 : 0);
			i++;
		}
	j++;
	}
	testMatrix->print();

	cout << endl;

	matrix.print();
	
	if (isEqual == 1)
		cout << "The saving and loading of a simple matrix are a success." << endl;
	else{
		cout << "We encountered a problem : the values loaded are not those saved..." << endl;
		return -1;
	}
	
	return 0;
}
