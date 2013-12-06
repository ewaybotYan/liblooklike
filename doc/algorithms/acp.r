acp<-function(n){
R<-matrix(255*runif(n*n),ncol=n)
X<-normalisation(R)
A<-t(X) %*% X
Householder<-householder_matrix(A)
	A<-Householder$A
	H<-Householder$H
Dac<-dac(A,1,10^(-15))
}
