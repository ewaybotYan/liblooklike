#include<CL/cl.hpp>
#include<context.h>
#include "../include/clalgorithm.hpp"


ClAlgorithm::ClAlgorithm(Context *context, cl::CommandQueue *queue ){
    m_context = context;
    m_queue = queue;
}

void ClAlgorithm::waitEndOfEvaluation(){
    m_endOfEvaluationEvent.wait();
    retrieveData();
}

cl::Event& ClAlgorithm::getEndOfEvaluation(){
    return m_endOfEvaluationEvent;
}
