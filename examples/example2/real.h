#ifndef REAL_H
#define REAL_H

#include<string>

#include <algorithm.h>
#include <context.h>
#include <thread>

// This time, we merge the value loader algorithm and the base object.
// In fact, we want to inherit from the base object class in every Algorithm
// that yields this object.
class Real: public Algorithm {

    public:

        Real(){}

        Real( float* valuePtr );

        float getResult();

        float* getValue();

        virtual void waitEndOfEvaluation() override;

    protected:

        void enqueue() override;

        // the buffer will contain the loaded or computed value.
        float* m_buffer = 0;

    private:

        virtual bool allocateForResult() override;

        virtual void deallocateForResult() override;

        // The Loader class has a field for the source of the data
        // This field is unlikely to be reused by subclasses of Real
        float* m_dataSrc = 0;

};


// this is our algorithm, sincce its output is also a real object, we inheritate
// from Real, which itself implements Algorithm.
// The enqueue() fonction will have a completely different behaviour though.
class RealSum: public Real {

    public:

        RealSum( Real &a, Real &b );

        void waitEndOfEvaluation() override;

    protected:

        void enqueue() override;

    private:

        // this time, we do not use m_children inherited from Algorithm because
        // this forces to upcast to Algorithm and then downcast to Real again
        // for the sum operation.
        Real* m_lOperand = 0;
        Real* m_rOperand = 0;

        bool allocateForResult() override;

        void deallocateForResult() override;

        // in this example, we do the computation in a separate thread
        std::thread m_t;

};

#endif
