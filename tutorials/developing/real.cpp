/**
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @file   real.cpp
 * @date   02/05/2014
 */


#include "real.h"
#include <iostream>

// The default constructor might be used by algorithms that want to create their
// empty result object at construction.
// NOTE: Since the value of the expression is not yet known, no memory has been
//       allocated to for it. The algorithm that will compute the value will
//       take care of allocating memory in time.
Real::Real()
{
}

// Almost all expressions will provide a constructor that can set the value(s)
// of the object immediately. This constructor will be used in the program.
// The default value of m_computedBy is nullptr and means the expression does
// not need to be computed, which is the case here.
Real::Real(float value)
{
  m_value = new float;
  *m_value = value;
}

// Do not forget to release memory when the object goes out of sight.
Real::~Real()
{
  releaseMemImpl();
}

// All expressions will provide an implementation specific method to give
// acess to the value(s).
float* Real::getValue()
{
  return m_value;
}

#ifndef NDEBUG
// It might also be usefull to add a print method to help debug tasks, although
// this is is not mandatory.
void Real::print()
{
  std::cout << *m_value << std::endl;
}
#endif

// This is the method that alllocate enouph memory to store the value(s) of the
// expression.
// Not that this method has a wrapper allocateMem() which takes care of double
// allocation.
bool Real::allocateMemImpl()
{
  m_value = new float;
  return true;// no error checking for this example
}

// This method releases any memory associated to the expression.
// Again, the wrapper release memory avoid double deallocations.
void Real::releaseMemImpl()
{
  delete m_value;
  m_value = nullptr;
}
