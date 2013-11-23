
find_approx_lambda <- function(c1,c2,c3,di,dj){
	b=c1+c2+c3*(di+dj)
	delta=(b)^2 - 4*c3*(c1*dj+c2*di+c3*di*dj)

	if (delta=0){
		return(b)/(2*c3)
	}
	else{ #pb
		lambda1=(b-sqrt(delta))/2*c3
		lambda2=(b+sqrt(delta))/2*c3

		if (c3>=0){
			return(lambda2)
		}
		else{
			return(lambda1)
		}
	}
}

dPsi <- function(v,d,i,x){ #attention, d est le vecteur TRIE des dk
	n <- length(v)
	DPsi1=DPsi2=0

	for (k in 1:i){
	DPsi1 <- DPsi1+((v[k]^2) / ((d[k]-x)^2))
	}
	for (k in (i+1):n){
	DPsi2 <- DPsi2+((v[k]^2) / ((d[k]-x)^2))
	}

	return( list(DPsi1=DPsi1,DPsi2=DPsi2) )
}

psi <- function(v,d,i,x){
	n <- length(v)
	Psi1=Psi2=0

	for (k in 1:i){
	Psi1 <- Psi1+((v[k]^2) / (d[k]-x))
	}
	for (k in (i+1):n){
	Psi2 <- Psi2+((v[k]^2) / (d[k]-x))
	}

	return( list(Psi1=Psi1,Psi2=Psi2) )
}

roots_secular_equation <- function (p,v,D,rate){ #d trié ou non? ATT: rate compris entre 0 et 1 + ATT: valeurs aux bornes à gérer avec p

	n <- length(v)
	sum <- trace(D)
	d... #à voir
	j <- 0
	i<-0 #number of eigenvalues found

	for (ind in 1:n){
		lambda_k[ind]=0
	}

	#et si p=0?
	if (p>0){
		lambda <- #?
		sum_lambdak <- lambda
		i<-i+1
		lambda_k[n+1-i]<-lambda
	}
	else{
		sum_lambdak <- 0
	}


	while ( ((sum_k/sum)<rate) '&& (j>n-1)' ){

		lambda <- (d[n-j-1]+d[n-j])/2

		for (k in 1:3){
		Psi <- psi(v,d,n-j-1,lambda)
		DPsi <- dPsi(v,d,n-j-1,lambda)
		c1 <- DPsi[1]*((d[n-j-1]-lambda)^2)
		c2 <- DPsi[1]*((d[n-j]-lambda)^2)
		c3 <- 1+Psi[1]+Psi[2]-DPsi[1]*(d[n-j-1]-lambda)-DPsi[2]*(d[n-j]-lambda)
		lambda=find_approx_lambda(c1,c2,c3,d[n-j-1],d[n-j])
		}

	sum_lambdak <- sum_lambdak+lambda
	j <- j+1
	i<-i+1
	lambda_k[n+1-i]<-lambda
	}


	if (p<0){
		lambda <- #?
		sum_lambdak <- sum_lambdak+lambda
		i<-i+1
		lambda_k[n+1-i]<-lambda
	}

return (lambda_k)
}

eigen_vector(p,n,d,lambda) <- function{ #d trié

	for (k in 1:n){
	p1=p2=p3=p4=1

		for (j in 1:(k-1)){
			p1 <- p1*(d[k]-lambda[j])
		}
		for (j in k:n){
			p2 <- p2*(lambda[j]-d[k])
		}
		for (j in 1:(k-1)){
			p3 <- p3*(d[k]-d[j])
		}
		for (j in (k+1):n){
			p4 <- p4*(d[j]-d[k])
		}

	v[k]<-sqrt((p1*p2)/(p*p3*p4))
	}

}
