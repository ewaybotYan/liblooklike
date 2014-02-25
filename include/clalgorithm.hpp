#ifndef CLALGORITHM_HPP
#define CLALGORITHM_HPP

#include"algorithm.h"
#include<CL/cl.hpp>

/// @warning do not forget to update m_computationEndEvent when you implement
///          @ref evaluate()
class ClAlgorithm : protected Algorithm {
    public:

        /// @detailed For a terminal node, this will most likely enqueue a
        ///            writeBuffer operation. Otherwise, this will ask the
        ///            @ref Context object for one or more kernels, set the
        ///            arguments accordingly and enqueue execution of the
        ///            algorithms.

        /// @brief   Must be called to bring back the computed results from
        ///          the computation device to the program memory.
        /// @warning Calling retrieveData before computation is over has an
        ///          undefined behaviour.
        virtual void retrieveData( ) = 0;

        /// Gives the OpenCL event associated to the end of the computation.
        /// @note You should check that computation is enqueued with
        ///       @ref getState() before waiting for this event.
        cl::Event& getEndOfEvaluation();
    private:

        /// The context on which this algorithm will be run
        Context& context;
        /// the queue that will recieve the commands used in the algorithm
        cl::CommandQueue& queue;

        ///         /// the event associated to the computation in the openCL queue
        /// This variable is most likely to be set by @ref enqueue
        cl::Event m_endOfEvaluationEvent;

};

#endif // CLALGORITHM_HPP
