/**
 * @file files.hpp
 * @author Gabriel de Longeaux
 * @brief Saving and loading matrices to and from text files.
 */

#ifndef FILES_HPP
#define FILES_HPP

#include "simplematrix.hpp"
#include <CL/cl.hpp> // for cl_float

/**
 * @page mat_import_export Matrix importation and exportation
 *
 * The matrices are saved following the csv format.
 * It means that consecutive values are comma separated and end of lines
 * separate the series of values of a *column* in the matrix.
 *
 * The matrix values are stored internally as a the concatenation of their
 * columns (from left to right) in an array. However, the .csv file is read line
 * by line. To simplify matrix<->file conversion, what is actually written in
 * the file is the transposed matrix.
 *
 *     Example:
 *     { {1, 2, 4},
 *       {9, 2, 0} }
 *     is stored as:
 *     1,9
 *     2,2
 *     4,0
 */

/// @brief Save a matrix in a text file
/// @param mat a matrix
/// @param filepath path to the saved file (you might want to use the .csv
///        extension)
/// @throw Error thrown if file cannot be opened
void save( SimpleMatrix<cl_float>& mat, std::string filepath );

/// @brief Save a matrix in a text file
/// @param mat a matrix
/// @param filepath path to the saved file (you might want to use the .csv
///        extension)
/// @throw Error thrown if file cannot be opened
void save( SimpleMatrix<cl_float>& mat, std::string filepath,
           SimpleMatrix<unsigned int>&indexes );


/// @brief Load the values of a text file into a Matrix
/// @param filepath Filepath to a matrix file (see: @ref mat_import_export).
/// @return A matrix with the loaded value
/// @throw CorruptedMatrixFile thrown if not all lines have the same number of
///        values, or if any value cannot be read/casted properly.
/// @todo create the exception
SimpleMatrix<cl_float>* load(std::string filepath);

#endif
