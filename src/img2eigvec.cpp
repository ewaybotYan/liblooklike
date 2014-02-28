#include <iostream>
#include <string>
#include <armadillo>
#include <image.h>
#include <matrix.h>
#include <matrixnorm.hpp>


void usage(){
    std::cout << "Usage:\n";
    std::cout << "img2eigvec [-o output_file] [-i database_id] path_to_kernels path_to_pictures\n";
}


int main(int argc, char* argv[]){
    if(argc < 2){
        usage();
        return -1;
    }
    std::string imagePath(argv[argc-1]);
    std::string kernelPath(argv[argc-2]);

    //bool verboseLog = true;

    try {

        Context ctx ( kernelPath );
        cl::CommandQueue queue = ctx.createQueue();
        ArrayOfImages array = arrayOfImagesFromFiles(imagePath);

        MatrixLoader images( *(array.pixels.get()),
                       array.nbImages,
                       array.avgHeight * array.avgWidth,
                       &ctx,
                       &queue );
#ifndef NDEBUG
        std::cout << "generated array of images of size"
                  << images.getWidth()
                  << "x"
                  << images.getHeight()
                  << " .\n";
#endif
        MatrixNorm normalized( images, &ctx, &queue );
#ifndef NDEBUG
        std::cout << "normalized matrix of size"
                  << normalized.getWidth()
                  << "x"
                  << normalized.getHeight()
                  << " .\n";
#endif

        MatrixCovariance covMat( *(normalized.getNormalizedMatrix() ),
                                 &ctx, &queue );
#ifndef NDEBUG
        std::cout << "generated covariance matrix of size"
                  << covMat.getWidth()
                  << "x"
                  << covMat.getHeight()
                  << " .\n";
#endif

        covMat.evaluate();

#ifndef NDEBUG
        std::cout << "done.\n";
#endif
        covMat.waitEndOfEvaluation();
        normalized.waitEndOfEvaluation();
        images.waitEndOfEvaluation();

        arma::fvec eigval;
        arma::fmat eigvec;
        arma::fmat covMat2(covMat.getResult(),
                           covMat.getWidth(),
                           covMat.getHeight());
        arma::eig_sym(eigval, eigvec, covMat2);

        arma::fmat X(normalized.getNormalizedMatrix()->getResult(),
                        normalized.getWidth(),
                        normalized.getHeight());
        arma::fmat vars = X * eigvec;
        //images.print();

        int previewW = array.avgWidth;
        int previewH = array.avgHeight;
#ifndef NDEBUG
        std::cout << "found " << eigvec.n_cols << " eigen vectors  of size " << eigvec.n_cols << " .\n";
        std::cout << "preparing preview of size : " << previewW << "x" << previewH << " .\n";
#endif
        float* previewData = new float[previewW*previewH];
        for(int i=0; i < previewW * previewH; i++){
                previewData[i] = vars(i,eigvec.n_cols-1);
        }
        std::cout << "done computing\n";
        MatrixLoader preview( previewData,
                        array.avgHeight,
                        array.avgWidth,
                        &ctx, &queue );
        MatrixToImage(preview,"/tmp/vectorPreview.jpg");
        delete previewData;
    } catch ( Error& err ) {
        err.printMsg();
        return -1;
    }

    return 0;
}
