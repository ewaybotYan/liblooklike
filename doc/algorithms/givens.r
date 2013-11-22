givens_rotation <- function( v, i, j ){
	n <- length(v)
	G <- diag(n) #generation of the givens rotation matrix

	r <- sqrt(v[i]*v[i]+v[j]*v[j]) #calculation of the coefficients..
	c <- v[i]/r #cosinus and...
	s <- v[j]/r #sinus of the matrix
	
	G[i,i]=c #affectation of coefficients
	G[j,j]=c
	G[j,i]=-sign(j-i)*s
	G[i,j]=sign(j-i)*s

	return(G)
}
