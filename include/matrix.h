#ifndef MATRIX_H
#define MATRIX_H

/**
 *  @file   matrix.h
 *  @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 *  @brief  implements methods from @ref Algorithm for matrices
 *  @todo   update constructors
 */

#include <vector>
#include <CL/cl.h>
#include <clalgorithm.hpp>
#include <context.h>

/// @brief Implements main aspects of mathematical matrices for
///        @ref Algorithm .
class Matrix: public ClAlgorithm {

    public:

        /// creates an empty matrix object
        /// @param dependencies of the matrix
        /// @param m     number of lines in the matrix
        /// @param n     number of columns in the matrix
        Matrix ( const unsigned int m,
                 const unsigned int n,
                 Context *context,
                 cl::CommandQueue *queue );

        /// creates a matrix object without value but with dependencies
        /// @param dependencies of the matrix
        /// @param m     number of lines in the matrix
        /// @param n     number of columns in the matrix
        Matrix ( std::vector<Algorithm*> dependencies,
                 const unsigned int m,
                 const unsigned int n,
                 Context *context,
                 cl::CommandQueue *queue );

        /// @return matrix width
        int getWidth() const;

        /// @return matrix height
        int getHeight() const;

        /// @return a 1D float array as the concatenation of the lines of the matrix
        cl_float* getResult() const;
        void setResult( float* values );

        /// @return the value of the matrix at line i and column j
        float getResultValue( const int i, const int j );

        cl::Buffer* getBuffer();

        void retrieveData () override;

#ifndef NDEBUG
        /// @brief Prints matrix on standard output
        void print();
#endif

    protected:

        virtual void enqueue () override;

        bool allocateForResult () override;

        void deallocateForResult() override;

        float* m_result = 0;

    private:

        cl::Buffer* m_buffer = 0;

        /// matrix height
        int m_m;

        /// matrix width
        int m_n;

};

class MatrixLoader : public Matrix{

    public:

        /// creates a matrix object from a data set
        /// @param values values of the matrix
        /// @param m     number of lines in the matrix
        /// @param n     number of columns in the matrix
        MatrixLoader ( float* values,
                 const unsigned int m,
                 const unsigned int n,
                 Context *context,
                 cl::CommandQueue *queue );

    protected:

        void enqueue() override;

};

class MatrixProd : public Matrix{

    public:
        /// @brief Algorithm that computes product of two matrices.
        /// @param A left operand
        /// @param B right operand
        MatrixProd ( Matrix& A, Matrix& B,
                     Context *context,
                     cl::CommandQueue *queue );

    protected:

        void enqueue() override;

    private:

        Matrix* m_lOperand = 0;
        Matrix* m_rOperand = 0;
};


class MatrixSum : public Matrix{

    public:
        /// @brief Algorithm that computes product of two matrices.
        /// @param A left operand
        /// @param B right operand
        MatrixSum ( Matrix& A, Matrix& B,
                    Context *context,
                    cl::CommandQueue *queue );

    protected:

        void enqueue() override;

    private:

        Matrix* m_lOperand = 0;
        Matrix* m_rOperand = 0;
};



/// @brief Computes t(A) * A.
class MatrixCovariance : public Matrix{

    public:
        /// @brief Algorithm that computes product of two matrices.
        /// @param A left operand
        MatrixCovariance ( Matrix& A,
                           Context *context,
                           cl::CommandQueue *queue );

    protected:

        void enqueue() override;

    private:

        Matrix* m_src = 0;
};


#endif // MATRIX_H
