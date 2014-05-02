/**
 * @file   algorithm.h
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @brief  Define a generic algorithm representation
 */

#ifndef ALGORITHM_H
#define ALGORITHM_H

class Expression;

#include "expression.hpp"

#include<vector>

/// @page algorithm Algorithm
/// @brief Description of the Algorithm class
///
/// This library automates the memory allocations, dependencies and execution
/// for the computation of expressions automatically. This is achieved by using
/// a standardized interface on the operations which is called Algorithm.
///
/// An algorithm takes expressions in and yields results which are expressions
/// themselves.
/// The input can also be considered as dependencies of the algorithm.
///
/// The method @ref evaluateTree is used to start the computation in the
/// algorithm. If the input values are not yet enqueued, it will automatically
/// start their evaluation.
///
/// Then the actual computation performed by the algorithm will start with
/// a call to @ref enqueue.
///
/// @warning When @ref enqueue is called, the dependencies are only enqueued,
///          not computed, @ref enqueue should use synchronisation mechanism if
///          necessary.
///
/// For details on the Algorithm class and its methods, see @ref algorithm.h.

/// Algorithms class as described in @ref algorithm
class Algorithm {
    friend class Expression;

    public:
        /// @brief blocking function that returns when computation is finished.
        /// @note This function might be called on a finished algorithm in which
        ///       case it should return immediately.
        virtual void waitEndOfEvaluation() = 0;

        /// Indicates the state of the algorithm
        /// @return true if algorithm is enqueued (or evaluated)
        bool isEnqueued() const;

    protected:

        /// allocate any temporary memory needed to perform computation
        virtual bool allocateTmpMem() = 0;

        /// realease any temporary memory reserved for computation
        virtual void releaseTmpMem() = 0;

        /// specify an expression whose value is needed to compute the algorithm
        /// @param dependency a dependency of the algorithm
        void addDependency(Expression* dependency);

        /// Specifiy a result expression of the algorithm.
        /// This is usually caled by the constructors when they create the
        /// results expressions.
        /// @note all results of an algorithm need to be added for memory
        ///       management and queueing to work properly.
        void addResult(Expression* result);


        /// dependencies requiered to compute this expression
        /// @warning Any dependency not specified here will be ignored by
        ///          @ref evaluateTree()
        std::vector<Expression*> m_dependencies;

        /// results produced by the algorithm
        /// @warning Any result not specified here will be ignored during
        ///          the evaluation.
        std::vector<Expression*> m_results;


        /// @brief launch evaluation of the object
        /// This function can assumes memory was allocated for all results and
        /// that dependencies are *enqueued* (not necessarily computed).
        /// @note  enqueue should not be blocking execution. Instead, it should
        ///        start computation in a separate thread. Synchronisation
        ///        can be achieved with waitEndOfEvaluation() or implementation
        ///        specific systems.
        virtual void enqueue( ) = 0;

        /// @brief  recursively evaluate the expression tree
        /// @note   for a clear understanding of its behavour, see
        ///         @computation_tree and @tree_evaluation
        /// @param  depth depth in the recursion (0 is the root of the tree)
        /// @return true if computation of the algorithm has started, false
        ///         otherwise
        bool evaluateTree( int depth=0 );

  private:
        /// allocate memory for the elements of m_results
        bool allocateResMem();

        /// recursively deallocate memory in the evaluation tree
        /// @param hierarchyOffset number of levels of the evaluation tree to
        ///        skip before actually deallocating memory.
        /// @note will deallocate results and temporary memory, but not
        ///       dependencies. However @ref releaseTreeMem will be called on
        ///       the parent algorithm of dependencies if any.
        void releaseTreeMem( const int hierarchyOffset = 0 );

        /// true if algorithm is enqueued or evaluated
        bool m_isEnqueued = false;
};

#endif //ALGORITHM_H
