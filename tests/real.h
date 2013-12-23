#ifndef REAL_H
#define REAL_H

#include<string>

#include "../include/mathexpression.h"

class Real : protected MathExpression{
  public:
    Real( const float value, const bool keepInCLMem = false );
    Real sum( Real& a, Real& b, const bool keepInCLMem = false );
    float getValue() const;
     
  protected:
    void setProgramName( const std::string programName );
    void setKernelName( const std::string kernelName );
    void enqueue();
    bool allocateForResult();
    void deallocateForResult();

  private:
    float* m_value;
    std::string m_programName;
    std::string m_kernelName;
};

#endif
