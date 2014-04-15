/**
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @file   simplematrix.hpp
 * @date   06/04/2014
 */

#ifndef SIMPLEMATRIX_HPP
#define SIMPLEMATRIX_HPP

#ifndef NDEBUG
#include<iostream>
#endif
#include<memory>
#include<vector>
#include"expression.hpp"


template<typename Scalar>
class SimpleMatrix : public Expression
{
  public:

    /// creates an empty SimpleMatrix object
    /// @param dependencies of the SimpleMatrix
    /// @param m     number of lines in the SimpleMatrix
    /// @param n     number of columns in the SimpleMatrix
    SimpleMatrix ( const unsigned int m,
                   const unsigned int n );

    SimpleMatrix(
        std::shared_ptr< std::vector<Scalar> > values,
        const unsigned int m,
        const unsigned int n );

    /// creates a SimpleMatrix object without value but with dependencies
    /// @param dependencies of the SimpleMatrix
    /// @param m     number of lines in the SimpleMatrix
    /// @param n     number of columns in the SimpleMatrix
    SimpleMatrix ( const std::vector<Scalar>& values,
                   const unsigned int m,
                   const unsigned int n);

    /// @return SimpleMatrix width
    int getWidth() const;

    /// @return SimpleMatrix height
    int getHeight() const;

    /// @return a 1D float array as the concatenation of the lines of the SimpleMatrix
    Scalar* getValues() const;

    /// @return the value of the SimpleMatrix at line i and column j
    Scalar& at( const int i, const int j );

    void resizeWidth( const int h );

#ifndef NDEBUG
    /// @brief Prints SimpleMatrix on standard output
    void print();
#endif

  protected:

    std::shared_ptr< std::vector<Scalar> > m_values;

    /// Matrix height
    int m_m;

    /// Matrix width
    int m_n;

    bool allocateMemImpl() override;

    void releaseMemImpl() override;
};


// ####################
// # methods definition

template<typename Scalar>
SimpleMatrix<Scalar>::SimpleMatrix( const unsigned int m, const unsigned int n )
{
  m_m = m;
  m_n = n;
}

template<typename Scalar>
SimpleMatrix<Scalar>::SimpleMatrix(
    std::shared_ptr< std::vector<Scalar> > values,
    const unsigned int m,
    const unsigned int n )
{
  m_values = values;
  m_m = m;
  m_n = n;
}

template<typename Scalar>
SimpleMatrix<Scalar>::SimpleMatrix( const std::vector<Scalar>& values,
                                    const unsigned int m,
                                    const unsigned int n )
{
  m_values =
      std::shared_ptr< std::vector<Scalar> >(new std::vector<Scalar>(values));
  m_m = m;
  m_n = n;
}

template<typename Scalar>
int SimpleMatrix<Scalar>::getWidth() const
{
  return m_n;
}

template<typename Scalar>
int SimpleMatrix<Scalar>::getHeight() const
{
  return m_m;
}

template<typename Scalar>
Scalar* SimpleMatrix<Scalar>::getValues() const
{
  return m_values->data();
}

template<typename Scalar>
Scalar& SimpleMatrix<Scalar>::at( const int i, const int j )
{
  return m_values.get()->at(i+m_m*j);
}

template<typename Scalar>
void SimpleMatrix<Scalar>::resizeWidth( const int w )
{
  m_n = w;
  m_values->resize(w*m_m);
}

#ifndef NDEBUG
/// @brief Prints SimpleMatrix on standard output
template<typename Scalar>
void SimpleMatrix<Scalar>::print()
{
  std::cout.precision(5);
  std::cout << std::scientific;
  for( int i=0;i<m_m;i++ ){
    for( int j=0;j<m_n;j++ )
      std::cout << m_values.get()->at(i+j*m_m) << " ";
    std::cout << "\n";
  }
}
#endif

template<typename Scalar>
bool SimpleMatrix<Scalar>::allocateMemImpl()
{
  if( needsComputation() ){
    m_values = std::make_shared< std::vector<Scalar> >(m_m*m_n);
  }
  return m_values != nullptr;
}

template<typename Scalar>
void SimpleMatrix<Scalar>::releaseMemImpl()
{
  if( !needsComputation() )
    m_values.reset();
}


#endif // SIMPLEMATRIX_HPP
