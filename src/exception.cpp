
#include <string>

#include"../include/exception.h"

using namespace std;

// #######################
// # local helper function

string clErrorCodeName( const int errorCode ){
  string errorCodeName;
  switch( errorCode ){
    case(0 ):
      errorCodeName = string("CL_SUCCESS");
      break;
    case(1 ):
      errorCodeName = string("CL_DEVICE_NOT_FOUND");
      break;
    case(2 ):
      errorCodeName = string("CL_DEVICE_NOT_AVAILABLE");
      break;
    case(3 ):
      errorCodeName = string("CL_COMPILER_NOT_AVAILABLE");
      break;
    case(4 ):
      errorCodeName = string("CL_MEM_OBJECT_ALLOCATION_FAILURE");
      break;
    case(5 ):
      errorCodeName = string("CL_OUT_OF_RESOURCES");
      break;
    case(6 ):
      errorCodeName = string("CL_OUT_OF_HOST_MEMORY");
      break;
    case(7 ):
      errorCodeName = string("CL_PROFILING_INFO_NOT_AVAILABLE");
      break;
    case(8 ):
      errorCodeName = string("CL_MEM_COPY_OVERLAP");
      break;
    case(9 ):
      errorCodeName = string("CL_IMAGE_FORMAT_MISMATCH");
      break;
    case(10 ):
      errorCodeName = string("CL_IMAGE_FORMAT_NOT_SUPPORTED");
      break;
    case(11 ):
      errorCodeName = string("CL_BUILD_PROGRAM_FAILURE");
      break;
    case(12 ):
      errorCodeName = string("CL_MAP_FAILURE");
      break;
    case(30 ):
      errorCodeName = string("CL_INVALID_VALUE");
      break;
    case(31 ):
      errorCodeName = string("CL_INVALID_DEVICE_TYPE");
      break;
    case(32 ):
      errorCodeName = string("CL_INVALID_PLATFORM");
      break;
    case(33 ):
      errorCodeName = string("CL_INVALID_DEVICE");
      break;
    case(34 ):
      errorCodeName = string("CL_INVALID_CONTEXT");
      break;
    case(35 ):
      errorCodeName = string("CL_INVALID_QUEUE_PROPERTIES");
      break;
    case(36 ):
      errorCodeName = string("CL_INVALID_COMMAND_QUEUE");
      break;
    case(37 ):
      errorCodeName = string("CL_INVALID_HOST_PTR");
      break;
    case(38 ):
      errorCodeName = string("CL_INVALID_MEM_OBJECT");
      break;
    case(39 ):
      errorCodeName = string("CL_INVALID_IMAGE_FORMAT_DESCRIPTOR");
      break;
    case(40 ):
      errorCodeName = string("CL_INVALID_IMAGE_SIZE");
      break;
    case(41 ):
      errorCodeName = string("CL_INVALID_SAMPLER");
      break;
    case(42 ):
      errorCodeName = string("CL_INVALID_BINARY");
      break;
    case(43 ):
      errorCodeName = string("CL_INVALID_BUILD_OPTIONS");
      break;
    case(44 ):
      errorCodeName = string("CL_INVALID_PROGRAM");
      break;
    case(45 ):
      errorCodeName = string("CL_INVALID_PROGRAM_EXECUTABLE");
      break;
    case(46 ):
      errorCodeName = string("CL_INVALID_KERNEL_NAME");
      break;
    case(47 ):
      errorCodeName = string("CL_INVALID_KERNEL_DEFINITION");
      break;
    case(48 ):
      errorCodeName = string("CL_INVALID_KERNEL");
      break;
    case(49 ):
      errorCodeName = string("CL_INVALID_ARG_INDEX");
      break;
    case(50 ):
      errorCodeName = string("CL_INVALID_ARG_VALUE");
      break;
    case(51 ):
      errorCodeName = string("CL_INVALID_ARG_SIZE");
      break;
    case(52 ):
      errorCodeName = string("CL_INVALID_KERNEL_ARGS");
      break;
    case(53 ):
      errorCodeName = string("CL_INVALID_WORK_DIMENSION");
      break;
    case(54 ):
      errorCodeName = string("CL_INVALID_WORK_GROUP_SIZE");
      break;
    case(55 ):
      errorCodeName = string("CL_INVALID_WORK_ITEM_SIZE");
      break;
    case(56 ):
      errorCodeName = string("CL_INVALID_GLOBAL_OFFSET");
      break;
    case(57 ):
      errorCodeName = string("CL_INVALID_EVENT_WAIT_LIST");
      break;
    case(58 ):
      errorCodeName = string("CL_INVALID_EVENT");
      break;
    case(59 ):
      errorCodeName = string("CL_INVALID_OPERATION");
      break;
    case(60 ):
      errorCodeName = string("CL_INVALID_GL_OBJECT");
      break;
    case(61 ):
      errorCodeName = string("CL_INVALID_BUFFER_SIZE");
      break;
    case(62 ):
      errorCodeName = string("CL_INVALID_MIP_LEVEL");
      break;
    case(63 ):
      errorCodeName = string("CL_INVALID_GLOBAL_WORK_SIZE");
      break;
    default:
      errorCodeName = string("unknown error code");
  }
  return errorCodeName;
}

// #######
// # Error

const char* Error::what(){
  return errorMsg.c_str();
}

// #########
// # CLError

CLError::CLError( const int clErrorNb, const std::string msg ){
  if( msg.compare("") == false ){
    this->errorMsg = msg + " : ";
  }else{
    this->errorMsg = "";
  }
  this->errorMsg += 
    "OpenCL error nb " + std::to_string(clErrorNb) + " (" 
    + clErrorCodeName( clErrorNb ) +") ";
}
