/**
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 * @file   imgprojection.cpp
 * @date   16/04/2014
 */

#include <iostream>
#include <string>
#include <image.h>
#include <simplematrix.hpp>
#include <clmatrixoperations.hpp>
#include <clmatrixnorm.hpp>
#include <files.hpp>

using namespace std;

typedef SimpleMatrix<cl_float> Matrix;

void usage()
{
  cout << "usage:\n"
       << "imgprojection kernels_path vectors_path images_path" << endl;
}

int main( int argc, char* argv[] )
{
  if ( argc < 4 ) {
    usage();
    return -1;
  }

  string imagesPath(argv[argc-1]);
  string vectorsAndCoefsPath(argv[argc-2]);
  string kernelPath(argv[argc-3]);

  try {

    // create OpenCL objects
    Context ctx ( kernelPath );
    cl::CommandQueue queue = ctx.createQueue();

    // load images
    ArrayOfImages imagesData = arrayOfImagesFromFiles(imagesPath);
    shared_ptr<Matrix> images = make_shared<Matrix>(
                                  imagesData.pixels,
                                  imagesData.avgHeight * imagesData.avgWidth,
                                  imagesData.nbImages );
    CLMatrixLoader imagesInCL( images, &ctx, &queue );

    // and normalize contrasts
    CLMatrixNorm imagesInCLStdContrast( imagesInCL.getResult(), &ctx, &queue );

    // load vectors
    shared_ptr<Matrix> vectors( load( vectorsAndCoefsPath+"/vectors.csv" ) );
    CLMatrixLoader vectorsInCL( vectors, &ctx, &queue );

    // load normalization coefficients
    shared_ptr<Matrix> coeffs( load( vectorsAndCoefsPath+"/coeffs.csv" ) );
    CLMatrixLoader coeffsInCL( coeffs, &ctx, &queue );

    // apply normalization coefficients to the images
    CLMatrixScale normalizedImages( imagesInCLStdContrast.getNormalizedMatrix(),
                                    coeffsInCL.getResult(),
                                    &ctx, &queue );

    // make projections
    CLMatrixProduct projections( vectorsInCL.getResult(),
                                 normalizedImages.getResult(),
                                 &ctx, &queue,
                                 true );

    // retrieve
    CLMatrixUnloader results( projections.getResult(), &ctx, &queue );
    results.getResult()->evaluate();
    results.getResult()->waitEndOfEvaluation();

    save( *results.getResult().get(), "/tmp/projections.csv" );

    // make preview of the image reconstituted from the projections
#ifndef NDEBUG
     CLMatrixProduct reconstituted( vectorsInCL.getResult(),
                                    projections.getResult(),
                                    &ctx, &queue );
     CLMatrixUnloader localReconstituted( reconstituted.getResult(),
                                          &ctx, &queue );
     localReconstituted.getResult()->evaluate();
     localReconstituted.getResult()->waitEndOfEvaluation();
     MatrixToImage( *localReconstituted.getResult().get(),
                    360, 260,
                    0, true,
                    "/tmp/reconstituted.jpg" );

     CLMatrixUnloader normalizedSamples( normalizedImages.getResult(),
                                         &ctx, &queue );
     normalizedSamples.getResult()->evaluate();
     normalizedSamples.getResult()->waitEndOfEvaluation();
     MatrixToImage( *normalizedSamples.getResult().get(),
                    360, 260,
                    0, true,
                    "/tmp/normalized.jpg" );

#endif

  } catch ( Error& err ) {
    err.printMsg();
    return -1;
  }


  return 0;
}
