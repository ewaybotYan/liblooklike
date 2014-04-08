/**
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @file   sort.hpp
 * @date   04/04/2014
 */

#ifndef SORT_HPP
#define SORT_HPP

#include "algorithm.h"
#include "simplematrix.hpp"
#include <memory>
#include <vector>
#include <thread> // separate computing thread from main execution

template<typename Scalar>
class Sort : public Algorithm
{
  public:
    std::shared_ptr< SimpleMatrix<Scalar> >& getResult();

  protected:
    std::shared_ptr< SimpleMatrix<Scalar> > m_src;
    std::shared_ptr< SimpleMatrix<Scalar> > m_result;
    std::shared_ptr< SimpleMatrix<unsigned int> > m_indexes;

    bool m_sortOnColumns = true;
    unsigned int m_sortRefIdx = 0;
    bool m_appendIdx = 0;

    Sort( std::shared_ptr< SimpleMatrix<Scalar> > data,
          bool sortOnColumns = true,
          unsigned int sortRefIdx = 0,
          bool appendIndex = false );
};

template<typename Scalar>
class InertiaSort : public Sort<Scalar>
{
  public:
    InertiaSort( std::shared_ptr< SimpleMatrix<Scalar> > data,
                 float inertia,
                 bool sortOnColumn = true,
                 unsigned int sortRefIdx = 0,
                 bool appendIndex = false );

    void waitEndOfEvaluation() override;

    void enqueue();

  private:

    static void InertiaSortWithThreshold(InertiaSort<Scalar>* sort);
    float m_inertia;
    unsigned int m_nbSortedValues;
    std::thread m_t;
};


template<typename Scalar>
Sort<Scalar>::Sort( std::shared_ptr< SimpleMatrix<Scalar> > data,
                    bool sortOnColumns,
                    unsigned int sortRefIdx,
                    bool appendIndex )
{
  typedef Algorithm A;
  unsigned int nbValues = m_sortOnColumns ?
                            data->getHeight() : data->getWidth();
  m_result = std::make_shared< SimpleMatrix<Scalar> >( nbValues, 1 );
  A::addResult(m_result.get());
  if (appendIndex) {
    m_indexes = std::make_shared< SimpleMatrix<unsigned int> >( nbValues, 1 );
    A::addResult(m_indexes.get());
  }
  m_src = data;
  A::addChild(data.get());
  m_sortOnColumns = sortOnColumns;
  m_sortRefIdx = sortRefIdx;
  m_appendIdx = appendIndex;
}

template<typename Scalar>
std::shared_ptr< SimpleMatrix<Scalar> >& Sort<Scalar>::getResult()
{
  return m_result;
}

template<typename Scalar>
InertiaSort<Scalar>::InertiaSort( std::shared_ptr< SimpleMatrix<Scalar> > data,
                                  float inertia,
                                  bool sortOnColumn,
                                  unsigned int sortRefIdx,
                                  bool appendIndex ) :
  Sort<Scalar>( data, sortOnColumn, sortRefIdx, appendIndex )
{
  m_inertia = inertia;
}

template<typename Scalar>
void InertiaSort<Scalar>::waitEndOfEvaluation(){
  m_t.join();
}

template<typename Scalar>
void InertiaSort<Scalar>::InertiaSortWithThreshold( InertiaSort<Scalar>* s )
{
  //InertiaSort& s = *sort;
  // how to find the values in the source SimpleMatrix<Scalar>
  unsigned int start = s->m_sortOnColumns ?
                         s->m_sortRefIdx :
                         s->m_sortRefIdx * s->m_src->getWidth();
  unsigned int step = s->m_sortOnColumns ? s->m_src->getWidth() : 1;

  // copy values and compute total inertia
  float* values = s->m_src->getValues();
  float* result = s->m_result->getValues();
  float totInertia = 0;
  for ( int i = 0;
        i < (s->m_sortOnColumns ? s->m_src->getHeight() : s->m_src->getWidth());
        ++i ) {
    result[i] = values[start+i*step];
    totInertia += values[start+i*step];
  }

  // extract values
  float inertia = 0;
  unsigned int i = 0;
  while (inertia < s->m_inertia * totInertia) {
    unsigned int idxMax = i;
    float max = result[idxMax];
    for ( int j = i;
          j < (s->m_sortOnColumns ? s->m_src->getHeight() : s->m_src->getWidth() );
          j++ ) {
      if (result[j] > max) {
        idxMax = j;
        max = result[j];
      }
    }
    float tmp = result[idxMax];
    inertia += tmp;
    result[idxMax] = result[i];
    result[i] = tmp;
    if (s->m_appendIdx)
      s->m_indexes.get()->getValues()[i] = idxMax;
    ++i;
  }
  s->m_nbSortedValues = i;
}

template<typename Scalar>
void InertiaSort<Scalar>::enqueue()
{
  typedef Sort<Scalar> S;
  for (auto child : S::m_children) {
    child->waitEndOfEvaluation();
  }

  m_t = std::thread( InertiaSort<Scalar>::InertiaSortWithThreshold, this );
  //InertiaSort<Scalar>::InertiaSortWithThreshold(this);
}


#endif // SORT_HPP
