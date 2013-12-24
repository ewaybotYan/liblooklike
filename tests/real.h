#ifndef REAL_H
#define REAL_H

#include<string>

#include "../include/mathexpression.h"
#include "../include/context.h"

class Real : public MathExpression{
  public:
    Real( const float value, const bool keepInCLMem = false );
    Real( const bool keepInCLMem, const std::string programName, const std::string kernelName  );
     ~Real(){};
    static Real sum( Real& a, Real& b, const bool keepInCLMem = false );
    float getValue( );
    /// nearly an alias of @ref retrieveData, creates a command queue before
    void retrieveData( Context& context );
    void retrieveData(Context& context, cl::CommandQueue& queue ) override;

  protected:
    void setProgramName( const std::string programName );
    void setKernelName( const std::string kernelName );
    void enqueue( Context& context, cl::CommandQueue& queue ) override;
    bool allocateForResult( Context& context ) override;
    void deallocateForResult() override;

  private:
    float* m_value;
    std::string m_programName;
    std::string m_kernelName;
};

#endif