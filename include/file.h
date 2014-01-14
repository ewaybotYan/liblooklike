#ifndef FILE_H
#define FILE_H

#include <cstdio>
#include <jpeglib.h>
#include <memory>

/** 
 * file management is done C-style in libjpeg,
 * we wrap the FILE object with File to handle errors and close automatically.
 */
class File{
  
private:
    
  FILE* m_file = nullptr;
  
public:
  
  File( const std::string filePath, const std::string mode = "rb" ){
        m_file = std::fopen( filePath.c_str(), mode.c_str() );
	if ( m_file == nullptr ) {
	  throw("opening file "+filePath+" failed");
	}
#ifndef NDEBUG
    std::cout << "opened file : " << filePath << "\n";
#endif
  }
  
   FILE* getFile() const{
    return m_file;
  }
  
  ~File(){
        std::fclose( m_file );
  }
};


class JPEGFile{
  
private:

  bool hasJPEGMagicNb( );
  struct jpeg_decompress_struct m_info;
  struct jpeg_error_mgr m_jerr;
  unsigned int m_width;
  unsigned int m_height;
  //unsigned char* m_data;
  std::shared_ptr<File> m_file;
  
public:
  
  JPEGFile( const std::string filePath );

  unsigned int getWidth() const;
  unsigned int getHeight() const;
  FILE* getFile() const;
  
  /// load the monochromatic image in memory
  /// @param width desired target image width,
  ///        the image will be scaled or cropped if necessary
  /// @param height desired target image height,
  ///        the image will be scaled or cropped if necessary
  /// @param mem a pointer to an allocated array
  ///        of size width * height of type ScalarT
  /// @note  the data is written in one dimension,
  ///        it is the concatenation of all lines (not columns) of pixels
  template<typename ScalarT>
  void writeToMemory( ScalarT* mem, int width, int height );
  
};


// #########
// # methods

template<typename ScalarT>
void JPEGFile::writeToMemory ( ScalarT* mem, int width, int height ) {
    // set output options such as color depth and scale
    m_info.output_height = height;
    m_info.output_width = width;
    m_info.out_color_space = JCS_GRAYSCALE;
    m_info.out_color_components = 1;
    (void)height;
    jpeg_start_decompress ( &m_info );

    // load values
    JSAMPLE buffer[ width*sizeof(JSAMPLE) ];
    JSAMPROW row = buffer;
    JSAMPARRAY rowp = &row;
    rowp = &row;// array of one row pointer
    while ( m_info.output_scanline < m_height ) {
        jpeg_read_scanlines ( &m_info, rowp, 1 );
        for ( int j=0; j< width; j++ )
                mem[m_info.output_scanline*width+j] =
                        static_cast< float >(buffer[j]);
    }

    jpeg_finish_decompress ( &m_info );
    jpeg_destroy_decompress ( &m_info );

}

#endif
