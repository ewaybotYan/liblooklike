#include "../include/exception.h"
#include "../include/algorithm.h"

#include "real.h"


// #############
// # Base Object

Real::Real( const float value) {
    m_value = value;
}

float Real::getValue(){
    return m_value;
}


// ####################################
// # Object as Algorithm implementation

RealValue::RealValue( Real& value ){
    m_src = &value;
}

Real RealValue::getResult() {
    // note that this function is not checking wether computation succeeded
    // or was even started
    return Real(*m_tmp);
}

void RealValue::waitEndOfEvaluation(){
    // nothing to wait for in this example
}

void RealValue::enqueue(){
    // this is just an algorithm that loads values from an object
    *m_tmp = m_src->getValue();
}

bool RealValue::allocateForResult(){
    m_tmp = new float;
    return m_tmp!= 0;
}

void RealValue::deallocateForResult(){
    if( m_tmp!= 0 )
        delete m_tmp;
}


// ##############################
// # sum Algorithm implementation

RealSum::RealSum( RealValue &a, RealValue &b ) {
    m_children.push_back(&a);
    m_children.push_back(&b);
}

Real RealSum::getResult() {
    // note that this function is not checking wether computation succeeded
    // or was even started
    return Real(*m_tmp);
}

void RealSum::waitEndOfEvaluation(){
    // nothing to wait for in this example
}

void RealSum::enqueue(){
    // we use the children array to find our dependencies back
    // unfirtunateley, we need to downcast before, this will be
    // adressed in example2.
    RealValue* a = dynamic_cast<RealValue*>(m_children[0]);
    RealValue* b = dynamic_cast<RealValue*>(m_children[1]);
    *m_tmp = a->getResult().getValue() + b->getResult().getValue();
}

bool RealSum::allocateForResult(){
    m_tmp = new float;
    return m_tmp!= 0;
}

void RealSum::deallocateForResult(){
    if( m_tmp!= 0 )
        delete m_tmp;
}
