
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

roots_secular_equation <- function (p,v,D,rate){ #d trié ou non?
	d...
	n <- length(v)
	sum <- trace(D)
	sum_k <- 0
	j <- 0
	while ((sum_k/sum)<rate){
	DPsi <- dPsi(v,d,n-j-1,(d[n-j-1]+d[n-j])/2)
	find_approx_lambda(c1,c2,c3,d[n-j-1],d[n-j])
	}
}
