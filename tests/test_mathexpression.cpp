/**
 * @file   test_mathexpression.cpp
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @brief  unit tests for MathExpression class
 */

#include "../include/exception.h"
#include "../include/context.h"
#include "real.h"

using namespace std;

void usage(){
  cerr << "Error in " << __FILE__ << " :\n"
    << "usage :\n test_mathexpression path_to_kernel_directory" 
    << endl;
}

int main( int argc, char* argv[] ){
  if( argc !=2 ){
    usage();
    return -1;
  }
  std::string path(argv[1]);

  try{
    
    Context ctx(path);
    Real a = Real(1.0);
    Real b = Real(2.0);
    Real c = Real::sum(a, b);
    c.evaluate( ctx );
    float d = c.getValue();
    if( d - 3.0 < 0.0000001 ){
      return 0;
    }else{
      return -1;
    }
  }catch( Error& e ){
    e.printMsg();
  }
}

