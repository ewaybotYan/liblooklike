#include<CL/cl.hpp>
#include<context.h>
#include "../include/clalgorithm.hpp"



cl::Event& ClAlgorithm::getEndOfEvaluation()
{
    return m_endOfEvaluationEvent;
}

ClAlgorithm::ClAlgorithm(Context *context, cl::CommandQueue *queue )
{
    m_context = context;
    m_queue = queue;
}

Context* ClAlgorithm::getContext() const
{
  return m_context;
}

cl::CommandQueue* ClAlgorithm::getCommandQueue() const
{
  return m_queue;
}
