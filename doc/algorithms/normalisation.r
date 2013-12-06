normalisation<-function(A){
	n<-dim(A)[1]
	mu<-(1:n)*0
	sigma<-(1:n)*1

	for (j in 1:n){
		for (i in 1:n){
			mu[j]<-mu[j]+A[i,j]
		}
	}
	mu<-mu/n
	print(mu)

	for (j in 1:n){
		for (i in 1:n){
			sigma[j]<-(A[i,j]-mu[j])^2
		}
	}
	sigma<-sigma/n
	print(sigma)

	for (j in 1:n){
		for (i in 1:n){
			A[i,j]<-(A[i,j]-mu[j])/sigma[j]
		}
	}

	return(A)
}
