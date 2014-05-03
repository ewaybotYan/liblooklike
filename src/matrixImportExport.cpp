#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <memory>
#include <CL/cl.hpp>
#include "../include/simplematrix.hpp"
#include "../include/exception.h"

#define u_int unsigned int

using namespace std;

int save(SimpleMatrix<cl_float> mat, string filepath){
	
	ofstream f(filepath.c_str());
	if (!f){
		throw IOException("Impossible to create the file.");
	}
	
	int columnNumber = mat.getWidth();
	int rowNumber = mat.getHeight();
	
	for (u_int i = 0; i < rowNumber; i++){
		for (u_int j = 0; j < columnNumber; j++){
			f << scientific << setprecision(8) << mat.at(i,j);

			if (j < (columnNumber - 1))
				f << ",";
		}
		f << endl;
	}
	return 0;
}

SimpleMatrix<cl_float> load(string filepath){
	
	ifstream f(filepath.c_str());
	if (!f) throw IOException ("Impossible to read the file.");
	// actually, float should be cl_float
	shared_ptr<vector<float>> data = make_shared<vector<float>>();

	u_int rowNumber = 0;
	u_int actualNbRowsByLine = 0;
	u_int columnNumber = 0;

	string line;
	string cell;
	while (getline(f, line)){
		cl_float tmp;
		stringstream lineStream(line);
		while (getline(lineStream, cell, ',')){
			stringstream value(cell);
			tmp << value;
			//tmp = stof(cell);
			data -> push_back(tmp);
			if (columnNumber < 1)
				rowNumber++;
		}
		columnNumber++;
	}

	if (!f.eof()){
		string s;
		f >> s;
		if (s != "")
			throw IOException ("Corrupted file : some values of the matrix could not be loaded.");
	}
	SimpleMatrix<cl_float> mat = SimpleMatrix<cl_float>(data, rowNumber, columnNumber);
	return mat;
	
}
