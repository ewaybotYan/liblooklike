#include <iostream>
#include <fstream>
#include <armadillo>
#include "../include/matrix.h"
#include "../include/files.hpp"

using namespace std;
using namespace arma;

int main(){
	
	const int n_rows = 77;
	const int n_cols = 77;
	
	arma_rng::set_seed_random();
	fmat matrix = randn<fmat>(n_rows,n_cols);
	
	save(matrix, "testArmadilloMatrixBackup.txt");
	cout << "Armadillo's matrix shoud have been saved." << endl;
	
	Matrix testMatrix = load("testArmadilloMatrixBackup.txt");
	float* array = testMatrix.getValues();
	
	bool isEqual = 1;
	int i = 0, j = 0;
	
	isEqual = (((testMatrix.getHeight() * testMatrix.getWidth()) == (int) (matrix.n_rows * matrix.n_cols)) ? 1 : 0);
	
	if (isEqual != 1)
		cout << "The matrix created have not the right size!" << endl;
	else cout << "All the values were loaded in a Matrix." << endl;
	
	while ((i < (int) matrix.n_rows) && (isEqual == 1)){
		while ((j < (int) matrix.n_cols) && (isEqual == 1)){
			isEqual = ((array[j+i*n_cols] == matrix(i,j)) ? 1 : 0);
			j++;
		}
	i++;
	}
	
	if (isEqual == 1)
		cout << "The saving and loading of an armadillo matrix are a success." << endl;
	else cout << "We encountered a problem : the values loaded are not those saved..." << endl;
	
	return 0;
}
