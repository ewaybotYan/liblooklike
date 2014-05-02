library("jpeg")

load <- function(f){
     j <- readJPEG(f)
    bw <- rowMeans(j,dims=2)
    return(c(bw))
}

f <- list.files(path = "/home/nicolas/dev/liblooklike/data/", pattern = ".jpg$", full.names = TRUE)
i <- apply(matrix(f),1,load)
n <- scale(i)
c <- t(n)%*%n
eig <- eigen(c)
eigOnVal <- n%*%eig$vectors

# fit <- princomp(i)
image(matrix(eigOnVal[,1],360),col = grey(seq(0, 1, length = 256)))

