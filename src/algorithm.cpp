#include<vector>

#include "../include/exception.h"
#include "../include/algorithm.h"
#include "../include/error.h"

using namespace std;


void Algorithm::addChild(Expression* child)
{
  m_children.push_back(child);
}

void Algorithm::addResult(Expression* result)
{
  m_results.push_back(result);
  result->m_parent = this;
}

bool Algorithm::allocateForResult()
{
  for ( Expression* res : m_results ){
    bool success = res->allocateMemory();
    if ( !success )
      return false;
  }
  return true;
}

void Algorithm::deallocateMemory(const int hierarchyOffset)
{
  for ( Expression* child : m_children ){
    if ( hierarchyOffset <= 0 )
      child->deallocateMemory();
    if ( !child->iscomputed() )
      child->m_parent->deallocateMemory(hierarchyOffset-1);
  }
}

bool Algorithm::recEvaluate ( int depth ) {
#ifndef NDEBUG
  cout << "evaluating" << std::endl;
#endif

  if ( m_state >= QUEUED)
    return true;

  // evaluate children first
  for( auto child = m_children.begin(); child != m_children.end(); child++ ){
    bool enqueued = (*child)->recEvaluate(depth+1);
    if (!enqueued){ // memory allocation probably failed
      for( auto older = m_children.begin(); older != child; older++ )
        (*older)->waitEndOfEvaluation();// wait for grandchildren
      deallocateMemory(1);// and free their memory
      enqueued = (*child)->recEvaluate( depth+1 );
      if (!enqueued){ // enqueing failed anyway
        if ( depth!=0 ) { // parent expression will handle the issue
          return false;
        }else{
          throw Error("Evaluation failed (memory allocation failed?)");
        }
      }
    }
  }

  bool allocated = allocateForResult();
  if (!allocated){ // memory allocation failed
    for( Expression* child : m_children ){
      child->waitEndOfEvaluation(); // wait for children
    }
    deallocateMemory(1); // free memory of grandchildren
    allocated = allocateForResult();
    if (!allocated) {
      if ( depth!=0 ) { // parent expression will handle the issue
        return false;
      } else {
        throw Error("Evaluation failed (memory allocation failed?)");
      }
    }
  }
  enqueue();
  m_state = QUEUED;
  return true;
}
