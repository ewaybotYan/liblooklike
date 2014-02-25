#ifndef REAL_H
#define REAL_H

#include<string>

#include "../include/algorithm.h"
#include "../include/context.h"

// This is out base object, a real number.
class Real{

  public:

    Real( const float value );

    float getValue();

  private:

    float m_value = 0;

};


// In almost all of your implementations, you will want to load values from
// memory. This operation can be considered as an algorithm. Inheritance from
// Algorithm allows us to consider a value as a child of another algorithm.
class RealValue: public Algorithm {

    public:

        RealValue(){}

        RealValue( Real& value );

        Real getResult();

        void waitEndOfEvaluation() override;

    protected:

        void enqueue() override;

    private:

        bool allocateForResult() override;

        void deallocateForResult() override;

        Real* m_src = 0;
        // The implementation of Algorithm is likely to have its own memory
        // space in order to store its initial or computed value in memory.
        float* m_tmp = 0;

};


// this is our algorithm
class RealSum: public Algorithm {

    public:

        RealSum( RealValue &a, RealValue &b );

        Real getResult();

        void waitEndOfEvaluation() override;

    protected:

        void enqueue() override;

    private:

        bool allocateForResult() override;

        void deallocateForResult() override;

        float* m_tmp = 0;

};

#endif
