

find_eigenvector22<-function(A,lambda,k){
	n<-dim(A)[1]
	if (n!=2){
		print("La matrice doit être de dimesion 2.")
	}

	if (A[1,2]==0){
		if (k==1){
			return(c(0,1))
		}else{
			return(c(1,0))
		}
	}else{
		u1<-1
		u2<-((lambda-A[1,1])/A[1,2])
		return(cbind(u1,u2))
	}
}

find_eigenvalues22<-function(A){
	a<-A[1,1]
	b<-A[1,2]
	d<-A[2,2]
	if (((a+d)^2-4*(a*d-b^2))<0){
		print("Problème de valeurs propres")
	}else{
		lambda1<-((a+d)-sqrt((a+d)^2-4*(a*d-b^2)))/2
		lambda2<-((a+d)+sqrt((a+d)^2-4*(a*d-b^2)))/2
		return(c(lambda1,lambda2))
	}
}

find_eigens22<-function(A){
	eigenvalues<-find_eigenvalues22(A)
	lambda1<-eigenvalues[1]
	lambda2<-eigenvalues[2]
	eigenvector1<-find_eigenvector22(A,lambda1,1)
	eigenvector2<-find_eigenvector22(A,lambda2,2)
	return(list(values=c(lambda1,lambda2),vectors=cbind(eigenvector1,eigenvector2)))
}

#A<-matrix(c(1,4,4,3),nrow=2,ncol=2,byrow=TRUE)
#print(A)
#res<-find_eigens22(A)
#print(res)
