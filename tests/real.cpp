#include "../include/exception.h"
#include "../include/mathexpression.h"

#include "real.h"
#include "../include/context.h"

const int maxNbAllocations = 4;
int nbAllocations = 0;

// ##############
// # constructors

Real::Real( const float value, const bool keepInCLMem ) : MathExpression( true, keepInCLMem ) {
    m_value = new float;
    *m_value = value;
}

Real::Real( const bool keepInCLMem, const std::string programName, const std::string kernelName  ) : MathExpression( false, keepInCLMem ) {
    m_value = 0;
    m_programName = programName;
    m_kernelName = kernelName;
}

Real Real::sum( Real& a, Real& b, const bool keepInCLMem ) {
    Real result( keepInCLMem, "test_real", "real_real_add" );
    result.addChild(&a);
    result.addChild(&b);
    return result;
}

// #####################
// # getters and setters

float Real::getValue() {
    if( isComputed() ) {
        if( getState() > QUEUED ) {
            getEndOfEvaluation().wait();
            retrieveData();
            return *m_value;
        } else {
            throw Error("Cannot get value as it has not been computed");
        }
    } else {
        return 0;
    }
}

void Real::setProgramName(const std::string programName) {
    m_programName = programName;
}


void Real::setKernelName(const std::string kernelName) {
    m_kernelName = kernelName;
}


// ###################
// # memory operations

bool Real::allocateForResult( Context& context ) {
  #ifndef NDEBUG
  std::cout << "allocating for result" << std::endl;
#endif
    if(m_data.size() != 0)//buffer already created
        return true;

    cl_int error;
    cl_mem_flags memFlags = 0;
    memFlags = CL_MEM_READ_WRITE;
    m_data.push_back( cl::Buffer(
                          context.getContext(),
                          memFlags,
                          sizeof(float),
                          NULL,
                          &error
                      ));
    if(error != CL_SUCCESS) {
        return false;
    } else {
        m_state = ALLOCATED;
        return true;
    }
}

void Real::deallocateForResult() {
  #ifndef NDEBUG
  std::cout << "deallocating for result" << std::endl;
#endif
    if( getState() == ALLOCATED ) {
        m_data.pop_back();
        m_state = INITIAL;
    }
}


// #########
// # methods

void Real::enqueue( Context& context, cl::CommandQueue& queue ) {
    if( isComputed() ) {
      #ifndef NDEBUG
  std::cout << "enqueing computation kernel" << std::endl;
#endif
        // create kernel
        cl::Kernel kernel = context.getKernel( m_programName, m_kernelName );
        kernel.setArg(0, m_data[0]);
        kernel.setArg(1, m_children[0]->getData()[0]);
        kernel.setArg(2, m_children[1]->getData()[0]);
// prepare dependencies
        std::vector<cl::Event> dependencies;
        for( MathExpression* child : m_children )
            dependencies.push_back((child->getEndOfEvaluation()));
        //enqueue kernel execution
        cl_int error;
        error = queue.enqueueNDRangeKernel(
                    kernel,
                    cl::NullRange,
                    cl::NDRange(1),
                    cl::NDRange(1, 1),
                    &dependencies,
                    &m_endOfEvaluation
                );
        if( error != CL_SUCCESS )
            throw( CLError(error, "failed to enqueue kernel execution") );
    } else { // this is a terminal expression, just load data in the buffer
      #ifndef NDEBUG
  std::cout << "enquing data load" << std::endl;
#endif
      cl_int error;
        error = queue.enqueueWriteBuffer(
                    getData()[0],
                    CL_FALSE,
                    0,
		    sizeof(float),
                    m_value,
                    0,
                    &getEndOfEvaluation()
                );
	 if( error != CL_SUCCESS )
            throw( CLError(error, "failed to enqueue memory transfer") );
    }
    m_state = QUEUED;
}

void Real::retrieveData(){
  #ifndef NDEBUG
  std::cout << "retrieving data" << std::endl;
#endif
 m_value = 0;
}