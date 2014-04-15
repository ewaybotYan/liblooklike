/**
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @file   sort.hpp
 * @date   04/04/2014
 */

#ifndef SORT_HPP
#define SORT_HPP

#include "algorithm.h"
#include "simplematrix.hpp"
#include "exception.h"
#include <cmath>
#include <memory>
#include <vector>
#include <thread> // separate computing thread from main execution


// #########################################
// # Generic sort algorithm (abstract class)

template<typename Scalar>
class Sort : public Algorithm
{
  public:
    std::shared_ptr< SimpleMatrix<Scalar> >& getSorted();
    std::shared_ptr< SimpleMatrix<unsigned int> >& getSortIdx();

  protected:

    std::shared_ptr< SimpleMatrix<Scalar> > m_data;
    std::shared_ptr< SimpleMatrix<Scalar> > m_sorted;
    std::shared_ptr< SimpleMatrix<unsigned int> > m_indexes;

    bool m_sortOnColumns = true;
    unsigned int m_sortRefIdx = 0;
    bool m_appendIdx = 0;

    Sort( std::shared_ptr< SimpleMatrix<Scalar> > data,
          bool sortOnColumns = true,
          unsigned int sortRefIdx = 0,
          bool appendIndex = false );
};


// ####################################
// # InertiaSort algorithms declaration

template<typename Scalar>
class InertiaSort : public Sort<Scalar>
{
  public:
    InertiaSort( std::shared_ptr< SimpleMatrix<Scalar> > data,
                 Scalar inertia,
                 bool sortOnColumn = true,
                 unsigned int sortRefIdx = 0,
                 bool appendIndex = false );

    void waitEndOfEvaluation() override;

    bool allocateTmpMem() override { return true; }

    void releaseTmpMem() override {}

    void enqueue();

    unsigned int getNbSortedValues() const;

  private:

    static void InertiaSortWithThreshold(InertiaSort<Scalar>* sort);
    Scalar m_inertia;
    unsigned int m_nbSortedValues;
    std::thread m_t;
};


// #########################
// # Sort methods definition

template<typename Scalar>
Sort<Scalar>::Sort( std::shared_ptr< SimpleMatrix<Scalar> > data,
                    bool sortOnColumns,
                    unsigned int sortRefIdx,
                    bool appendIndex )
{
  typedef Algorithm A;
  unsigned int nbValues = sortOnColumns ?
                            data->getHeight() : data->getWidth();
  m_sorted = std::make_shared< SimpleMatrix<Scalar> >( nbValues, 1 );
  A::addResult(m_sorted.get());
  if (appendIndex) {
    m_indexes = std::make_shared< SimpleMatrix<unsigned int> >( nbValues, 1 );
    A::addResult(m_indexes.get());
  }
  m_data = data;
  A::addDependency(data.get());
  m_sortOnColumns = sortOnColumns;
  m_sortRefIdx = sortRefIdx;
  m_appendIdx = appendIndex;
}

template<typename Scalar>
std::shared_ptr< SimpleMatrix<Scalar> >& Sort<Scalar>::getSorted()
{
  return m_sorted;
}

template<typename Scalar>
std::shared_ptr< SimpleMatrix<unsigned int> >& Sort<Scalar>::getSortIdx()
{
  return m_indexes;
}


// ################################
// # InertiaSort methods definition

template<typename Scalar>
InertiaSort<Scalar>::InertiaSort( std::shared_ptr< SimpleMatrix<Scalar> > data,
                                  Scalar inertia,
                                  bool sortOnColumn,
                                  unsigned int sortRefIdx,
                                  bool appendIndex ) :
  Sort<Scalar>( data, sortOnColumn, sortRefIdx, appendIndex )
{
  m_inertia = inertia;
}

template<typename Scalar>
void InertiaSort<Scalar>::waitEndOfEvaluation()
{
  if ( !Algorithm::isEnqueued() ) {
    throw EvaluationProcessViolation(
        "Cannot wait for evaluation as it is not enqued.");
  } else {
    if(m_t.joinable())
      m_t.join();
  }
}

template<typename Scalar>
unsigned int InertiaSort<Scalar>::getNbSortedValues() const
{
  return m_nbSortedValues;
}

template<typename Scalar>
void InertiaSort<Scalar>::InertiaSortWithThreshold( InertiaSort<Scalar>* s )
{
  // simplify method to find the values in the source SimpleMatrix<Scalar>
  unsigned int start = s->m_sortOnColumns ?
                         s->m_sortRefIdx * s->m_data->getHeight() :
                         s->m_sortRefIdx;
  unsigned int step = s->m_sortOnColumns ? 1 : s->m_data->getHeight();

  // copy values and compute total inertia
  Scalar* values = s->m_data->getValues();
  Scalar* result = s->m_sorted->getValues();
  unsigned int* indexes = s->m_appendIdx ? s->m_indexes.get()->getValues() : nullptr;
  Scalar totInertia = 0;
  unsigned int nbValues = s->m_sortOnColumns ? s->m_data->getHeight() :
               s->m_data->getWidth();
  for (unsigned int i = 0; i < nbValues; ++i) {
    result[i] = values[start+i*step];
    totInertia += std::abs(values[start+i*step]);
  }
  if (s->m_appendIdx)
    for( unsigned int i=0; i<nbValues; i++ )
      indexes[i] = i;

  // extract values
  Scalar inertia = 0;
  unsigned int i = 0;
  while (inertia < s->m_inertia * totInertia) {
    unsigned int idxMax = i;
    Scalar max = result[idxMax];
    for ( unsigned int j = i; j < nbValues; j++ ) {
      if (std::abs(result[j]) > max) {
        idxMax = j;
        max = std::abs(result[j]);
      }
    }
    Scalar tmp = result[idxMax];
    inertia += std::abs(tmp);
    result[idxMax] = result[i];
    result[i] = tmp;
    if (s->m_appendIdx) {
      unsigned int tmpIdx = indexes[i];
      indexes[i] = indexes[idxMax];
      indexes[idxMax] = tmpIdx;
    }
    ++i;
  }
  s->m_nbSortedValues = i;
}

template<typename Scalar>
void InertiaSort<Scalar>::enqueue()
{
  typedef Sort<Scalar> S;
  for (auto child : S::m_dependencies) {
    child->waitEndOfEvaluation();
  }

  m_t = std::thread( InertiaSort<Scalar>::InertiaSortWithThreshold, this );
}


#endif // SORT_HPP
