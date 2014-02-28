#include "../include/matrixnorm.hpp"

MatrixNorm::MatrixNorm ( Matrix& A,
                         Context *context,
                         cl::CommandQueue *queue ) :
    ClAlgorithm ( context, queue ) {
    addChild(&A);
    m_src = &A;
    m_normalized = new Matrix( A.getHeight(), A.getWidth(),
                                       context, queue );
    m_normCoeffs = new Matrix( A.getHeight(), 1,
                                       context, queue );
}

int MatrixNorm::getWidth() const{
    return m_src->getWidth();
}

int MatrixNorm::getHeight() const{
    return m_src->getHeight();
}

Matrix* MatrixNorm::getNormalizedMatrix(){
    return m_normalized;
}

Matrix* MatrixNorm::getNormCoeffs(){
    return m_normCoeffs;
}

void MatrixNorm::retrieveData() {
    cl_int error;

    float* normalizedData = new cl_float[getWidth()*getHeight()];
    error = m_queue->enqueueReadBuffer (
                *m_normalized->getBuffer(),
                CL_TRUE,
                0,
                sizeof ( cl_float ) * getHeight() * getWidth(),
                normalizedData );
    if ( error != CL_SUCCESS )
        throw ( CLError ( error, "failed to enqueue data reading" ) );
    m_normalized->setResult(normalizedData);

    float* coefficients = new cl_float[getWidth()];
    error = m_queue->enqueueReadBuffer (
                *m_normCoeffs->getBuffer(),
                CL_TRUE,
                0,
                sizeof ( cl_float ) * getWidth(),
                coefficients );
    if ( error != CL_SUCCESS )
        throw ( CLError ( error, "failed to enqueue data reading" ) );
    m_normCoeffs->setResult(coefficients);
}

void MatrixNorm::enqueue(){
    // get kernel
    cl::Kernel kernel = m_context->getKernel ( "matrix_norm",
                                               "matrix_normalize" );

    // set arguments
    kernel.setArg ( 0, *(m_normalized->getBuffer()) );
    kernel.setArg ( 1, *(m_normCoeffs->getBuffer()) );
    kernel.setArg ( 2, *(m_src->getBuffer()) );
    kernel.setArg<int> ( 3, getHeight() );
    kernel.setArg<int> ( 4, getWidth() );

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

// todo : use try catch and return proper value
bool MatrixNorm::allocateForResult(){
    m_normalized->evaluate(); // this will only allocate memory
    m_normCoeffs->evaluate();
    return true;
}

void MatrixNorm::deallocateForResult(){
/*    m_normalized->deallocateForResult();
    m_normCoeffs->deallocateForResult();*/
}

#ifndef NDEBUG
void MatrixNorm::print(){
    m_normalized->print();
    std::cout << "\n";
    m_normCoeffs->print();
}
#endif
