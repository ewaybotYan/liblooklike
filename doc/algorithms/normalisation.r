normalisation<-function(A){
        m<-dim(A)[1]
	n<-dim(A)[2]
	mu<-(1:n)*0
	sigma<-(1:n)*1

	for (j in 1:n){
		for (i in 1:m){
			mu[j]<-mu[j]+A[i,j]
		}
	}
	mu<-mu/n

	for (j in 1:n){
		for (i in 1:m){
			sigma[j]<-sigma[j] + (A[i,j]-mu[j])^2
		}
	}
	sigma<-sigma/n

	for (j in 1:n){
		for (i in 1:m){
			A[i,j]<-(A[i,j]-mu[j])/sigma[j]
		}
	}

	return(A)
}
