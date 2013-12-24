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
  for( auto plf : platformList ){
    error = plf.getDevices(CL_DEVICE_TYPE_ALL, &m_devices);
    if( error != CL_SUCCESS )
      throw CLError( error, "No device found on on of the platforms" );
  }
  if( m_devices.size() == 0 )
    throw( Error("no device found") );
#ifndef NDEBUG
  printDevices( m_devices );
#endif

  // start a context (OpenCL chooses a platform automatically)
  m_context = cl::Context( m_devices, NULL, NULL, NULL, &error );
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
  std::string fileName(m_programsPath + "/" + programName + ".cl");
    std::string prog;
    std::ifstream file;
    file.open( fileName, std::ios::in );
    if( !file )
        throw Error( "could not open " + fileName);
    prog = std::string(
               std::istreambuf_iterator<char>(file),
               (std::istreambuf_iterator<char>())
           );
  // load and build kernels
  cl::Program::Sources source = 
    cl::Program::Sources(1, std::make_pair(prog.c_str(), prog.length()+1));
  cl::Program program(m_context, source);
  error = program.build(m_devices,"");
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
  if( iter == m_programs.end() ){// program is not loaded
    program = loadProgram( programName );
    iter = m_programs.insert( std::make_pair( programName, program ) ).first;
  }

  // get kernel
  cl::Kernel kernel = cl::Kernel( iter->second, kernelName.c_str(), &error );
  if( error != CL_SUCCESS )
    throw( CLError( error, "could not find or load kernel") );

  return kernel;
}

#ifndef NDEBUG
cl::Context& Context::getContext(){
  return m_context;
}

std::vector<cl::Device> Context::getDevices(){
  return m_devices;
}
#endif

cl::CommandQueue Context::createQueue(){
  // we will loop on devices each time we want to create a queue
  static int nextDeviceToUse = 0;

  cl_int error;
  cl::CommandQueue queue(m_context, m_devices[nextDeviceToUse], 0, &error);
  if( error != CL_SUCCESS )
    throw CLError(error, "failed to create command queue");

  // update the next device to use
  nextDeviceToUse = ( nextDeviceToUse + 1 ) % m_devices.size();

  return queue;
}
