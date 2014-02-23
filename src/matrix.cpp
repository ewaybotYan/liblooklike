/**
 *  @file   matrix.cpp
 *  @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 *  @brief  implements methods from @ref Algorithm for matrices
 */

#ifndef NDEBUG
#include<iostream>
#endif

#include "../include/exception.h"
#include "../include/algorithm.h"

#include "../include/matrix.h"
#include "../include/context.h"

// trick to supress unused variable warning
#define _unused(x) ((void)x)


// ##############
// # constructors

Matrix::Matrix ( cl_float* values, 
                 const unsigned int m,
                 const unsigned int n,
                 const bool keepInCLMem) :
    Algorithm ( true, keepInCLMem ) {
    m_value = new cl_float[m*n];
    for( unsigned int i = 0; i < m*n; i++ )
        m_value[i] = values[i];
    m_m = m;
    m_n = n;
}


Matrix::Matrix ( const std::string programName, 
                 const std::string kernelName,
                 const unsigned int m,
                 const unsigned int n,
                 const bool keepInCLMem ) :
    Algorithm ( false, keepInCLMem ) {
    m_value = 0;
    m_m = m;
    m_n = n;
    m_programName = programName;
    m_kernelName = kernelName;
}


// #########################
// # mathematical operations

Matrix  Matrix::mul ( Matrix& A, Matrix& B, const bool keepInCLMem) {
    if( A.getWidth() != B.getHeight() )
        throw( Error("cannot multiply matrices: dimensions mismatch") );

    if( B.getWidth() == 1 ) {// optimised matrix vector product
        Matrix result ( "matrix_mult", "matrix_vector_multiplication",
                        A.getHeight(),
                        1,
                        keepInCLMem );
        result.addChild ( &A );
        result.addChild ( &B );
        return result;
    }else{
        Matrix result ( "matrix_mult", "matrix_matrix_multiplication",
                        A.getHeight(),
                        B.getWidth(),
                        keepInCLMem );
        result.addChild ( &A );
        result.addChild ( &B );
        return result;
    }
}


Matrix Matrix::add ( Matrix& A, Matrix& B, const bool keepInCLMem) {
    if( (A.getHeight() != B.getHeight())||(A.getWidth()!=B.getWidth() ) )
        throw( Error("cannot add matrices: dimensions mismatch") );
    Matrix result ( "matrix", "matrix_matrix_add",
                    A.getHeight(), A.getWidth(),
                    keepInCLMem );
    result.addChild ( &A );
    result.addChild ( &B );
    return result;
}


Matrix Matrix::normalize ( Matrix& A, const bool keepInCLMem) {
    Matrix result ( "matrix_norm", "matrix_normalize",
                    A.getHeight(), A.getWidth(),
                    keepInCLMem );
    result.addChild ( &A );
    return result;
}


Matrix Matrix::covariance( Matrix& A, const bool keepInCLMem) {
    Matrix result ( "matrix_mult", "matrix_covariance",
                    A.getHeight(), A.getHeight(),
                    keepInCLMem );
    result.addChild ( &A );
    return result;
}


// ####################
// # getters and setter

float Matrix::getValue ( const int i, const int j ) {
    if ( m_value == 0 ){
        throw ( Error ( "Cannot get value as it has not been computed" ) );
        return 0;
    }
    return m_value[ i*m_n+j ];
}


cl_float* Matrix::getValues() const{
    return m_value;
}


int Matrix::getWidth() const{
    return m_n;
}


int Matrix::getHeight() const{
    return m_m;
}


#ifndef NDEBUG
void Matrix::print(){
    std::cout.precision(5);
    std::cout << std::scientific;
    for( int i=0;i<m_m;i++ ){
        for( int j=0;j<m_n;j++ )
            std::cout << m_value[i*m_n+j] << " ";
        std::cout << "\n";
    }
}
#endif


// ##################
// # memory managment

void  Matrix::retrieveData ( Context& context, cl::CommandQueue& queue ){
#ifndef NDEBUG
    std::cout << "retrieving data" << std::endl;
#endif
    _unused ( context );

    cl_int error;
    if( m_value != 0 )
        delete( m_value );
    m_value = new cl_float[m_m*m_n];
    // m_endOfEvaluation.wait(); ---> up to the user to check
    error = queue.enqueueReadBuffer (
                getData() [0],
            CL_TRUE,
            0,
            sizeof ( cl_float ) * m_m * m_n,
            m_value );
    if ( error != CL_SUCCESS )
        throw ( CLError ( error, "failed to enqueue data reading" ) );
}


bool Matrix::allocateForResult ( Context& context ) {
#ifndef NDEBUG
    std::cout << "allocating for result " << m_n << "x" << m_m << std::endl;
#endif
    if ( m_data.size() != 0 ) //buffer already created
        return true;

    cl_int error;
    cl_mem_flags memFlags = 0;
    memFlags = CL_MEM_READ_WRITE;
    m_data.push_back ( cl::Buffer (
                           context.getContext(),
                           memFlags,
                           sizeof ( cl_float ) * m_m * m_n,
                           0,
                           &error
                           ) );
    if ( error != CL_SUCCESS ) {
        return false;
    } else {
        m_state = ALLOCATED;
        return true;
    }
}


