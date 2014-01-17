#ifndef JPEGFILE_H
#define JPEGFILE_H

#include <jpeglib.h>

#include "image.h"
#include "file.h"

class JPEGImageInFile : protected ImageInFile {

private:

  File m_file;
  bool hasJPEGMagicNb( );
  struct jpeg_decompress_struct m_info;
  struct jpeg_error_mgr m_jerr;

public:

  /// @note make a call to @ref setSourceFile
  JPEGImageInFile( const std::string filePath );

  void setSourceFile(std::string filePath) override;

  template<typename ScalarT>
  void writeToMemory( ScalarT* mem, int width, int height ) override;

};


class JPEGImageOutFile : public ImageOutFile{

    public:

        template<typename ScalarT>
        virtual void write ( ScalarT* mem,
                                       const unsigned int width,
                                       const unsigned int height,
                                       const std::string filePath ) override;

};


#endif // JPEGFILE_H
