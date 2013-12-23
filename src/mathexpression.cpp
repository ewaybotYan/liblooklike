#include<vector>

#include<CL/cl.hpp>

#include "../include/exception.h"
#include "../include/mathexpression.h"

// #################
// # local functions

bool checkChildrenEnqueued( const std::vector<MathExpression*> children ){
  bool res = true;
  for( MathExpression* child : children )
    res = res && child->getState() >= QUEUED;
  return res;
}


// ################
// # constructor(s)

MathExpression::MathExpression( const bool isTerminal, const bool keepInCLMem ){
  m_isTerminal = isTerminal;
  if( keepInCLMem )
    m_nbParents = 1;
}


// #################
// # getters/setters

bool MathExpression::isComputed(){
  return !m_isTerminal;
}

ExpressionState MathExpression::getState(){
  return m_state;
}

cl::Event& MathExpression::getEndOfEvaluation(){
  return m_endOfEvaluation;
}

// #########
// # methods

void MathExpression::evaluate( cl::CommandQueue queue ){
  if( m_state >= QUEUED)
    return; 
  // m_nbParents++ ????
  bool areChildrenEnqueued = checkChildrenEnqueued( m_children );
  while( !areChildrenEnqueued ){
    for( MathExpression* child : m_children )
      if( child->getState() == QUEUED )
        child->getEndOfEvaluation().wait();
    deallocateMemory();
    AllocationResult allocationRes = allocateMemory();
    if( allocationRes < ONE_COMPUTED_EXPRESSION_ALLOCATED )
      throw Error("not enouph memory to compute expression");
    for( MathExpression* child : m_children )
      child->evaluate( queue );
  }
  enqueue();
  m_state = QUEUED;
}

AllocationResult MathExpression::allocateMemory(){
  if( m_state >= QUEUED ) // if memory is not needed, do as if we had it
    return NONE_ALLOCATED; 
  if( m_isTerminal ){
    if( allocateForResult() ){
      return TERMINAL_EXPRESSION_ALLOCATED; 
    }else{
      return NONE_ALLOCATED;
    }
  }else{
    AllocationResult res;
    AllocationResult best;// best memory allocation state
    AllocationResult worst;// worst memory allocation state
    for( MathExpression* child : m_children ){
      AllocationResult tmp = child->allocateMemory();
      best = best < tmp ? tmp : best;
      worst = worst > tmp ? tmp : worst;
    }
    if( worst <= ONE_COMPUTED_EXPRESSION_ALLOCATED ){
      return best;
    }else{// all subexpressions allocated
      bool resAllocated = allocateForResult();
      if( resAllocated ){
        m_state = ALLOCATED;
        return COMPUTED_EXPRESSION_ALLOCATED;
      }else{
        if( best == TERMINAL_EXPRESSION_ALLOCATED ){
          deallocateMemory();
          return NONE_ALLOCATED;
        }else{
          return ONE_COMPUTED_EXPRESSION_ALLOCATED;
        }
      }
    }
  }
}

void MathExpression::deallocateMemory(){
  for( MathExpression* child : m_children )
    child->deallocateMemory();
  if( m_nbParents <= 0 )
    deallocateForResult();
}
