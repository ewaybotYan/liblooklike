/**
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @file   clmatrix.cpp
 * @date   12/04/2014
 */

#include "../include/exception.h"
#include "../include/clmatrix.hpp"


// ##################
// # CLMatrix methods

CLMatrix::CLMatrix( const unsigned int m, const unsigned int n )
{
  m_m = m;
  m_n = n;
}

int CLMatrix::getWidth() const
{
  return m_n;
}

int CLMatrix::getHeight() const
{
  return m_m;
}

std::shared_ptr<cl::Buffer> CLMatrix::getValues() const
{
  if( m_values.get() == nullptr )
    throw EvaluationProcessViolation(
        "Cannot return CL buffer as it is not allocated");
  return m_values;
}

bool CLMatrix::allocateMemImpl()
{
  cl_int error;
  cl_mem_flags memFlags = 0;
  memFlags = CL_MEM_READ_WRITE;
  cl::Buffer* buf = new cl::Buffer(
                      ((ClAlgorithm*)m_computedBy)->getContext()->getContext(),
                      memFlags,
                      sizeof ( cl_float ) * m_m * m_n,
                      0,
                      &error);
  m_values = std::shared_ptr< cl::Buffer >( buf );
  return error == CL_SUCCESS;
}

void CLMatrix::releaseMemImpl()
{
  m_values.reset();
}


// ##########################
// # Basic Algorithms methods

CLMatrixLoader::CLMatrixLoader( std::shared_ptr< SimpleMatrix<cl_float> > source,
                                Context *context,
                                cl::CommandQueue *queue )
  : ClAlgorithm ( context, queue )
{
  m_src = source;
  addDependency(m_src.get());
  m_result = std::make_shared< CLMatrix >( m_src->getHeight(),
                                          m_src->getWidth() );
  addResult(m_result.get());
}

void CLMatrixLoader::waitEndOfEvaluation()
{
  if ( !isEnqueued() ) {
    throw EvaluationProcessViolation(
          "Cannot wait for evaluation as it is not enqued.");
  } else {
    getEndOfEvaluation().wait();
  }
}

std::shared_ptr< CLMatrix > CLMatrixLoader::getResult()
{
  return m_result;
}

void CLMatrixLoader::enqueue()
{
  m_src->waitEndOfEvaluation();

  //enqueue transfer execution
  cl_int error;
  error = getCommandQueue()->enqueueWriteBuffer (
            *(m_result->getValues()),
            CL_FALSE,
            0,
            sizeof ( cl_float ) * m_src->getWidth() * m_src->getHeight(),
            m_src->getValues(),
            0,
            &(getEndOfEvaluation())
            );

  if ( error != CL_SUCCESS )
    throw ( CLError ( error, "failed to enqueue memory transfer" ) );
}


CLMatrixUnloader::CLMatrixUnloader(std::shared_ptr<CLMatrix> source,
                                   Context *context,
                                   cl::CommandQueue *queue )
  : ClAlgorithm ( context, queue )
{
  m_src = source;
  addDependency(m_src.get());
  m_result = std::make_shared< SimpleMatrix<cl_float> >( m_src->getHeight(),
                                                         m_src->getWidth() );
  addResult(m_result.get());
}

void CLMatrixUnloader::waitEndOfEvaluation()
{
  if ( !isEnqueued() ) {
    throw EvaluationProcessViolation(
          "Cannot wait for evaluation as it is not enqued.");
  } else {
    getEndOfEvaluation().wait();
  }
}

std::shared_ptr< SimpleMatrix<cl_float> > CLMatrixUnloader::getResult()
{
  return m_result;
}

void CLMatrixUnloader::enqueue()
{
  // prepare dependencies
  ClAlgorithm* dependency = (ClAlgorithm*)(m_src->getParentAlgorithm());
  m_dependenciesEvents.push_back(dependency->getEndOfEvaluation());

  //enqueue transfer execution
  cl_int error;
  error = getCommandQueue()->enqueueReadBuffer(
            *(m_src->getValues().get()),
            CL_FALSE,
            0,
            sizeof ( cl_float ) * m_src->getWidth() * m_src->getHeight(),
            m_result->getValues(),
            &m_dependenciesEvents,
            &(getEndOfEvaluation())
            );
  if ( error != CL_SUCCESS )
    throw ( CLError ( error, "failed to enqueue memory transfer" ) );
}
