/**
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @file   clmatrix.hpp
 * @date   12/04/2014
 */

#ifndef CLMATRIX_HPP
#define CLMATRIX_HPP

#include<CL/cl.hpp>
#include<memory>
#include "expression.hpp"
#include "clalgorithm.hpp"
#include "simplematrix.hpp"


// ################
// # CLMatrix class

/// An @ref Expression implementation for matrices of cl_float values stored in
/// OpenCL memory.
class CLMatrix : public Expression
{
  public:

    /// empty CLMatrix constructor
    /// @param h     number of lines in the CLMatrix
    /// @param w     number of columns in the CLMatrix
    CLMatrix ( const unsigned int h,
               const unsigned int w );

    /// @return matrix width
    int getWidth() const;

    /// @return matrix height
    int getHeight() const;

    /// @return cl::Buffer buffer with the values of the matrix.
    /// @note   see @ref matrix_mem_representation for memory representation.
    std::shared_ptr<cl::Buffer> getValues() const;

  protected:

    /// buffer that holds the elements of the matrix
    std::shared_ptr< cl::Buffer > m_values;

    /// number of lines in the matrix
    int m_m;

    /// number of columns in the matrix
    int m_n;

    bool allocateMemImpl() override;

    void releaseMemImpl() override;
};


// ##################
// # Basic algorithms

/// specialization of @ref ClAlgorithm that handles creates a @ref CLMatrix with
/// the values of an @ref SimpleMatrix\<cl_float\> object.
class CLMatrixLoader : public ClAlgorithm
{
  public:

    /// @param source the input matrix
    /// @param context an OpenCL @ref Context that will hold the matrix memory
    /// @param queue cl::CommandQueue on wich memory transfer will be enqueued
    CLMatrixLoader( std::shared_ptr<SimpleMatrix<cl_float> > source,
                    Context *context, cl::CommandQueue *queue );

    void waitEndOfEvaluation() override;

    /// @return a @ref CLMatrix with the elements of the source matrix
    std::shared_ptr<CLMatrix> getResult();

  protected:

    bool allocateTmpMem() {return true;} // no need for temporary memory
    void releaseTmpMem() {}

    void enqueue();

  private:

    std::shared_ptr< SimpleMatrix<cl_float> > m_src;
    std::shared_ptr< CLMatrix > m_result;
};


/// handles conversion from @ref CLMatrix to @ref SimpleMatrix\<cl_float\>
class CLMatrixUnloader : public ClAlgorithm
{
  public:

    /// @param source the input matrix
    /// @param context The OpenCL context that will hold the matrix
    /// @param queue a cl::CommandQueue in wich memory transfer will be enqueued
    CLMatrixUnloader( std::shared_ptr<CLMatrix> source,
                    Context *context,
                    cl::CommandQueue *queue );

    void waitEndOfEvaluation() override;

    /// @return a @ref SimpleMatrix \<cl_float\> that has the elements of the
    /// source matrix specified in constructor @ref CLMatrixUnloader()
    std::shared_ptr< SimpleMatrix<cl_float> > getResult();

  protected:

    bool allocateTmpMem() {return true;} // no need for temporary memory
    void releaseTmpMem() {}

    void enqueue();

  private:

    std::shared_ptr< CLMatrix > m_src;
    std::shared_ptr< SimpleMatrix<cl_float> > m_result;
};
#endif // CLMATRIX_HPP
