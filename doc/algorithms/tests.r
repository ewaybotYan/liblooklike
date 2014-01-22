source('householder.r')
source('dac.r')

tests <- function(n,inertia_rate,epsilon){
	Test<-make_random_sym_matrix(n)
	Matrices<-householder_matrix(Test)
	A<-Matrices$A
	H<-Matrices$H
	res <- dac(A,inertia_rate,epsilon)
	print("maximum difference between diagonalized matrix and computed eigenvalues")
	print(max(abs( ((t(res$vectors) %*% A %*% res$vectors)) - diag(res$values))))
	print("maximum difference between the computed eigen values and R ones")
	print(max(abs( sort(res$values) - sort(eigen(Test)$values) )))
	print("maximum difference between the computed eigen vectors and R ones")
	print(max(abs( sort(res$vectors[1]) - sort(eigen(Test)$vectors[1]) )))
    return (Test)
}
