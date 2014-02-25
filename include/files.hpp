/**
 * @file files.hpp
 * @brief Saving armadillo matrices of type Mat<float> in text files and loading matrices in text files into liblooklike matrices of type Matrix
 * @author Gabriel de Longeaux <gabriel.de_longeaux@telecom-sudparis.eu>
 */

#ifndef FILES_HPP
#define FILES_HPP

#include <armadillo>
#include "../include/matrix.h"

/**
 * @brief Save a matrix in a text file : all the values on the same row are separated with one space and the rows are separated by a newline.
 * @param mat Mat<float> (matrix of floats) of the library Armadillo
 * @param filepath Filepath with the name of the file where the program will save the matrix and its extension if wanted (the file will be anyway a text file)
 * @return 0 will be returned if there was no problem to save the matrix and -1 if the program was unable to create the file
 */
int save(arma::fmat mat, std::string filepath);

/**
 * @brief Load the values of a text file into a Matrix : all the values must be separated with spaces and these values will be load in the Matrix row by row.
 * @brief The file must end by the last value of the matrix, or by space(s) or newline(s).
 * @brief Number of rows and columns of the matrix must be indicated at the beginning of the file in a specific way (cf. files.cpp).
 * @param filepath Filepath with the full name of the text file where the values must be taken to be loaded in a Matrix
 * @return Matrix with the loaded value if there was no problem and throw an error (MissingDataInTheFile or MatrixLoadingIncomplete) if there was
 */
Matrix load(std::string filepath);

#endif
