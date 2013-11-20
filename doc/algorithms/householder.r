
householder_symetry_vector <- function( u, k ){
  n <- length( u )
  v <- u * ((1:n)>k)
  v <- v - (-1) * sign( u[k+1] ) * norm(v) * ((1:n)==k+1)
  return (v)
}

P_k <- function( A, k ){
  n <- dim(A)[1]
  v = householder_symetry_vector( A[,k], k )
  return( diag(n) - 2 * ( v %*% t(v) ) / ( norm(v)^2 ) )
}

householder_matrix <- function( A ){
	n<-sqrt(length(A))

	H<-P(A,1)
	A<-H%*%A%*%t(H)

	for (i in 2:n){
		P<-P_k(A,i)
		H<-P%*%H
		A<-P%*%A%*%t(P)
	}
	A
	return(H)
}
