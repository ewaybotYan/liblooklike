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
/// @note The state of an expression is not public but is used internally
///       in the evaluation process and for integrity checks.
enum ExpressionState{
  INITIAL, ///< initial state
  ALLOCATED, ///< memory is allocated to store the value of the expression
  ENQUEUED, ///< the algorithm that compute the value is running
  COMPUTED, ///< the value computation is done, the value can be read
};

/// @page expression Expression
/// @brief Description of the Expression class
///
/// An expression is a generic representation of any mathematical object that
/// holds information, it can be a scalar value, a matrix, a vector...
///
/// @ref Expression is an abstract class and defines a set of common
/// characterteristics to what we define as an expression in this project
///
/// An expression contains data, therefor it has to have provide methods for
/// memory allocation and deallocation.
///
/// The result of a computation is also an expression. This library does not
/// make distinction between the expressions that are computed and expressions
/// that serve as the input for computations. Indeed, some expressions might be
/// used for both.
///
/// Computing the value of an expression is called evaluating an expression.
/// When @ref evaluate is called on an expression, the algorithm attached to
/// the expression will start its execution.
///
/// If no parent algorithm exists, it means the expression does not need to be
/// computed, in which case @ref needsComputation() returns false and
/// @ref evaluate does nothing.
///
/// For details about the class and the specification of the methods, see
/// @ref expression.hpp.

/// Expression object as defined in @ref expression.
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

    /// pointer to the algorithm that computes the expression value
    /// @details if values is set to nullptr, it means the expression is not c
    /// computed but has its value set at construction instead.
    Algorithm* m_computedBy = nullptr;

  private:

    bool recEvaluate( int depth );

    ExpressionState m_state = INITIAL;
};

#endif // EXPRESSION_HPP
