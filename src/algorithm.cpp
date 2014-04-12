#include<vector>

#include "../include/exception.h"
#include "../include/algorithm.h"
#include "../include/error.h"

using namespace std;


bool Algorithm::isEnqueued() const
{
  return m_isEnqueued;
}

bool Algorithm::allocateResMem()
{
  for ( Expression* res : m_results ){
    bool success = res->allocateMem();
    if ( !success )
      return false;
  }
  return true;
}

void Algorithm::releaseTreeMem(const int hierarchyOffset)
{
  m_isEnqueued = false;

  releaseTmpMem();

  if ( hierarchyOffset <= 0 )
    for ( Expression* result : m_results )
        result->releaseMem();

  for ( Expression* dependency : m_dependencies )
    if( dependency->needsComputation() )
      dependency->m_computedBy->releaseTreeMem(hierarchyOffset-1);
}

void Algorithm::addDependency(Expression* dependency)
{
  m_dependencies.push_back(dependency);
}

void Algorithm::addResult(Expression* result)
{
  m_results.push_back(result);
  result->m_computedBy = this;
}

bool Algorithm::evaluateTree ( int depth ) {
#ifndef NDEBUG
  cout << "evaluating" << std::endl;
#endif

  if ( isEnqueued() )
    return true;

  // evaluate dependencies first
  for( auto dependency = m_dependencies.begin();
       dependency != m_dependencies.end();
       dependency++ ){
    if( !(*dependency)->needsComputation() )
      continue;
    bool enqueued = (*dependency)->m_computedBy->evaluateTree(depth+1);
    if (!enqueued){ // memory allocation probably failed
      for( auto older = m_dependencies.begin(); older != dependency; older++ )
        (*older)->waitEndOfEvaluation();// wait for grand children dependencies
      releaseTreeMem(1);// and free their memory
      enqueued = (*dependency)->m_computedBy->evaluateTree( depth+1 );
      if (!enqueued){ // enqueing failed anyway
        if ( depth!=0 ) { // parent expression will handle the issue
          return false;
        }else{
          throw Error("Evaluation failed (memory allocation failed?)");
        }
      }
    }
  }

  bool allocated = allocateResMem();
  if (!allocated){ // memory allocation failed
    for ( Expression* dependency : m_dependencies )
      dependency->waitEndOfEvaluation(); // wait for dependencies
    releaseTreeMem(1); // free memory of granddependencies
    allocated = allocateResMem();
    if (!allocated) {
      if ( depth!=0 ) { // parent expression will handle the issue
        return false;
      } else {
        throw Error("Evaluation failed (memory allocation failed?)");
      }
    }
  }

  enqueue();

  m_isEnqueued = true;

  return true;
}
