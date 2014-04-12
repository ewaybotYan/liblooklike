/**
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @file   test_sort.cpp
 * @date   06/04/2014
 */


#include <exception.h>
#include <simplematrix.hpp>
#include <sort.hpp>


#include<iostream>
#include<vector>
#include<cmath>

using namespace std;

vector<float> data = {
  9, 9, 4, 5,
  9, 4, 9, 2,
  9, 4, 1, 6,
  1, 1, 2, 4,
  2, 3, 8, 7
};


int main ( int argc, char* argv[] ) {

  string path ( argv[argc-1] );

  try {
    cout << "starting sort test\n";

    typedef SimpleMatrix<float> Matrix;

    shared_ptr<Matrix> mp( new Matrix(data, 5, 4) );
    InertiaSort<float> s1(mp, 0.8, true, 0, false);
    InertiaSort<float> s2(mp, 1);
    InertiaSort<float> s3(mp, 0.95, false, 1, true );

    s1.getSorted()->evaluate();
    s2.getSorted()->evaluate();
    s3.getSorted()->evaluate();

    s1.getSorted()->waitEndOfEvaluation();
    s2.getSorted()->waitEndOfEvaluation();
    s3.getSorted()->waitEndOfEvaluation();

    s1.getSorted()->print();
    cout << "\n";
    s2.getSorted()->print();
    cout << "\n";
    s3.getSorted()->print();
    cout << "\n";
    s3.getSortIdx()->print();

    cout << "end of the evaluations";
  } catch ( Error& err ) {
    err.printMsg();
    return -1;
  }
  return 0;
}
