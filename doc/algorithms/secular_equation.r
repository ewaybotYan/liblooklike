find_approx_lambda <- function(c1,c2,c3,di,dj){
    b <- -1 * (c1+c2+c3*(di+dj))
        delta <- ((b)^2 - 4*c3*(c1*dj+c2*di+c3*di*dj))

        if (delta==0){
            return((b)/(2*c3))
        }
        else{ #pb
            lambda1 <- ((-b-sqrt(delta))/(2*c3))
                lambda2 <- ((-b+sqrt(delta))/(2*c3))

                if ((lambda2>di)&&(lambda2<dj)){
                    return(lambda2)
                }
                else{
                    return(lambda1)
                }
        }
}

dPsi <- function(v,d,p,i,x){ #attention, d est le vecteur TRIE des dk
    n <- length(v)
        DPsi1 <- 0
        DPsi2 <- 0

        for (k in 1:i){
            DPsi1 <- (DPsi1+((v[k]^2) / ((d[k]-x)^2)))
        }
    for (k in (i+1):n){
        DPsi2 <- (DPsi2+((v[k]^2) / ((d[k]-x)^2)))
    }

    if( is.nan(DPsi1 + DPsi2)) {
        print(c("!dpsi",d,x))
    }

    return(p * c(DPsi1,DPsi2) )
}

psi <- function(v,d,p,i,x){
    n <- length(v)
        Psi1 <- 0
        Psi2 <- 0

        for (k in 1:i){
            Psi1 <- (Psi1+((v[k]^2) / (d[k]-x)))
        }
    for (k in (i+1):n){
        Psi2 <- (Psi2+((v[k]^2) / (d[k]-x)))
    }

    return( p*c(Psi1,Psi2) )
}


# d must be sorted before calling this function!
roots_secular_equation <- function (p, v, d, trace){ 

# eliminate trivial case
    if(p == 0){
        return(d)
    }

    n  <- length(v)
#print(c("inertia",inertia))
        found_rank <- 0
        i  <- 0 #number of eigenvalues found
        j  <- 0 # d[n-j-1] d[n-j] is the interval in which we look for a root

        lambda_k <- (1:n) * 0

        if(p>0){
            i <- i+1
        }

    while (j < n -1){

        lambda <- (d[n-j-1]+d[n-j])/2
#print(c("#",d[n-j-1],d[n-j]))
            for (k in 1:10){
                Psi <- psi(v,d,p,n-j-1,lambda)
                    DPsi <- dPsi(v,d,p,n-j-1,lambda)
                    c1 <-DPsi[1] * ( ( d[n-j-1] - lambda )^2 )
                    c2 <- DPsi[2] * ((d[n-j]-lambda)^2)
                    c3 <- 1+Psi[1]+Psi[2]-DPsi[1]*(d[n-j-1]-lambda)-DPsi[2]*(d[n-j]-lambda)
                    if( is.nan(c1 * c2 * c3* d[n-j-1] * d[n-j]) ){
                        print( c( "! ",j,c1 , c2 , c3, d[n-j-1] , d[n-j]))
                    }
                lambda	<- find_approx_lambda(c1,c2,c3,d[n-j-1],d[n-j])
                    if( (lambda < d[n-j-1]) || (lambda >d[n-j]) ){
                        print(c("wtf???", d[n-j-1], lambda, d[n-j] ))
                            print(c("d=",d))
                            exit()
                    }
#	print(lambda)
            }

        found_rank <- (found_rank+lambda)
            j <- j+1
            i<-i+1
            lambda_k[n+1-i]<-lambda
    }

    if (p > 0){
        lambda <- trace - found_rank
        if( lambda < d[n] )
            lambda <- d[n] + 10^-5
        for(k in 1:10){
            c1 = 1 + p * (lambda - d[n]) * sum( (v[1:n-1]^2) / (lambda - d[1:n-1])^2 ) - p * sum( (v[1:n-1]^2) / ( lambda - d[1:n-1] ) )
            c2 = (v[n]^2) + ((lambda - d[n])^2) * sum( (v[1:n-1]^2) / ( lambda - d[1:n-1] )^2 )
            new = d[n] + p * c2 / c1
            if( new < d[n] ){
                lambda <- (lambda+d[n])/2
            }else{
                lambda <- new
            }
        }
        lambda_k[n] = lambda
    }else{
        lambda <- trace - found_rank
         if( lambda > d[1] )
            lambda <- d[1] - 10^-5
        for(k in 1:20){
            c1 = 1 + p * (lambda - d[1]) * sum( (v[seq(2,n)]^2) / (lambda - d[seq(2,n)])^2 ) - p * sum( (v[seq(2,n)]^2) / ( lambda - d[seq(2,n)] ) )
            c2 = (v[1]^2) + ((lambda - d[1])^2) * sum( (v[seq(2,n)]^2) / ( lambda - d[seq(2,n)] )^2 )
            new = d[1] + p * c2 / c1
            if( new > d[1] ){
                lambda <- (lambda+d[1])/2
            }else{
                lambda <- new
            }
        }
        lambda_k[1] = lambda
    }

return (lambda_k)
    }

gu_eisenstat_vector <- function(p,n,d,lambda){ #d trié
    v <- 1:n
        for (k in 1:n){
            p1 <- 1
                p2 <- 1
#		p2 <- prod( (lambda - d[k])[seq(k,n)] )
                p2 <- 1 
                p3 <- 1
                p4 <- 1
#		if( k != 1 ){
#			p1 <- prod( (lambda - d[k])[1:k-1] )
#			p3 <- prod( (d - d[k])[1:k-1] )
#		}
#		if( k!= n ){
#			p4 <- prod( (d - d[k])[seq(k+1,n)])
#		}
                if( k!=1 ){
                    for (j in 1:(k-1)){
                        p1 <- p1*(d[k]-lambda[j])
                    }
                }
            for (j in seq(k,n)){
                p2 <- p2*(lambda[j]-d[k])
            }
            if( k != 1 ){
                for (j in 1:(k-1)){
                    p3 <- p3*(d[k]-d[j])
                }
            }
            if( k!= n ){
                for (j in seq(k+1,n)){
                    p4 <- p4*(d[j]-d[k])
                }
            }
#print(c(n,p,p1,p2,p3,p4))
            v[k] <- sqrt( (p1*p2) / (p*p3*p4) )
        }

    return(v)

}
