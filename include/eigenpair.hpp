/**
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @file   eigenpair.hpp
 * @date   14/04/2014
 */

#ifndef EIGENPAIR_HPP
#define EIGENPAIR_HPP

#include<CL/cl.hpp>
#include<memory>
#include<thread>
#include"algorithm.h"
#include"simplematrix.hpp"

class EigenPair : public Algorithm
{
  public:

    /// Performs computation of the eigen pair of a matrix ( eigen values and
    /// eigen vectors)
    EigenPair( std::shared_ptr< SimpleMatrix<cl_float> > m );

    /// @return a matrix with the eigen vectors in columns
    /// @note the ith column (= ith eigen vector) correspond to the ith column
    ///       of the eigenvalues given by @ref getVectors()
    std::shared_ptr< SimpleMatrix<cl_float> >& getVectors();

    /// @return a vector (matrix of width 1) with the eigenValues
    /// @note the ith eigen value correspond to the ith column of the vectors
    ///       matrix given by @ref getVectors()
    std::shared_ptr< SimpleMatrix<cl_float> >& getValues();

    void waitEndOfEvaluation() override;

    bool allocateTmpMem() override { return true; }

    void releaseTmpMem() override {}

    void enqueue();

  private:

    /// the function that performs the computation
    static void computeEigenPairs(EigenPair* a );

    std::shared_ptr< SimpleMatrix<cl_float> > m_src;
    std::shared_ptr< SimpleMatrix<cl_float> > m_vectors;
    std::shared_ptr< SimpleMatrix<cl_float> > m_values;

    /// the computation thead in which the evaluation is done
    std::thread m_ComputationThread;
};

#endif // EIGENPAIR_HPP

