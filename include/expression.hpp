/**
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @file   expression.hpp
 * @date   06/04/2014
 */

#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

class Algorithm;

#include "algorithm.h"

#include<vector>

class Expression
{
  friend class Algorithm;

  public:

  // says if object has to be computed or not before one can read its value.
  bool iscomputed();

  // blocks execution until object computation is finished.
  // should retrieve data to local memory if it is not computed locally.
  // @warning should return immediately if object is not evaluating
  void waitEndOfEvaluation();

  void evaluate();

 private:
    /// @brief   allocate memory in which we will put the result of this
    ///          computation
    /// @warning do not forget to update m_state if allocation is
    ///          successfull
    virtual bool allocateMemory() = 0;

    /// Deallocate the memory buffers
    virtual void deallocateMemory() = 0;

    bool recEvaluate( int depth );

    Algorithm* m_parent = nullptr;
};

#endif // EXPRESSION_HPP
