#ifndef NDEBUG
#include <iostream>
#endif

#include <cstdio>
#include <jpeglib.h>

#include <setjmp.h>

#include "../include/file.h"
#include "../include/exception.h"


// #########################
// # private helper function

bool JPEGFile::hasJPEGMagicNb ( ) {
    FILE* f = m_file->getFile();
    std::fseek ( f,SEEK_SET,0 );
    uint8_t magic[3];
    std::fread ( magic,1,3,f );
    if ( magic[0] == 0xff && magic[1] == 0xd8 && magic[2] == 0xff )
        return true;
    return false;
}


// ##############################
// # constructors and destructors

JPEGFile::JPEGFile ( const std::string filePath ) {
    m_file = std::make_shared<File>( filePath, "rb" );
    if ( !hasJPEGMagicNb() ){ // check this is actually a jpeg
#ifndef NDEBUG
            std::cout << filePath << " is not a valid jpeg file.\n";
#endif
        throw Error( "this is not a JPEG file" );
    }
#ifndef NDEBUG
        std::cout << filePath << " opened.\n";
#endif

    m_info.err = jpeg_std_error(&m_jerr);

    // read file characteristic
    std::fseek ( m_file->getFile(), SEEK_SET, 0 );
    jpeg_create_decompress ( &m_info );
    jpeg_stdio_src ( &m_info, m_file->getFile() );
    jpeg_read_header ( &m_info, TRUE );
    m_width  = m_info.image_width;
    m_height = m_info.image_height;
#ifndef NDEBUG
    std::cout << "new Jpeg image of size " << m_width << "x" << m_height << "\n";
#endif
}


// #####################
// # getters and setters

unsigned int JPEGFile::getWidth() const{
    return m_width;
}

unsigned int JPEGFile::getHeight() const{
    return m_height;
}

FILE* JPEGFile::getFile() const{
    return m_file->getFile();
}
