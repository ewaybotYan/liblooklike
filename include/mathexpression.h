/**
 * @file   mathexpression.h
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @brief  The evaluation of a whole expression is handled by an Expression
 *         object. It makes it easier to understand the process.
 * @note   All objects should inheritate from Expression so that computation can
 *         be done with them.
 */

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include<CL/cl.hpp>

#include "context.h"

// ###################
// # State description

/// Used to explicitely define the state of a MathExpression.
enum ExpressionState {
    INITIAL,///< initial state
    ALLOCATED,///< Data buffers are allocated on computation device.
    QUEUED///< Computation is in the computation queue.
    /* COMPUTED// computation is over, result can be taken back*/
};

/// @brief Gives the result of a memory alocation on a Mathexpression
enum AllocationResult {
    /// No memory allocation happened
    NONE_ALLOCATED,
    /// A terminal expression, ie an expression that is just a data transfer
    /// from computer memory, has had memory allocated on computation device.
    TERMINAL_EXPRESSION_ALLOCATED,
    /// One expression requiring computation has memory allocated for its input
    /// and output data buffers, this is not the top MathExpression
    ONE_COMPUTED_EXPRESSION_ALLOCATED,
    /// present expression and all its direct dependencies have memory allocated
    COMPUTED_EXPRESSION_ALLOCATED
};


// ###############
// # MathExpresion

/// @brief Abstract class that describes a mathematical object that need
///        (or not) to be evaluated before having a value.
/// @detailed The principle of Mathexpression is to hold the elements that can
///            be computed in a mathematical expression.
///
///            Example:
///                 in a = b + c <br/>
///                 a, b and c are all mathexpressions. It can also be
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
///            The behaviour of evaluate is fixed, however, it uses several
///            virtual functions that are specific to the type of
///            expressions, namely:
///            - @ref allocateMemoryForResult()
///            - @ref enqueue()
///            - @ref retrieveData()
///            It is highly recommanded to read the description of these
///            methods and the @ref test_mathexpression example in /examples
///            before trying to implement them.
///
/// @note The memory objects on the computer side can be stored anywhere in the
///       implementation of this class. However, OpenCL memory buffers, should
///       always be stored in @ref m_data as they will be automatically
///       deallocated.
///
/// @startuml{MathExpression_state.png}
///     [*] --> Initial
///     Initial --> Evaluating : evaluate()
///     state Evaluating{
///         [*] --> MemoryAllocationForChildren
///         MemoryAllocationForChildren --> enqueuedChildren
///         enqueuedChildren --> MemoryAllocatedForResult : [All_children_enqueued] allocateMemoryForResult()
///         MemoryAllocatedForResult --> Enqueued : [Memory allocation OK] enqueue()
///         Enqueued --> [*]
///     }
///     Evaluating --> Computed : [end event triggered]
///     Computed --> [*]
/// @enduml
class MathExpression {

    public:

        // ##########################
        // # constructors/destructors

        ~MathExpression(){}

    protected:

        /// @todo remove the use of isTerminal and rely on m_children size.
        /// @param isTerminal indicates wether the expression needs computation
        ///        or if it is just loaded from memory.
        /// @param keepInCLMem indicates wether data of the expression has to be
        ///        kept in OpenCL device memory even if no parent expression is
        ///        using it.
        MathExpression( const bool isTerminal, const bool keepInCLMem );

    public:
        // #################
        // # getters/setters

        /// @brief says if the expression has to be computed before one can read
        ///        its value
        /// @todo  use m_children to give the answer
        bool isComputed();

        /// @brief Gives the general state of the MathExpression (see state
        ///        diagram for a better understanding).
        ExpressionState getState();

        /// @brief Return data buffers associated to the object value.
        /// @note  Buffer order might be important, see MathExpression
        ///        implementations for the meaning.
        /// @todo  rename to getBuffer
        std::vector<cl::Buffer> getData();

        /// Gives the OpenCL event associated to the end of the computation.
        /// @note You should check that computation is enqueued with
        ///       @ref getState() before waiting for this event.
        cl::Event& getEndOfEvaluation();


        // #########
        // # methods

        /// @brief creates a cl CommandQueue that will be used internally for
        ///        computation
        void createQueue();

