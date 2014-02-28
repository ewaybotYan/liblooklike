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

void ClAlgorithm::setEndOfEvaluation( cl::Event event ){
    m_endOfEvaluationEvent = event;
}

void ClAlgorithm::addExecutionDependence( cl::Event event ){
    m_dependencies.push_back(event);
}

void ClAlgorithm::clearExecutionDependencies(){
    m_dependencies.clear();
}

std::vector<cl::Event>* ClAlgorithm::getExecutionDependencies(){
    return &m_dependencies;
}
