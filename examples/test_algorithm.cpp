/**
 * @file   test_algorithm.cpp
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @brief  unit tests for Algorithm class
 * @detailled This file along with real.h and real.cpp contain an example
 * implementation of the @ref Mathexpression class and how to use it.
 *
 * @page test_algorithm Algorithm Example
 * @section program Example program
 *  @include test_algorithm.cpp
 * @section header Example Algorithm children
 *  @include real.h
 * @section definition Example Algorithm implementation
 *  @include real.cpp
 */

#include "../include/exception.h"
#include "../include/context.h"
#include "../include/algorithm.h"
#include "real.h"

#include<iostream>
#include<cmath>

using namespace std;

void usage(){
  cerr << "Error in " << __FILE__ << " :\n"
    << "usage :\n test_algorithm path_to_kernel_directory" 
    << endl;
}

int main( int argc, char* argv[] ){
  if( argc < 2 ){
    usage();
    return -1;
  }
  std::string path(argv[argc-1]);

  try{
    // The Mathexpression requires references to the OpenCL environment,
    // namely a context from wich it will retrieve kernels, and a command queue
    // that will take the execution orders.
    Context ctx(path);
    cl::CommandQueue queue = ctx.createQueue();

    // Some Mathexpression objects are directly created from values, they will
    // be terminal nodes in the evaluation tree. The enqueue() function on these
    // will likely consist in ordering a writeBuffer operation.
    Real a = Real(1.0);
    Real b = Real(2.0);

    // Some Mathexpression objects have no initially known value, they are
    // instead computed with other values called children and stored in
    // m_children
    Real c = Real::sum(a, b);

    // Start the evaluation of this expression, it will recursively call the
    // evaluation of the children
    c.evaluate( ctx, queue );

    // Wait for the end of the evaluation
    c.getEndOfEvaluation().wait();

    // Data has to be explicitely brought back from the Opencl Memory buffers
    c.retrieveData( ctx, queue );

    // Then we use an implementation specific method to read the value of the
    // computed expression
    float d = c.getValue();

    // The rest of this program is used for testing.
    if( std::abs( d - 3.0 ) < 0.0000001 ){
      std::cout << "computed result matches with error : " << d - 3.0 << "\n";
      return 0;
    }else{
      std::cerr << "computed result matches with error : " << d - 3.0 << "\n";
      return -1;
    }
  }catch( Error& e ){
    e.printMsg();
    return -1;
  }
}

