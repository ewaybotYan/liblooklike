/**
 * @file   expression.h
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @brief  The evaluation of a whole expression is handled by an Expression
 *         object. It makes it easier to understand the process.
 */

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include<CL/cl.hpp>

class Expression{
  public:
    Expression();

    /// @brief compute the value of the Expression
    /// @note  this is a synchronous method, will return when computation is
    ///        over or if it has ended.
    virtual bool evaluate();

  protected:
    /// @brief put any dependent computation on the execution queue and then
    ///        adds the execution on the execution queue
    /// @note  evaluate may fail if no memory space was allocated for the
    ///        object. It is up to the parent Expression evaluate function to
    ///        allocate memory and execute evaluate again.
    /// @note  evaluate might be a recursive function, if it depends on the
    ///        existence of other operations results
    virtual void tryEvaluate();

    /// @brief  try to allocate memory on the opencl device for this element
    /// @return true if allocation succeeded, false otherwise
    virtual bool allocateMemory();

  private:
    /// data location in openCL memory
    cl::Buffer data;
    /// the event associated to the computation in the openCL queue
    cl::Event  computationState;
    /// dependencies requiered to compute this
    cl::vector<Expression> children;
};

#endif
