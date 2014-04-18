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


// ###########################
// # Matrix class for

/// An @ref Expression implementation for matrices of cl_float values stored in
/// OpenCL memory.
class CLMatrix : public Expression
{
  public:

    /// empty CLMatrix constructor
    /// @param m     number of lines in the CLMatrix
    /// @param n     number of columns in the CLMatrix
    CLMatrix ( const unsigned int m,
               const unsigned int n );

    /// @return CLMatrix width
    int getWidth() const;

    /// @return CLMatrix height
    int getHeight() const;

    /// @return cl::Buffer buffer with the values of the matrix.
    /// @note   See @ref m_values for the memory representation of matrices
    ///         elements.
    std::shared_ptr<cl::Buffer> getValues() const;

  protected:

    /// buffer that holds the elements of the matrix

    std::shared_ptr< cl::Buffer > m_values;

    /// Matrix height
    int m_m;

    /// Matrix width
    int m_n;

    bool allocateMemImpl() override;

    void releaseMemImpl() override;
};


// ##################
// # Basic algorithms

/// specialization of @ref ClAlgorithm that handles conversion from
/// @ref SimpleMatrix <cl_float> to @ref CLMatrix
class CLMatrixLoader : public ClAlgorithm
{
  public:

    /// @param source the input matrix
    /// @param context The OpenCL context that will hold the matrix
    /// @param queue a cl::CommandQueue in wich memory transfer will be launched
    CLMatrixLoader( std::shared_ptr<SimpleMatrix<cl_float> > source,
                    Context *context, cl::CommandQueue *queue );

    void waitEndOfEvaluation() override;

    /// @return a CLMatrix that has the elements of the source matrix specified
    /// in constructor @ref CLMatrixLoader()
    std::shared_ptr<CLMatrix> getResult();

  protected:

    bool allocateTmpMem() {return true;} // no need for temporary memory
    void releaseTmpMem() {}

    void enqueue();

  private:

    std::shared_ptr< SimpleMatrix<cl_float> > m_src;
    std::shared_ptr< CLMatrix > m_result;
};


/// handles conversion from @ref CLMatrix to @ref SimpleMatrix \<cl_float\>
class CLMatrixUnloader : public ClAlgorithm
{
  public:

    /// @param source the input matrix
    /// @param context The OpenCL context that will hold the matrix
    /// @param queue a cl::CommandQueue in wich memory transfer will be launched
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
