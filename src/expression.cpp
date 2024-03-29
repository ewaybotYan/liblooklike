/**
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @file   expression.cpp
 * @date   06/04/2014
 */

#include "../include/expression.hpp"
#include "../include/exception.h"

bool Expression::needsComputation()
{
  return m_computedBy != nullptr;
}

void Expression::waitEndOfEvaluation()
{
  if ( needsComputation() ) {
    if (!m_computedBy->isEnqueued() ) {
      throw EvaluationProcessViolation(
            "Cannot wait for evaluation as computation is not enqueued");
    } else {
      m_computedBy->waitEndOfEvaluation();
      m_state = COMPUTED;
    }
  }
}

Algorithm* Expression::getParentAlgorithm() const
{
  return m_computedBy;
}

void Expression::evaluate()
{
    if( needsComputation() )
      m_computedBy->evaluateTree(0);
}

bool Expression::allocateMem()
{
  if( m_state >= ALLOCATED )
    return true;

  bool allocationStatus = allocateMemImpl();
  if( allocationStatus )
    m_state = ALLOCATED;
  return allocationStatus;
}

void Expression::releaseMem()
{
  if( m_state >= ALLOCATED )
    releaseMemImpl();
  m_state = INITIAL;
}
