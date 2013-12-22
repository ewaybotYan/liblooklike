#include<iostream>
#include<fstream>
#include<iterator>
#include<string>
#include<map>

#include<CL/cl.hpp>

#include "../include/exception.h"
#include "../include/context.h"

// #################
// # local functions

void printPlatforms( cl::vector<cl::Platform> platformList ){
  cout << platformList.size() << " platform(s) found:" << endl;
  for(  auto plf : platformList ){
    std::string platformVendor;
    std::string platformName;
    std::string platformVersion;
    plf.getInfo((cl_platform_info)CL_PLATFORM_VENDOR, &platformVendor);
    plf.getInfo((cl_platform_info)CL_PLATFORM_NAME, &platformName);
    plf.getInfo((cl_platform_info)CL_PLATFORM_VERSION, &platformVersion);
    cout << "name:    " << platformName << endl;
    cout << "version: " << platformVersion << endl;
    cout << "vendor:  " << platformVendor << endl;
  }
}

void printDevices( cl::vector<cl::Device> devices ){
  // print informations on devices
  cout << devices.size() << " devices found\n";
  for( auto dev : devices ){
    string devName;
    string devOpenCLVersion;
    string devProfile;
    string devVersion;
    string driverVersion;
    cl::vector<size_t> maxWorkItemSizes;
    dev.getInfo( CL_DEVICE_MAX_WORK_ITEM_SIZES, &maxWorkItemSizes );
    dev.getInfo( CL_DEVICE_NAME             , &devName );
    dev.getInfo( CL_DEVICE_OPENCL_C_VERSION , &devOpenCLVersion );
    dev.getInfo( CL_DEVICE_PROFILE          , &devProfile );
    dev.getInfo( CL_DEVICE_VERSION          , &devVersion );
    dev.getInfo( CL_DRIVER_VERSION          , &driverVersion );
    cout << "device    : " << devName       << "\n";
    cout << "OpenCl ver: " << devOpenCLVersion << "\n";
    cout << "profile   : " << devProfile    << "\n";
    cout << "device ver: " << devVersion    << "\n";
    cout << "driver ver: " << driverVersion << "\n";
    cout << "max work item sizes:";
    for( size_t size : maxWorkItemSizes )
      cout << " " << size;
    cout << "\n";
  }
}


// #############
// # constructor

Context::Context( std::string programsPath ){
  // # set up opencl context
  // find platforms
  cl::vector< cl::Platform > platformList;
  cl::Platform::get(&platformList);
  if( platformList.size() == 0 )
    throw Error( "No platform found" );
#ifndef NDEBUG
  printPlatforms(platformList);
#endif

  // get devices for all platform
  cl::vector<cl::Device> devices;	
  for( auto plf : platformList ){
    cl_int error;
    error = plf.getDevices(CL_DEVICE_TYPE_ALL, &devices);
    if( error != CL_SUCCESS )
      throw CLError( error );
  }
  if( devices.size() == 0 )
    throw Error("no device found");
#ifndef NDEBUG
  printDevices( devices );
#endif

  // start a context (OpenCL chooses a platform automatically)
  cl_int error;
  cl::Context context( devices, NULL, NULL, NULL, &error );
  if( error != CL_SUCCESS )
    throw CLError( error, "Cannot start a context" );

  // # member variables
  m_programsPath = programsPath;
}

// #########
// # Methods

cl::Kernel Context::getKernel( const std::string programName, 
    const std::string kernelName ){

  // get program
  cl::Program program;
  auto iter = m_programs.find(programName);
  if( iter == std::map::end ){
    // program is not loaded
    std::ifstream file(m_programsPath + "/" + programName + ".cl" );
    std::string prog(
        std::istreambuf_iterator<char>(file),
        (std::istreambuf_iterator<char>())
        );
    // load kernels code
    cl::Program::Sources source(1, std::make_pair(prog.c_str(), prog.length()+1));
    cl::Program program(context, source);
    error = program.build(devices,"");
    if( error != CL_SUCCESS )
      throw CLError( error, "Failed to load kernel" );
    //////
    //
    // todo : j'en suis ici dans le code
    // bonne nuit ;-)
    //
    //////
  }
}
