#include "../include/clalgorithm.hpp"

ClAlgorithm::ClAlgorithm () {
}



cl::Event& ClAlgorithm::getEndOfEvaluation(){
    return m_endOfEvaluation;
}
