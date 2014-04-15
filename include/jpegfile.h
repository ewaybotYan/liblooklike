#ifndef JPEGFILE_H
#define JPEGFILE_H

#include <jpeglib.h>
#include <memory>

#include "image.h"
#include "file.h"


// #########
// # classes

/// Handles Jpeg file reading
class JPEGImageInFile : public ImageInFile {

    public:

        /// @param filePath path to the jpeg file to read
        /// @note makes a call to @ref setSourceFile
        JPEGImageInFile( const std::string filePath );

        /// default constructor
        JPEGImageInFile(){}
        /// default destructor
        ~JPEGImageInFile(){}

        void setSourceFile(std::string filePath) override;
        void load ( float *mem,
                    unsigned int width, unsigned int height ) override;

    private:

        std::shared_ptr<File> m_file;
        bool hasJPEGMagicNb( );
        struct jpeg_decompress_struct m_info;
        struct jpeg_error_mgr m_jerr;
};


/// Handle monochromatic to JPEG file conversion
class JPEGImageOutFile : public ImageOutFile{

    public:

        /// default constructor
        JPEGImageOutFile(){}

        /// default destructor
        ~JPEGImageOutFile(){}

        void write ( float* mem,
                     const unsigned int width,
                     const unsigned int height,
                     const std::string filePath ) override;

};

#endif // JPEGFILE_H
