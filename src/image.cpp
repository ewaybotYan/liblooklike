#ifndef NDEBUG
#include <iostream>
#endif

#include <cstdlib>
#include <dirent.h>
#include <string>
#include <deque>
#include <memory>

#include "../include/file.h"
#include "../include/matrix.h"
#include "../include/exception.h"

// #######################
// # Functions declaration

Matrix arrayOfImagesFromFiles ( const std::string path ) {
    DIR *dir;
    struct dirent *ent;
    std::deque<JPEGFile> imagefiles;

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
    for ( JPEGFile f : imagefiles ) {
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
    Matrix m ( values, imagefiles.size(), avgWidth*avgHeight );

    //then load the pixels from these files
    float* offset = values;
    for ( JPEGFile f : imagefiles ) {
#ifndef NDEBUG
    std::cout << "loading pixels\n" ;
#endif
        f.writeToMemory<float> ( offset, avgWidth, avgHeight );
        offset += avgWidth * avgHeight;
    }

    delete[] values;
    return m;
}
