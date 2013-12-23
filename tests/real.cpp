#include "../include/exception.h"
#include "../include/mathexpression.h"

#include "real.h"

const int maxNbAllocations = 4;
int nbAllocations = 0;

// ##############
// # constructors

Real::Real( const float value, const bool keepInCLMem ) : MathExpression( true, keepInCLMem ){
  m_value = new float;
  *m_value = value;
}

Real Real::sum( Real& a, Real& b, const bool keepInCLMem ) : MathExpression( false, keepInCLMem ){
  Real result;
  result.setProgramName( "reals" );
  result.setKernelName( "real_real_addition" );
}

// #####################
// # getters and setters

void Real::getValue(){
  if( isComputed() ){
    if( getState() > QUEUED ){
      getEndOfEvaluation().wait();
      retrieveData();
    }else{
      throw Error("Cannot get value as it has not been computed");
    }
  }else{
    
  }
}

void Real::setProgramName(const std::string programName){
  m_programName = programName;
}


void Real::setKernelName(const std::string kernelName){
  m_kernelName = kernelName;
}


