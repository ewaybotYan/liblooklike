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

enum ExpressionState{
  INITIAL,
  ALLOCATED,
  ENQUEUED,
  COMPUTED,
};

class Expression
{
    friend class Algorithm;

  public:

    /// says if object has to be computed or not before one can read its value.
    bool needsComputation();

    /// blocks execution until object computation is finished.
    /// will retrieve data to local memory if it is not computed locally.
    void waitEndOfEvaluation();

    /// start the evaluation of the expression
    void evaluate();

    /// @brief   request memory allocation for the expression value
    bool allocateMem();

    /// release memory buffers associated to the expression
    void releaseMem();

  protected:

    /// @brief  actual memory allocation for Expression implementation
    /// @return true if allocation was sucessful
    /// @note   might never be called directly but through allocateMem
    virtual bool allocateMemImpl() = 0;

    /// @brief implementation of the memory deallocation for the implementation
    /// @note this is not called at object destruction, make sure to create
    ///       a destructor in the implementation if needed.
    virtual void releaseMemImpl() = 0;

    Expression(){}

  private:

    bool recEvaluate( int depth );

    Algorithm* m_computedBy = nullptr;

    ExpressionState m_state = INITIAL;
};

#endif // EXPRESSION_HPP
