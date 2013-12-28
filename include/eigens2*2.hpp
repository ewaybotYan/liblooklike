#ifndef EIGENS2*2_H
#define EIGENS2*2_H

#include<matrix.h>

#define N 2

class Eigens{
//attributes
private:
float eigenvalues[N];
float eigenvectors[N*N];
//methods
public:
vector<float> getEigenvalues(){float eigenvaluesCopy[N]; for(int i=0; i<N; i++) eigenvaluesCopy[i]=eigenvalues[i]; return eigenvaluesCopy;};
vector<float> getEigenvectors(){float eigenvectorsCopy[2*N]; for(int i=0; i<N*N; i++) eigenvectorsCopy[i]=eigenvectors[i]; return eigenvectorsCopy;};
void setEigenvalue(float eigenvalue, int i){eigenvalues[i]=eigenvalue;};
void setEigenvector(float eigenvector, int i){eigenvectors[i]=eigenvector};
}

Eigens eigens2_2(Matrix m const);

#endif // EIGENS2*2_H
