/**
 * @file   clmatrixnorm.hpp
 * @brief  define algorithms on matrices related to the norms and distances
 * @date   28/04/2014
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 */

#ifndef CLMATRIXNORM_HPP
#define CLMATRIXNORM_HPP

#include<memory>
#include "clalgorithm.hpp"
#include "clmatrix.hpp"


/// @ref ClAlgorithm specialization for normalization of the lines or columns
///      of a @ref CLMatrix object
class CLMatrixNorm : public ClAlgorithm{

  public:

    /// @brief Algorithm that normlizes lines or columns in a matrix.
    /// @param A input matrix
    /// @param context OpenCL context in which normalization will be enqueued
    /// @param queue cl::CommandQueue that in which computation will be queued
    /// @param onCols wether normalization is done on columns or lines
    CLMatrixNorm( std::shared_ptr<CLMatrix> A,
                  Context *context,
                  cl::CommandQueue *queue,
                  bool onCols = true );

    void waitEndOfEvaluation() override;

    /// @return the normalized matrix
    std::shared_ptr<CLMatrix> getNormalizedMatrix();

    /// @return a vector (matrix of width 1) with the average computed during
    ///         the normalization process
    std::shared_ptr<CLMatrix> getAvgs();

    /// @return a vector (matrix of width 1) with the standard deviations
    ///         computed during the normalization process
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
    std::shared_ptr<CLMatrix> m_avgs = 0;
};


/// @brief Algorithm that computes the distances between all the vectors for
/// the euclidian distance
class CLInterDistance : public ClAlgorithm{

  public:

    /// @param A matrix with vectors of coordinates (written vertically)
    /// @param context The OpenCL context that will hold the matrix
    /// @param queue a cl::CommandQueue in wich memory transfer will be launched
    CLInterDistance( std::shared_ptr<CLMatrix> A,
                  Context *context,
                  cl::CommandQueue *queue );

    void waitEndOfEvaluation() override;

    /// @return the inter-vectors distance matrix
    ///         the element at position i,j is the distance between vectors in
    ///         columns i and j of the input matrix.
    /// @note   Since the matrix is symmetric, the down left triangle is filled
    ///         with 0.
    std::shared_ptr<CLMatrix> getDistances();

  protected:

    bool allocateTmpMem() {return true;} // no need for temporary memory
    void releaseTmpMem() {}

    void enqueue();

  private:

    std::shared_ptr<CLMatrix> m_src = 0;
    std::shared_ptr<CLMatrix> m_distances = 0;

};

#endif //CLMATRIXNORM_HPP
