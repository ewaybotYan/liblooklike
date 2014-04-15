/**
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @file   test_eigenpair.cpp
 * @date   14/04/2014
 */

#include<vector>
#include<memory>
#include<simplematrix.hpp>
#include<eigenpair.hpp>

using namespace std;
typedef SimpleMatrix<float> Matrix;

vector<cl_float> data = {
  2, 0, 0,
  0, 3, 4,
  0, 4, 9
};

int main()
{
   shared_ptr<Matrix> m1 ( new Matrix( data, 3, 3 ) );
   EigenPair eigenpair( m1 );
   Matrix* eigenVectors = eigenpair.getVectors().get();
   Matrix* eigenValues = eigenpair.getValues().get();
   eigenVectors->evaluate();
   eigenVectors->waitEndOfEvaluation();
   eigenVectors->print();
   eigenValues->print();
   return 0;
}
