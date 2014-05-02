/**
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @file   tutorial1.cpp
 * @date   02/05/2014
 */

#include<memory>

using namespace std;

// In this tutorial, we will use the basic functions of the liblooklike library,
// namely expressions and algorithms.

// This example will use real numbers and the operations that we define on real
// numbers are the addition and the product.
// However, we won't study the details of these objects now.
#include "real.h"
#include "realoperations.h"


const float aValue = 1.0;
const float bValue = 2.0;
const float cValue = 3.0;


// In this example, we have 3 real values a,b and c and we want to compute
// (a+b)+c
int main(){
  // We will first create Expression objects that correspond to constant values.
  // More precisely we use Real which is a specialization of Expression for
  // real values
  Real a(aValue);
  Real b(bValue);
  Real c(cValue);

  // Then we create an instance of Sum for the sum of a and b. Sum is a
  // specialization of Algorithm for the sum of two real values.
  Sum sum_a_b(a,b);

  // Sum has a method getResult wich yields a Real expression: the sum of the
  // input expressions given to the constructor.
  // Note: the method that gives the result is not standard but specific to the
  //       implementation of Algorithm.
  Real& a_plus_b = sum_a_b.getResult();

  // We can now create the second sum
  Sum a_p_b_p_c(a_plus_b,c);
  Real& result = a_p_b_p_c.getResult();

  // we can now start the computation process
  result.evaluate();
  // block execution until computations are finished
  result.waitEndOfEvaluation();
  // and print the result (print is specific to Real, not available in any
  // expression)
  result.print();

  // We can use that value to compute the product with c
  return 0;
}
