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

/// @brief Gives the result of a memory alocation on a Mathexpresion
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

/// @brief Abstract class that describe a mathematical object both as the result
///        of a computation and as an OpenCl object.
class MathExpression {

public:

    // ##########################
    // # constructors/destructors

    ~MathExpression(){}

protected:

    // @param keepInCLMem indicates wether data of the expression has to be kept
    //        in OpenCL device memory even if no parent expression is using it.
    MathExpression( const bool isTerminal, const bool keepInCLMem );

public:
    // #################
    // # getters/setters

    /// says if the expression has to be computed before one can read its value
    bool isComputed();

    /// @brief Gives the general state of the MathExpresion (see state diagram
    ///        for a better understanding).
    ExpressionState getState();

    /// @brief Return data buffers associated to the object value.
    /// @note  Buffer order might be important, see MathExpression
    ///        implementations for the meaning.
    std::vector<cl::Buffer> getData();

    /// Gives the OpenCL event associated to the end of the computation.
    /// @note You should check that computation is enqueued with @ref getState()
    ///       before waiting for this event.
    cl::Event& getEndOfEvaluation();


    // #########
    // # methods
    
    /// @brief creates a cl CommandQueue that will be used internally for computation
    void createQueue();
    
    /// @brief compute the value of the Expression
    /// @param ctx a valid context
    /// @param queue an optionnal queue to use, it will be created if not provided
    //void evaluate( Context& ctx );
    void evaluate( Context& ctx,  cl::CommandQueue& queue );

    /// @brief adds a dependency to the expression
    /// @param child the expression needed to evaluate
    /// @warning the user has to push the children in the right order
    void addChild( MathExpression* child );

    /// @todo handle the number of parents for memory deallocation
    void increaseParentNb();
    
protected:

    /// @brief launch evaluation
    /// @note  evaluate may fail if no memory space was allocated for the
    ///        object. It is up to the parent Expression evaluate function to
    ///        allocate memory and execute evaluate again.
    /// @note  evaluate might be a recursive function, if it depends on the
    ///        existence of other operations results
    virtual void enqueue( Context& context, cl::CommandQueue& queue ) = 0;

    /// @brief   Must be called to bring back the computed results from
    ///          the computation device to the program memory.
    /// @warning Calling retrieveData before computation is over has an
    ///          undefined behaviour.
    virtual void retrieveData( Context& context, cl::CommandQueue& queue ) = 0;

    /// describe the state of the expression
    ExpressionState m_state = INITIAL;

    /// @warning all buffers allocated by allocateForResult should come here
    std::vector<cl::Buffer> m_data;

    /// dependencies requiered to compute this
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
    ///          m_data because memory deallocation is automatic.
    /// @warning do not forget to update m_state if allocation is successfull
    virtual bool allocateForResult( Context& context ) = 0;

    /// @brief Deallocate the memory for this expression AND its dependent
    ///        expressions computation on the computation device.
    /// @todo  check circular dependencies?
    void deallocateMemory();

    /// Deallocate the memory for storing the value of this expression (m_data).
    void deallocateForResult();

    // #########
    // # members

    // is it a terminal expression or a computed one?
    // this is used in allocation to check wether a expression that needs to be
    // computed has buffers in order to store its value
    bool m_isTerminal;

    // the event associated to the computation in the openCL queue
    cl::Event m_computationState;

    // number of parent expressions that will require our data
    int m_nbParents = 0;

};

#endif
