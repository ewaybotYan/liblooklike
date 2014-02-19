#ifndef NDEBUG
#include <iostream>
#endif

#include <cstdlib>
#include <dirent.h>
#include <string>
#include <deque>
#include <memory>

#include "../include/jpegfile.h"
#include "../include/matrix.h"
#include "../include/exception.h"
#include "../include/image.h"

// #######################
// # Functions declaration

ArrayOfImages arrayOfImagesFromFiles ( const std::string path ) {
    DIR *dir;
    struct dirent *ent;
    std::deque<JPEGImageInFile> imagefiles;

    // first, get all jpeg files fro path
    if ( ( dir = opendir ( path.c_str() ) ) == NULL )
        throw Error ( "could not open specified directory" );

    while ( ( ent = readdir ( dir ) ) != NULL ) {
        std::string fileName ( ent->d_name );
        if ( ent->d_type != DT_REG )
            continue;
        try {
            imagefiles.push_back ( path+"/"+fileName );
        } catch ( Error const& e ) {
            // that was not a valid JPEG file, just skip it
#ifndef NDEBUG
            std::cout << "and I don't care\n";
#endif
        }
    }
    closedir ( dir );

    // then get the average size
    unsigned int avgWidth = 0;
    unsigned int avgHeight = 0;
    for ( JPEGImageInFile f : imagefiles ) {
        avgWidth += f.getWidth();
        avgHeight += f.getHeight();
    }
    avgWidth /= imagefiles.size();
    avgHeight /= imagefiles.size();
#ifndef NDEBUG
    std::cout << "chosen image size is : " << avgWidth << "x" << avgHeight << "\n";
#endif

    // create result object
    float* values = new float[avgWidth*avgHeight*imagefiles.size()];

    //then load the pixels from these files
    float* offset = values;
    for ( JPEGImageInFile f : imagefiles ) {
#ifndef NDEBUG
    std::cout << "loading pixels\n" ;
#endif
        f.load( offset, avgWidth, avgHeight );
        offset += avgWidth * avgHeight;
    }

    Matrix m ( values, imagefiles.size(), avgWidth*avgHeight );

    delete[] values;// Matrix constructor keeps a copy of the values

    ArrayOfImages res = {
        avgWidth,
        avgHeight,
        m
    };
    return res;
}


void MatrixToImage( const Matrix src, const std::string savePath ) {

    // scale data
    float* it = src.getValues();
    float minVal = *it;
    float maxVal = *it;
    float avg=0;
    for( int i= 0; i < src.getWidth()*src.getHeight(); i++){
        minVal = std::min(minVal, it[i]);
        maxVal = std::max(maxVal, it[i]);
        avg += it[i];
    }
    avg/=src.getWidth()*src.getHeight();
#ifndef NDEBUG
    std::cout << "vector rng : " << minVal << " " << avg << " " << maxVal << "\n";
#endif
    it = src.getValues();
    for( int i= 0; i < src.getWidth()*src.getHeight(); i++){
        it[i] = (it[i] - minVal)*255/(maxVal-minVal);
    }

    // save it
    JPEGImageOutFile fout;
    fout.write( src.getValues(),
                src.getWidth(),
                src.getHeight(),
                savePath);
}


unsigned int ImageInFile::getWidth() const {
    return m_width;
}


unsigned int ImageInFile::getHeight() const {
    return m_height;
}
