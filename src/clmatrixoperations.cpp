/**
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @file   clmatrixoperations.cpp
 * @date   13/04/2014
 */


#include "../include/exception.h"
#include "../include/clmatrixoperations.hpp"


CLMatrixProduct::CLMatrixProduct(std::shared_ptr<CLMatrix> lop,
                                 std::shared_ptr<CLMatrix> rop,
                                 Context *context, cl::CommandQueue *queue)
  : ClAlgorithm ( context, queue )
{
  m_lop = lop;
  addDependency(lop.get());
  m_rop = rop;
  addDependency(rop.get());
  m_result = std::make_shared<CLMatrix>( lop->getHeight(), rop->getWidth() );
  addResult(m_result.get());
}

void CLMatrixProduct::waitEndOfEvaluation()
{
  if ( !isEnqueued() ) {
    throw EvaluationProcessViolation(
          "Cannot wait for evaluation as it is not enqued.");
  } else {
    getEndOfEvaluation().wait();
  }
}

std::shared_ptr< CLMatrix > CLMatrixProduct::getResult()
{
  return m_result;
}

void CLMatrixProduct::enqueue()
{
  // get kernel
  cl::Kernel kernel = getContext()->getKernel("matrix_mult",
                                              "matrix_matrix_multiplication" );

  // set arguments
  kernel.setArg ( 0, *(m_result->getValues().get()) );
  kernel.setArg ( 1, *(m_lop->getValues().get()) );
  kernel.setArg ( 2, *(m_rop->getValues().get()) );
  kernel.setArg<int> ( 3, m_result->getHeight() );
  int productDepth = m_lop->getWidth();
  kernel.setArg<int> ( 4, productDepth );

  // prepare dependencies
  m_dependenciesEvents.push_back(
        ((ClAlgorithm*)m_lop->getParentAlgorithm())->getEndOfEvaluation() );
  m_dependenciesEvents.push_back(
        ((ClAlgorithm*)m_rop->getParentAlgorithm())->getEndOfEvaluation() );

  //enqueue kernel execution
  cl_int error;
  error = getCommandQueue()->enqueueNDRangeKernel (
            kernel,
            cl::NullRange,
            cl::NDRange ( m_result->getWidth(), m_result->getHeight() ),
            cl::NDRange ( 1, 1 ),
            &m_dependenciesEvents,
            &(getEndOfEvaluation())
            );

  if ( error != CL_SUCCESS )
    throw ( CLError ( error, "failed to enqueue kernel execution" ) );
}


// ####################
// # CLMatrixCovariance

CLMatrixCovariance::CLMatrixCovariance(std::shared_ptr<CLMatrix> m,
                                       Context *context, cl::CommandQueue *queue)
  : ClAlgorithm ( context, queue )
{
  m_src = m;
  addDependency(m.get());
  m_result = std::make_shared<CLMatrix>( m->getWidth(), m->getWidth() );
  addResult(m_result.get());
}

void CLMatrixCovariance::waitEndOfEvaluation()
{
  if ( !isEnqueued() ) {
    throw EvaluationProcessViolation(
          "Cannot wait for evaluation as it is not enqued.");
  } else {
    getEndOfEvaluation().wait();
  }
}

std::shared_ptr< CLMatrix > CLMatrixCovariance::getResult()
{
  return m_result;
}

void CLMatrixCovariance::enqueue()
{
  // get kernel
  cl::Kernel kernel = getContext()->getKernel("matrix_mult",
                                              "matrix_covariance" );

  // set arguments
  kernel.setArg ( 0, *(m_result->getValues().get()) );
  kernel.setArg ( 1, *(m_src->getValues().get()) );
  kernel.setArg<int> ( 2, m_result->getWidth() );
  kernel.setArg<int> ( 3, m_result->getHeight() );

  // prepare dependencies
  m_dependenciesEvents.push_back(
        ((ClAlgorithm*)m_src->getParentAlgorithm())->getEndOfEvaluation() );

  //enqueue kernel execution
  cl_int error;
  error = getCommandQueue()->enqueueNDRangeKernel (
            kernel,
            cl::NullRange,
            cl::NDRange ( m_result->getWidth(), m_result->getHeight() ),
            cl::NDRange ( 1, 1 ),
            &m_dependenciesEvents,
            &(getEndOfEvaluation())
            );

  if ( error != CL_SUCCESS )
    throw ( CLError ( error, "failed to enqueue kernel execution" ) );
}
