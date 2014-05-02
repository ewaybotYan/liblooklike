/**
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @file   clmatrixoperations.cpp
 * @date   13/04/2014
 */


#include "../include/exception.h"
#include "../include/clmatrixoperations.hpp"
#ifndef NDEBUG
 #include <iostream>

#endif
using namespace std;


// #################
// # ClMatrixProduct

CLMatrixProduct::CLMatrixProduct( std::shared_ptr<CLMatrix> lop,
                                  std::shared_ptr<CLMatrix> rop,
                                  Context *context, cl::CommandQueue *queue,
                                  bool transposeLop )
  : ClAlgorithm ( context, queue )
{
  m_lop = lop;
  addDependency(lop.get());
  m_rop = rop;
  addDependency(rop.get());
  if ( transposeLop ) {
    m_result = std::make_shared<CLMatrix>( lop->getWidth(), rop->getWidth() );
  } else {
    m_result = std::make_shared<CLMatrix>( lop->getHeight(), rop->getWidth() );
  }
  addResult(m_result.get());
  m_transposeLop = transposeLop;
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
  cl::Kernel kernel;
  if( m_transposeLop ){
    kernel = getContext()->getKernel(
               "matrix_mult",
               "matrix_transpose_matrix_multiplication" );
  }else{
    kernel = getContext()->getKernel(
               "matrix_mult",
               "matrix_matrix_multiplication" );
  }

  // set arguments
  kernel.setArg ( 0, *(m_result->getValues().get()) );
  kernel.setArg ( 1, *(m_lop->getValues().get()) );
  kernel.setArg ( 2, *(m_rop->getValues().get()) );
  kernel.setArg<int> ( 3, m_result->getHeight() );
  int productDepth = m_rop->getHeight();
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
  kernel.setArg<int> ( 2, m_src->getWidth() );
  kernel.setArg<int> ( 3, m_src->getHeight() );

  // prepare dependencies
  m_dependenciesEvents.push_back(
        ((ClAlgorithm*)m_src->getParentAlgorithm())->getEndOfEvaluation() );

  //enqueue kernel execution
  cl_int error;
  error = getCommandQueue()->enqueueNDRangeKernel (
            kernel,
            cl::NullRange,
            cl::NDRange ( m_result->getHeight(), m_result->getWidth() ),
            cl::NDRange ( 1, 1 ),
            &m_dependenciesEvents,
            &(getEndOfEvaluation())
            );

  if ( error != CL_SUCCESS )
    throw ( CLError ( error, "failed to enqueue kernel execution" ) );
}


// ###############
// # CLMatrixScale

CLMatrixScale::CLMatrixScale( std::shared_ptr<CLMatrix> m,
                              std::shared_ptr<CLMatrix> v,
                              Context *context,
                              cl::CommandQueue *queue )
  : ClAlgorithm ( context, queue )
{
  if( m->getHeight() != v->getHeight() )
    throw InvalidAlgorithmParameter("Dimensions mismatch.");
  m_src = m;
  addDependency(m.get());
  m_coeffs = v;
  addDependency(v.get());
  m_result = std::make_shared<CLMatrix>( m->getHeight(), m->getWidth() );
  addResult(m_result.get());
}

void CLMatrixScale::waitEndOfEvaluation()
{
  if ( !isEnqueued() ) {
    throw EvaluationProcessViolation(
          "Cannot wait for evaluation as it is not enqued.");
  } else {
    getEndOfEvaluation().wait();
  }
}

std::shared_ptr< CLMatrix > CLMatrixScale::getResult()
{
  return m_result;
}

void CLMatrixScale::enqueue()
{
  // get kernel
  cl::Kernel kernel = getContext()->getKernel("matrix_mult",
                                              "matrix_matrix_scale" );

  // set arguments
  kernel.setArg ( 0, *(m_result->getValues().get()) );
  kernel.setArg ( 1, *(m_src->getValues().get()) );
  kernel.setArg ( 2, *(m_coeffs->getValues().get()) );
  kernel.setArg ( 3, m_src->getHeight() );

  // prepare dependencies
  m_dependenciesEvents.push_back(
        ((ClAlgorithm*)m_src->getParentAlgorithm())->getEndOfEvaluation() );
  m_dependenciesEvents.push_back(
        ((ClAlgorithm*)m_coeffs->getParentAlgorithm())->getEndOfEvaluation() );

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
