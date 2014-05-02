/**
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @file   real.cpp
 * @date   02/05/2014
 */

// NOTE : If you are looking for an example of implementation of Expression,
//        please refer to tutorial/developing/real.cpp instead.

#include "real.h"
#include <iostream>

Real::Real()
{
}

Real::Real(float value)
{
  m_value = new float;
  *m_value = value;
}

Real::~Real()
{
  releaseMemImpl();
}

float* Real::getValue()
{
  return m_value;
}

void Real::print()
{
  std::cout << *m_value << std::endl;
}

bool Real::allocateMemImpl()
{
  m_value = new float;
  return true;// no error checking for this example
}

void Real::releaseMemImpl()
{
  if (m_value != nullptr) {
    delete m_value;
    m_value = nullptr;
  }
}
