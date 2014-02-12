#ifndef MATRIX_H
#define MATRIX_H

/**
 *  @file   matrix.h
 *  @author Nicolas Granger <nicolas.granger@telecom-sudparis.eu>
 *  @brief  implements methods from @ref MathExpression for matrices
 */

#include<CL/cl.hpp>

#include "mathexpression.h"
#include "context.h"

/// @brief Implements main aspects of mathematical matrices for
///        @ref MathExpression .
class Matrix: public MathExpression {

    public:

	/// creates a matrix object from a data set
	/// @param values values of the matrix
    /// @param m     number of lines in the matrix
    /// @param n     number of columns in the matrix
    /// @param keepInCLMem indicates wether the computed result should be kept
    ///        in OpenCL memory even if there are no parent expressions.
	Matrix ( float* values,
		const unsigned int m,
		const unsigned int n,
		const bool keepInCLMem = false );

	/// create an matrix whose value is the result of a computation
	/// @param programName name of the file where the kernel is (without .cl)
	/// @param kernelName  name of the kernel to call for the computation
    /// @param m number of lines in the matrix
    /// @param n number of columns in the matrix
    /// @param keepInCLMem indicates wether the computed result should be kept
    ///        in OpenCL memory even if there are no parent expressions.
	Matrix ( const std::string programName,
		const std::string kernelName,
		const unsigned int m,
		const unsigned int n,
		const bool keepInCLMem = false );

    /// @brief Creates a computed matrix as the result of the multiplication of
    ///        two matrices.
    /// @param A left operand
    /// @param B right operand
    /// @param keepInCLMem indicates wether the computed result should be kept
    ///        in OpenCL memory even if there are no parent expressions.
    /// @return result of the matrix product A*B
	static Matrix mul ( Matrix& A, Matrix& B, const bool keepInCLMem = false );

    /// @brief Creates a computed matrix as the result of the sum of two
    ///        matrices.
    /// @param A left operand
    /// @param B right operand
    /// @param keepInCLMem indicates wether the computed result should be kept
    ///        in OpenCL memory even if there are no parent expressions.
    /// @return result of the matrix product A+B
	static Matrix add ( Matrix& A, Matrix& B, const bool keepInCLMem = false );

    /// Center and normalizes the columns of a matrix.
    /// @param A Input matrix
    /// @param keepInCLMem indicates wether the computed result should be kept
    ///        in OpenCL memory even if there are no parent expressions.
    /// @return input matrix with normalized columns
	static Matrix normalize ( Matrix& A, const bool keepInCLMem = false );

	void retrieveData ( Context& context, cl::CommandQueue& queue ) override;
	
	/// @return the value of the matrix at line i and column j
	float getValue ( const int i, const int j );
	
	/// @return a 1D float array as the concatenation of the lines of the matrix
	float* getValues() const;
	
    /// @return matrix width
	int getWidth() const;
	
    /// @return matrix height
    int getHeight() const;

#ifndef NDEBUG
    /// @brief Prints matrix on standard output in scientific notation with
    ///        5 decimals.
	void print();
#endif

    protected:

	void enqueue ( Context& context, cl::CommandQueue& queue ) override;

	bool allocateForResult ( Context& context ) override;

    int m_productDepth; // width of A or height of B if the is the result of A * B

    private:

	float* m_value;
	/// matrix height
	int m_m;
	/// matrix width
	int m_n;
	std::string m_programName;
	std::string m_kernelName;
};

#endif // MATRIX_H
