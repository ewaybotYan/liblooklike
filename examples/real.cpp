#include "../include/exception.h"
#include "../include/mathexpression.h"

#include "real.h"
#include "../include/context.h"

// trick to supress unused variable warning
#define _unused(x) ((void)x)

// ##############
// # constructors

Real::Real( const float value, const bool keepInCLMem ) : MathExpression( true, keepInCLMem ) {
    m_value = new float;
    *m_value = value;
}

Real::Real( const bool keepInCLMem, const std::string programName, const std::string kernelName  ) : MathExpression( false, keepInCLMem ) {
    m_value = 0;
    m_programName = programName;
    m_kernelName = kernelName;
}

Real Real::sum( Real& a, Real& b, const bool keepInCLMem ) {
    Real result( keepInCLMem, "test_real", "real_real_add" );
    result.addChild(&a);
    result.addChild(&b);
    return result;
}

Real Real::mul( Real& a, Real& b, const bool keepInCLMem ) {
    Real result( keepInCLMem, "test_real", "real_real_mul" );
    result.addChild(&a);
    result.addChild(&b);
    return result;
}

// #####################
// # getters and setters

float Real::getValue( ) {
    if( m_value == 0 )
        throw( Error("Cannot get value as it has not been computed") );
    return *m_value;
}

// ###################
// # memory operations

// allocateForResult is in charge of creating a buffer in order to store the
// result of the computation in OpenCL memory
bool Real::allocateForResult( Context& context ) {
#ifndef NDEBUG
    std::cout << "allocating for result" << std::endl;
#endif
    if(m_data.size() != 0)//buffer already created
        return true;

    cl_int error;
    cl_mem_flags memFlags = CL_MEM_READ_WRITE;

    // we make sure to store the buffer in m_data
    m_data.push_back( cl::Buffer(
                          context.getContext(),
                          memFlags,
                          sizeof(float),
                          NULL,
                          &error
                          ));

    if(error != CL_SUCCESS) {
        return false;
    } else {
        // in case of successful memory allocation, we upfate the state flag
        m_state = ALLOCATED;
        return true;
    }
}


// #########
// # methods


// enqueue() is supposed to put the necessary operations in the OpenCL command
// queue so that the
void Real::enqueue( Context& context, cl::CommandQueue& queue ) {

    // Tho different cases: either this is a terminal expression, or a computed
    // one.
    if( isComputed() ) {
        // In the case of a computed expression, we identify the operation to
        // use and load the appropriate kernel. In this example, all computed
        // are the result of binary operations, and kernels have the same
        // arguments, so we just start the kernel with those two arguments.

        // create kernel
        cl::Kernel kernel = context.getKernel( m_programName, m_kernelName );
        kernel.setArg(0, m_data[0]);
        kernel.setArg(1, m_children[0]->getData()[0]);
        kernel.setArg(2, m_children[1]->getData()[0]);

        // prepare dependencies
        std::vector<cl::Event> dependencies;
        for( MathExpression* child : m_children )
            dependencies.push_back((child->getEndOfEvaluation()));

        // enqueue kernel execution
        cl_int error;
        error = queue.enqueueNDRangeKernel(
                    kernel,
                    cl::NullRange,
                    cl::NDRange(1),
                    cl::NDRange(1, 1),
                    &dependencies,
                    &m_endOfEvaluation
                    );

        // handle errors
        if( error != CL_SUCCESS )
            throw( CLError(error, "failed to enqueue kernel execution") );

    } else {
        // This is a terminal expression, the typical behaviour is to enqueue
        // the memory transfer from the local memory.

        cl_int error;
        error = queue.enqueueWriteBuffer(
                    getData()[0],
                CL_FALSE,
                0,
                sizeof(float),
                m_value,
                0,
                &getEndOfEvaluation()
                );

        if( error != CL_SUCCESS )
            throw( CLError(error, "failed to enqueue memory transfer") );
    }

    // Do not forget to update the object state
    m_state = QUEUED;
}


// RetrieveData has no predefined behaviour. It is intended to bring back the
// computed value of the expression from the openCL memory.
// Note that no checking is done to ensure the object computation is over, or
// has even started.
void Real::retrieveData( Context& context, cl::CommandQueue& queue ){
#ifndef NDEBUG
    std::cout << "retrieving data" << std::endl;
#endif
    _unused(context);
    
    cl_int error;
    m_value = new float;
    error = queue.enqueueReadBuffer(
                getData()[0],
            CL_TRUE,
            0,
            sizeof(float),
            m_value);
    if( error != CL_SUCCESS )
        throw( CLError( error, "failed to enqueue data reading" ) );
}
