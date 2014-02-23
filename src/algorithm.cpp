#include<vector>

#include<CL/cl.hpp>

#include "../include/exception.h"
#include "../include/algorithm.h"

// #################
// # local functions

bool checkChildrenEnqueued( const std::vector<Algorithm*> children ){
    bool res = true;
    for( Algorithm* child : children )
        res = res && child->getState() >= QUEUED;
    return res;
}

// ################
// # constructor(s)

Algorithm::Algorithm( const bool isTerminal, const bool keepInCLMem ){
    m_isTerminal = isTerminal;
    if( keepInCLMem )
        m_nbParents = 1;
}



// #################
// # getters/setters

bool Algorithm::isComputed(){
    return !m_isTerminal;
}


ExpressionState Algorithm::getState(){
    return m_state;
}


std::vector<cl::Buffer> Algorithm::getData(){
    return m_data;
}


cl::Event& Algorithm::getEndOfEvaluation(){
    return m_endOfEvaluation;
}


// #########
// # methods

void Algorithm::addChild(Algorithm* child){
    m_children.push_back(child);
    child->increaseParentNb();
}


void Algorithm::increaseParentNb(){
    m_nbParents++;
}


void Algorithm::evaluate( Context& ctx,  cl::CommandQueue& queue ){
#ifndef NDEBUG
    std::cout << "evaluating" << std::endl;
#endif
    if( m_state >= QUEUED)
        return;
    if( m_children.size() > 0 ){
        while( !checkChildrenEnqueued( m_children ) ){
            for( Algorithm* child : m_children )
                if( child->getState() == QUEUED )
                    child->getEndOfEvaluation().wait();
            deallocateMemory();
            AllocationResult allocationRes = allocateMemory( ctx );
            if( allocationRes < ONE_COMPUTED_EXPRESSION_ALLOCATED )
                throw Error("not enough memory to compute child expression");
            // evaluate children for whom memory is available
            for( Algorithm* child : m_children )
                if( child->getState() == ALLOCATED )
                    child->evaluate( ctx,  queue );
        }
    }else{ // allocate for terminal expression
        AllocationResult allocationRes = allocateMemory( ctx );
        if( allocationRes < ONE_COMPUTED_EXPRESSION_ALLOCATED )
            throw Error("not enough memory to compute expression");
    }
    enqueue( ctx, queue );
    m_state = QUEUED;
}


AllocationResult Algorithm::allocateMemory( Context& context ){
    // skip simple cases
    if( m_state >= QUEUED ) // if memory is not needed, nothing is done
        return NONE_ALLOCATED;
    if( m_state >= ALLOCATED )
        return COMPUTED_EXPRESSION_ALLOCATED;

#ifndef NDEBUG
    std::cout << "allocating memory" << std::endl;
#endif
    if( m_isTerminal ){
        if( allocateForResult(context) ){
            return TERMINAL_EXPRESSION_ALLOCATED;
        }else{
            return NONE_ALLOCATED;
        }
    }else{
        AllocationResult best = NONE_ALLOCATED;// best memory allocation state
        AllocationResult worst = TERMINAL_EXPRESSION_ALLOCATED;// worst memory allocation state
        for( Algorithm* child : m_children ){
            AllocationResult tmp = child->allocateMemory( context );
            best = best < tmp ? tmp : best;
            worst = worst > tmp ? tmp : worst;
        }
        if( worst != NONE_ALLOCATED ){
            bool resAllocated = allocateForResult( context );
            if( resAllocated ){
                m_state = ALLOCATED;
                return COMPUTED_EXPRESSION_ALLOCATED;
            }
        }
        if( best == TERMINAL_EXPRESSION_ALLOCATED ){
            deallocateMemory();
            return NONE_ALLOCATED;
        }else{
            return ONE_COMPUTED_EXPRESSION_ALLOCATED;
        }
    }
}


void Algorithm::deallocateMemory(){
#ifndef NDEBUG
    std::cout << "deallocating memory" << std::endl;
#endif
    for( Algorithm* child : m_children )
        child->deallocateMemory();
    if( m_nbParents <= 0 )
        deallocateForResult();
}


void Algorithm::deallocateForResult() {
#ifndef NDEBUG
    std::cout << "deallocating for result" << std::endl;
#endif
    m_data.clear();
    m_state = INITIAL;
}
