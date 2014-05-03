/**
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @file   real.h
 * @date   02/05/2014
 */

#ifndef REAL_H
#define REAL_H

#include <expression.hpp>

class Real : public Expression
{
  public:

    Real();
    Real( float value );
    ~Real();

    float* getValue();

    void print();

  protected:
    float* m_value = nullptr;

    bool allocateMemImpl() override;
    void releaseMemImpl() override;
};

#endif // REAL_H
