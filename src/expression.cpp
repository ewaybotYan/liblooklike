/**
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @file   expression.cpp
 * @date   06/04/2014
 */

#include "../include/expression.hpp"

bool Expression::iscomputed()
{
  return m_parent != nullptr;
}

bool Expression::recEvaluate(int depth)
{
  if( m_parent )
    return m_parent->recEvaluate( depth );
  return true;
}

void Expression::waitEndOfEvaluation()
{
  if( iscomputed() )
    m_parent->waitEndOfEvaluation();
}

void Expression::evaluate()
{
  if( iscomputed() )
    m_parent->recEvaluate(0);
}
