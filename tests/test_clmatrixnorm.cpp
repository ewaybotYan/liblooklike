/**
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @file   test_matrixnorm.cpp
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
#include<clmatrixnorm.hpp>


using namespace std;


void usage();


const int m = 6;
const int n = 5;
vector<cl_float> a = {
  -50.972997,  7.267182,-80.362807, 33.435276, 89.826729, 85.247810,
   52.026942,-70.416841, 41.437409,-86.200249, 93.689581, 90.498121,
   58.237788, 62.407472,-89.736342, 91.276346, 32.108500,-98.458315,
  -56.182364, 43.596687,-80.273997,-90.728443,-81.887168, 26.229231,
   89.249611, 34.373032,-24.401737, 63.499165, 15.006422, 47.089691
};

vector<cl_float> b =
{
  -0.93161954,-0.09748298,-1.35255085, 0.27730579, 1.08496424, 1.01938334,
   0.40235823,-1.14424725, 0.26860027,-1.34360980, 0.92860517, 0.88829337,
   0.59447376, 0.64513129,-1.20326502, 0.99585945, 0.27702885,-1.30922833,
  -0.27497795, 1.40744797,-0.68119936,-0.85747716,-0.70839987, 1.11460635,
   1.31154401,-0.07842712,-1.56713566, 0.65930990,-0.56896478, 0.24367366
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

        // test norm
        cout << "testing normalization\n";
        shared_ptr<Matrix> A ( new Matrix( a, m, n ) );
        CLMatrixLoader Al( A, &ctx, &queue );
        CLMatrixNorm C( Al.getResult(), &ctx, &queue );
        CLMatrixUnloader R( C.getNormalizedMatrix(), &ctx, &queue);
        R.getResult()->evaluate();
        R.getResult()->waitEndOfEvaluation();
        cl_float* res2 = R.getResult()->getValues();

        cl_float error = 0;
        for( int k = 0; k < m*n; k++ ){
            error = max( error, abs( res2[k] - b[k] ) );
        }
        if ( error < 0.0001 ) {
            std::cerr << "computed without noticeable error\n";
        } else {
            std::cout << "computed result matches with error!: " << error << "\n";
            R.getResult()->print();
            return -1;
        }

    } catch ( Error& err ) {
        err.printMsg();
        return -1;
    }
    return 0;
}
