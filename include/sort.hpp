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

/// abstract class for sort algorithms on SimpleMatrix objects
/// @note sort algorithms take a matrix as input but only sort a line or a
///       column of the matrix unless a different behaviour is specified
template<typename Scalar>
class Sort : public Algorithm
{
  public:

    /// @return the values sorted with the implemented sort
    std::shared_ptr< SimpleMatrix<Scalar> >& getSorted();

    /// @return the indexes of the sorted elements in the original unsorted
    ///         object
    /// @note   index computation must be enabled in 
    std::shared_ptr< SimpleMatrix<unsigned int> >& getSortIdx();

  protected:

    /// @param data the data on which sort is applied
    /// @param sortOnColumns indicated if the data to sort is in line or column
    /// @param sortRefIdx indicates the index of the line or column to sort in
    ///        a matrix of values.
    /// @param appendIndex wether the indexes of the sorted elements in the
    ///        original unsorted object should be computed
    Sort( std::shared_ptr< SimpleMatrix<Scalar> > data,
          bool sortOnColumns = true,
          unsigned int sortRefIdx = 0,
          bool appendIndex = false );

    /// the unsorted values
    std::shared_ptr< SimpleMatrix<Scalar> > m_data;

    /// the sorted result
    std::shared_ptr< SimpleMatrix<Scalar> > m_sorted;

    /// the indexes of the sorted elements in the m_data
    std::shared_ptr< SimpleMatrix<unsigned int> > m_indexes;

    /// wether sort is done horizontaly or vertiacally
    bool m_sortOnColumns = true;

    /// if sort happens on a column or a line of the matrix, this indicates
    /// the index of this column or line.
    unsigned int m_sortRefIdx = 0;

    /// wether indexes of the sorted elements should be generated too
    bool m_appendIdx = 0;
};


// ####################################
// # InertiaSort algorithms declaration

/// Performs inertia sort on value.
/// The sum of the absolute values is the total inertia
/// The values are sorted in descending order, regardless of the sign until 
/// a target proportion of the total inertia is reached
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

    /// number of values sorted before reaching target inertia
    unsigned int getNbSortedValues() const;

    Scalar getTotalInertia() const;

  private:

    static void InertiaSortWithThreshold(InertiaSort<Scalar>* sort);
    Scalar m_inertia;
    unsigned int m_nbSortedValues;
    Scalar m_totalInertia;
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
Scalar InertiaSort<Scalar>::getTotalInertia() const
{
  return m_totalInertia;
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
  s->m_totalInertia = totInertia;
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

  // resize indexes and sort array to the number of sorted values
  if( s->m_appendIdx )
    s->getSortIdx()->resizeVecHeight(i);
  s->getSorted()->resizeVecHeight(i);

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