        /// @brief compute the value of the Expression
        /// @detailed This function will allocated memory for children and
        ///           evaluate them recursively until it enqueues the execution
        ///           for this expression.
        /// @param ctx a valid context
        /// @param queue the queue in which execution will start
        /// @startuml{MathExpression_evaluate_activity.png}
        ///     skinparam monochrome true
        ///     (*) --> evaluate()
        ///     if "already\nqueued?" then
        ///      --> [true] "SUCCESS"
        ///     else
        ///
        ///      if "has children?" then
        ///
        ///       -->[true] "treat\n children" as preChildrenSteps
        ///
        ///       if "has non-\nenqueued\nchildren?" then
        ///        -->[true] "try memory\nallocation\nfor children"
        ///
        ///        if "all failed?" then
        ///         --> [true] "FAILURE"
        ///        else
        ///         --> [false] "enqueue\nchildren"
        ///         --> preChildrenSteps
        ///        endif
        ///       else
        ///        --> [false] "Allocate\nfor result" as postChildrenStep
        ///       endif
        ///      else
        ///       -->[false] postChildrenStep
        ///
        ///       if "allocation\n succeeded?" then
        ///        --->[true] "enqueue"
        ///        ---> "SUCCESS"
        ///        ---> (*)
        ///       else
        ///        --->[false] "FAILURE"
        ///        ---> (*)
        ///       endif
        ///      endif
        ///
        ///     endif
        ///
        /// @enduml
        void evaluate( Context& ctx,  cl::CommandQueue& queue );

        /// @brief adds a dependency to the expression
        /// @param child the expression needed to evaluate
        /// @warning the user has to push the children in the right order
        void addChild( MathExpression* child );

        /// @todo handle the number of parents for memory deallocation
        void increaseParentNb();

    protected:

        /// @brief launch evaluation of the object
        /// @detailed For a terminal node, this will most likely enqueue a
        ///            writeBuffer operation. Otherwise, this will ask the
        ///            @ref Context object for one or more kernels, set the
        ///            arguments accordingly and enqueue execution of the
        ///            algorithms.
        /// @note  evaluate may fail if no memory space was allocated for the
        ///        object. It is up to the parent Expression evaluate function
        ///        to allocate memory and execute evaluate again.
        /// @note  evaluate might be a recursive function, if it depends on the
        ///        existence of other operations results
        virtual void enqueue( Context& context, cl::CommandQueue& queue ) = 0;

        /// @brief   Must be called to bring back the computed results from
        ///          the computation device to the program memory.
        /// @warning Calling retrieveData before computation is over has an
        ///          undefined behaviour.
        virtual void retrieveData( Context& context,
                                   cl::CommandQueue& queue ) = 0;

        /// describe the state of the expression
        ExpressionState m_state = INITIAL;

        /// @warning all buffers allocated by allocateForResult should come here
        std::vector<cl::Buffer> m_data;

        /// dependencies requiered to compute this expression
        /// @warning Any dependency not specified here will be ignored during
        ///          the evaluation.
        std::vector<MathExpression*> m_children;

        /// end of computation event
        cl::Event m_endOfEvaluation;

    private:

        // #################
        // # private methods

        /// @brief  try to allocate memory on the opencl device for this element
        ///         and its childs, priority is given to the children
        /// @return true if allocation succeeded, false otherwise
        AllocationResult allocateMemory( Context& context );

        /// @brief   allocate memory in which we will put the result of this
        ///          computation
        /// @warning All the memory buffers created here must be stored inside
        ///          m_data because memory deallocation is done by the
        ///          Mathexpression mother class.
        /// @warning do not forget to update m_state if allocation is
        ///          successfull
        virtual bool allocateForResult( Context& context ) = 0;

        /// @brief Deallocate the memory for this expression AND its dependent
        ///        expressions computation on the computation device.
        /// @todo  check circular dependencies?
        void deallocateMemory();

        /// Deallocate the memory buffers from @ref m_data
        void deallocateForResult();

        // #########
        // # members

        // is it a terminal expression or a computed one?
        // this is used in allocation to check wether a expression that needs to
        // be computed has buffers in order to store its value
        bool m_isTerminal;

        // the event associated to the computation in the openCL queue
        cl::Event m_computationState;

        // number of parent expressions that will require our data
        int m_nbParents = 0;

};

#endif
