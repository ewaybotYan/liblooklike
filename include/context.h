/** 
 *  @file   context.h
 *  @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 *  @brief  Defines a Context class that has pretty much the same role as the
 *          standard OpenCL Context. Context also takes care of loading programs 
 *          and kernels. Device management has also been merged inside Context.
 */

#ifndef CONTEXT_H
#define CONTEXT_H

#include<string>
#include<map>

#include<CL/cl.hpp>

#include "../include/exception.h"

/// @brief Manages the OpenCL environment during the program execution.
/// This class is similar to cl::Context with extended roles.
/// It handles environment setup, kernel loading and compilation, the creation
/// of cl::CommandQueue objects.
class Context{
  public:
    Context();
    /// @param programsPath absolute path to the opencl .cl programms files
    /// @throw Error
    Context( std::string programsPath );

    /// Obtain a compiled cl::Kernel from the @ref Context.
    /// @ref getKernel will load the program from the program path, build
    /// it and returns the kernel if it was not already loaded
    /// @param  programName name of the program without the .cl extension
    /// @param  kernelName  name of the kernel function
    /// @return returns the kernel named (kernelName) from the program file
    ///         (programName).cl in the path set at context creation
    /// @throw  CLError
    cl::Kernel getKernel( const std::string programName,
        const std::string kernelName );

#ifndef NDEBUG
    /// @return the cl::Context associated to the @ref Context object
    cl::Context& getContext(); 

    /// @return an array of the cl::devices associated to the current context
    std::vector<cl::Device> getDevices();
#endif

    /// start a queue on a device of the context
    /// @return a new empty queue
    /// @note   Normally, you have to select a device on which the commands of
    ///         the queue are run. Device selection is automatic here.
    /// @throw  CLError
    cl::CommandQueue  createQueue();

  private:
    cl::Program loadProgram( const std::string programName );

    std::string m_programsPath;
    std::vector<cl::Device> m_devices;	
    cl::Context  m_context;
    std::map< std::string, cl::Program > m_programs;
    std::vector< cl::CommandQueue > m_CmdQueues;
};


#endif //CONTEXT_H
