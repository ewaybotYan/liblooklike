#ifndef REAL_H
#define REAL_H

#include<string>

#include "../include/algorithm.h"
#include "../include/context.h"

// Real is an implementation of the Mathexpression class with the (simplified)
// behaviour of a real number.
class Real : public Algorithm{

  public:

    // Most expressions will provide constructors that permit the creation of an
    // object with a value. It the evaluation tree of an expression, these
    // instances will be the terminal nodes.
    Real( const float value, const bool keepInCLMem = false );

    Real( const bool keepInCLMem, const std::string programName, const std::string kernelName  );

    ~Real(){}

    // Then comes expression generators that create expressions as the result
    // of an operation or an algorithm.
    static Real sum( Real& a, Real& b, const bool keepInCLMem = false );
    static Real mul( Real& a, Real& b, const bool keepInCLMem = false );

    // An implementation specific method be be used to access the value(s) of
    // the expression from the program.
    float getValue();

    // virtual method from Algorithm, see real.cpp
    void retrieveData(Context& context, cl::CommandQueue& queue ) override;

  protected:

    // virtual methods from Algorithm, see real.cpp
    void enqueue( Context& context, cl::CommandQueue& queue ) override;
    bool allocateForResult( Context& context ) override;

  private:

    // The implementation of Algorithm is likely to have its own memory
    // space in order to store its initial or computed value in memory.
    float* m_value;

    // One way to distinguish the type of expression is to store the program
    // and kernel that will be used for the computation. This is only valid if
    // all computations use their own specific kernel.
    std::string m_programName;
    std::string m_kernelName;
};

#endif
