make_random_sym_matrix <- function( n ){
	A <- array( sample(0:128,n*n), dim=c(n,n) )
	return( A %*% t(A) )
}
