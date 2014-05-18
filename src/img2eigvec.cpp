#include <iostream>
#include <string>
#include <sstream>
#include <image.h>
#include <simplematrix.hpp>
#include <clmatrixoperations.hpp>
#include <clmatrixnorm.hpp>
#include <eigenpair.hpp>
#include <sort.hpp>
#include <matrixImportExport.h>

using namespace std;

typedef SimpleMatrix<cl_float> Matrix;


/// @todo reorder agruments
void usage(){
  cout << "Usage:\n";
  cout << "img2eigvec path_to_kernels output_path path_to_pictures\n";
}


int main(int argc, char* argv[]){
  if(argc < 4){
    usage();
    return -1;
  }
  string imagePath(argv[argc-1]);
  string outputPath(argv[argc-2]);
  string kernelPath(argv[argc-3]);

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
    CLMatrixLoader imagesInCL( images, &ctx, &queue );
    CLMatrixNorm isocontrast( imagesInCL.getResult(),
                              &ctx, &queue,
                              true ); // on each image
    CLMatrixNorm normalized( isocontrast.getNormalizedMatrix(),
                             &ctx, &queue,
                             false ); // normalize pixels values across images

    // compute covariance matrix
    CLMatrixCovariance covMat( normalized.getNormalizedMatrix(),
                               &ctx, &queue );
    CLMatrixUnloader localCovMat( covMat.getResult(), &ctx, &queue );

    // compute eigen values and get the eigenvectors on variables
    EigenPair eigenpair(localCovMat.getResult());
    CLMatrixLoader eigenVectors(eigenpair.getVectors(), &ctx, &queue);
    CLMatrixProduct eigenVectorsOnValues( normalized.getNormalizedMatrix(),
                                          eigenVectors.getResult(),
                                          &ctx, &queue );

    // normalize eigen vectors
    CLMatrixNorm eigenVecOnValsNorm( eigenVectorsOnValues.getResult(),
                                     &ctx, &queue );
    CLMatrixUnloader localEigenVectorsOnVars(
          eigenVecOnValsNorm.getNormalizedMatrix(),
          &ctx, &queue );

    // get biggest eigen values
    InertiaSort<cl_float> sort( eigenpair.getValues(), 0.90, true, 0, true );

    // perform computations
    localEigenVectorsOnVars.getResult()->evaluate();
    localEigenVectorsOnVars.getResult()->waitEndOfEvaluation();
    sort.getSorted()->evaluate();
    sort.getSorted()->waitEndOfEvaluation();

    // save normalization coefficients
    CLMatrixUnloader coeffs(normalized.getNormCoeffs(), &ctx, &queue );
    coeffs.getResult()->evaluate();
    coeffs.getResult()->waitEndOfEvaluation();
    save(*coeffs.getResult(), outputPath+"/coeffs.csv" );

    CLMatrixUnloader avgs(normalized.getAvgs(), &ctx, &queue );
    avgs.getResult()->evaluate();
    avgs.getResult()->waitEndOfEvaluation();
    save(*avgs.getResult(), outputPath+"/avgs.csv" );

    // save eigen vectors
    Matrix* vectors = localEigenVectorsOnVars.getResult().get();
    save(*vectors, outputPath+"/vectors.csv", *sort.getSortIdx().get());

    int nbVectors = sort.getNbSortedValues();

    // debug + export pictures of the eigen vectors
#ifndef NDEBUG
    cout << "Total inertia: " << sort.getTotalInertia() << "\n";
    cout << "biggest values: " << endl;
    sort.getSorted()->print();

    for ( int i = 0; i < nbVectors; i++) {
      std::stringstream ss;
      ss << outputPath << "/vectorPreview" << i << ".jpg";
      cout << "saving: " << ss.str() << endl;
      MatrixToImage(*vectors,
                    array.avgHeight,
                    array.avgWidth,
                    sort.getSortIdx()->at(i,0),
                    true,
                    ss.str());
    }
    MatrixToImage(*avgs.getResult(),
                  array.avgHeight,
                  array.avgWidth,
                  0,
                  true,
                  outputPath + "/average.jpg");
    CLMatrixUnloader localNormalized(normalized.getNormalizedMatrix(),
                                     &ctx, &queue );
    localNormalized.getResult()->evaluate();
    localNormalized.getResult()->waitEndOfEvaluation();
    MatrixToImage(*localNormalized.getResult(),
                  360, 260, 0, true,
                  outputPath + "/sampleNormalized.jpg");
#endif

  } catch ( Error& err ) {
    err.printMsg();
    return -1;
  }

  return 0;
}
