u1 <- c(6,5,0)
u2 <- c(5,1,4)
u3 <- c(0,4,3)

A <- matrix(c(6,5,0,5,1,4,0,4,3),nrow=3,ncol=3)

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
