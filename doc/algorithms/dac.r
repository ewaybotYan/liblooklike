dac <- function( T ){
	n <- dim(T)[1]

	# cuppen method to represent the matrix
	k    <- round( n / 2 )
	beta <- sign( T[k,k] ) # todo : limit cancelation for real
	p    <- beta * T[k,k+1]
	u    <- beta * ((1:n)==k) + ((1:n) == k+1)
	Tmp  <- T - p * u %*% t(u)
	T1   <- Tmp[1:k,1:k]
	T2   <- Tmp[seq(k+1,n),seq(k+1,n)]
	# at this stage T = [T1  ] + p * u %*% t(u)
	#                   [  T2]

	#debug (rebuild T with the expression above)
	#Id    <- diag(1,k,n)
	#check <- t(Id) %*% T1 %*% Id
	#Id    <- array( c( 1:(k*(n-k)) * 0, 1:(n-k)^2 %% (n-k+1) == 1 ) , dim=c(n-k,n) )
	#check <- check + t(Id) %*% T2 %*% Id
	#check <- check + p * u %*% t(u)
	#return( check )

	# apply divide and conquer
	# note we rely on internal R function, so no actual recursion here
	Q1   <- eigen(T1,symmetric=TRUE)$vectors
	Q2   <- eigen(T2,symmetric=TRUE)$vectors
	ctor <- diag(1,k,n) # used to construct Q
	Q    <- t(ctor) %*% Q1 %*% ctor
	Id   <- array( c( 1:(k*(n-k)) * 0, 1:(n-k)^2 %% (n-k+1) == 1 ) , dim=c(n-k,n) )
	Q    <- Q + t(Id) %*% Q2 %*% Id
	v    <- t(Q) %*% u
	# at this stage T = Q %*% [D1  ] %*% t(Q) + p * v %*% t(v)
	D     <- diag( c( eigen(T1,symmetric=TRUE)$values,
			 eigen(T2,symmetric=TRUE)$values ) )

	#debug (check that we actually find T with the expression above)
	check <- Q %*% ( D + p * v %*% t(v) ) %*% t(Q) 
	return(check)

	# deflation
	P <- diag(n) # deflation  operations
	# zero out values too small for single precision
	eps <- 10^-20
	h <- eps * norm( D + p * v %*% t(v) )
	i <- 1 # active column
	j <- n # target column for switching
	while( i < j ){
		while( v[i] < h )
			
	}
	# zero multiple eigen values


}
