/**
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @file   expression.hpp
 * @date   06/04/2014
 */

#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

class Algorithm;

#include<vector>
#include "algorithm.h"


/// describe the state of an expression (only meaningful if it is a computed
/// expression)
enum ExpressionState{
  INITIAL, ///< initial state
  ALLOCATED, ///< memory is allocated to store the value of the expression
  ENQUEUED, ///< the algorithm that compute the value is running
  COMPUTED, ///< the value computation is done, the value can be read
};

class Expression
{
    friend class Algorithm;

  public:

    /// says if object has to be computed before the value can be read
    bool needsComputation();

    /// @return the algorithm that computes the expression if ther is one, null
    ///         otherwise
    Algorithm* getParentAlgorithm() const;

    /// blocks execution until object computation is finished
    /// @note  calls @ref Algorithm::waitEndOfEvaluation() of the parent
    ///        algorithm if any, returns immediately otherwise
    /// @throw EvaluationProcessViolation thrown if parent algorithm is not yet
    ///        enqueued
    void waitEndOfEvaluation();

    /// start the evaluation of the expression if it has to be computed
    void evaluate();

    /// allocate memory so that the expression value can be stored
    bool allocateMem();

    /// release memory buffers allocated by @ref allocateMem()
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

    Algorithm* m_computedBy = nullptr;

  private:

    bool recEvaluate( int depth );

    ExpressionState m_state = INITIAL;
};

#endif // EXPRESSION_HPP
