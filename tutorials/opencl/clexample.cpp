/**
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @file   clexample.cpp
 * @date   02/05/2014
 */

#include<string>
#include<vector>
#include<memory>
#include<CL/cl.hpp>// for  cl_float
#include<context.h>
#include<simplematrix.hpp>
#include<clmatrix.hpp>
#include<clmatrixoperations.hpp>


using namespace std;


void usage();

const int aw = 3;
const int ah = 3;
vector<cl_float> a = {
  1, 4, 7,
  2, 5, 8,
  3, 6, 9
};

const int bw = 1;
const int bh = 3;
vector<cl_float> b = {
  1, 2, 3
};

void usage() {
  cerr << "Error in " << __FILE__ << " :\n"
       << "usage :\n clexample path_to_kernel_directory"
       << endl;
}

int main ( int argc, char* argv[] ) {
  if ( argc < 2 ) {
    usage();
    return -1;
  }

  try {
    // get the path to the OpenCL kernels
    string kernelsPath ( argv[argc-1] );

    // initialize a context object, it will choose the first OpenCL
    // platform it finds
    Context ctx ( kernelsPath );
    // create a command queue for the context
    cl::CommandQueue queue = ctx.createQueue();

    // just a shortcut
    typedef SimpleMatrix<cl_float> Matrix;

    // load the matrix locally
    shared_ptr<Matrix> A ( new Matrix( a, ah, aw ) );
    shared_ptr<Matrix> B ( new Matrix( b, bh, bw ) );

    // pushing the matrix in the OpenCL memory is done with an algorithm
    // that takes a SimpleMatrix<cl_float> in input
    CLMatrixLoader Al( A, &ctx, &queue );
    CLMatrixLoader Bl( B, &ctx, &queue );
    CLMatrixProduct C( Al.getResult(), Bl.getResult(), &ctx, &queue);

    // similary, we can unload a CLMatrix to the local memory
    CLMatrixUnloader R( C.getResult(), &ctx, &queue);

    // then we ask for the evaluation of the final expression
    R.getResult()->evaluate();
    // wait for the end of the evaluation
    R.getResult()->waitEndOfEvaluation();
    // and print the result
    R.getResult()->print();

  } catch ( Error& err ) {
    err.printMsg();
    return -1;
  }
  return 0;
}
