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

typedef unsigned int u_int;

using namespace std;

int save(SimpleMatrix<cl_float> mat, string filepath){
	
	ofstream f(filepath.c_str());
	if (!f){
		throw IOException("Impossible to create the file.");
	}
	
	int columnNumber = mat.getWidth();
	int rowNumber = mat.getHeight();

	for (int j = 0; j < columnNumber; j++){	
		for (int i = 0; i < rowNumber; i++){
			f << scientific << setprecision(8) << mat.at(i,j);

			if (i < (rowNumber - 1))
				f << ",";
		}
		f << endl;
	}
	return 0;
}

int save(SimpleMatrix<cl_float> mat, string filepath,
                   SimpleMatrix<unsigned int>indexes)
{
  ofstream f(filepath.c_str());
  if (!f){
    throw IOException("Impossible to create the file.");
  }

  int rowNumber = mat.getHeight();

  for ( int idx=0; idx< indexes.getHeight(); idx++ ){
    int j = indexes.getValues()[idx];
    for (int i = 0; i < rowNumber; i++){
      f << scientific << setprecision(8) << mat.at(i,j);

      if (i < (rowNumber - 1))
        f << ",";
    }
    f << endl;
  }
  return 0;
}

SimpleMatrix<cl_float>* load(string filepath){
	
	ifstream f(filepath.c_str());
	if (!f) throw IOException ("Impossible to read the file.");
	shared_ptr< vector<cl_float> > data = make_shared< vector<cl_float> >();

	u_int rowNumber = 0;
	u_int columnNumber = 0;

	string line;
	string cell;
	while (getline(f, line)){
		cl_float tmp;
		stringstream lineStream(line);
		while (getline(lineStream, cell, ',')){
			stringstream value(cell);
			value >> tmp;
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
	SimpleMatrix<cl_float>* mat = 
		new SimpleMatrix<cl_float>(data, rowNumber, columnNumber);
	return mat;	
}
