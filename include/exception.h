/**
 *  @file   exception.h
 *  @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 *  @brief  liblooklike specific exceptions are usefull, the are defined here
 */
#ifndef EXCEPTION_H
#define EXCEPTION_H

#include<exception>
#include<string>

/// default class for exception handling
class Error: public std::exception{
  public:
    Error( const char* errorMsg ) const throw();
    const char* what() const throw();
  private:
    std::string errorMsg;
};

/// exception that prints a human readable message for OpenCL error
class CLError : public Error{
  public:
    /// param clErrorNb the error number returned/set by most OpenCL functions
    CLError( const int clErrorNb ) const throw();
};

#endif //EXCEPTION_H
