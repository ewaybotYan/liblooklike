/**
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @file   clexpression.cpp
 * @date   12/04/2014
 */

#ifndef CLALGORITHM_HPP
#define CLALGORITHM_HPP

#include"algorithm.h"
#include<vector>
#include<CL/cl.hpp>
#include<context.h>


class ClAlgorithm : public Algorithm {

  public:

    /// Gives the OpenCL event associated to the end of the computation.
    /// @note You should check that computation is enqueued with
    ///       @ref getState() before waiting for this event.
    cl::Event& getEndOfEvaluation();

    Context* getContext() const;

    cl::CommandQueue* getCommandQueue() const;

  protected:

    ClAlgorithm( Context* context, cl::CommandQueue* queue );

  private:

    /// The end event associated to m_endOfEvaluation
    /// Every command started in OpenCL has an associated end event
    /// @note    This variable is most likely to be set by @ref enqueue
    /// @warning do not foget to set this value when you define
    ///          @ref enqueue()
    cl::Event m_endOfEvaluationEvent;

    /// The context on which this algorithm will be run
    Context* m_context = 0;

    /// the queue that will recieve the commands used in the algorithm
    cl::CommandQueue* m_queue = 0;
};

#endif // CLALGORITHM_HPP
