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

class CLMatrix : public Expression
{
  public:

    /// creates an empty CLMatrix object
    /// @param dependencies of the CLMatrix
    /// @param m     number of lines in the CLMatrix
    /// @param n     number of columns in the CLMatrix
    CLMatrix ( const unsigned int m,
               const unsigned int n );

    /// @return CLMatrix width
    int getWidth() const;

    /// @return CLMatrix height
    int getHeight() const;

    /// @return buffer with the values as an array with all lines concatenated
    std::shared_ptr<cl::Buffer> getValues() const;

  protected:

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

class CLMatrixLoader : public ClAlgorithm
{
  public:

    CLMatrixLoader( std::shared_ptr<SimpleMatrix<cl_float> > source,
                    Context *context,
                    cl::CommandQueue *queue );

    void waitEndOfEvaluation() override;

    std::shared_ptr<CLMatrix> getResult();

  protected:

    bool allocateTmpMem() {return true;}
    void releaseTmpMem() {}

    void enqueue();

  private:

    std::shared_ptr< SimpleMatrix<cl_float> > m_src;
    std::shared_ptr< CLMatrix > m_result;
};


class CLMatrixUnloader : public ClAlgorithm
{
  public:

    CLMatrixUnloader( std::shared_ptr<CLMatrix> source,
                    Context *context,
                    cl::CommandQueue *queue );

    void waitEndOfEvaluation() override;

    std::shared_ptr< SimpleMatrix<cl_float> > getResult();

  protected:

    bool allocateTmpMem() {return true;}
    void releaseTmpMem() {}

    void enqueue();

  private:

    std::shared_ptr< CLMatrix > m_src;
    std::shared_ptr< SimpleMatrix<cl_float> > m_result;
};
#endif // CLMATRIX_HPP
