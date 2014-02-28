#ifndef MATRIXNORM_HPP
#define MATRIXNORM_HPP

#include<vector>
#include "algorithm.h"
#include "matrix.h"


class MatrixNorm : public ClAlgorithm{

    public:

        /// @brief Algorithm that computes product of two matrices.
        /// @param A left operand
        /// @param B right operand
        MatrixNorm ( Matrix& A,
                     Context *context,
                     cl::CommandQueue *queue );

        /// @return matrix width
        int getWidth() const;

        /// @return matrix height
        int getHeight() const;

        Matrix* getNormalizedMatrix();

        Matrix* getNormCoeffs();

        void retrieveData () override;

#ifndef NDEBUG
        /// @brief Prints matrix on standard output
        void print();
#endif

    protected:

        void enqueue () override;

        bool allocateForResult () override;

        void deallocateForResult() override;

    private:

        Matrix* m_src = 0;

        Matrix* m_normalized = 0;

        Matrix* m_normCoeffs = 0;

};

#endif //MATRIXNORM_HPP
