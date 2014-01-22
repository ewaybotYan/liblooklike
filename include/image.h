#ifndef IMAGE_H
#define IMAGE_H

#include "file.h"
#include "matrix.h"


// ############################
// # genereal purpose functions

Matrix arrayOfImagesFromFiles(const std::string path);


// #######################
// # generic image classes

class ImageInFile{

    public:

        /// @return width of the image if a source file was specified,
        ///         default value is 0 otherwise.
        /// @note   setSourceFile can be used in order to set width
        unsigned int getWidth() const;

        /// @return height of the image if a source file was specified,
        ///         default value is 0 otherwise.
        /// @note   setSourceFile can be used in order to set height
        unsigned int getHeight() const;

        /// read the header of a source file, after a call to this function,
        /// getWidth and getHeight must return appropriate values after
        /// @warning see implementations of this class for exception catching
        virtual void setSourceFile(std::string filePath) = 0;

        /// load the monochromatic image in memory
        /// @param width desired target image width,
        ///        the image will be scaled or cropped if necessary
        /// @param height desired target image height,
        ///        the image will be scaled or cropped if necessary
        /// @param mem a pointer to an allocated array
        ///        of size width * height of type ScalarT
        /// @note  the data is written in one dimension,
        ///        it is the concatenation of all lines (not columns) of pixels
        virtual void load( float* mem,
                           unsigned int width,
                           unsigned int height ) = 0;

        ~ImageInFile(){}
    protected:

        unsigned int m_width = 0;
        unsigned int m_height = 0;
        ImageInFile(){}
};


class ImageOutFile{

    public:

        /// saves a monochromatic image in a file
        /// @param mem a pointer to an allocated array
        ///        of size width * height of type ScalarT
        /// @note  mem data must be given in one dimension, that is
        ///        as the concatenation of lines (not columns) of pixels
        /// @param width desired target image width,
        ///        the image will be scaled or cropped if necessary
        /// @param height desired target image height,
        ///        the image will be scaled or cropped if necessary
        /// @param filePath path to the file where image is to be saved
        /// @note  Implementations of
        virtual void write ( float* mem,
                                       const unsigned int width,
                                       const unsigned int height,
                                       const std::string filePath ) = 0;

    protected:

        ImageOutFile(){} // this is an interface class
        ~ImageOutFile(){}
};

#endif //IMAGE_H
