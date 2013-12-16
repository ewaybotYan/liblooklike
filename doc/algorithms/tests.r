source('householder.r')

tests <- function(n,inertia_rate,esilon){
	Test<-make_random_sym_matrix(n)
	Matrices<-householder_matrix(Test)
	A<-Matrices$A
	H<-Matrices$H
	res <- dac(A,inertia_rate,epsilon)
	print("maximum difference between diagonalized matrix and computed eigen
	      values")
	max(abs( ((t(res$vectors) %*% A %*% res$vectors)) - diag(res$values[,1])))
	print("maximum difference between the computed eigen values and R ones")
	max(abs( sort(res$values) - sort(eigen(Test)) ))
}


