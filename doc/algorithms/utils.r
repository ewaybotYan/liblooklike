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

random_deflation_matrix <- function(p,z,h){
#set.seed(1)
	n <- length(z)

#generation of a diagonal matrix with deflation properties
	for (i in 1:n){
		D[i,i]<-runif(1,0,1)
	}

	for (i in 1:floor(runif(1,n/2,n)){
		j<-floor(runif(1,1,n))
		k<-floor(runif(1,1,n))
		D[k,k]<-(D[j,j]+((h*sqrt(z[k]^2+z[j]^2)/(abs(z[k]*z[j]))) #almost the same eigenvalues
		m<-floor(runif(1,1,n))
		r<-floor(runif(1,1,n))
		D[m,m]<-D[r,r] #same eigenvalues
	}

P <- matrix(runif(n*n,-10,10),n,n)

return(solve(P) %*% D %*% P)
}
