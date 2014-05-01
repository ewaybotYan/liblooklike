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

    // normalize images values
#ifdef PREPROCESSING
    CLMatrixLoader imagesInCL( images, &ctx, &queue );
    CLMatrixNorm normalized( imagesInCL.getResult(),
                              &ctx, &queue,
                              true ); // on each image
#elif NORMALIZE_PXL
    CLMatrixLoader imagesInCL( images, &ctx, &queue );
    CLMatrixNorm normalized1( imagesInCL.getResult(),
                              &ctx, &queue,
                              true ); // on each image
    CLMatrixNorm normalized( imagesInCL.getResult(),
                              &ctx, &queue,
                              true ); // normalize pixels values across images
#endif

    // compute covariance matrix
#if defined(NORMALIZE_PXL) || defined(PREPROCESSING)
    CLMatrixCovariance covMat( normalized.getNormalizedMatrix(),
                               &ctx, &queue );
#else
    CLMatrixCovariance covMat( imagesInCL.getResult(),
                               &ctx, &queue );
#endif
    CLMatrixUnloader localCovMat( covMat.getResult(), &ctx, &queue );
    // compute eigen values and get the eigenvectors on variables
    EigenPair eigenpair(localCovMat.getResult());
    CLMatrixLoader eigenVectors(eigenpair.getVectors(), &ctx, &queue);

#if defined(NORMALIZE_PXL) || defined(PREPROCESSING)
    CLMatrixProduct eigenVectorsOnValues( normalized.getNormalizedMatrix(),
                                          eigenVectors.getResult(),
                                          &ctx, &queue );
#else
    CLMatrixProduct eigenVectorsOnValues( imagesInCL.getResult(),
                                          eigenVectors.getResult(),
                                          &ctx, &queue );

#endif
    CLMatrixNorm eigenVecOnValsNorm( eigenVectorsOnValues.getResult(),
                                     &ctx, &queue );
    CLMatrixUnloader localEigenVectorsOnVars(
          eigenVecOnValsNorm.getNormalizedMatrix(),
          &ctx, &queue );

    // get biggest eigen values
    InertiaSort<cl_float> sort( eigenpair.getValues(), 0.9, true, 0, true );
    localEigenVectorsOnVars.getResult()->evaluate();
    localEigenVectorsOnVars.getResult()->waitEndOfEvaluation();
    sort.getSorted()->evaluate();
    sort.getSorted()->waitEndOfEvaluation();

#ifndef NDEBUG
    cout << "# images" << endl;
    images->print();
    save(*images,"/tmp/images.csv");
    cout << "# covariance matrix" << endl;
    localCovMat.getResult()->print();
    cout << "values: \n";
    eigenpair.getValues()->print();
    cout << "Total inertia: " << sort.getTotalInertia() << "\n";
    cout << "biggest values: " << endl;
    sort.getSorted()->print();
#endif
    Matrix* vectors = localEigenVectorsOnVars.getResult().get();

    int nbVectors = sort.getNbSortedValues();

    // export sample of the normalization
#ifndef NDEBUG
#if defined(NORMALIZE_PXL) || defined(PREPROCESSING)
    CLMatrixUnloader localNormalized(normalized.getNormalizedMatrix(),
                                     &ctx, &queue);
    localNormalized.getResult()->evaluate();
    localNormalized.getResult()->waitEndOfEvaluation();
    cout << "# normalized" << endl;
    localNormalized.getResult()->print();
    for( int i=0; i<1;i++ ){
      MatrixToImage(*localNormalized.getResult().get(),
                    array.avgHeight,
                    array.avgWidth,
                    i,
                    true,
                    "/tmp/sampleNormalized"+to_string(i)+".jpg");
    }
#endif
#endif

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
#ifdef NORMALIZE_PXL
    CLMatrixUnloader coeffs(normalized.getNormCoeffs(), &ctx, &queue );
    coeffs.getResult()->evaluate();
    coeffs.getResult()->waitEndOfEvaluation();
    save(*coeffs.getResult(), "/tmp/coeffs.csv" );
#endif

    // save eigen vectors
    save(*vectors, "/tmp/vectors.csv", *sort.getSortIdx().get());

  } catch ( Error& err ) {
    err.printMsg();
    return -1;
  }

  return 0;
}
