/**
 *  @file   exception.h
 *  @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 *  @brief  liblooklike specific exceptions are usefull, the are defined here
 */
#ifndef EXCEPTION_H
#define EXCEPTION_H

#include<exception>
#include<string>

#ifndef NDEBUG
#include<iostream>
#endif


/// default class for exception handling
class Error: public std::exception{

  public:
    Error( ) throw();
    Error( const char* errorMsg ) throw();
    Error( std::string errorMsg ) throw();
    const char* what() const throw();
#ifndef NDEBUG
    /// prints a formatted error message on the standard error output stream
    void printMsg();
#endif

  protected:
    std::string errorMsg;

};

/// exception that prints a human readable message for OpenCL error
class CLError : public Error{

  public:
    /// param clErrorNb the error number returned/set by most OpenCL functions
    CLError( const int clErrorNb, const std::string msg = "" ) throw();

};


#endif //EXCEPTION_H
