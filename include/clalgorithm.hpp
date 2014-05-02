/**
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @file   clalgorithm.hpp
 * @date   12/04/2014
 */

#ifndef CLALGORITHM_HPP
#define CLALGORITHM_HPP

#include"algorithm.h"
#include<vector>
#include<CL/cl.hpp>
#include<context.h>

/// OpenCL aware specialization of @ref Algorithm
class ClAlgorithm : public Algorithm {

  public:

    /// @return the OpenCL event associated to the end of the computation.
    /// @note this event is undefined unless the algorithm is enqueued
    ///       ( @ref isEnqueued() ).
    cl::Event& getEndOfEvaluation();

    /// @return context associated to the algorithm
    Context* getContext() const;

    /// @return command queue assigned to the algorithm
    cl::CommandQueue* getCommandQueue() const;

  protected:

    /// @param context the context that will be used for OpenCL operations
    /// @param queue the command queue that will hold OpenCL operations
    ClAlgorithm( Context* context, cl::CommandQueue* queue );

    /// list of the dependencies end of evaluation events
    /// @details can be filled by @ref enqueue() and used in cl::enqueue*
    ///          functions
    std::vector<cl::Event> m_dependenciesEvents;

  private:

    /// The cl::event associated to the end of the evaluation of this algorithm
    /// @warning This value must be set by @ref enqueue() implementations.
    cl::Event m_endOfEvaluationEvent;

    /// The context on which this algorithm will be run
    Context* m_context = 0;

    /// the queue that will recieve the commands used in the algorithm
    cl::CommandQueue* m_queue = 0;

};

#endif // CLALGORITHM_HPP
