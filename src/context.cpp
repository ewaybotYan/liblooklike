#include<iostream>
#include<fstream>
#include<iterator>
#include<string>
#include<map>
#include<vector>

#include<CL/cl.hpp>

#include "../include/exception.h"
#include "../include/context.h"

// #################
// # local functions

void printPlatforms( std::vector<cl::Platform> platformList ){
  std::cout << platformList.size() << " platform(s) found:\n";
  for(  auto plf : platformList ){
    std::string platformVendor;
    std::string platformName;
    std::string platformVersion;
    plf.getInfo((cl_platform_info)CL_PLATFORM_VENDOR, &platformVendor);
    plf.getInfo((cl_platform_info)CL_PLATFORM_NAME, &platformName);
    plf.getInfo((cl_platform_info)CL_PLATFORM_VERSION, &platformVersion);
    std::cout << "name:    " << platformName << "\n";
    std::cout << "version: " << platformVersion << "\n";
    std::cout << "vendor:  " << platformVendor << "\n";
  }
}

void printDevices( std::vector<cl::Device> devices ){
  // print informations on devices
  std::cout << devices.size() << " devices found\n";
  for( auto dev : devices ){
    std:: string devName;
    std::string devOpenCLVersion;
    std::string devProfile;
    std::string devVersion;
    std::string driverVersion;
    std::vector<size_t> maxWorkItemSizes;
    dev.getInfo( CL_DEVICE_MAX_WORK_ITEM_SIZES, &maxWorkItemSizes );
    dev.getInfo( CL_DEVICE_NAME             , &devName );
    dev.getInfo( CL_DEVICE_OPENCL_C_VERSION , &devOpenCLVersion );
    dev.getInfo( CL_DEVICE_PROFILE          , &devProfile );
    dev.getInfo( CL_DEVICE_VERSION          , &devVersion );
    dev.getInfo( CL_DRIVER_VERSION          , &driverVersion );
    std::cout << "device    : " << devName       << "\n";
    std::cout << "OpenCl ver: " << devOpenCLVersion << "\n";
    std::cout << "profile   : " << devProfile    << "\n";
    std::cout << "device ver: " << devVersion    << "\n";
    std::cout << "driver ver: " << driverVersion << "\n";
    std::cout << "max work item sizes:";
    for( size_t size : maxWorkItemSizes )
      std::cout << " " << size;
    std::cout << "\n";
  }
}


// #############
// # constructor

Context::Context( std::string programsPath ){

  cl_int error;

  // # set up opencl context
  // find platforms
  std::vector< cl::Platform > platformList;
  cl::Platform::get(&platformList);
  if( platformList.size() == 0 )
    throw Error( "No platform found" );
#ifndef NDEBUG
  printPlatforms(platformList);
#endif

  // get devices for all platform
  std::vector<cl::Device> devices;	
  for( auto plf : platformList ){
    error = plf.getDevices(CL_DEVICE_TYPE_ALL, &devices);
    if( error != CL_SUCCESS )
      throw CLError( error );
  }
  if( devices.size() == 0 )
    throw( Error("no device found") );
#ifndef NDEBUG
  printDevices( devices );
#endif

  // start a context (OpenCL chooses a platform automatically)
  cl::Context context( devices, NULL, NULL, NULL, &error );
  if( error != CL_SUCCESS )
    throw( CLError( error, "Cannot start a context" ) );

  // # member variables
  m_programsPath = programsPath;

}

// #########
// # Methods

cl::Program Context::loadProgram( const std::string programName ){

  cl_int error;

  // load file
  try{
    std::string fileName(m_programsPath + "/" + programName + ".cl");
    std::ifstream file( fileName );
    std::string prog(
        std::istreambuf_iterator<char>(file),
        (std::istreambuf_iterator<char>())
        );
  }catch( std::ios::exceptions e ){
    throw( Error( "could not open " + fileName
          + " : " + e.what() ) );
  }

  // load and build kernels
  cl::Program::Sources source = 
    cl::Program::Sources(1, std::make_pair(prog.c_str(), prog.length()+1));
  cl::Program program(context, source);
  error = program.build(devices,"");
  if( error != CL_SUCCESS )
    throw( CLError( error, "Failed to build kernel" ) );
  return program;

}

cl::Kernel Context::getKernel( const std::string programName, 
    const std::string kernelName ){

  cl_int error;

  // get program
  cl::Program program;
  std::map<std::string, cl::Program>::iterator iter = 
    m_programs.find(programName);
  if( iter == std::map::end ){// program is not loaded
    program = loadProgram( programName );
    iter = m_programs.insert( std::make_pair( programName, program ) ).first;
  }

  // get kernel
  cl::Kernel kernel( iter->second, kernelName, &error );
  if( error != CL_SUCCESS )
    throw( CLError( error, "could not find or load kernel") );

  return kernel;
}