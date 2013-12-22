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


class Context{
  public:
    Context();
    /// @param programs_path absolute path to the opencl .cl programs files
    /// @throw Error
    Context( std::string programspath );

    /// @note   getKernel will load the program from the program path, build 
    ///         it and returns the kernel if it was not already loaded
    /// @param  programName name of the program without the .cl extension
    /// @param  kernelName  name of the kernel function
    /// @return returns the kernel name kernelName from the program file
    ///         <programName>.cl in the path set at context creation
    /// @throw  CLError
    cl::Kernel& getKernel( const std::string programName,
        const std::string kernelName );

#ifndef NDEBUG
    cl::Context& getContext(); 
#endif

  private:
    cl::Program loadProgram( const std::string programName );

    std::string m_programsPath;
    cl::Context  m_context;
    std::map< std::string, cl::Program > m_programs;
    std::map< std::string, cl::Kernel >  m_kernels;
};


#endif //CONTEXT_H
