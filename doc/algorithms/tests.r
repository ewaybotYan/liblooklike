source('normalisation.r')
source('householder.r')
source('dac.r')

tests <- function(m,n,inertia_rate,epsilon){
        A <- matrix(runif(n*m,0,255),m,n)
        A <- normalisation(A)
        B <- A %*% t(A)
	M <- householder_matrix(B)
	A<-Matrices$A
	H<-Matrices$H
	res <- dac(A,inertia_rate,epsilon)
	print("maximum difference between diagonalized matrix and computed eigenvalues")
	print(max(abs( ((t(res$vectors) %*% A %*% res$vectors)) - diag(c(res$values)))))
	print("maximum difference between the computed eigen values and R ones")
    return (Test)
	print(max(abs( sort(c(res$values)) - sort(eigen(Test)$values) )))
}
