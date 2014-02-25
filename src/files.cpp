#include <iostream>
#include <iomanip>
#include <fstream>
#include <armadillo>
#include "../include/matrix.h"
#include "../include/exception.h"

#define u_int unsigned int

using namespace std;
using namespace arma;

int save(fmat mat, string filepath){
	
	ofstream f(filepath.c_str());
	if (!f){
	cerr << "Impossible to open the file.";
	return -1;
	}
	
	f << "#Number of rows (matrix height) = " << mat.n_rows << endl
	<< "#Number of columns (matrix width) = " << mat.n_cols << endl;
	
		for (u_int i=0; i < mat.n_rows; i++){
			for (u_int j=0; j < mat.n_cols; j++){
				f << scientific << setprecision(15) << mat(i,j);
				f << " ";
			}
		f << endl;
		}
		
	return 0;
}

Matrix load(string filepath){
	
	ifstream f(filepath.c_str());
	if (!f) throw Error ("Impossible to read the file.");
	
	int n_rows, n_cols;
	
	f.seekg(33,ios::beg);
	f >> n_rows;
	
	f.seekg(36,ios::cur);
	f >> n_cols;
	
	float* data = new float[n_cols * n_rows];
	
	int i=0;
	while ((!f.eof()) && (i < n_cols * n_rows)){
		f >> data[i];
		i++;
	}
	
	if (i!= n_cols * n_rows){
		throw Error ("Corrupted file : the file seems not to contain all the data.");
	}else if (!f.eof()){
		string s;
		f >> s;
		if (s != "")
			throw Error ("Corrupted file : some values of the matrix could not be loaded.");
	}
	
	Matrix mat = Matrix(data, n_rows, n_cols);
	return mat;
	
}
