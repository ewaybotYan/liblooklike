#include <iostream>
#include <string>
#include <armadillo>
#include "../include/image.h"
#include "../include/matrix.h"


void usage(){
    std::cout << "Usage:\n";
    std::cout << "img2eigvec [-o output_file] [-i database_id] path_to_files\n";
}

/*
void help(){
    std::cout << "img2eigvec is a programm that generates the eigen vectors "
                 + "of the Principal Component Analysis algorithm.\n"
                 + "Its output is a text file containing ";

}*/

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
        Matrix images = array.images;
#ifndef NDEBUG
        std::cout << "generated array of images of size"
                  << images.getWidth()
                  << "x"
                  << images.getHeight()
                  << " .\n";
#endif
        Matrix normalized = Matrix::normalize(images);
#ifndef NDEBUG
        std::cout << "normalized matrix of size"
                  << normalized.getWidth()
                  << "x"
                  << normalized.getHeight()
                  << " .\n";
#endif
        Matrix covMat = Matrix::covariance(normalized);
#ifndef NDEBUG
        std::cout << "generated covariance matrix of size"
                  << covMat.getWidth()
                  << "x"
                  << covMat.getHeight()
                  << " .\n";
#endif

        covMat.evaluate(ctx, queue);
        covMat.getEndOfEvaluation().wait();
        covMat.retrieveData(ctx, queue);

        arma::fvec eigval;
        arma::fmat eigvec;
        arma::fmat covMat2(covMat.getValues(),
                           covMat.getWidth(),
                           covMat.getHeight());
        arma::eig_sym(eigval, eigvec, covMat2);

        int previewW = array.avgWidth;
        int previewH = array.avgHeight;
#ifndef NDEBUG
        std::cout << "found " << eigvec.n_cols << " eigen vectors  of size " << eigvec.n_cols << " .\n";
        std::cout << "preparing preview of size : " << previewW << "x" << previewH << " .\n";
#endif
        float* previewData = new float[previewW*previewH];
        for(int i=0; i < previewW * previewH; i++)
                previewData[i] = eigvec(eigvec.n_cols-1,i);
        Matrix preview(previewData,array.avgHeight,array.avgWidth);
        MatrixToImage(preview,"/tmp/vectorPreview.jpg");

    } catch ( Error& err ) {
        err.printMsg();
        return -1;
    }

    return 0;
}
