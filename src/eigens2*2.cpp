#include<eigens2*2.h>

Eigens eigens2_2(Matrix m const){
Eigens eigens;
//eigenvalues
    int a = m.getValue(0,0);
    int b = m.getValue(0,1);
    int d = m.getValue(1,1);
    int lambda1 = ((a+d)-sqrt((a+d)^2-4*(a*d-b^2)))/2;
    int lambda2 = ((a+d)+sqrt((a+d)^2-4*(a*d-b^2)))/2;
    eigens.setEigenvalues(lambda1,0);
    eigens.setEigenvalues(lambda2,1);
//eigenvectors
	if (m.getValue(0,1)==0){
		eigens.setEigenvectors(0,0);
        eigens.setEigenvectors(1,1);
        eigens.setEigenvectors(1,2);
        eigens.setEigenvectors(0,3);
	}else{
		eigens.setEigenvectors(1,0);
		eigens.setEigenvectors((lambda1-m.getValue(0,0))/m.getValue(0,1),1);
        eigens.setEigenvectors(1,2);
        eigens.setEigenvectors(((lambda2-m.getValue(0,0))/m.getValue(0,1)3);
	}
return eigens;
}
