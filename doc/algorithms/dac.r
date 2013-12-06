source('secular_equation.r')

dac <- function( T, inertia, epsilon){
	#print(T)
	n <- dim(cbind(T))[1]
	if( n == 1 ){
		#return( list(values=diag(1), vectors=diag(T)) )
		 return(eigen(T))
	}
	if( n == 2 ){
		 return(eigen(T))
	}


	# cuppen method to represent the matrix
	k    <- round( n / 2 )
	beta <- sign( T[k,k] ) # todo : limit cancellation for real
	p    <- beta * T[k,k+1]
	u    <- beta * ((1:n)==k) + ((1:n) == k+1)
	Tmp  <- T - p * u %*% t(u)
	T1   <- Tmp[1:k,1:k]
	T2   <- Tmp[seq(k+1,n),seq(k+1,n)]
	# at this stage T = [T1  ] + p * u %*% t(u)
	#                   [  T2]

#	print('cuppen')

	#debug (rebuild T with the expression above)
	#check <- check + p * u %*% t(u)
	#return( check )

	# apply divide and conquer
	# note we rely on internal R function, so no actual recursion here
	#res1   <- eigen(T1,symmetric=TRUE)
	#res2   <- eigen(T2,symmetric=TRUE)
	res1    <- dac(T1, inertia, epsilon )
	res2    <- dac(T2, inertia, epsilon )
	Q1      <- res1$vectors
	Q2      <- res2$vectors
	# r1     <- dac(T1, 1.1, epsilon)
	ctor <- diag(1,k,n) # used to construct Q
	Q    <- t(ctor) %*% Q1 %*% ctor
	Id   <- array( c( 1:(k*(n-k)) * 0, 1:(n-k)^2 %% (n-k+1) == 1 ) , dim=c(n-k,n) )
	Q    <- Q + t(Id) %*% Q2 %*% Id
	v    <- t(Q) %*% u
	# at this stage T = Q %*% ( [D1  ]  + p * v %*% t(v) ) %*% t(Q)
	#                           [  D2]
	d     <- c( res1$values, res2$values )

	# we want positive values on s
	S    <- diag( sapply(v,sign) )
	d    <- d  # is not changed in this new base
	v    <- S %*% v
	#debug (check that we actually find T with the expression above)
	#check <- Q %*% ( diag(d) + p * v %*% t(v) ) %*% t(Q) 
	#return(check)

	#print(c("d",d,"v",v,"Q1",Q1,"Q2",Q2))
#	print('deflation part 1')
	# deflation
	P <- diag(n) # deflation  operations
	# zero out values too small for single precision
	h <- epsilon * max(sapply(1:n,function(k){return
				  (norm((diag(d)+p*v%*%t(v))[,k]))}))
	i <- 1 # active column
	j <- n # target column for switching
	while( i < j ){
		if( abs(p*v[i]^2) < h ){
			while( abs(p*(v[j])^2) < h && j > i ){
				j <- j-1
			}
			print("deflation part1 triggered")
			if( i != j ){
				# switch values manually
				v[i] <- v[j]
				v[j] <- 0
				tmp  <- d[i]
				d[i] <- d[j]
				d[j] <- tmp
				# save permutation matrix
				P <- P %*% permutation_matrix(n,i,j)
				j <- j-1
				# todo i<- i+1 ??
			}
		}else{
			i <- i+1
		}
	}

	# check matrix state
	for( i in 1:n )
		for( j in 1:n)
			if( is.nan(P[i,j]))
				print(c("!",i,j ))

	#debug (test permutation matrix)
	#return(list(D=D,v=v,P=P))


	# erase multiple eigen values
#	print('deflation part2')
	m <- 1
	while( m < j ){
		for( k in seq(m+1,j) ){
			if( 
			   abs( v[m]*v[k] 
			       * ( d[m] - d[k] ) 
			       / sqrt( v[m]^2 + v[k]^2 ) ) < h ){
				print("deflation part2 triggered")
				# rotate with givens
				v[k] <- 0 
				# D is not changed by Givens rotations
				P <- P %*% givens_rotation( v, m, k )
				#print(P)
				# move diagonal part in the down right part of T
				if( k != j ){
					v[k] <- v[j]
					v[j] <- 0
					d[k] <- d[j]
					d[j] <- d[m]
					P <- P %*% permutation_matrix(n,m,j)
				}
				j <- j-1
			}
		}
		m <- m+1
	}

	# check matrix state
	for( i in 1:n )
		for( j in 1:n)
			if( is.nan(P[i,j]))
				print(c("!d2",i,j ))

	#debug 
	#return( Q %*% P %*% ( diag(d) + p * v %*% t(v) ) %*% t(P) %*% t(Q) )
#	print( "finding eigen values" )
	ordered <- sort( d[1:j], index.return = TRUE, decreasing=FALSE )
	dsort <- ordered$x
	order <- ordered$ix
	vsort <- v[order]
	trace <- sum( diag( diag(d[1:j]) + p * v[1:j] %*% t(v[1:j])))
	lambdas <- roots_secular_equation(p, vsort, dsort, trace)


	# return(list(v=v, val=lambdas, d=d, H=Q%*%P, p=p ) )

	#print(c("@@",dsort))
	#print(c("##",lambdas))

#	print("gu and eisenstat method")
	v2 <- gu_eisenstat_vector( p, j, dsort, lambdas )
	#print( "finding eigen vectors" )
	eigen_vector <- function(k){
		r <-  (diag (1/(lambdas[k] - dsort)) %*% v2 )
		r <- r / sqrt((t(r)%*%r)[1])
		return(r)
	}
	V <- sapply(1:j,eigen_vector)

	# bring back deflated values
	if(j<n)
		lambdas <- cbind( c(lambdas, d[seq(j+1,n)]) )

	# check matrix state
	for( i in 1:n )
		if( is.nan(lambdas[i]))
			print(c("!l",i))

	V <- cbind( V, diag(0,j,n-j) )
	V <- rbind( V, diag(0,n-j,n) )
	V <- V + diag( (1:n>j) )

	# check matrix state
	for( i in 1:n )
		for( j in 1:n)
			if( is.nan(V[i,j]))
				print(c("!V",i,j ))

	#print(A)
	U1 <- ( diag(d) + p * v %*% t(v) )

	# permutation matrix to restore order
	O  <- cbind(sapply(1:j, function(k){ return(1:n == order[k]) }
			   ),rbind(matrix(0,j,n-j),diag(n-j)))

	U2 <- ( diag(dsort) + p * v2 %*% t(v2) ) 


	#return(list(Q=Q,P=P,S=S,O=O,U1=U1,U2=U2,V=V,lambdas=lambdas,v=v,v2=v2,p=p,d=dsort))
	#V <- res$O %*% res$V %*% t(S)
	return( list(vectors = Q %*% S %*% P %*% O %*% V, values=lambdas, U2=U2,
		     U1=U1, V=V, O=O ) )
}
