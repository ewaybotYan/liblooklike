#include <string>
#include <jpeglib.h>

#include "../include/file.h"
#include "../include/jpegfile.h"


// ########################
// # private helper methods

bool JPEGInFile::hasJPEGMagicNb ( ) {
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

JPEGImageInFile::JPEGImageInFile(const std::string filePath){
    setSourceFile(filePath);
}


// ################
// # public methods

void JPEGImageInFile::setSourceFile(std::string filePath){
    m_file = File(filePath, "rb");
    if ( !hasJPEGMagicNb() ){ // check this is actually a jpeg
#ifndef NDEBUG
        std::cout << filePath << " is not a valid jpeg file.\n";
#endif
        throw Error( "this is not a JPEG file" );
    }
    std::fseek ( m_file->getFile(), SEEK_SET, 0 );
    m_info.err = jpeg_std_error(&m_jerr);
    jpeg_create_decompress(&m_info);
    jpeg_stdio_src(&m_info, m_file.getFile());
    jpeg_read_header(&m_info, TRUE);
    m_width = m_info.output_width;
    m_height = m_info.output_height;
#ifndef NDEBUG
    std::cout << "new Jpeg image of size " << m_width << "x" << m_height << "\n";
#endif
}

template<typename ScalarT>
void JPEGImageInFile::writeToMemory ( ScalarT* mem, int width, int height ) {
#ifndef NDEBUG
    std::cout << "loading JPEG image of size "
              << width << "x" << height << "\n";
#endif
    // set output options such as color depth and scale
    m_info.output_height = height;
    m_info.output_width = width;
    m_info.out_color_space = JCS_GRAYSCALE;
    m_info.out_color_components = 1;
    jpeg_start_decompress ( &m_info );

    // load values
    JSAMPLE buffer[ width*sizeof(JSAMPLE) ];
    JSAMPROW row = buffer;
    JSAMPARRAY rowp = &row;
    rowp = &row;// array of one row pointer
    while ( m_info.output_scanline < height ) {
        jpeg_read_scanlines ( &m_info, rowp, 1 );
        for ( int j=0; j< width; j++ ){
                ScalarT tmp = static_cast< ScalarT >(buffer[j]);
                mem[m_info.output_scanline*width+j] = tmp;
        }
    }

    jpeg_finish_decompress ( &m_info );
    jpeg_destroy_decompress ( &m_info );

}

template<typename ScalarT>
void JPEGImageOutFile::writeFromMemory ( ScalarT* mem,
                                 const unsigned int width,
                                 const unsigned int height,
                                 const std::string filePath) {

#ifndef NDEBUG
    std::cout << "saving JPEG image of size " << width << "x" << height << "\n";
#endif
    File file(filePath,"wb");

    struct jpeg_compress_struct cInfo;
    struct jpeg_error_mgr jerr;
    cInfo.err = jpeg_std_error(&jerr);

    jpeg_create_compress(&cInfo);

    File file(filePath, "wb");
    FILE* outfile = file.getFile();
    jpeg_stdio_dest(&cInfo, outfile);

    cInfo.image_width = width;
    cInfo.image_height = height;
    cInfo.input_components = 1;
    cInfo.in_color_space = JCS_GRAYSCALE;
    jpeg_set_defaults(&cInfo);
    jpeg_set_quality(&cInfo, 100, TRUE);

    jpeg_start_compress(&cInfo, TRUE);

    int rowStride = width;
    JSAMPLE buffer[width];
    JSAMPROW rowPointer[1];

    while (cInfo.next_scanline < cInfo.image_height) {
        for( int j=0; j<width; j++ )
            buffer[j] = static_cast<JSAMPLE>(
                        mem[cInfo.next_scanline * rowStride + j]);
      rowPointer[0] = buffer;
      jpeg_write_scanlines(&cInfo, rowPointer, 1);
    }

    jpeg_finish_compress(&cInfo);

    jpeg_destroy_compress(&cInfo);
}
