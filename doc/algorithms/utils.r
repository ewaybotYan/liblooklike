make_random_sym_matrix <- function( n ){
	A <- array( sample(0:128,n*n), dim=c(n,n) )
	return( A %*% t(A) )
}

make_random_tridiag_sym_matrix <- function (n){
	X <- n
	A <- matrix(runif(X*X,0,1),X,X)
 	A<-(A+t(A))/2

	for (i in 1:n){
		for (j in i:n){
			if (j>i+1){
				A[i,j]=0
				A[j,i]=0
			}
		}
	}

	return(A)
}

permutation_matrix <- function( n, i, j ){
	ei  <- 1:n == i
	ej  <- 1:n == j
	m   <- ei - ej
	per <- diag(n) - m %*% t(m)
	return(per)
}
