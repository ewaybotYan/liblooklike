#ifndef IMAGE_H
#define IMAGE_H

/**
 * @file  image.h
 * @brief classes for image files
*/

#include "file.h"
#include "matrix.h"
#include<memory>


// ############################
// # genereal purpose functions

struct ArrayOfImages{
        unsigned int avgWidth;
        unsigned int avgHeight;
        unsigned long nbImages;
        std::shared_ptr<float*> pixels;
};

/// @brief Generates an array of monochomatic images from JPEG files in a
///        directory
/// @param path Path to a directory where the input files are
/// @return A matrix with the pixels of the images, each line correspond to one
///         image, each line is the concatenation of the lines of pixel of one
///         image.
/// @note The directory may contain non JPEG files, they will be ignored.
/// @warning There is no rule to sort the lines according to the filenames.
ArrayOfImages arrayOfImagesFromFiles(const std::string path);


/// @brief Saves a matrix as an monochomatic image.
/// @detailed This function takes the float values of the matrix, scale and
///           round them to fit in range 0 to 255.
/// @param src the matrix whoose elements are the pixels
void MatrixToImage( const Matrix src, const std::string savePath );


// #######################
// # generic image classes

/// @brief Abstract class for image file loading
/// @startuml{ImageInFile_state.png}
///     [*] --> Empty
///     Empty --> AssociatedToAFile : setSourceFile()
///     AssociatedToAFile --> AssociatedToAFile : read operations
///     AssociatedToAFile --> [*]
/// @enduml
class ImageInFile{

    public:

        /// @return Width of the image if a source file was specified,
        ///         default value is 0 otherwise.
        /// @note   @ref setSourceFile can be used in order to set width.
        unsigned int getWidth() const;

        /// @return Height of the image if a source file was specified,
        ///         default value is 0 otherwise.
        /// @note   @ref setSourceFile can be used in order to set height
        unsigned int getHeight() const;

        /// @brief Read the header of a source file.
        /// After a call to this function, getWidth and getHeight must return
        /// appropriate values after.
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

        /// the image width in pixels
        unsigned int m_width = 0;

        /// the image height in pixels
        unsigned int m_height = 0;

        /// @note constructor is protected because this is an abstract class
        ImageInFile(){}
};


/// @brief Abstract class for image file creation
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
