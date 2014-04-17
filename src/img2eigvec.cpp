#include <iostream>
#include <string>
#include <armadillo>
#include <image.h>
#include <simplematrix.hpp>
#include <clmatrixoperations.hpp>
#include <clmatrixnorm.hpp>
#include <eigenpair.hpp>
#include <sort.hpp>

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
    shared_ptr<Matrix> images = make_shared<Matrix>( array.pixels,
                                                     array.avgHeight * array.avgWidth,
                                                     array.nbImages );
    CLMatrixLoader imagesInCL( images, &ctx, &queue );
#ifndef NDEBUG
    cout << "imagesInCL " << imagesInCL.getResult()->getHeight()
         << "x" << imagesInCL.getResult()->getWidth() << endl;
#endif
    // normalize images values
    CLMatrixNorm normalized1( imagesInCL.getResult(),
                              &ctx, &queue,
                              false );// on each image
    CLMatrixNorm normalized2( normalized1.getNormalizedMatrix(),
                              &ctx, &queue,
                              true );// normalize pixels values across images
#ifndef NDEBUG
    cout << "normalized2 " << normalized2.getNormalizedMatrix()->getHeight()
         << "x" << normalized2.getNormalizedMatrix()->getWidth() << endl;
    CLMatrixUnloader bug1( normalized2.getNormalizedMatrix(), &ctx, &queue );
    bug1.getResult()->evaluate();
    bug1.getResult()->waitEndOfEvaluation();
    MatrixToImage(*bug1.getResult(),
                  array.avgHeight,
                  array.avgWidth,
                  1,
                  true,
                  "/tmp/previewNormalized.jpg");
#endif
    CLMatrixCovariance covMat( normalized2.getNormalizedMatrix(),
                               &ctx, &queue );
#ifndef NDEBUG
    cout << "covMat " << covMat.getResult()->getHeight()
         << "x" << covMat.getResult()->getWidth() << endl;
#endif
    CLMatrixUnloader localCovMat( covMat.getResult(), &ctx, &queue );
    EigenPair eigenpair(localCovMat.getResult());
#ifndef NDEBUG
    cout << "eigenpair " << eigenpair.getVectors()->getHeight()
         << "x" << eigenpair.getVectors()->getWidth() << endl;
#endif
    CLMatrixLoader eigenVectors(eigenpair.getVectors(), &ctx, &queue);
    CLMatrixProduct eigenVectorsOnValues( normalized2.getNormalizedMatrix(),
                                          eigenVectors.getResult(),
                                          &ctx, &queue );
    CLMatrixUnloader localEigenVectorsOnVars( eigenVectorsOnValues.getResult(),
                                              &ctx, &queue );
#ifndef NDEBUG
    cout << "localEigenVectorsOnVars " << localEigenVectorsOnVars.getResult()->getHeight()
         << "x" << localEigenVectorsOnVars.getResult()->getWidth() << endl;
#endif
    InertiaSort<cl_float> sort( eigenpair.getValues(), 0.9, true, 0, true );
    localEigenVectorsOnVars.getResult()->evaluate();
    localEigenVectorsOnVars.getResult()->waitEndOfEvaluation();
    sort.getSorted()->evaluate();
    sort.getSorted()->waitEndOfEvaluation();
    sort.getSorted()->print();
    sort.getSortIdx()->print();
    cout << "\nNumber of important vectors: " << sort.getNbSortedValues() << "\n";
    cout << "total inertia: " << sort.getTotalInertia() << "\n";
    Matrix* vectors = localEigenVectorsOnVars.getResult().get();

    int nbVectors = sort.getNbSortedValues();
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

  } catch ( Error& err ) {
    err.printMsg();
    return -1;
  }

  return 0;
}
