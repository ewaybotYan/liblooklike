/**
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @file   tutorial1.cpp
 * @date   02/05/2014
 */

// This file is the same as tutorials/usage/tutorial1.cpp but without the
// comments. The current tutorial focuses on the real.{h,cpp} and
// realoperations.{h,cpp} files.

#include<memory>

using namespace std;

#include "real.h"
#include "realoperations.h"


const float aValue = 1.0;
const float bValue = 2.0;
const float cValue = 3.0;


int main()
{
  Real a(aValue);
  Real b(bValue);
  Real c(cValue);

  Sum sum_a_b(a,b);

  Real& a_plus_b = sum_a_b.getResult();

  Sum a_p_b_p_c(a_plus_b,c);
  Real& result = a_p_b_p_c.getResult();

  result.evaluate();
  result.waitEndOfEvaluation();
  result.print();

  return 0;
}
