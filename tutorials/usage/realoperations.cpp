/**
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @file   realoperations.cpp
 * @date   02/05/2014
 */

// NOTE : If you are looking for an example of implementation of Algorithm,
//        please refer to tutorial/developing/realoperations.cpp instead.

#include "realoperations.h"

Sum::Sum(Real &lOperand, Real &rOperand)
{
  m_lop = &lOperand;
  m_rop = &rOperand;
  addDependency(m_lop);
  addDependency(m_rop);
  addResult(&m_result);
}

Real& Sum::getResult()
{
  return m_result;
}

// since no form of thread is used in this example, waiting is useless
void Sum::waitEndOfEvaluation()
{
}

// no need for temporary memory in this example
bool Sum::allocateTmpMem()
{
  return true;
}
void Sum::releaseTmpMem()
{
}

// this is a bad implementation of enqueue() because this function is
// blocking the execution and does not wait for the children to be computed.
void Sum::enqueue()
{
  *(m_result.getValue()) = *(m_lop->getValue()) + *(m_rop->getValue());
}
