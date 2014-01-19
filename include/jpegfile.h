#ifndef JPEGFILE_H
#define JPEGFILE_H

#include <jpeglib.h>
#include <memory>

#include "image.h"
#include "file.h"


// #########
// # classes

class JPEGImageInFile : public ImageInFile {

private:

  std::shared_ptr<File> m_file;
  bool hasJPEGMagicNb( );
  struct jpeg_decompress_struct m_info;
  struct jpeg_error_mgr m_jerr;

public:

  /// @note make a call to @ref setSourceFile
  JPEGImageInFile( const std::string filePath );
  JPEGImageInFile(){}
  ~JPEGImageInFile(){}

  void setSourceFile(std::string filePath) override;
  void load ( float *mem, unsigned int width, unsigned int height ) override;

};


class JPEGImageOutFile : public ImageOutFile{

    public:
        JPEGImageOutFile(){}
        ~JPEGImageOutFile(){}

        void write ( float* mem,
                     const unsigned int width,
                     const unsigned int height,
                     const std::string filePath ) override;

};

#endif // JPEGFILE_H
