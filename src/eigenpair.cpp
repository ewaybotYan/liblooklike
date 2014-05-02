/**
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @file   eigenpair.cpp
 * @date   14/04/2014
 */

#ifndef NDEBUG
#include <iostream>
#endif
#include "../include/eigenpair.hpp"
#include "../include/exception.h"
#include <armadillo>

using namespace std;

EigenPair::EigenPair(std::shared_ptr< SimpleMatrix<cl_float> > m)
{
  if( m->getHeight() != m->getWidth() )
    throw InvalidAlgorithmParameter(
        "Cannot compute eigen-pairs on non-square matrix");
  m_src = m;
  addDependency(m.get());
  m_vectors = make_shared< SimpleMatrix<cl_float> >( m_src->getHeight(),
                                                     m_src->getWidth() );
  addResult( m_vectors.get() );
  m_values = make_shared< SimpleMatrix<cl_float> >( m_src->getHeight(),
                                                    m_src->getWidth() );
  addResult( m_values.get() );
}

void EigenPair::waitEndOfEvaluation()
{
  if ( !Algorithm::isEnqueued() )
    throw EvaluationProcessViolation(
        "Cannot wait for evaluation as it is not enqued.");

  if( m_ComputationThread.joinable() )
    m_ComputationThread.join();
}

std::shared_ptr< SimpleMatrix<cl_float> >& EigenPair::getVectors()
{
  return m_vectors;
}

std::shared_ptr< SimpleMatrix<cl_float> >& EigenPair::getValues()
{
  return m_values;
}

void EigenPair::computeEigenPairs(EigenPair* a)
{
  // wrap matrices in armadillo matrices
  arma::fmat src( a->m_src->getValues(),
                  a->m_src->getHeight(),
                  a->m_src->getWidth(),
                  false, true );
#ifndef NDEBUG
  cout << a->getValues()->getHeight() << endl;
#endif
  arma::fvec eigval( a->getValues()->getValues(),
                     a->getValues()->getHeight(),
                     false, true); // use auxiliary memory and disable resize
  arma::fmat eigvec( a->getVectors()->getValues(),
                     a->getValues()->getHeight(),
                     a->getValues()->getWidth(),
                     false, true );

  // perform eigen pairs computation
  arma::eig_sym(eigval, eigvec, src, "dc");

  a->getValues()->resizeWidth(1);
}

void EigenPair::enqueue()
{
  // wait for dependencies to be computed
  for (auto child : m_dependencies) {
    child->waitEndOfEvaluation();
  }

  // start eigen pairs computation in a separate thread
  m_ComputationThread = std::thread( EigenPair::computeEigenPairs, this );
}
