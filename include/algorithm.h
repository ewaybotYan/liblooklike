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


class Algorithm {
    friend class Expression;

    public:
        virtual void waitEndOfEvaluation() = 0;

        bool isEnqueued() const;

    protected:

        /// allocate any temporary memory needed to perform computation
        virtual bool allocateTmpMem() = 0;

        bool allocateResMem();

        /// realease any temporary memory reserved for computation
        virtual void releaseTmpMem() = 0;

        /// recursively deallocate memory for the evaluation tree
        /// @param hierarchyOffset number of levels of the evaluation tree to
        ///        skip before actually deallocating memory.
        /// @note will deallocate results temporary memory, but not
        ///       dependencies. However @ref releaseTreeMem will be called on
        ///       the parent algorithm of dependencies if any.
        void releaseTreeMem( const int hierarchyOffset = 0 );


        /// specify an expression whose value is needed to compute the algorithm
        /// @param dependency a dependency of the algorithm
        void addDependency(Expression* dependency);

        /// appends a result to the list of the algorithm's results
        /// @note all results need to be added for memory management to work
        void addResult(Expression* result);

        /// dependencies requiered to compute this expression
        /// @warning Any dependency not specified here will be ignored by
        ///          @ref evaluateTree()
        std::vector<Expression*> m_dependencies;

        /// dependencies requiered to compute this expression
        /// @warning Any result not specified here will be ignored during
        ///          the evaluation.
        std::vector<Expression*> m_results;


        /// @brief evaluate the expression tree in a reverse recursive way
        bool evaluateTree( int depth=0 );

        /// @brief launch evaluation of the object
        virtual void enqueue( ) = 0;

  private:

        bool m_isEnqueued = false;
};

#endif //ALGORITHM_H
