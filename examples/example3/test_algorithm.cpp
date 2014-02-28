/**
 * @file   test_algorithm.cpp
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @brief  unit tests for Algorithm class
 * @detailled This file along with real.h and real.cpp contain an example
 * implementation of the @ref Algorithm class and how to use it.
 *
 * @page test_algorithm Algorithm Example
 * @section program Example program
 *  @include test_algorithm.cpp
 * @section header Example Algorithm children
 *  @include real.h
 * @section definition Example Algorithm implementation
 *  @include real.cpp
 */

#include<CL/cl.hpp>
#include<context.h>
#include <error.h>
#include <algorithm.h>
#include "real.h"

#include<string>
#include<iostream>
#include<cmath>

using namespace std;

void usage(){
    cout << "usage: test_algorithm3 pathToKernel" << endl;
}

int main( int argc, char* argv[] ){
    if( argc<2 ){
        usage();
    }

    try{
        string path(argv[argc-1]);
        Context context(path);
        cl::CommandQueue queue = context.createQueue();

        float aValue = 1.0;
        float bValue = 2.0;

        // Some Mathexpression objects are directly created from values, they will
        // be terminal nodes in the evaluation tree. The enqueue() function on these
        // will likely consist in ordering a writeBuffer operation.
        Real a = Real(aValue, &context, &queue);
        Real b = Real(bValue, &context, &queue);

        // Some Mathexpression objects have no initially known value, they are
        // instead computed with other values called children and stored in
        // m_children
        RealSum sum = RealSum( a, b, &context, &queue );

        // Start the evaluation of this expression, it will recursively call the
        // evaluation of the children
        sum.evaluate();

        // Wait for the end of the evaluation and retrieve the data from OpenCL
        // buffers
        sum.waitEndOfEvaluation();

        // Then we use an implementation specific method to read the value of the
        // computed expression
        float s = sum.getResult();

        // The rest of this program is used for testing.
        if( std::abs( s - 3.0 ) < 0.0000001 ){
            std::cout << "computed result matches with error : " << s - 3.0 << "\n";
            return 0;
        }else{
            std::cerr << "computed result matches with error : " << s - 3.0 << "\n";
            return -1;
        }
    }catch( Error& e ){
        e.printMsg();
        return -1;
    }
}

