/**
 * @file   clmatrixoperations.hpp
 * @brief  set of basic algorithms on matrices for CLMatrix objects
 * @date   13/04/2014
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 */

#ifndef CLMATRIXOPERATIONS_HPP
#define CLMATRIXOPERATIONS_HPP

#include<CL/cl.hpp>
#include<memory>
#include"expression.hpp"
#include"clalgorithm.hpp"
#include"clmatrix.hpp"


/// performs the dot product of two matrices
class CLMatrixProduct : public ClAlgorithm
{
  public:

    /// @param lop left operand of the dot product
    /// @param rop right operand of the dot product
    CLMatrixProduct(std::shared_ptr<CLMatrix> lop,
                    std::shared_ptr<CLMatrix> rop,
                    Context *context,
                    cl::CommandQueue *queue );

    void waitEndOfEvaluation() override;

    /// @return the product of the matrices given to the constructor
    std::shared_ptr<CLMatrix> getResult();

  protected:

    bool allocateTmpMem() {return true;} // no need for temporary memory
    void releaseTmpMem() {}

    void enqueue();

  private:

    std::shared_ptr<CLMatrix> m_lop;
    std::shared_ptr<CLMatrix> m_rop;
    std::shared_ptr<CLMatrix> m_result;
};


/// for a given matrix A, compute t(A)*A with t() the transpose function and *
/// the dot product for matrices
class CLMatrixCovariance : public ClAlgorithm
{
  public:

    /// @param m the input matrix
    /// @param context The OpenCL context that will hold the matrix
    /// @param queue a cl::CommandQueue in wich memory transfer will be launched
    CLMatrixCovariance(std::shared_ptr<CLMatrix> m,
                    Context *context,
                    cl::CommandQueue *queue );

    void waitEndOfEvaluation() override;

    /// @return t(m) * m with m the input matrix given to the constructor
    std::shared_ptr<CLMatrix> getResult();

  protected:

    bool allocateTmpMem() {return true;}
    void releaseTmpMem() {}

    void enqueue();

  private:

    std::shared_ptr<CLMatrix> m_src;
    std::shared_ptr<CLMatrix> m_result;
};

#endif // CLMATRIXOPERATIONS_HPP
