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

    /// Default constructor
    Error( ) throw();

    /// @param errorMsg error message that will be displayed
    Error( const char* errorMsg ) throw();

    /// @param errorMsg error message that will be displayed
    Error( const std::string& errorMsg ) throw();

    /// @return the message associated to this exception
    const char* what() const throw() override;

#ifndef NDEBUG
    /// prints a formatted error message on the standard error output stream
    void printMsg();
#endif

  protected:

    /// the content of this variable is printed by @ref printMsg()
    std::string errorMsg;

};


/// exception that prints a human readable message for OpenCL error
class CLError : public Error{

  public:
    /// @param clErrorNb the error number returned/set by most OpenCL functions
    /// @param msg optionnal error message
    CLError( const int clErrorNb, const std::string msg = "" ) throw();

};

class EvaluationProcessViolation : public Error
{
  public:
    EvaluationProcessViolation(const std::string& msg) throw();
    EvaluationProcessViolation(const char* msg) throw();
};

#endif //EXCEPTION_H
