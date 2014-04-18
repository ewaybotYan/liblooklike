#ifndef CLMATRIXNORM_HPP
#define CLMATRIXNORM_HPP

#include<memory>
#include "clalgorithm.hpp"
#include "clmatrix.hpp"


/// @ref ClAlgorithm specialization for normalization of the lines or columns
///      a @ref CLMatrix
class CLMatrixNorm : public ClAlgorithm{

  public:

    /// @brief Algorithm that computes product of two matrices.
    /// @param A left operand
    /// @param B right operand
    /// @param context The OpenCL context that will hold the matrix
    /// @param queue a cl::CommandQueue in wich memory transfer will be launched
    /// @param onCols wether normalizatio happens on the columns or the lines
    CLMatrixNorm( std::shared_ptr<CLMatrix> A,
                  Context *context,
                  cl::CommandQueue *queue,
                  bool onCols = true );

    void waitEndOfEvaluation() override;

    /// @return the normalized matrix
    std::shared_ptr<CLMatrix> getNormalizedMatrix();

    /// @return a vector (matrix of width 1) with the standard deviation of
    ///         each line or column depending of the onCols parameter specified
    ///         on with the constructor
    std::shared_ptr<CLMatrix> getNormCoeffs();

  protected:

    bool allocateTmpMem() {return true;} // no need for temporary memory
    void releaseTmpMem() {}

    void enqueue();

  private:

    bool m_normalizationOnCols;

    std::shared_ptr<CLMatrix> m_src = 0;
    std::shared_ptr<CLMatrix> m_normalized = 0;
    std::shared_ptr<CLMatrix> m_normCoeffs = 0;

};

#endif //CLMATRIXNORM_HPP
