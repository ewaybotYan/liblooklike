#ifndef MATRIX_H
#define MATRIX_H

/**
 *  @file   matrix.h
 *  @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 *  @brief  implements methods from @ref MathExpression for matrices
 */

#include<CL/cl.hpp>

#include "../include/mathexpression.h"
#include "../include/context.h"

class Matrix: public MathExpression {

    public:

	/// creates a matrix object from a data set
	/// @param values values of the matrix
	/// @m     number of lines in the matrix
	/// @n     number of columns in the matrix
	Matrix ( float* values,
		const unsigned int m,
		const unsigned int n,
		const bool keepInCLMem = false );

	/// create an matrix whose value is the result of a computation
	/// @param programName name of the file where the kernel is (without .cl)
	/// @param kernelName  name of the kernel to call for the computation
	/// @m     number of lines in the matrix
	/// @n     number of columns in the matrix
	Matrix ( const std::string programName,
		const std::string kernelName,
		const unsigned int m,
		const unsigned int n,
		const bool keepInCLMem = false );

	/// creates a computed matrix as the result of the multiplication of two matrices
	static Matrix mul ( Matrix& A, Matrix& B, const bool keepInCLMem = false );

	/// creates a computed matrix as the result of the sum of two matrices
	static Matrix add ( Matrix& A, Matrix& B, const bool keepInCLMem = false );

	/// center and normalizes the columns of a matrix
	static Matrix normalize ( Matrix& A, const bool keepInCLMem = false );

	void retrieveData ( Context& context, cl::CommandQueue& queue ) override;
	
	/// @return the value of the matrix at line i and column j
	float getValue ( const int i, const int j );
	
	/// @return a 1D float array as the concatenation of the lines of the matrix
	float* getValues() const;
	
	int getWidth() const;
	
	int getHeight() const;

    protected:

	void enqueue ( Context& context, cl::CommandQueue& queue ) override;

	bool allocateForResult ( Context& context ) override;

    private:
      
	float* m_value;
	int m_m;
	int m_n;
	std::string m_programName;
	std::string m_kernelName;
};

#endif // MATRIX_H
