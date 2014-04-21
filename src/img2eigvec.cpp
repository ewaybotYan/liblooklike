#include <iostream>
#include <string>
#include <sstream>
#include <image.h>
#include <simplematrix.hpp>
#include <clmatrixoperations.hpp>
#include <clmatrixnorm.hpp>
#include <eigenpair.hpp>
#include <sort.hpp>
#include <files.hpp>

using namespace std;

typedef SimpleMatrix<cl_float> Matrix;


/// @todo support the options
void usage(){
  cout << "Usage:\n";
  cout << "img2eigvec path_to_kernels path_to_pictures\n";
}


int main(int argc, char* argv[]){
  if(argc < 2){
    usage();
    return -1;
  }
  string imagePath(argv[argc-1]);
  string kernelPath(argv[argc-2]);

  try {
    // create OpenCL objects
    Context ctx ( kernelPath );
    cl::CommandQueue queue = ctx.createQueue();

    // load images
    ArrayOfImages array = arrayOfImagesFromFiles(imagePath);
    shared_ptr<Matrix> images = make_shared<Matrix>(
                                  array.pixels,
                                  array.avgHeight * array.avgWidth,
                                  array.nbImages );
    CLMatrixLoader imagesInCL( images, &ctx, &queue );

    // normalize images values
    CLMatrixNorm normalized1( imagesInCL.getResult(),
                              &ctx, &queue,
                              true ); // on each image
    CLMatrixNorm normalized2( normalized1.getNormalizedMatrix(),
                              &ctx, &queue,
                              false ); // normalize pixels values across images
    CLMatrixCovariance covMat( normalized2.getNormalizedMatrix(),
                               &ctx, &queue );
    CLMatrixUnloader localCovMat( covMat.getResult(), &ctx, &queue );

    // compute eigen values and get the eigenvectors on variables
    EigenPair eigenpair(localCovMat.getResult());
    CLMatrixLoader eigenVectors(eigenpair.getVectors(), &ctx, &queue);
    CLMatrixProduct eigenVectorsOnValues( normalized2.getNormalizedMatrix(),
                                          eigenVectors.getResult(),
                                          &ctx, &queue );
    CLMatrixUnloader localEigenVectorsOnVars( eigenVectorsOnValues.getResult(),
                                              &ctx, &queue );

    // get biggest eigen values
    InertiaSort<cl_float> sort( eigenpair.getValues(), 0.95, true, 0, true );
    localEigenVectorsOnVars.getResult()->evaluate();
    localEigenVectorsOnVars.getResult()->waitEndOfEvaluation();
    sort.getSorted()->evaluate();
    sort.getSorted()->waitEndOfEvaluation();

#ifndef NDEBUG
    cout << "Total inertia: " << sort.getTotalInertia() << "\n";
    cout << "biggest values: " << endl;
    sort.getSorted()->print();
#endif
    Matrix* vectors = localEigenVectorsOnVars.getResult().get();

    int nbVectors = sort.getNbSortedValues();

    // export pictures of the eigen vectors
#ifndef NDEBUG
    for ( int i = 0; i < nbVectors; i++) {
      std::stringstream ss;
      ss << "/tmp/vectorPreview" << i << ".jpg";
      cout << "saving: " << ss.str() << endl;
      MatrixToImage(*vectors,
                    array.avgHeight,
                    array.avgWidth,
                    sort.getSortIdx()->at(i,0),
                    true,
                    ss.str());
    }
#endif

    // save normalization coefficients
    CLMatrixUnloader coeffs(normalized2.getNormCoeffs(), &ctx, &queue );
    coeffs.getResult()->evaluate();
    coeffs.getResult()->waitEndOfEvaluation();
    save(*coeffs.getResult(), "/tmp/coeffs.csv" );

    // save eigen vectors
    save(*vectors, "/tmp/vectors.csv", *sort.getSortIdx().get());

  } catch ( Error& err ) {
    err.printMsg();
    return -1;
  }

  return 0;
}
