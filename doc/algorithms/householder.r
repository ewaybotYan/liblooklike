
householder_symetry_vector <- function( u, k ){
  n <- length( u )
  v <- matrix(u * ((1:n)>k),n,1)
  v <- v - (-1) * sign( u[k+1] ) * sqrt(t(v)%*%v) * ((1:n)==k+1)
  return ( v )
}

P_k <- function( A, k ){
  v = householder_symetry_vector( A[,k], k )
  n <- dim(v)[1]
  return( diag(n) - 2 * ( v %*% t(v) ) / matrix(( t(v) %*% v ),n,n) )
}

householder_matrix <- function( A ){
	n <- sqrt(length(A))

	H <- diag(n)

	for (i in 1:(n-1)){
		P <- P_k(A,i)
		H <- P %*% H
		A <- P %*% A %*% P
	}

	return( list(A=A,H=H) )
}
