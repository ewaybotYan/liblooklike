/**
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @file   test_clmatrix.cpp
 * @date   12/04/2014
 */

#include<iostream>
#include<vector>
#include<memory>
#include<CL/cl.hpp>
#include<context.h>
#include<simplematrix.hpp>
#include<clmatrix.hpp>

using namespace std;
typedef SimpleMatrix<float> Matrix;

vector<cl_float> data = {
  9, 9, 4, 5,
  9, 4, 9, 2,
  9, 4, 1, 6,
  1, 1, 2, 4,
  2, 3, 8, 7
};

const unsigned int w = 4;
const unsigned int h = 5;

int main()
{
  try{
    Context ctx("");
    cl::CommandQueue queue = ctx.createQueue();
    cout << "Testing CLMatrix" << endl;
    shared_ptr<Matrix> m1 ( new Matrix( data, h, w ) );
    CLMatrixLoader l(m1, &ctx, &queue );
    CLMatrixUnloader ul(l.getResult(),&ctx,&queue);
    std::shared_ptr< SimpleMatrix<cl_float> > m2 = ul.getResult();
    m2->evaluate();
    m2->waitEndOfEvaluation();
    m2->print();
  }catch( Error& e ){
    e.printMsg();
    return -1;
  }
  return 0;
}
