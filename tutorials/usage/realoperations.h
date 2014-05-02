/**
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @file   realoperations.h
 * @date   02/05/2014
 */

#ifndef REALOPERATIONS_H
#define REALOPERATIONS_H

#include <algorithm.h>
#include "real.h"

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
};

#endif // REALOPERATIONS_H
