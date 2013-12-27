#ifndef MATRIX_H
#define MATRIX_H

#include<CL/cl.hpp>

#include "../include/mathexpression.h"
#include "../include/context.h"

class Matrix: public MathExpression{
  public:
    Matrix( float* values, 
	    const unsigned int m, 
	    const unsigned int n,
	    const bool keepInCLMem = false );
    Matrix( const std::string programName, 
	    const std::string kernelName, 
	    const unsigned int m, 
	    const unsigned int n, 
	    const bool keepInCLMem = false );
     
    static Matrix mul( Matrix& A, Matrix& B, const bool keepInCLMem = false );
    static Matrix add( Matrix& A, Matrix& B, const bool keepInCLMem = false );
    static Matrix normalize( Matrix& A, const bool keepInCLMem = false );
    
    void retrieveData(Context& context, cl::CommandQueue& queue ) override;
    float getValue( const int i, const int j );
    float* getValues();
    int getWidth() const;
    int getHeight()const;
    
  protected:
    void enqueue( Context& context, cl::CommandQueue& queue ) override;
    bool allocateForResult( Context& context ) override;

  private:
    float* m_value;
    int m_m;
    int m_n;
    std::string m_programName;
    std::string m_kernelName;
};

#endif // MATRIX_H
