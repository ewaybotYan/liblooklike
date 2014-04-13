/**
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @file   clmatrixoperations.hpp
 * @date   13/04/2014
 */

#ifndef CLMATRIXOPERATIONS_HPP
#define CLMATRIXOPERATIONS_HPP

#include<CL/cl.hpp>
#include<memory>
#include"expression.hpp"
#include"clalgorithm.hpp"
#include"clmatrix.hpp"


class CLMatrixProduct : public ClAlgorithm
{
  public:

    CLMatrixProduct(std::shared_ptr<CLMatrix> lop,
                    std::shared_ptr<CLMatrix> rop,
                    Context *context,
                    cl::CommandQueue *queue );

    void waitEndOfEvaluation() override;

    std::shared_ptr<CLMatrix> getResult();

  protected:

    bool allocateTmpMem() {return true;}
    void releaseTmpMem() {}

    void enqueue();

  private:

    std::shared_ptr<CLMatrix> m_lop;
    std::shared_ptr<CLMatrix> m_rop;
    std::shared_ptr<CLMatrix> m_result;
};


class CLMatrixCovariance : public ClAlgorithm
{
  public:

    CLMatrixCovariance(std::shared_ptr<CLMatrix> m,
                    Context *context,
                    cl::CommandQueue *queue );

    void waitEndOfEvaluation() override;

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
