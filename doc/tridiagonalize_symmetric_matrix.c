////////////////////////////////////////////////////////////////////////////////
// File: tridiagonalize_symmetric_matrix.c                                    //
// Routine(s):                                                                //
//    Tridiagonalize_Symmetric_Matrix                                         //
//                                                                            //
// Required Externally Defined Routines:                                      //
//    Copy_Matrix                                                             //
//    Inner_Product                                                           //
////////////////////////////////////////////////////////////////////////////////

#include <math.h>                 // required for sqrt()
#include <float.h>                // required for DBL_MIN and DBL_EPSILON

//                    Required Externally Defined Routines 
void   Copy_Matrix(double *A, double *B, int nrows, int ncols);
double Inner_Product(double u[], double v[], int n);

////////////////////////////////////////////////////////////////////////////////
//  int Tridiagonalize_Symmetric_Matrix(double *A, int storage_class,         //
//               double diagonal[], double off_diagonal[], double *U, int n)  //
//                                                                            //
//  Description:                                                              //
//     This program transforms the symmetric square matrix A to a similar     //
//     tridiagonal matrix by a multiplying A on the right and left by a       //
//     sequence of Householder transformations.                               //
//     Def:  Two matrices A and B are said to be similar if there exists a    //
//           nonsingular matrix S such that A S = S B.                        //
//     Def:  A Householder transformation is an orthogonal transformation of  //
//           the form Q = I - 2 uu'/u'u, where u is a n x 1 column matrix and //
//           ' denotes the transpose.                                         //
//     Thm:  If Q is a Householder transformation then Q' = Q  and  Q' Q = I, //
//           i.e. Q is a symmetric involution.                                //
//     First the input matrix A is copied / expanded to form the matrix U.    //
//     The algorithm proceeds by successivly selecting rows i = n - 1, ..., 1 //
//     and then calculating the Householder transformation Q which annihilates//
//     the components to the left of the subdiagonal for that row and leaves  //
//     the previously selected rows invariant.  The algorithm then updates    //
//     the matrix U, in place, by premultiplication by Q followed by          //
//     postmultiplication by Q.                                               //
//     If the i-th row of U is (a[0],...,a[n-1]), then  choose u' =           //
//     (u[0],...,u[n-1]) by u[0] = 0, ... , u[j] = 0, u[j+2] = a[j+2],...,    //
//     u[n-1] = a[n-1].  The remaining component u[j+1] = a[j+1] - s, where   //
//     s^2 = a[j+1]^2 + ... + a[n-1]^2, and the choice of sign for s,         //
//     sign(s) = -sign(a[j+1]) maximizes the number of significant bits for   //
//     u[j+1].                                                                //
//                                                                            //
//     Note: In general accuracy is improved rows and columns of the matrix   //
//     are ordered so that the elements with the smallest magnitude are       //
//     located at the top left-most corner of the matrix and the elements with//
//     the largest magnitude are located at the bottom right-most corner.     //
//                                                                            //
//  Arguments:                                                                //
//     double *A                                                              //
//            Pointer to the first element of the matrix A[n][n].             //
//     int    storage_class                                                   //
//            The storage class of the matrix A, 0 if full symmetric matrix   //
//            -1 if stored in lower triangular form, +1 if stored in upper    //
//            triangular form.                                                //
//     double diagonal[]                                                      //
//            The diagonal of the tridiagonal symmetric matrix.  The array    //
//            diagonal[] should be dimensioned n in the calling routine.      //
//     double off_diagonal[]                                                  //
//            The subdiagonal and superdiagonal of the tridiagonal symmetric  //
//            matrix.  The subdiagonal and superdiagonal are (off_diagonal[1],//
//            ,,, off_diagonal[n-1]), off_diagonal[0] is set to 0.0.  The     //
//            array off_diagonal[] should be dimensioned n in the calling     //
//            routine.  If QL_Tridiagonal_Symmetric_Matrix is called using    //
//            using diagonal[], off_diagonal[], and U, then off_diagonal      //
//            should be dimensioned n + 1 in the calling routine.             //
//     double *U                                                              //
//            The n x n orthogonal matrix such that AU = UT, where T is the   //
//            tridiagonal matrix with diagonal 'diagonal[]' and off-diagonal  //
//            'off_diagonal[]'.                                               //
//     int    n                                                               //
//            The number of rows or columns of the symmetric matrix A and of  //
//            the matrix U.                                                   //
//                                                                            //
//  Return Values:                                                            //
//     0  Success                                                             //
//    -1  Failure - Illegal storage class.                                    //
//                                                                            //
//  Example:                                                                  //
//     #define N                                                              //
//     double A[N][N], B[N * (N + 1) / 2], U[N][N], diagonal[N];              //
//     double off_diagonal[N];                                                //
//                                                                            //
//     (your code to create the matrices A and B)                             //
//     Tridiagonalize_Symmetric_Matrix((double*)A, 0, diagonal, off_diagonal, //
//                                                          (double*)U, n);   //
//     (if B is the matrix stored in lower triangular form )                  //
//     Tridiagonalize_Symmetric_Matrix((double*)B, -1, diagonal, off_diagonal,//
//                                                          (double*)U, n);   //
//     (if B is the matrix stored in upper triangular form )                  //
//     Tridiagonalize_Symmetric_Matrix((double*)B, 1, diagonal, off_diagonal, //
//                                                          (double*)U, n);   //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
int Tridiagonalize_Symmetric_Matrix(double *A, int storage_class,
                   double diagonal[], double off_diagonal[], double *U, int n)
{
   int i, j, k, col;
   double zero_tolerance = DBL_MIN / DBL_EPSILON; 
   double *pU, *ppU, *pUrow, *pUcol;
   double ss;                             // signed sqrt of sum of squares

       // n x n matrices for which n <= 2 are already in Tridiagonal form

   if (n <= 2) { 
      for (i = 0, k = 0; i < n; i++) { 
         for (j = 0; j < i; j++, k++) *(U + k) = 0.0;
         *(U + k++) = 1.0;
         for (j = i + 1; j < n; j++, k++) *(U + k) = 0.0;
      } 
      return 0;
   }

                             // Initialize U to A.

   switch (storage_class) {
      case -1:  // A stored in lower triangular form
         pUrow = U;
         pUcol = U;
         for (i = 0; i < n; pUcol++, pUrow += n)
            for (j = 0, pU = pUcol; j <= i; j++, pU += n) {
               *(pUrow + j) = *A;
               *pU = *A++;
            }
         break;
      case 0:   // A stored in full matrix form
         Copy_Matrix(U, A, n, n);
         break;
      case 1:   // A stored in upper triangular form
         for (i = 0, pUcol = U; i < n; pUcol += n + 1) {
            pUrow = pUcol;
            for (j = i, pU = pUcol; j < n; j++, pU +=n) {
               *pUrow++ = *A;
               *pU = *A++;
            }
         }
         break;
      default:  // Illegal storage class
         return -1;
   }

           // For each row starting with the last row find the 
           // Householder transformation to zero all entries to
           // the left of the subdiagonal.

   for (i = n - 1, pUrow = U + n * (n - 1); i > 0; i--, pUrow -= n) {

          // Calculate the sum of squares of the elements to the left
          // of the subdiagonal.
      
      pU = pUrow + i - 1;
      ss = Inner_Product(pUrow, pUrow, i - 1);

          // If the sum of squares is too small set, don't transform.

      if (ss < zero_tolerance) {
         off_diagonal[i] = *pU;
         diagonal[i] = 0.0;
         continue;
      }

         // Calculate the signed square root of the sum of squares to the
         // left of the diagonal.

      ss += *pU * *pU;
      off_diagonal[i] = (*pU >= 0.0) ? -sqrt(ss) : sqrt(ss);
      diagonal[i] = ss - *pU * off_diagonal[i];
      *pU -= off_diagonal[i];
      for (j = 0, pU = U, ss = 0.0; j < i; j++, pU += n) {
         *(pU + i) = *(pUrow +j) / diagonal[i];
         off_diagonal[j] = Inner_Product(pU, pUrow, j + 1);
         for (k = j+1, ppU = pU + n; k < i; k++, ppU += n)
            off_diagonal[j] += *(ppU + j) * *(pUrow + k);
         ss += off_diagonal[j] * *(pU + i);
         off_diagonal[j] /= diagonal[i];
      } 
      ss /= (diagonal[i] + diagonal[i]);
      for (j = 0, pU = U; j < i; j++, pU += n) {
        off_diagonal[j] -= ss * *(pUrow + j);
        for (k = 0; k <= j; k++)
           *(pU + k) -= ( *(pUrow + j) * off_diagonal[k]
                          + off_diagonal[j] * *(pUrow + k) );
      }
   }
   diagonal[0] = 0.0;
   off_diagonal[0] = 0.0; 
   for (i = 0, pUrow = U; i < n; i++, pUrow += n) {
      if ( diagonal[i] != 0.0 ) 
         for (j = 0; j < i; j++) {
            for (k = 0, pU = U, ss = 0.0; k < i; k++, pU += n)
               ss += *(pUrow + k) * *(pU + j);
            for (k = 0, pU = U; k < i; k++, pU += n)
               *(pU + j) -= ss * *(pU + i);
         }
      diagonal[i] = *(pUrow + i);
      *(pUrow + i) = 1.0;
      for (j = 0, pU = U; j < i; j++, pU += n) {
         *(pUrow + j) = 0.0;
         *(pU + i) = 0.0;
      }
   }    
   return 0;
}
