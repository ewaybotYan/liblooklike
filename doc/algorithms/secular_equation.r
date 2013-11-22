
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
	dPsi1=dPsi2=0

	for (k in 1:i){
	dPsi1 <- dPsi1+(v[k]^2) / ((d[k]-x)^2)
	}
	for (k in (i+1):n){
	dPsi2 <- dPsi2+(v[k]^2) / ((d[k]-x)^2)
	}

	return( list(dPsi1=dPsi1,dPsi2=dPsi2) )

}

roots_secular_equation <- function (p,v,d){ #d trié ou non?

}
