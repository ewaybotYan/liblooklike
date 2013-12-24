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

/*
cl::Context getContextFromQueue( cl::CommandQueue queue ){
	return queue.getInfo<CL_QUEUE_CONTEXT>();
}*/


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

std::vector<cl::Buffer> MathExpression::getData(){
    return m_data;
}

cl::Event& MathExpression::getEndOfEvaluation(){
  return m_endOfEvaluation;
}

// #########
// # methods

void MathExpression::addChild(MathExpression* child){
  m_children.push_back(child);
  child->increaseParentNb();
}

void MathExpression::increaseParentNb(){
  m_nbParents++;
}

void MathExpression::evaluate( Context& ctx ){
  cl::CommandQueue queue = ctx.createQueue();
  evaluate( ctx, queue );
}

void MathExpression::evaluate( Context& ctx,  cl::CommandQueue& queue ){
  #ifndef NDEBUG
  std::cout << "evaluating" << std::endl;
#endif
  if( m_state >= QUEUED)
    return;
  while( !checkChildrenEnqueued( m_children ) ){
    for( MathExpression* child : m_children )
      if( child->getState() == QUEUED )
        child->getEndOfEvaluation().wait();
    deallocateMemory();
    AllocationResult allocationRes = allocateMemory( ctx );
    if( allocationRes < ONE_COMPUTED_EXPRESSION_ALLOCATED )
      throw Error("not enouph memory to compute expression");
    for( MathExpression* child : m_children )
      child->evaluate( ctx,  queue );
  }
  enqueue( ctx, queue );
  m_state = QUEUED;
}

AllocationResult MathExpression::allocateMemory( Context& context ){
  #ifndef NDEBUG
  std::cout << "allocating memory" << std::endl;
#endif
  if( m_state >= QUEUED ) // if memory is not needed, do as if we had it
    return NONE_ALLOCATED; 
  if( m_isTerminal ){
    if( allocateForResult(context) ){
      return TERMINAL_EXPRESSION_ALLOCATED; 
    }else{
      return NONE_ALLOCATED;
    }
  }else{
    AllocationResult best = NONE_ALLOCATED;// best memory allocation state
    AllocationResult worst = TERMINAL_EXPRESSION_ALLOCATED;// worst memory allocation state
    for( MathExpression* child : m_children ){
      AllocationResult tmp = child->allocateMemory( context );
      best = best < tmp ? tmp : best;
      worst = worst > tmp ? tmp : worst;
    }
    if( worst <= ONE_COMPUTED_EXPRESSION_ALLOCATED ){
      return best;
    }else{// all subexpressions allocated
      bool resAllocated = allocateForResult( context );
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
  #ifndef NDEBUG
  std::cout << "deallocating memory" << std::endl;
#endif
  for( MathExpression* child : m_children )
    child->deallocateMemory();
  if( m_nbParents <= 0 )
    deallocateForResult();
}
