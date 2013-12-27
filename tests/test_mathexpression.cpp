/**
 * @file   test_mathexpression.cpp
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @brief  unit tests for MathExpression class
 */

#include "../include/exception.h"
#include "../include/context.h"
#include "../include/mathexpression.h"
#include "real.h"

#include<iostream>
#include<cmath>

using namespace std;

void usage(){
  cerr << "Error in " << __FILE__ << " :\n"
    << "usage :\n test_mathexpression path_to_kernel_directory" 
    << endl;
}

int main( int argc, char* argv[] ){
  if( argc < 2 ){
    usage();
    return -1;
  }
  std::string path(argv[argc-1]);

  try{
    
    Context ctx(path);
    cl::CommandQueue queue = ctx.createQueue();
    Real a = Real(1.0);
    Real b = Real(2.0);
    Real c = Real::sum(a, b);
    c.evaluate( ctx, queue );
    c.retrieveData( ctx, queue );
    float d = c.getValue();
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

