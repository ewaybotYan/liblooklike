#include <cstdio>
#include <jpeglib.h>

#include "../include/file.h"


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
    m_file = std::make_shared<File>( File(filePath, "rb") );
    if ( !hasJPEGMagicNb() ) // check this is actually a jpeg
        throw ( "this is not a JPEG file" );
    // read file characteristic
    std::fseek ( m_file->getFile(), SEEK_SET, 0 );
    struct jpeg_error_mgr jerr;
    m_info.err = jpeg_std_error ( &jerr );
    jpeg_create_decompress ( &m_info );
    jpeg_stdio_src ( &m_info, m_file->getFile() );
    jpeg_read_header ( &m_info, TRUE );
    m_width  = m_info.output_width;
    m_height = m_info.output_height;
}

JPEGFile::JPEGFile ( const JPEGFile& file ) {
    m_file = file.getFile();
    // read file characteristic
    std::fseek ( m_file->getFile(), SEEK_SET, 0 );
    struct jpeg_error_mgr jerr;
    m_info.err = jpeg_std_error ( &jerr );
    jpeg_create_decompress ( &m_info );
    jpeg_stdio_src ( &m_info, getFile() );
    jpeg_read_header ( &m_info, TRUE );
    m_width  = file.getWidth();
    m_height = file.getHeight();
}


JPEGFile::~JPEGFile() {
    jpeg_finish_decompress ( &m_info );
    jpeg_destroy_decompress ( &m_info );
}

// #####################
// # getters and setters

unsigned int JPEGFile::getWidth() {
    return m_width;
}

unsigned int JPEGFile::getHeight() {
    return m_height;
}

FILE* JPEGFile::getFile() {
    return m_file->getFile();
}
