#include "../include/exception.h"
#include "../include/clmatrixnorm.hpp"

CLMatrixNorm::CLMatrixNorm(std::shared_ptr<CLMatrix> A,
                           Context *context,
                           cl::CommandQueue *queue,
                           bool onCols ) :
  ClAlgorithm ( context, queue )
{
  // set child expression as usual
  m_src = A;
  addDependency(m_src.get());
  m_normalizationOnCols = onCols;
  m_normalized = std::make_shared<CLMatrix>( A->getHeight(), A->getWidth() );
  addResult(m_normalized.get());
  m_normCoeffs = std::make_shared<CLMatrix>(
                   onCols? A->getWidth():A->getHeight(), 1 );
  addResult(m_normCoeffs.get());
}

void CLMatrixNorm::waitEndOfEvaluation()
{
  if ( !isEnqueued() ) {
    throw EvaluationProcessViolation(
          "Cannot wait for evaluation as it is not enqued.");
  } else {
    getEndOfEvaluation().wait();
  }
}

std::shared_ptr< CLMatrix > CLMatrixNorm::getNormalizedMatrix()
{
  return m_normalized;
}

std::shared_ptr< CLMatrix > CLMatrixNorm::getNormCoeffs(){
  return m_normCoeffs;
}

void CLMatrixNorm::enqueue(){
  // get kernel
  cl::Kernel kernel;
  if(m_normalizationOnCols){
    kernel = getContext()->getKernel ( "matrix_norm",
                                    "matrix_normalize_cols" );
  }else{
    kernel = getContext()->getKernel ( "matrix_norm",
                                    "matrix_normalize_rows" );
  }

  // set arguments
  kernel.setArg ( 0, *(m_normalized->getValues().get()) );
  kernel.setArg ( 1, *(m_normCoeffs->getValues().get()) );
  kernel.setArg ( 2, *(m_src->getValues().get()) );
  kernel.setArg<int> ( 3, m_src->getHeight() );
  kernel.setArg<int> ( 4, m_src->getWidth() );

  // prepare dependencies
  m_dependenciesEvents.push_back(
        ((ClAlgorithm*)m_src->getParentAlgorithm())->getEndOfEvaluation() );

  //enqueue kernel execution
  int size;
  if( m_normalizationOnCols ){
    size = m_src->getWidth();
  }else{
    size = m_src->getHeight();
  }
  cl_int error;
  error = getCommandQueue()->enqueueNDRangeKernel (
            kernel,
            cl::NullRange,
            cl::NDRange ( size ),
            cl::NDRange ( 1 ),
            &m_dependenciesEvents,
            &(getEndOfEvaluation())
            );

  if ( error != CL_SUCCESS )
    throw ( CLError ( error, "failed to enqueue kernel execution" ) );
}
