/**
 * @file   test_algorithm.cpp
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @brief  unit tests for Algorithm class
 */

#include "../include/exception.h"
#include "../include/context.h"
#include "../include/algorithm.h"
#include "../examples/real.h"

#include<iostream>
#include<cmath>

using namespace std;

void usage(){
  cerr << "Error in " << __FILE__ << " :\n"
    << "usage :\n test_algorithm path_to_kernel_directory" 
    << endl;
}

int main( int argc, char* argv[] ){
  if( argc !=2 ){
    usage();
    return -1;
  }
  std::string path(argv[argc-1]);

  try{
    
    Context ctx(path);
    cl::CommandQueue queue = ctx.createQueue();
    Real a = Real(1.0);
    Real b = Real(2.0);
    Real c = Real(10.0);
    Real d = Real::sum( a, b );
    Real e = Real::mul( c, d );
    
    e.evaluate( ctx, queue );
    e.retrieveData( ctx,queue );
    float f = e.getValue();
    if( std::abs( f - 30 ) < 0.0000001 ){
      std::cout << "computed result matches with error : " << f - 30.0 << "\n";
      return 0;
    }else{
      std::cerr << "computed result matches with error : " << f - 30.0 << "\n";
      return -1;
    }
  }catch( Error& err ){
    err.printMsg();
    return -1;
  }
}
