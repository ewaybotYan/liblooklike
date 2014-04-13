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

const int aw = 20;
const int ah = 5;
vector<cl_float> a = {
    0.859555,-0.03640075,0.6000662,-0.5373392,-0.3938806,
    0.6738222,0.7656746,-0.04991303,0.02807761,-0.4494087,
    0.2088377,-0.443116,0.7175984,0.3928747,0.6738882,
    0.9615358,-0.584719,-0.8826968,-0.6712232,0.3820503,
    -0.7648916,0.8049297,-0.9832899,0.5130607,0.6799698,
    0.04960564,-0.7604051,0.7494427,-0.4499969,0.3272896,
    -0.5214519,-0.6111996,-0.2553094,0.02527038,0.9416582,
    -0.1265489,-0.2117826,0.1951463,-0.1326128,-0.4972729,
    0.4347242,-0.8319752,-0.7967115,0.9188513,-0.5115155,
    0.9872034,-0.08862734,-0.526807,-0.8525717,0.4018377,
    0.3042503,-0.3617497,-0.4929073,-0.5827707,0.3516664,
    0.03433609,0.8897263,-0.8111832,-0.4255632,-0.5449469,
    0.7135514,0.1829167,0.4542794,0.1631866,-0.8150671,
    0.627327,0.3003503,-0.5743717,-0.04975398,0.5319428,
    0.4314376,-0.749185,-0.3976288,0.5695818,0.3604919,
    0.5780151,-0.1797865,0.1222054,0.5170793,-0.1334166,
    -0.2427173,0.5936625,-0.2806849,-0.5019634,0.008278464,
    -0.2818211,-0.537629,0.7154435,0.3202205,-0.9462456,
    0.3479562,-0.9487649,-0.936046,-0.4562833,-0.6306744,
    -0.408071,0.4703855,-0.2602751,0.8684106,-0.686391
};

const int bw = 3;
const int bh = 20;
vector<cl_float> b = {
    -0.8109678,0.4771343,-0.1241322,-0.4850215,0.3545739,
    -0.1954033,-0.723628,0.3272067,0.2350641,0.995608,
    0.6095526,0.547823,0.321548,-0.1527461,-0.1135489,
    0.3035452,-0.1496363,0.9774102,-0.7570364,0.6717283,
    -0.4070171,-0.6013085,0.1414013,-0.157462,0.1706753,
    -0.7331206,0.574413,0.7305363,0.6330202,-0.5820638,
    0.2125267,-0.315942,0.6787913,0.6091844,-0.5477502,
    0.3490736,-0.6451745,0.5556291,-0.949415,0.8641615,
    0.5162845,-0.342827,0.1364762,-0.4772968,-0.3141688,
    -0.4461043,-0.832453,0.8003385,-0.7543328,0.5924721,
    -0.9466763,0.7425793,-0.5374306,-0.5069781,-0.3524633,
    0.2532249,0.3560301,-0.7638707,-0.1731585,0.7326497
};

const int cw = 3;
const int ch = 5;

vector<cl_float> c = {
    -3.39954e+00, -2.43957e-02, 1.19527e+00,
    2.59287e+00, -2.18775e-01, -1.33932e+00,
    1.19353e+00, 1.16815e+00, 5.26290e-01,
    -8.57741e-02, 8.53603e-01, 1.16509e+00,
    -1.65876e+00, -3.03817e-01, 4.52261e-01
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
