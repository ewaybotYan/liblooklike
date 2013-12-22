/**
 *  @file   operation.h
 *  @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 *  @brief  Defines the needed objects for the manipulation of mathematic
 *          operations inside the OpenCL environment
 *  @note   Only virtual classes and internal objects are defines here.
 */

#ifndef OPERATION_H
#define OPERATION_H

#include<vector>
#include<CL/cl.hpp>

#include"context.h"

struct OperationResult{
    cl::Event computationEnd;
    cl::Buffer result;
};

class Operation{
  public:
    /// @param context a context for a valid OpenCL environment
    /// @param queue   the queue on which OpenCL commands will be put
    Operation( Context& context, cl::CommandQueue queue );

    /// @brief execute any preparation routine needed before evaluation
    virtual void prepare();

    /// @brief put any dependent computation on the execution queue and then
    ///        adds the execution on the execution queue
    /// @note  evaluate may fail if no memory space was allocated for the
    ///        object. It is up to the parent Expression evaluate function to
    ///        allocate memory and execute evaluate again.
    /// @note  evaluate might be a recursive function, if it depends on the
    ///        existence of other operations results
    virtual void evaluate();

  private:
    Context& context;
};

#endif //OPERATION_H
