/**
 * @file   algorithm.h
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @brief  Define a gneric algorithm representation
 */

#ifndef ALGORITHM_H
#define ALGORITHM_H

class Expression;

#include "expression.hpp"

#include<vector>


/**
 * Algorithms offers a generic framework to write algorithms. It allows the
 * automation of memory managment, computation dependencies and queues in a
 * transparent manner.
 */
class Algorithm {
    friend class Expression;

    public:
        /// @brief blocking fonction that returns when evaluation of the
        ///        algorithm is finished
        /// @note This function can assume the algorithm is at least enqueued.
        ///       However, it might be called on a finished algorithm in which
        ///       case it should return immediately
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

        /// appends a result to the list of the algorithm's results
        /// @note all results of an algorithm need to be added for memory
        ///       management and queueing to work
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
        /// htat dependencies are *enqueued* (not necessarily computed).
        /// @note  enqueue should not be blocking execution. Instead, it should
        ///        start computation in a separate thread. Synchronisation
        ///        is acheived by using waitEndOfEvaluation() instead.
        virtual void enqueue( ) = 0;

        /// @brief evaluate the expression tree in a recursive way
        /// @note  for a clear understanding of its behavour, see
        ///        @computation_tree and @tree_evaluation
        /// @param depth depth in the recursion, used in order to identify the
        ///        root.
        /// @return true if all the elements were enqueued, and especially the
        ///         results of the algorithm, false otherwise.
        bool evaluateTree( int depth=0 );

  private:
        /// allocate memory for the elements of m_results
        bool allocateResMem();

        /// recursively deallocate memory for the evaluation tree
        /// @param hierarchyOffset number of levels of the evaluation tree to
        ///        skip before actually deallocating memory.
        /// @note will deallocate results temporary memory, but not
        ///       dependencies. However @ref releaseTreeMem will be called on
        ///       the parent algorithm of dependencies if any.
        void releaseTreeMem( const int hierarchyOffset = 0 );

        /// true if algorithm is enqueued or evaluated
        bool m_isEnqueued = false;
};

#endif //ALGORITHM_H
