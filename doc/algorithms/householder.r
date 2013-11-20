
householder_symetry_vector <- function( u, k ){
  n <- length( u )
  v <- u * ((1:n)>k)
  v <- v - (-1) * sign( u[k+1] ) * norm(v) * ((1:n)==k+1)
  return (v)
}

P <- function( A, k ){
  n <- dim(A)[1]
  v = householder_symetry_vector( A[,k], k )
  return( diag(n) - 2 * ( v %*% t(v) ) / ( norm(v)^2 ) )
}