void Matrix::enqueue ( Context& context, cl::CommandQueue& queue ) {
    if ( isComputed() && m_state != QUEUED ) { // this expression has to be computed

#ifndef NDEBUG
        std::cout << "enqueing computation kernel" << std::endl;
#endif

        // create kernel
        cl::Kernel kernel = context.getKernel ( m_programName, m_kernelName );

        // by this point, the code is specific to the operation
        if ( m_kernelName == "matrix_normalize" ) {
            //set arguments
            kernel.setArg ( 0, m_data[0] );
            kernel.setArg ( 1, m_children[0]->getData() [0] );
            kernel.setArg<int> ( 2, m_m );
            kernel.setArg<int> ( 3, m_n );

            // prepare dependencies
            std::vector<cl::Event> dependencies;
            dependencies.push_back ( m_children[0]->getEndOfEvaluation() );

            //enqueue kernel execution
            cl_int error;
            error = queue.enqueueNDRangeKernel (
                        kernel,
                        cl::NullRange,
                        cl::NDRange ( m_n ),
                        cl::NDRange ( 1 ),
                        &dependencies,
                        &m_endOfEvaluation
                        );
            if ( error != CL_SUCCESS )
                throw ( CLError ( error, "failed to enqueue kernel execution" ) );

        }else if ( m_kernelName == "matrix_covariance" ) {
            //set arguments
            kernel.setArg ( 0, m_data[0] );
            kernel.setArg ( 1, m_children[0]->getData() [0] );
            kernel.setArg<int> ( 2, ((Matrix*)m_children[0])->getWidth() );
            kernel.setArg<int> ( 3, ((Matrix*)m_children[0])->getHeight() );

            // prepare dependencies
            std::vector<cl::Event> dependencies;
            dependencies.push_back ( m_children[0]->getEndOfEvaluation() );

            //enqueue kernel execution
            cl_int error;
            error = queue.enqueueNDRangeKernel (
                        kernel,
                        cl::NullRange,
                        cl::NDRange ( m_m, m_n ),
                        cl::NDRange ( 1, 1 ),
                        &dependencies,
                        &m_endOfEvaluation
                        );
            if ( error != CL_SUCCESS )
                throw ( CLError ( error, "failed to enqueue kernel execution" ) );

        }else if ( m_kernelName == "matrix_matrix_multiplication" ){

#ifndef NDEBUG
            std::cout << "evaluating multiplication\n";
#endif

            //set arguments
            kernel.setArg ( 0, m_data[0] );
            kernel.setArg ( 1, m_children[0]->getData() [0] );
            kernel.setArg ( 2, m_children[1]->getData() [0] );
            kernel.setArg<int> ( 3, m_n );
            /// @todo use cast
            int productDepth = ((Matrix*)m_children[0])->getWidth();
            kernel.setArg<int> ( 4, productDepth );

            // prepare dependencies
            std::vector<cl::Event> dependencies;
            dependencies.push_back ( m_children[0]->getEndOfEvaluation() );
            dependencies.push_back ( m_children[1]->getEndOfEvaluation() );

            //enqueue kernel execution
            cl_int error;
            error = queue.enqueueNDRangeKernel (
                        kernel,
                        cl::NullRange,
                        cl::NDRange ( m_n, m_m ),
                        cl::NDRange ( 1, 1 ),
                        &dependencies,
                        &m_endOfEvaluation
                        );

            if ( error != CL_SUCCESS )
                throw ( CLError ( error, "failed to enqueue kernel execution" ) );

        }else if (m_kernelName == "matrix_vector_multiplication"){

            //set arguments
            kernel.setArg ( 0, m_data[0] );
            kernel.setArg ( 1, m_children[0]->getData() [0] );
            kernel.setArg ( 2, m_children[1]->getData() [0] );
            /// @todo use cast
            int productDepth = ((Matrix*)m_children[0])->getWidth();
            kernel.setArg<int> ( 3, productDepth );

            // prepare dependencies
            std::vector<cl::Event> dependencies;
            dependencies.push_back ( m_children[0]->getEndOfEvaluation() );
            dependencies.push_back ( m_children[1]->getEndOfEvaluation() );

            //enqueue kernel execution
            cl_int error;
            error = queue.enqueueNDRangeKernel (
                        kernel,
                        cl::NullRange,
                        cl::NDRange ( 1, m_m ),
                        cl::NDRange ( 1, 1 ),
                        &dependencies,
                        &m_endOfEvaluation
                        );

            if ( error != CL_SUCCESS )
                throw ( CLError ( error, "failed to enqueue kernel execution" ) );
        }else{
#ifndef NDEBUG
            std::cerr << "no kernel specified!";
#endif
        }

    } else { // this is a terminal expression, just load data in the buffer
#ifndef NDEBUG
        std::cout << "enquing data load" << std::endl;
#endif
        cl_int error;
        error = queue.enqueueWriteBuffer (
                    getData()[0],
                CL_TRUE,
                0,
                sizeof ( cl_float ) * m_m * m_n,
                m_value,
                0,
                &getEndOfEvaluation()
                );
        if ( error != CL_SUCCESS )
            throw ( CLError ( error, "failed to enqueue memory transfer" ) );
    }

    m_state = QUEUED;
}
