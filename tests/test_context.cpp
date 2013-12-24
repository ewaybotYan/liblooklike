/**
 * @file   test_context.cpp
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @brief  unit tests for Context class
 */

#include<string>
#include<iostream>

#include "../include/exception.h"
#include "../include/context.h"

using namespace std;

// the test string which is also a constant in the kernel code
const std::string hw("Hello World\n");
const std::string programName("test_kernel");
const std::string kernelName("hello");

void usage(){
  cerr << "Error in " << __FILE__ << " :\n"
    << "usage :\n test_context path_to_kernel_directory" 
    << endl;
}

int main( const int argc, const char* argv[] ){
  if( argc !=2 ){
    usage();
    return -1;
  }
  std::string path(argv[1]);

  cl_int error;

  try{
    Context ctx(path);
    cl::Kernel ker = ctx.getKernel( programName, kernelName );
    
    // we don't use the library after this step, we just check context in this
    // test program

    // allocate memory
    char* outH = new char[hw.length()+1];
    cl::Buffer outCL(
        ctx.getContext(),
        CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR,
        hw.length()+1,
        outH,
        &error);
    if( error != CL_SUCCESS ){
      cerr << "failed to result buffer\n";
      return -1;
    }

    // set kernel arguments
    error = ker.setArg(0, outCL);
    if( error != CL_SUCCESS ){
      cerr << "Failed to set kernel arguments\n";
      return -1;
    }

    // create command queue
    cl::CommandQueue queue = ctx.createQueue();

    // enqueue kernel execution
    cl::Event event;
    error = queue.enqueueNDRangeKernel(
        ker,
        cl::NullRange,
        cl::NDRange(hw.length()+1),
        cl::NDRange(1, 1),
        NULL,
        &event);
    if( error != CL_SUCCESS ){
      cerr << "Failed to load coammand queue\n";
      return -1;
    }

    // wait 
    event.wait();

    // retrieve data
    error = queue.enqueueReadBuffer(
        outCL,
        CL_TRUE,
        0,
        hw.length()+1,
        outH);
    if( error != CL_SUCCESS ){
      cerr << "Failed to execute kernel\n";
      return -1;
    }

    // check result
    std::string result(outH);
    if(result == hw){
      return 0;
    }else{
      cerr << "Kernel returned \"" + result + "\"" << endl;
      cerr << "Expected :      \"" + hw +"\"" << endl;
      return -1;
    }

  }catch( Error& e ){
    e.printMsg();
    return -1;
  }

  return 0;
}
