source('secular_equation.r')

dac <- function( T, inertia, epsilon){
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

	print('cuppen')

	#debug (rebuild T with the expression above)
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
	# at this stage T = Q %*% ( [D1  ]  + p * v %*% t(v) ) %*% t(Q)
	d     <- c( eigen(T1,symmetric=TRUE)$values,
			 eigen(T2,symmetric=TRUE)$values )

	#debug (check that we actually find T with the expression above)
	#check <- Q %*% ( D + p * v %*% t(v) ) %*% t(Q) 
	#return(check)

	print('deflation part 1')
	# deflation
	P <- diag(n) # deflation  operations
	# zero out values too small for single precision
	h <- epsilon * sqrt(sum( ( diag(d) + p * v %*% t(v) )^2 ) )
	i <- 1 # active column
	j <- n # target column for switching
	while( i < j ){
		if( abs(p*v[i]) < h || abs(d[i]) <h ){
			while( abs(p*v[j]) < h && j > i ){
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

	#debug (test permutation matrix)
	#return(list(D=D,v=v,P=P))

	
	# erase multiple eigen values
	print('deflation part2')
	m <- 1
	while( m < j ){
		for( k in seq(m+1,j) ){
			if( 
			   abs( v[m]*v[k] 
				* ( d[m] - d[k] ) 
				/ sqrt( v[m]^2 + v[k]^2 ) ) < h ){
				print("deflation part1 triggered")
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

	#debug 
	#return( Q %*% P %*% ( diag(d) + p * v %*% t(v) ) %*% t(P) %*% t(Q) )
	print( "finding eigen values" )
	ordered <- sort( d, index.return = TRUE, decreasing=FALSE )
	dsort <- ordered$x
	order <- ordered$ix
	vsort <- v[order]
	lambdas <- roots_secular_equation(p, vsort, dsort, inertia)
	# return(list(v=v, val=lambdas, d=d, H=Q%*%P, p=p ) )

	print("gu and eisenstat method")
	v2 <- gu_eisenstat_vector( p, n, d, lambdas )

	#print( "finding eigen vectors" )
	eigen_vector <- function(k){
			r <-  (diag (1/(lambdas[k] - d)) %*% v2 )
			r <- r / sqrt((t(r)%*%r)[1])
			return(r)
	}
	V <- sapply(1:n,eigen_vector)
	#print(A)
	U1 <- ( diag(d) + p * v %*% t(v) )
	U2 <- ( diag(d) + p * v2 %*% t(v2) )

	
	return(list(Q=Q,P=P,U1=U1,U2=U2,V=V,p=p,d=d,v=v,v2=v2))
}
