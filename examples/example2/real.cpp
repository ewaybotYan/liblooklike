#include <exception.h>
#include <algorithm.h>
#include <thread> // we will mimick the use of multithreading in this example

#include "real.h"


// #############
// # Base Object

Real::Real( float* valuePtr) {
    m_dataSrc = valuePtr;
}

float Real::getResult(){
    if( m_buffer != 0 )
        return *m_buffer;
    else{
        // in a real program, you would want to throw an exception here
        return 0;
    }
}

float* Real::getValue(){
    return m_buffer;
}

void Real::waitEndOfEvaluation(){
    // again, we have nothing to wait for, no thread running...
}

void Real::enqueue() {
    *m_buffer = *m_dataSrc;
}

bool Real::allocateForResult(){
    m_buffer = new float;
    return m_buffer != 0;
}

void Real::deallocateForResult(){
    delete m_buffer;
    m_buffer = 0;
}


// ##############################
// # sum Algorithm implementation

RealSum::RealSum( Real &a, Real &b ) {
    m_children.push_back(&a);
    m_children.push_back(&b);
    m_lOperand = &a;
    m_rOperand = &b;
}

void RealSum::waitEndOfEvaluation(){
    m_t.join();
}

void computationFunction( float* s, float* a, float* b ){
    *s = *a + *b;
}

void RealSum::enqueue(){
    for(auto child:m_children){
        child->waitEndOfEvaluation();
    }
    m_t = std::thread(computationFunction,
                      m_buffer,
                      m_lOperand->getValue(),
                      m_rOperand->getValue());
}

bool RealSum::allocateForResult(){
    m_buffer = new float;
    return m_buffer!= 0;
}

void RealSum::deallocateForResult(){
    if( m_buffer!= 0 )
        delete m_buffer;
}
