/**
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @file   realoperations.h
 * @date   02/05/2014
 */

#ifndef REALOPERATIONS_H
#define REALOPERATIONS_H

#include <algorithm.h>
#include "real.h"
#include <thread>

class Sum : public Algorithm
{
  public:
    Sum( Real& lOperand, Real& rOperand );

    Real& getResult();

    void waitEndOfEvaluation() override;

  protected:
    bool allocateTmpMem() override;
    void releaseTmpMem() override;

    void enqueue() override;

  private:
    Real* m_lop;
    Real* m_rop;
    Real m_result;

    // In order to have enqueue as a non blocking function, we need to
    // use a thread, it will then be used to wait for the end of the
    // computation.
    std::thread m_thread;

    // It is convenient to put the computation function as a thread, because it
    // will have access to the private memebers of the class
    // the argument is the object itself when it start s the execution.
    static void add(Sum* s);
};

#endif // REALOPERATIONS_H
