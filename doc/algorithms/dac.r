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


	print('cuppen')
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
	check <- S %*% Q %*% ( diag(d) + p * v %*% t(v) ) %*% t( S %*% Q ) 
	if(max(abs(check-T))> epsilon ){
            print(T)
            print(check)
            exit()
        }



	#print(c("d",d,"v",v,"Q1",Q1,"Q2",Q2))
	print('deflation part 1')
	# deflation
	P <- diag(n) # deflation  operations
	# zero out values too small for single precision
	h <- epsilon * max(abs(sapply(1:n,function(k){return
				  (norm((diag(d)+p*v%*%t(v))[,k]))})))
	i <- 1 # active column
	j <- n # target column for switching
	while( i < j ){
		if( abs(p*v[i])*sqrt(t(v)%*%v) < h ){
			while( abs(p*(v[j])*sqrt(t(v)%*%v) ) < h && j > i ){
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
			}
		}else{
			i <- i+1
		}
	}

	# check matrix state
	#for( l in 1:n )
	#	for( k in 1:n)
	#		if( is.nan(P[l,k]))
	#			print(c("!",l,k ))

	#debug (test permutation matrix)
	#return(list(D=D,v=v,P=P))
	check <- P %*% S %*% Q %*% ( diag(d) + p * v %*% t(v) ) %*% t( P %*% S %*% Q )
	if(max(abs(check-T))> epsilon ){
            print(T)
            print(check)
            exit()
        }


	# we sort d values
        print("sort")
        print(d)
	ordered <- sort( d[1:j], index.return = TRUE, decreasing=FALSE )
	order <- ordered$ix
	if(j<n){
		order <- c(order,seq(j+1,n))
	}
	d <- d[order]
	v <- v[order]
	#print(order)

	# permutation matrix to restore order
	O  <- cbind(sapply(1:j, function(k){ return(1:n == order[k]) }),rbind(matrix(0,j,n-j),diag(n-j)))
#        O <- t(O)
	check <- O %*% P %*% S %*% Q %*% ( diag(d) + p * v %*% t(v) ) %*% t( O %*% P %*% S %*% Q )
	if(max(abs(check-T))> epsilon ){
            print(O)*1
            print(T)
            print(O %*% d)
            print(check)
            exit()
        }

	# erase multiple eigen values
	print('deflation part2')
	m <- 1
	while( m < j ){
		k <- m+1
		while( (k<=j) &&
		      (abs(d[m] - d[k]) * v[m]*v[k] / sqrt(v[m] * v[k]))  < h ){
			print(c("deflation part2 triggered",m,k,v[m]))
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
			k <- k+1
		}
		m <- m+1
	}

	# we sort d values
	print("sort")
	ordered <- sort( d[1:j], index.return = TRUE, decreasing=FALSE )
	order <- ordered$ix
	if(j<n){
		order <- c(order,seq(j+1,n))
	}
	d <- d[order]
	v <- v[order]


	# permutation matrix to restore order
	O  <- O %*% cbind(sapply(1:j, function(k){ return(1:n == order[k]) }
			   ),rbind(matrix(0,j,n-j),diag(n-j)))

#	# check matrix state
#	for( k in 1:n )
#		for( l in 1:n)
#			if( is.nan(P[k,l]))
#				print(c("!d2",k,l ))

	#debug 

	print( "finding eigen values" )

	V <- diag(n)
	lambdas <- d

	if(j != 0){
		trace <- sum( diag( diag(d[1:j]) + p * v[1:j] %*% t(v[1:j])))
		lambdas <- roots_secular_equation(p, v[1:j], d[1:j], trace)

		# return(list(v=v, val=lambdas, d=d, H=Q%*%P, p=p ) )

		#print(c("@@",dsort))
		#print(c("##",lambdas))

		print("gu and eisenstat method")
		v2 <- gu_eisenstat_vector( p, j, d[1:j], lambdas )
		print( "finding eigen vectors" )
		eigen_vector <- function(k){
			r <-  diag(1/(lambdas[k] - d[1:j])) %*% v2
			r <- r / sqrt((t(r)%*%r)[1])
			if( is.nan(sum(r)) ){
				print(c("!ev", k,h))
			       	print(c("lambdas=",lambdas))
				print(c("d=",d))
				exit()
			}
			return(r)
		}
		V <- sapply(1:j,eigen_vector)
		# bring back deflated values
		if(j<n){
			lambdas <- cbind( c(lambdas, d[seq(j+1,n)]) )
			V <- cbind( V, diag(0,j,n-j) )
			V <- rbind( V, diag(0,n-j,n) )
			V <- V + diag( (1:n>j) )
		}
	}

	# check matrix state
	for( i in 1:n )
		if( is.nan(lambdas[i]))
			print(c("!l",i))


	# check matrix state
	for( i in 1:n )
		for( j in 1:n)
			if( is.nan(V[i,j])){
				print(c("!!V",i,j ))
				print(V)
				exit()
			}

	#print(A)
	U1 <- ( diag(d) + p * v %*% t(v) )

	#U2 <- ( diag(dsort) + p * v2 %*% t(v2) ) 


	#return(list(Q=Q,P=P,S=S,O=O,U1=U1,U2=U2,V=V,lambdas=lambdas,v=v,v2=v2,p=p,d=dsort))
	#V <- res$O %*% res$V %*% t(S)
	return( list(vectors = Q %*% S %*% P %*% O %*% V, values=lambdas ) )
}
