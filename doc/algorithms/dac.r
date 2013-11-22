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

	print('cuppen')

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
	# at this stage T = Q %*% ( [D1  ]  + p * v %*% t(v) ) %*% t(Q)
	D     <- diag( c( eigen(T1,symmetric=TRUE)$values,
			 eigen(T2,symmetric=TRUE)$values ) )

	#debug (check that we actually find T with the expression above)
	#check <- Q %*% ( D + p * v %*% t(v) ) %*% t(Q) 
	#return(check)

	print('deflation part 1')
	# deflation
	P <- diag(n) # deflation  operations
	# zero out values too small for single precision
	eps <- 10^-20
	h <- eps * sqrt(sum( ( D + p * v %*% t(v) )^2 ) )
	i <- 1 # active column
	j <- n # target column for switching
	while( i < j ){
		if( abs(p*v[i]) < h || abs(D[i,i]) <h ){
			while( abs(p*v[j]) < h && j > i ){
				j <- j-1
			}
			if( i != j ){
				# switch values manually
				v[i] <- v[j]
				v[j] <- 0
				tmp  <- D[i,i]
				D[i,i] <- D[j,j]
				D[j,j] <- tmp
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
	print(D)
	print(v)
	print(P)
	print(j)
	#return(list(D=D,v=v,P=P))

	
	# erase multiple eigen values
	print('deflation part2')
	m <- 1
	while( m < j ){
		for( k in seq(m+1,j) ){
			if( 
			   abs( v[m]*v[k] 
				* ( D[m,m] - D[k,k] ) 
				* sqrt( v[m]^2 + v[k]^2 ) ) < h ){
				print(m)
				print(k)
				print(v[m])
				print(v[k])
				print(D[m,m])
				print(D[k,k])
				# rotatea with givens
				v[k] <- 0 
				# D is not changed by Givens rotations
				P <- P %*% givens_rotation( v, m, k )
				print(P)
				# move diagonal part in the down right part of T
				if( k != j ){
					v[k] <- v[j]
					v[j] <- 0
					D[k,k] <- D[j,j]
					D[j,j] <- D[m,m]
					P <- P %*% permutation_matrix(n,m,j)
				}
				j <- j-1
			}
		}
		m <- m+1
	}
	return(list(D=D,v=v,Q=Q,P=P,p=p))

	#debug 
	# return( Q %*% P %*% ( D + p * v %*% t(v) ) %*% t(P) %*% t(Q) )
}

