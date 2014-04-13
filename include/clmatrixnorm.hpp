#ifndef CLMATRIXNORM_HPP
#define CLMATRIXNORM_HPP

#include<memory>
#include "clalgorithm.hpp"
#include "clmatrix.hpp"


class CLMatrixNorm : public ClAlgorithm{

    public:

        /// @brief Algorithm that computes product of two matrices.
        /// @param A left operand
        /// @param B right operand
        CLMatrixNorm( std::shared_ptr<CLMatrix> A,
                      Context *context,
                      cl::CommandQueue *queue,
                      bool onCols = true );

        void waitEndOfEvaluation() override;

        std::shared_ptr<CLMatrix> getNormalizedMatrix();

        std::shared_ptr<CLMatrix> getNormCoeffs();

    protected:

        bool allocateTmpMem() {return true;}
        void releaseTmpMem() {}

        void enqueue();

    private:

        bool m_normalizationOnCols;

        std::shared_ptr<CLMatrix> m_src = 0;
        std::shared_ptr<CLMatrix> m_normalized = 0;
        std::shared_ptr<CLMatrix> m_normCoeffs = 0;

};

#endif //CLMATRIXNORM_HPP
