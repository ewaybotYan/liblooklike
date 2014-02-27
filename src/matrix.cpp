/**
 *  @file   matrix.cpp
 *  @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 *  @brief  implements methods from @ref Algorithm for matrices
 */

#ifndef NDEBUG
#include<iostream>
#endif

#include <CL/cl.hpp>
#include <clalgorithm.hpp>
#include <context.h>
#include <error.h>

#include "../include/matrix.h"


// ####################
// # Base Matrix object

Matrix::Matrix ( cl_float* values, 
                 const unsigned int m,
                 const unsigned int n,
                 Context *context,
                 cl::CommandQueue *queue ) :
    ClAlgorithm ( context, queue ) {
    if( values != 0 ){
        m_result = new cl_float[m*n];
        for( unsigned int i = 0; i < m*n; i++ )
            m_result[i] = values[i];
    }
    m_m = m;
    m_n = n;
}

int Matrix::getWidth() const{
    return m_n;
}


int Matrix::getHeight() const{
    return m_m;
}

cl_float* Matrix::getResult() const{
    return m_result;
}

float Matrix::getResultValue ( const int i, const int j ) {
    return m_result[ i*m_n+j ];
}

cl::Buffer* Matrix::getBuffer(){
    return m_buffer;
}

void  Matrix::retrieveData (){
    cl_int error;
    if(m_result == 0)
        m_result = new cl_float[m_m*m_n];
    error = m_queue->enqueueReadBuffer (
                *m_buffer,
                CL_TRUE,
                0,
                sizeof ( cl_float ) * m_m * m_n,
                m_result );
    if ( error != CL_SUCCESS )
        throw ( CLError ( error, "failed to enqueue data reading" ) );
}

void Matrix::enqueue() {
    cl_int error;
    error = m_queue->enqueueWriteBuffer (
                *getBuffer(),
                CL_TRUE,
                0,
                sizeof ( cl_float ) * m_m * m_n,
                m_result,
                0,
                &getEndOfEvaluation()
                );
    if ( error != CL_SUCCESS )
        throw ( CLError ( error, "failed to enqueue memory transfer" ) );
}

bool Matrix::allocateForResult () {
#ifndef NDEBUG
    std::cout << "allocating for result\n";
#endif
    // todo check if this is possible
    if ( m_buffer != 0 ) //buffer already created
        return true;
    cl_int error;
    cl_mem_flags memFlags = 0;
    memFlags = CL_MEM_READ_WRITE;
    m_buffer = new cl::Buffer( m_context->getContext(),
                               memFlags,
                               sizeof ( cl_float ) * m_m * m_n,
                               0,
                               &error
                               );
    return error == CL_SUCCESS;
}

void Matrix::deallocateForResult(){
    if( m_buffer == 0 )
        return;
    delete[] m_buffer;
    m_buffer = 0;
}

#ifndef NDEBUG
void Matrix::print(){
    std::cout.precision(5);
    std::cout << std::scientific;
    for( int i=0;i<m_m;i++ ){
        for( int j=0;j<m_n;j++ )
            std::cout << m_result[i*m_n+j] << " ";
        std::cout << "\n";
    }
}
#endif

// ########################
// # main Matrix algorithms

MatrixProd::MatrixProd ( Matrix& A, Matrix& B,
                         Context *context,
                         cl::CommandQueue *queue ) :
    Matrix ( 0,
             A.getHeight(), B.getWidth(),
             context, queue ) {
    if( A.getWidth() != B.getHeight() )
        throw Error("Matrix dimensions are incompatible for matrix product.");
    addChild(&A);
    addChild(&B);
    m_lOperand = &A;
    m_rOperand = &B;
}

void MatrixProd::enqueue(){
    // get kernel
    cl::Kernel kernel = m_context->getKernel ( "matrix_mult",
                                               "matrix_matrix_multiplication" );

    // set arguments
    kernel.setArg ( 0, *getBuffer() );
    kernel.setArg ( 1, *(m_lOperand->getBuffer()) );
    kernel.setArg ( 2, *(m_rOperand->getBuffer()) );
    kernel.setArg<int> ( 3, getWidth() );
    int productDepth = m_lOperand->getWidth();
    kernel.setArg<int> ( 4, productDepth );

    // prepare dependencies
    std::vector<cl::Event> dependencies;
    dependencies.push_back ( m_lOperand->getEndOfEvaluation() );
    dependencies.push_back ( m_rOperand->getEndOfEvaluation() );

    //enqueue kernel execution
    cl_int error;
    error = m_queue->enqueueNDRangeKernel (
                kernel,
                cl::NullRange,
                cl::NDRange ( getWidth(), getHeight() ),
                cl::NDRange ( 1, 1 ),
                &dependencies,
                &getEndOfEvaluation()
                );

    if ( error != CL_SUCCESS )
        throw ( CLError ( error, "failed to enqueue kernel execution" ) );
}


