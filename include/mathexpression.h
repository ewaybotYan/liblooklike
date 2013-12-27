/**
 * @file   expression.h
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

enum ExpressionState {
    INITIAL,
    ALLOCATED,// a buffer was allocated to recieve the value on a device
    QUEUED// computation is in the computation queue
    /* COMPUTED// computation is over, result can be taken back*/
};

enum AllocationResult {
    NONE_ALLOCATED,
    ONE_COMPUTED_EXPRESSION_ALLOCATED,
    TERMINAL_EXPRESSION_ALLOCATED,
    COMPUTED_EXPRESSION_ALLOCATED
};

cl::Context* getContextFromQueue( cl::CommandQueue queue );

class MathExpression {
public:
    ~MathExpression(){};
    // ##########################
    // # constructors/destructors

    // @param keepInCLMem indicates wether data of the expression has to be kept
    //        in OpenCL device memory even if no parent expression is using it.
    MathExpression( const bool isTerminal, const bool keepInCLMem );

    // #################
    // # getters/setters

    /// says if the expression has to be computed before one can read its value
    bool isComputed();

    ExpressionState getState();

    std::vector<cl::Buffer> getData();

    cl::Event& getEndOfEvaluation();
    
    void setQueue( cl::CommandQueue queue );
    cl::CommandQueue& getQueue();
    

    // #########
    // # methods
    
    /// @brief creates a cl CommandQueue that will be used internally for computation
    void createQueue();
    
    /// @brief compute the value of the Expression
    /// @param ctx a valid context
    /// @param queue an optionnal queue to use, it will be created if not provided
    void evaluate( Context& ctx );
    void evaluate( Context& ctx,  cl::CommandQueue& queue );

    /// @brief adds a dependency to the expression
    /// @param child the expression needed to evaluate
    /// @warning the user has to push the children in the right order
    void addChild( MathExpression* child );

    void increaseParentNb();
    

    
protected:

    /// @brief launch evaluation
    /// @note  evaluate may fail if no memory space was allocated for the
    ///        object. It is up to the parent Expression evaluate function to
    ///        allocate memory and execute evaluate again.
    /// @note  evaluate might be a recursive function, if it depends on the
    ///        existence of other operations results
    virtual void enqueue( Context& context, cl::CommandQueue& queue ) = 0;

    virtual void retrieveData( Context& context, cl::CommandQueue& queue ) = 0;

    /// describe the state of the expression
    ExpressionState m_state = INITIAL;

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

    /// @brief  allocate memory in which we will put the result of this
    ///         computation
    virtual bool allocateForResult( Context& context ) = 0;

    /// @brief  try to deallocate memory if it has no parent
    void deallocateMemory();

    void deallocateForResult();

    // #########
    // # members

    /// is it a terminal expression or a computed one?
    // this is used in allocation to check wether a expression that needs to be
    //  computed has buffers in order to store its value
    bool m_isTerminal;

    /// the event associated to the computation in the openCL queue
    cl::Event m_computationState;

    /// number of parent expressions that will require our data
    int m_nbParents = 0;

};

#endif
