/**
 * @file  image.h
 * @brief classes for image files
 * @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
*/

#ifndef IMAGE_H
#define IMAGE_H

#include"file.h"
#include"simplematrix.hpp"
#include<CL/cl.hpp>
#include<vector>
#include<memory>


// ############################
// # genereal purpose functions

/// describes an array of images loaded in one big array of pixels
struct ArrayOfImages{
    /// width of all the images (average of the source images widths)
    unsigned int avgWidth;
    /// height of all the images (average of the source images height)
    unsigned int avgHeight;
    /// number of loaded images
    unsigned long nbImages;
    /// @brief Images pixels
    /// The pixels are read line by line and concatenated in pixels, and files
    /// picels are concatenated one after the other
    std::shared_ptr< std::vector<cl_float> > pixels;
};

/// @brief   Generates an array of monochomatic images from JPEG files in a
///          directory
/// @param   path Path to a directory containing the input files
/// @return  A matrix with the pixels of the images, each column corresponds to
///          one image, each column is the concatenation of the lines of pixel
///          from the image.
/// @note    The directory may contain non JPEG files, they will be ignored.
/// @warning Files are read in no specific order
ArrayOfImages arrayOfImagesFromFiles(const std::string path);


/// @brief Saves a matrix as an monochomatic image.
/// @param src the matrix where the image is, see @ref ArrayOfImages for the
///        details
/// @param h heigth of the image
/// @param w width of the image
/// @param column column or line that contains the pixels of the image
/// @param onCols true if the the pixels are stored on a column of src, false
///        if they are on a line.
/// @note  This function takes the values of the matrix, scale and round them
///        to fit in the range 0 to 255.
void MatrixToImage(SimpleMatrix<cl_float>& src,
                   unsigned int h, unsigned int w,
                   unsigned int column, bool onCols,
                   const std::string savePath );


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
    ///        After a call to this function, getWidth and getHeight must
    ///        return appropriate values after.
    /// @warning see implementations of this class for exception catching
    virtual void setSourceFile(std::string filePath) = 0;

    /// load the monochromatic image in memory
    /// @param width desired target image width,
    ///        the image will be scaled or cropped if necessary
    /// @param step distance to jump between two consecutive pixels
    ///        (usually 1)
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
    virtual void write ( float* mem,
                         const unsigned int width,
                         const unsigned int height,
                         const std::string filePath ) = 0;

  protected:

    ImageOutFile(){} // this is an interface class
    ~ImageOutFile(){}
};

#endif //IMAGE_H