MatrixSum::MatrixSum ( Matrix& A, Matrix& B,
                       Context *context,
                       cl::CommandQueue *queue ) :
    Matrix ( 0,
             A.getHeight(), A.getWidth(),
             context, queue ) {
    if( A.getWidth() != B.getWidth() || A.getHeight() != B.getHeight() )
        throw Error("Matrix dimensions are incompatible for matrix sum.");
    addChild(&A);
    addChild(&B);
    m_lOperand = &A;
    m_rOperand = &B;
}

void MatrixSum::enqueue(){
    // get kernel
    cl::Kernel kernel = m_context->getKernel ( "matrix_sum",
                                               "matrix_matrix_sum" );

    // set arguments
    kernel.setArg ( 0, *getBuffer() );
    kernel.setArg ( 1, *(m_lOperand->getBuffer()) );
    kernel.setArg ( 2, *(m_lOperand->getBuffer()) );
    kernel.setArg<int> ( 3, getHeight() );
    kernel.setArg<int> ( 4, getWidth() );

    // prepare dependencies
    std::vector<cl::Event> dependencies;
    dependencies.push_back ( m_lOperand->getEndOfEvaluation() );
    dependencies.push_back ( m_rOperand->getEndOfEvaluation() );

    //enqueue kernel execution
    cl_int error;
    error = m_queue->enqueueNDRangeKernel (
                kernel,
                cl::NullRange,
                cl::NDRange ( getWidth(), getHeight() ),
                cl::NDRange ( 1, 1 ),
                &dependencies,
                &getEndOfEvaluation()
                );

    if ( error != CL_SUCCESS )
        throw ( CLError ( error, "failed to enqueue kernel execution" ) );
}


MatrixNorm::MatrixNorm ( Matrix& A,
                         Context *context,
                         cl::CommandQueue *queue ) :
    Matrix ( 0,
             A.getHeight(), A.getWidth(),
             context, queue ) {
    addChild(&A);
    m_src = &A;
}

void MatrixNorm::enqueue(){
    // get kernel
    cl::Kernel kernel = m_context->getKernel ( "matrix_norm",
                                               "matrix_normalize" );

    // set arguments
    kernel.setArg ( 0, *getBuffer() );
    kernel.setArg ( 1, *(m_src->getBuffer()) );
    kernel.setArg<int> ( 2, getHeight() );
    kernel.setArg<int> ( 3, getWidth() );

    // prepare dependencies
    std::vector<cl::Event> dependencies;
    dependencies.push_back ( m_src->getEndOfEvaluation() );

    //enqueue kernel execution
    cl_int error;
    error = m_queue->enqueueNDRangeKernel (
                kernel,
                cl::NullRange,
                cl::NDRange ( getWidth() ),
                cl::NDRange ( 1 ),
                &dependencies,
                &getEndOfEvaluation()
                );

    if ( error != CL_SUCCESS )
        throw ( CLError ( error, "failed to enqueue kernel execution" ) );
}


MatrixCovariance::MatrixCovariance ( Matrix& A,
                                     Context *context,
                                     cl::CommandQueue *queue ) :
    Matrix ( 0,
             A.getHeight(), A.getHeight(),
             context, queue ) {
    addChild(&A);
    m_src = &A;
}

void MatrixCovariance::enqueue(){
    // get kernel
    cl::Kernel kernel = m_context->getKernel ( "matrix_norm",
                                               "matrix_matrix_sum" );

    //set arguments
    kernel.setArg ( 0, *getBuffer() );
    kernel.setArg ( 1, *(m_src->getBuffer()) );
    kernel.setArg<int> ( 2, m_src->getWidth() );
    kernel.setArg<int> ( 3, m_src->getHeight() );

    // prepare dependencies
    std::vector<cl::Event> dependencies;
    dependencies.push_back ( m_src->getEndOfEvaluation() );

    //enqueue kernel execution
    cl_int error;
    error = m_queue->enqueueNDRangeKernel (
                kernel,
                cl::NullRange,
                cl::NDRange ( getHeight(), getWidth() ),
                cl::NDRange ( 1, 1 ),
                &dependencies,
                &getEndOfEvaluation()
                );

    if ( error != CL_SUCCESS )
        throw ( CLError ( error, "failed to enqueue kernel execution" ) );
}
