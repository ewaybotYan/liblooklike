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

#include <exception.h>
#include <algorithm.h>
#include "real.h"

#include<iostream>
#include<cmath>

using namespace std;

int main(){

  try{

    // Some Mathexpression objects are directly created from values, they will
    // be terminal nodes in the evaluation tree. The enqueue() function on these
    // will likely consist in ordering a writeBuffer operation.
    Real a = Real(1.0);
    Real b = Real(2.0);

    RealValue aWrapper = RealValue(a);
    RealValue bWrapper = RealValue(b);

    // Some Mathexpression objects have no initially known value, they are
    // instead computed with other values called children and stored in
    // m_children
    RealSum sum = RealSum( aWrapper, bWrapper );

    // Start the evaluation of this expression, it will recursively call the
    // evaluation of the children
    sum.evaluate();

    // Wait for the end of the evaluation
    sum.waitEndOfEvaluation();

    // Then we use an implementation specific method to read the value of the
    // computed expression
    Real res = sum.getResult();
    float s = res.getValue();

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

