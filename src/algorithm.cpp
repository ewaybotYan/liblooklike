#include<vector>

#include<CL/cl.hpp>

#include "../include/exception.h"
#include "../include/algorithm.h"
#include "../include/error.h"

// #################
// # getters/setters

ExpressionState Algorithm::getState(){
    return m_state;
}

// #########
// # methods

void Algorithm::addChild(Algorithm* child){
    m_children.push_back(child);
    child->addParent(this);
}


void Algorithm::addParent(Algorithm* parent){
    m_parents.push_back(parent);
}


void Algorithm::evaluate(){
    recEvaluate(0);
}


void Algorithm::deallocateMemory(const int hierarchyOffset){
    if( hierarchyOffset == 0 )
        deallocateForResult();
    for( Algorithm* child : m_children )
        child->deallocateMemory(hierarchyOffset-1);
}


bool Algorithm::recEvaluate ( int depth ) {
#ifndef NDEBUG
    std::cout << "evaluating" << std::endl;
#endif
    if( m_state >= QUEUED)
        return true;

    // evaluate children first
    for( auto child = m_children.begin(); child != m_children.end(); child++ ){
        bool enqueued = (*child)->recEvaluate(depth+1);
        if(!enqueued){ // memory allocation probably failed
            for( auto older = m_children.begin(); older != child; older++ ){
                (*older)->waitEndOfEvaluation();// wait for grandchildren
                (*older)->deallocateMemory(1);// and free their memory
            }
            enqueued = (*child)->recEvaluate( depth+1 );
            if(!enqueued){ // enqueing failed anyway
                if( depth!=0 ) { // parent expression will handle the issue
                    return false;
                }else{
                    throw Error("Evaluation failed (memory allocation failed?)");
                }
            }
        }
    }

    bool allocated = allocateForResult();
    if(!allocated){ // memory allocation failed
        for( auto child = m_children.begin();
             child != m_children.end();
             child++  ){
            (*child)->waitEndOfEvaluation(); // wait for children
            (*child)->deallocateMemory(1); // free memory of grandchildren
        }
        allocated = allocateForResult();
        if(!allocated) {
            if( depth!=0 ) { // parent expression will handle the issue
                return false;
            }else{
                throw Error("Evaluation failed (memory allocation failed?)");
            }
        }
    }
    enqueue();
    m_state = QUEUED;
    return true;
}
