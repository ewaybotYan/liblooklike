#include <CL/cl.hpp>
#include <clalgorithm.hpp>
#include <context.h>
#include <error.h>

#include "real.h"


// #############
// # Base Object

Real::Real( const float value, Context *context, cl::CommandQueue *queue ) :
    ClAlgorithm( context, queue ){
    m_result = value;
}

float Real::getResult(){
    return m_result;
}

cl::Buffer* Real::getValue(){
    return m_buffer;
}

void Real::retrieveData(){
    cl_int error;
    // m_endOfEvaluation.wait(); ---> up to the user to check
    error = m_queue->enqueueReadBuffer (
            *m_buffer,
            CL_TRUE,
            0,
            sizeof ( cl_float ),
            &m_result );
    if ( error != CL_SUCCESS )
        throw ( CLError ( error, "failed to enqueue data reading" ) );
}

void Real::enqueue() {
    cl_int error;
    error = m_queue->enqueueWriteBuffer (
                *m_buffer,
                CL_TRUE,
                0,
                sizeof ( cl_float ),
                &m_result,
                0,
                &getEndOfEvaluation()
                );
    if ( error != CL_SUCCESS )
        throw ( CLError ( error, "failed to enqueue memory transfer" ) );
}

bool Real::allocateForResult(){
    if( m_buffer != 0 )
        return true;
    cl_int error;
    cl_mem_flags memFlags = 0;
    memFlags = CL_MEM_READ_WRITE;
    m_buffer = new cl::Buffer( m_context->getContext(),
                               memFlags,
                               sizeof ( cl_float ),
                               0,
                               &error
                               );
    return error == CL_SUCCESS;
}

void Real::deallocateForResult(){
    if( m_buffer == 0 )
        return;
    delete m_buffer;
    m_buffer = 0;
}


// ##############################
// # sum Algorithm implementation

RealSum::RealSum( Real &a, Real &b, Context *context, cl::CommandQueue *queue ) :
    Real(0, context, queue ){
    m_children.push_back(&a);
    m_children.push_back(&b);
    m_lOperand = &a;
    m_rOperand = &b;
}

void RealSum::enqueue(){
    cl::Kernel kernel = m_context->getKernel ( "real", "real_real_sum" );

    //set arguments
    kernel.setArg ( 0, *(getValue()) );
    kernel.setArg ( 1, *(m_lOperand->getValue()) );
    kernel.setArg ( 2, *(m_rOperand->getValue()) );

    // prepare dependencies
    std::vector<cl::Event> dependencies;
    dependencies.push_back ( m_lOperand->getEndOfEvaluation() );
    dependencies.push_back ( m_rOperand->getEndOfEvaluation() );

    //enqueue kernel execution
    cl_int error;
    error = m_queue->enqueueNDRangeKernel (
                kernel,
                cl::NullRange,
                cl::NDRange ( 1 ),
                cl::NDRange ( 1 ),
                &dependencies,
                &getEndOfEvaluation()
                );
    if ( error != CL_SUCCESS )
        throw ( CLError ( error, "failed to enqueue kernel execution" ) );
}
