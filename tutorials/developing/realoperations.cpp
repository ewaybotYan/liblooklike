/**
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @file   realoperations.cpp
 * @date   02/05/2014
 */

#include "realoperations.h"
#include <thread>
#include <exception.h>


Sum::Sum(Real &lOperand, Real &rOperand)
{
  m_lop = &lOperand;
  m_rop = &rOperand;
  // the constructor *must* register all the results and dependencies,
  // otherwise evaluation will fail.
  addDependency(m_lop);
  addDependency(m_rop);
  addResult(&m_result);
}

// Most algorithms will provide methods to acess the resulting expressions.
Real& Sum::getResult()
{
  return m_result;
}

// This method blocks execution until the computation is finished.
// If the execution is finished, it must return immediately.
// When called before teh algorithm is enqueued, a EvaluationProcessViolation
// can be thrown to notify the developper about the error.
void Sum::waitEndOfEvaluation()
{
  if ( !isEnqueued() ) {
    throw EvaluationProcessViolation(
        "Cannot wait for evaluation as it is not enqued.");
  } else {
    if(m_thread.joinable())
      m_thread.join();
  }
  // m_thread is not joinable if execution is finished, in which case we return
  // immediately as specified in the definition of waitEndOfEvaluation().
}

// no need for temporary memory in this example
// Once again, it would not be necessary to handle double allocation
bool Sum::allocateTmpMem()
{
  return true;
}
void Sum::releaseTmpMem()
{
}

// Enqueue should not block execution, so we start computation in a different
// thread.
void Sum::enqueue()
{
  // first wait for the children to finish, we use here the default mechanism
  // using waitEndOfEvaluation(), implementation specific mechanisms such as
  // OpenCL events might be more suitable.
  for (auto child : m_dependencies) {
    child->waitEndOfEvaluation();
  }

  // start execution in a separate thread so that execution enqueue can return
  // immediately
  m_thread = std::thread( Sum::add, this );
}

void Sum::add(Sum* s)
{
  *(s->m_result.getValue()) = *(s->m_lop->getValue())
                              + *(s->m_rop->getValue());
}
