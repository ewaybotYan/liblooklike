#ifndef NDEBUG
#include <iostream>
#endif

#include <cstdlib>
#include <dirent.h>
#include <string>
#include <deque>

#include "../include/file.h"
#include "../include/matrix.h"
#include "../include/exception.h"


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
            JPEGFile f ( path+"/"+fileName );
            imagefiles.push_back ( f );
        } catch ( ... ) {
            // that was not a valid JPEG file, just skip it
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

    // create result object
    float* values = new float[avgWidth*avgHeight*imagefiles.size()];
    Matrix m ( values, imagefiles.size(), avgWidth*avgHeight );

    //then load the pixels from these files
    float* offset = values;
    for ( JPEGFile f : imagefiles ) {
        f.writeToMemory<float> ( offset, avgWidth, avgHeight );
        offset += avgWidth*avgHeight*sizeof ( float );
    }

    return m;
}
