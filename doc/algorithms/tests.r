source('householder.r')

tests <- function(n,inertia_rate,esilon){
Test<-make_random_sym_matrix(n)
Matrices<-householder_matrix(Test)
A<-Matrices(1)
H<-Matrices(2)
dac(A,inertia_rate,epsilon)



}
