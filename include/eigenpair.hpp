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

    EigenPair( std::shared_ptr< SimpleMatrix<cl_float> > m );

    std::shared_ptr< SimpleMatrix<cl_float> >& getVectors();
    std::shared_ptr< SimpleMatrix<cl_float> >& getValues();

    void waitEndOfEvaluation() override;

    bool allocateTmpMem() override { return true; }

    void releaseTmpMem() override {}

    void enqueue();

  private:

    static void computeEigenPairs(EigenPair* a );

    std::shared_ptr< SimpleMatrix<cl_float> > m_src;
    std::shared_ptr< SimpleMatrix<cl_float> > m_vectors;
    std::shared_ptr< SimpleMatrix<cl_float> > m_values;

    std::thread m_ComputationThread;
};

#endif // EIGENPAIR_HPP

