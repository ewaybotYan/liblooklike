/**
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @file   test_clmatrixoperations.cpp
 * @date   13/04/2014
 */

#include<iostream>
#include<string>
#include<vector>
#include<memory>
#include<cmath>
#include<CL/cl.hpp>
#include<context.h>
#include<simplematrix.hpp>
#include<clmatrix.hpp>
#include<clmatrixoperations.hpp>


using namespace std;


void usage();


cl_float id[9] = {
    1,0,0,
    0,1,0,
    0,0,1
};

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

const int cw = 1;
const int ch = 3;

vector<cl_float> c = {
  14, 32, 50
};

void usage() {
    cerr << "Error in " << __FILE__ << " :\n"
         << "usage :\n test_matrix path_to_kernel_directory"
         << endl;
}

int main ( int argc, char* argv[] ) {
    if ( argc < 2 ) {
        usage();
        return -1;
    }

    try {

        string kernelsPath ( argv[argc-1] );
        Context ctx ( kernelsPath );
        cl::CommandQueue queue = ctx.createQueue();

        typedef SimpleMatrix<cl_float> Matrix;

        // test multiplication
        cout << "testing multiplication\n";
        shared_ptr<Matrix> A ( new Matrix( a, ah, aw ) );
        shared_ptr<Matrix> B ( new Matrix( b, bh, bw ) );
        CLMatrixLoader Al( A, &ctx, &queue );
        CLMatrixLoader Bl( B, &ctx, &queue );
        CLMatrixProduct C( Al.getResult(), Bl.getResult(), &ctx, &queue );
        CLMatrixUnloader R( C.getResult(), &ctx, &queue);
        R.getResult()->evaluate();
        R.getResult()->waitEndOfEvaluation();
        cl_float* res2 = R.getResult()->getValues();

        cl_float error = 0;
        for( int k = 0; k < ch*cw; k++ ){
            error = max( error, abs( res2[k] - c[k] ) );
        }
        if ( error < 0.0001 ) {
            std::cout << "computed without noticeable error\n";
        } else {
            std::cerr << "computed result matches with error!: " << error << "\n";
            R.getResult()->print();
            return -1;
        }

    } catch ( Error& err ) {
        err.printMsg();
        return -1;
    }
    return 0;
}
