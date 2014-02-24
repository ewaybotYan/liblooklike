#include<vector>

#include<CL/cl.hpp>

#include "../include/exception.h"
#include "../include/algorithm.h"
#include "../include/error.h"

// #################
// # local functions

bool checkChildrenEnqueued( const std::vector<Algorithm*> children ){
    bool res = true;
    for( Algorithm* child : children )
        res = res && child->getState() >= QUEUED;
    return res;
}

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


bool Algorithm::evaluate () {
    evaluate(0);
}


void Algorithm::evaluate () {
#ifndef NDEBUG
    std::cout << "evaluating" << std::endl;
#endif
    if( m_state >= QUEUED)
        return;

    // evaluate children first
    for( auto child = m_children.begin(); child != m_children.end(); child++ ){
        bool enqueued = child->evaluate();
        if(!enqueued){ // memory allocation probably failed
            for( auto older = m_children.begin(); older != child; older++ ){
                older->waitEndOfEvaluation();// wait for grandchildren
                older->deallocateMemory(1);// and free their memory
            }
        }
        enqueued = child->evaluate();
        if(!enqueued) // enqueing failed anyway
            throw Error("Evaluation failed (memory allocation failed?)");
    }

    bool allocation = allocateForResult();
    if(!allocated){ // memory allocation failed
        for( auto child = m_children.begin();
             child != m_children.end();
             child++  ){
            child->waitEndOfEvaluation(); // wait for children
            child->deallocateMemory(1); // free memory of grandchildren
        }
    }
    allocation = allocateForResult();
    if(!allocated)
        throw Error("Evaluation failed (memory allocation failed?)");
    enqueue();
    m_state = QUEUED;
}


void Algorithm::addParent(Algorithm* parent){
    m_parent.push_back(parent);
}


void Algorithm::deallocateMemory(const int hierarchyOffset){
    if( hierarchyOffset == 0 )
        deallocateForResult();
    for( Algorithm child : m_children )
        child.deallocateMemory(hierarchyOffset-1);
}
