#ifndef MATRIX_H
#define MATRIX_H

class Matrix{
  public:
    Matrix();
    Matrix leftMul( const Matrix A) const;
    Matrix rightMul( const Matrix B) const;
    Matrix leftDotProd( cont float l ) const;
    Matrix rightDotProd( const float l ) const;
    Matrix sum( const Matrix A ) const;
  private:
};

#endif // MATRIX_H
