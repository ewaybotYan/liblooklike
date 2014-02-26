#ifndef REAL_H
#define REAL_H

#include<string>

#include <CL/cl.h>
#include <clalgorithm.hpp>
#include <context.h>

// This time, we merge the value loader algorithm and the base object.
// In fact, we want to inherit from the base object class in every Algorithm
// that yields this object.
class Real: public ClAlgorithm {

    public:

        Real(){}

        Real( const float value, Context *context, cl::CommandQueue *queue );

        float getResult();

        cl::Buffer* getValue();

        void retrieveData();

    protected:

        void enqueue() override;

    private:

        bool allocateForResult() override;

        void deallocateForResult() override;

        // This time we do not bother allocating memory for the result in the
        // computer memory. Instead, we assume the result can fit in memory.
        // we use the word result, but in the case of the loader function,
        // this value is set at the creation of the class.
        float m_result = 0;

        // Memory management happens on the OpenCL device
        cl::Buffer* m_buffer = 0;

};


// this is our algorithm, sincce its output is also a real object, we inheritate
// from Real, which itself implements Algorithm.
// The enqueue() fonction will have a completely different behaviour though.
class RealSum: public Real {

    public:

        RealSum( Real &a, Real &b, Context *context, cl::CommandQueue *queue );

    protected:

        void enqueue() override;

    private:

        // this time, we do not use m_children inherited from Algorithm because
        // this forces to upcast to Algorithm and then downcast to Real again
        // for the sum operation.
        Real* m_lOperand = 0;
        Real* m_rOperand = 0;

};

#endif
