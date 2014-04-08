/**
 * @file   algorithm.h
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @brief  The evaluation of a whole expression is handled by an Expression
 *         object. It makes it easier to understand the process.
 * @note   All objects should inheritate from Expression so that computation can
 *         be done with them.
 */

#ifndef ALGORITHM_H
#define ALGORITHM_H

class Expression;

#include "expression.hpp"

#include<vector>


/// @brief Abstract class that describes a mathematical object that needs
///        (or not) to be evaluated before having a value.
/// @detailed The principle of Mathexpression is to hold the elements that can
///            be computed in a mathematical expression.
///
///            Example:
///                 in a = b + c <br/>
///                 a, b and c are all algorithms. It can also be
///                 represented as a tree where a is the root node,
///                 and has two children b and c.
///                 In order to compute a, we will execute evaluate().
///                 And then retrieve data from the computation device with
///                 retrieveData()
///
///            As shown in the example above, the expression is represented as
///            a tree. The terminal nodes are basically values known by the
///            program on the computer side, whereas the other nodes are values
///            obtained by computation on their children nodes.
///
///            The behaviour of @ref evaluate() is fixed however: recursively
///            evaluate children expressions then the value of the current
///            expression. However, it relies on two virtual functions:
///                - @ref allocateMemoryForResult()
///                - @ref enqueue()
///            It is highly recommanded to read the description of these
///            methods and the @ref test_algorithm example in /examples
///            before trying to implement them.
///
/// @note @ref allocateForResult() and @ref deallocateForResult() are meant to
///       be used for special computation systems that have their own memory
///       like OpenCL. If the memory is the RAM, you might not want to throw the
///       result of your computation away.

enum ExpressionState{
  INITIAL,
  QUEUED
};

class Algorithm {
    friend class Expression;

    public:
        virtual void waitEndOfEvaluation() = 0;

        /// @brief compute the value of the Expression
        /// @detailed This function will allocated memory for children and
        ///           evaluate them recursively until it enqueues the execution
        ///           for this expression.
        void evaluate();

        /// Deallocate memory for a whole evaluation tree
        /// @param hierarchyOffset number of levels to skip before actually
        ///        deallocating memory.
        void deallocateMemory( const int hierarchyOffset = 0 );

        /// Allocate memory for a whole evaluation tree
        bool allocateForResult();

        void addChild(Expression* child);

        void addResult(Expression* result);

    protected:

        /// @brief constructor is private because this is an abstract class.
        Algorithm(){}

        /// dependencies requiered to compute this expression
        /// @warning Any dependency not specified here will be ignored during
        ///          the evaluation.
        std::vector<Expression*> m_children;

        /// dependencies requiered to compute this expression
        /// @warning Any dependency not specified here will be ignored during
        ///          the evaluation.
        std::vector<Expression*> m_results;

  private:

        /// @brief launch evaluation of the object
        virtual void enqueue( ) = 0;

        /// @brief the actual recursive evaluation method
        bool recEvaluate( int depth );

        ExpressionState m_state = INITIAL;
};

#endif //ALGORITHM_H
