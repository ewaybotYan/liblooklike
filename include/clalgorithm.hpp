#ifndef CLALGORITHM_HPP
#define CLALGORITHM_HPP

#include"algorithm.h"
#include<CL/cl.hpp>
#include<context.h>


class ClAlgorithm : public Algorithm {

    public:

        ClAlgorithm(){}

        /// @param context the context to be used by OpenCL functions of this
        ///                object
        /// @param queue   a queue that will be used to put the OpenCL commands
        ClAlgorithm( Context* context, cl::CommandQueue* queue );

        /// The default behaviour is to wait for the end event defined in
        /// @ref m_endOfEvaluationEvent and then retrieve the data from the
        /// OpenCL environment with @ref retrieveData() .
        virtual void waitEndOfEvaluation() override;

        /// @brief   Bring back the result of the OpenCL computation in computer
        ///          memory.
        /// @warning Calling retrieveData before computation has an undefined
        ///          behaviour.
        virtual void retrieveData() = 0;

        /// Gives the OpenCL event associated to the end of the computation.
        /// @note You should check that computation is enqueued with
        ///       @ref getState() before waiting for this event.
        cl::Event& getEndOfEvaluation();

        /// The context on which this algorithm will be run
        Context* m_context = 0;

        /// the queue that will recieve the commands used in the algorithm
        cl::CommandQueue* m_queue = 0;
    private:

        /// The end event associated to m_endOfEvaluation
        /// Every command started in OpenCL has an associated end event
        /// @note    This variable is most likely to be set by @ref enqueue
        /// @warning do not foget to set this value when you define
        ///          @ref enqueue()
        cl::Event m_endOfEvaluationEvent;

};

#endif // CLALGORITHM_HPP
