#include "maths_functions.hpp"

#include "../logger.hpp"
#include <stdio.h>

namespace spaceBrain
{

void conv_cpu(int stride, int pad,
		int inputSize, int kernelSize, int outputSize, int inputDepth,
		int outRowStart, int outRowEnd,
		int outColStart, int outColEnd,
		int outDepthStart, int outDepthEnd,
		int inDepthStart, int inDepthEnd,
		const int* input, const int* weights, int* output
)
{
	int paddedRow, paddedCol;
	for(int outRowIndex = outRowStart; outRowIndex < outRowEnd; outRowIndex++)
	{
		for(int outColIndex = outColStart; outColIndex < outColEnd; outColIndex++)
		{
			for(int outDepthIndex = outDepthStart; outDepthIndex < outDepthEnd; outDepthIndex++)
			{
				for(int inDepthIndex = inDepthStart; inDepthIndex < inDepthEnd; inDepthIndex++)
				{
					for(int kernelRow = 0; kernelRow < kernelSize; kernelRow++)
					{
						paddedRow = stride * outRowIndex + kernelRow - pad;

						for(int kernelCol = 0; kernelCol < kernelSize; kernelCol++)
						{
							paddedCol = stride * outColIndex + kernelCol - pad;

							if(paddedCol < 0 || paddedCol >= inputSize || paddedRow < 0 || paddedRow >= inputSize)
							{
								// point is in padded area
								output[(outDepthIndex * outRowEnd + outRowIndex) * outColEnd + outColIndex] += 0;
							}
							else
							{
								output[(outDepthIndex * outputSize + outRowIndex) * outputSize + outColIndex] +=
//								output[outDepthIndex][rowIndex][colIndex] +=
									weights[((outDepthIndex * inputDepth + inDepthIndex) * kernelSize + kernelRow) * kernelSize + kernelCol] *
//									weights[outDepthIndex][inDepthIndex][kernelRow][kernelCol] *
									input[(inDepthIndex * inputSize + paddedRow) * inputSize + paddedCol];
//									input[inDepthIndex][inputRow][inputCol];
							}

						}
					}
				}
			}
		}
	}
}

void conv_cpu_hw(int stride, int pad,
		int inputSize, int kernelSize, int outputSize, int inputDepth,
		int outRowStart, int outRowEnd,
		int outColStart, int outColEnd,
		int outDepthStart, int outDepthEnd,
		int inDepthStart, int inDepthEnd,
		const int* inputTile, const int* weightsTile, int* outputTile
)
{

	int outRowTileSize = outRowEnd - outRowStart;
	int outColTileSize = outColEnd - outColStart;
	int outDepthTileSize = outDepthEnd - outDepthStart;
	int inDepthTileSize = inDepthEnd - inDepthStart;

	int inRowTileSize = outRowTileSize + 2;
	int inColTileSize = outColTileSize + 2;

	int paddedRow, paddedCol;
	for(int outRowIndex = outRowStart; outRowIndex < outRowEnd; outRowIndex++)
	{
		for(int outColIndex = outColStart; outColIndex < outColEnd; outColIndex++)
		{
			for(int outDepthIndex = outDepthStart; outDepthIndex < outDepthEnd; outDepthIndex++)
			{
				int result = 0;
				for(int inDepthIndex = inDepthStart; inDepthIndex < inDepthEnd; inDepthIndex++)
				{
					for(int kernelRow = 0; kernelRow < kernelSize; kernelRow++)
					{
						paddedRow = stride * outRowIndex + kernelRow - pad;

						for(int kernelCol = 0; kernelCol < kernelSize; kernelCol++)
						{
							paddedCol = stride * outColIndex + kernelCol - pad;

							if(!(paddedCol < 0 || paddedCol >= inputSize || paddedRow < 0 || paddedRow >= inputSize))
							{
								//outputTile[(outDepthIndex * outRowTileSize + outRowIndex) * outColTileSize + outColIndex] +=

								result +=
//								output[outDepthIndex][rowIndex][colIndex] +=
									weightsTile[((outDepthIndex * inDepthTileSize + inDepthIndex) * kernelSize + kernelRow) * kernelSize + kernelCol] *
//									weights[outDepthIndex][inDepthIndex][kernelRow][kernelCol] *
									inputTile[(inDepthIndex * inRowTileSize + paddedRow) * inColTileSize + paddedCol];
//									input[inDepthIndex][inputRow][inputCol];

								//std::cout << (outDepthIndex * outRowTileSize + outRowIndex) * outColTileSize + outColIndex << " " << outputTile[(outDepthIndex * outRowTileSize + outRowIndex) * outColTileSize + outColIndex] << std::endl;
							}
						}
					}
				}
				outputTile[(outDepthIndex * outRowTileSize + outRowIndex) * outColTileSize + outColIndex] = result;
			}
		}
	}
}

void conv_cpu_transB(int stride, int pad,
		int inputSize, int kernelSize, int outputSize, int outputDepth,
		int outRowStart, int outRowEnd,
		int outColStart, int outColEnd,
		int outDepthStart, int outDepthEnd,
		int inDepthStart, int inDepthEnd,
		const int* input, const int* weights, int* output
)
{
	int paddedRow, paddedCol;
	for(int rowIndex = outRowStart; rowIndex < outRowEnd; rowIndex++)
	{
		for(int colIndex = outColStart; colIndex < outColEnd; colIndex++)
		{
			for(int outDepthIndex = outDepthStart; outDepthIndex < outDepthEnd; outDepthIndex++)
			{
				for(int inDepthIndex = inDepthStart; inDepthIndex < inDepthEnd; inDepthIndex++)
				{
					for(int kernelRow = 0; kernelRow < kernelSize; kernelRow++)
					{
						paddedRow = stride * rowIndex + kernelRow - pad;

						for(int kernelCol = 0; kernelCol < kernelSize; kernelCol++)
						{
							paddedCol = stride * colIndex + kernelCol - pad;

							if(paddedCol < 0 || paddedCol >= inputSize || paddedRow < 0 || paddedRow >= inputSize)
							{
								// point is in padded area
								output[(outDepthIndex * outRowEnd + rowIndex) * outColEnd + colIndex] += 0;
							}
							else
							{
								output[(outDepthIndex * outputSize + rowIndex) * outputSize + colIndex] +=
//								output[outDepthIndex][rowIndex][colIndex] +=
									weights[((inDepthIndex * kernelSize + kernelRow) * kernelSize + kernelCol) * outputDepth + outDepthIndex] *
//									weights[inDepthIndex][kernelRow][kernelCol][outDepthIndex] *
									input[(inDepthIndex * inputSize + paddedRow) * inputSize + paddedCol];
//									input[inChannelIndex][paddedRow][paddedCol];
							}
						}
					}
				}
			}
		}
	}
}

void gemm_cpu(const bool isTransposeA, const bool isTransposeB, const int m, const int n, const int k, const int alpha, const int* A, const int* B, const int beta, int* C)
{
	Logger::GetLogger()->LogMessage("\tgemm computation, transposeA = %i, transposeB = %i, alpha = %.1f, beta = %.1f", isTransposeA, isTransposeB, alpha, beta);

	//	(alpha*op(A)*op(B) + beta*C)
	//no need for size check as k is both the width of op(A) and height of op(B)
	if(!isTransposeA && !isTransposeB)
	{
		for (int mIndex = 0; mIndex < m; mIndex++)
		{
			for (int nIndex = 0; nIndex < n; nIndex++)
			{
				int result = 0.0;
				for (int kIndex = 0; kIndex < k; kIndex++)
				{
					result += alpha * A[mIndex*k+kIndex] * B[kIndex*n+nIndex];
				}
				C[mIndex*n+nIndex] = result + beta * C[mIndex*n+nIndex];
			}
		}
	}
	else if(!isTransposeA && isTransposeB)// B only is tranpose
	{
		for (int mIndex = 0; mIndex < m; mIndex++)
		{
			for (int nIndex = 0; nIndex < n; nIndex++)
			{
				int result = 0.0;
				for (int kIndex = 0; kIndex < k; kIndex++)
				{
					result += alpha * A[mIndex*k+kIndex] * B[nIndex*k+kIndex];
				}
				C[mIndex*n+nIndex] = result + beta * C[mIndex*n+nIndex];
			}
		}
	}
	else if(isTransposeA && !isTransposeB) // A only is tranpose
	{
		for (int mIndex = 0; mIndex < m; mIndex++)
		{
			for (int nIndex = 0; nIndex < n; nIndex++)
			{
				int result = 0.0;
				for (int kIndex = 0; kIndex < k; kIndex++)
				{
					result += alpha * A[kIndex*m+mIndex] * B[kIndex*n+nIndex];
				}
				C[mIndex*n+nIndex] = result + beta * C[mIndex*n+nIndex];
			}
		}
	}
	else  // both transpose
	{
		for (int mIndex = 0; mIndex < m; mIndex++)
		{
			for (int nIndex = 0; nIndex < n; nIndex++)
			{
				int result = 0.0;
				for (int kIndex = 0; kIndex < k; kIndex++)
				{
					result += alpha * A[kIndex*m+mIndex] * B[nIndex*k+kIndex];
				}
				C[mIndex*n+nIndex] = result + beta * C[mIndex*n+nIndex];
			}
		}
	}
}

#define OUT_ROW_TILE_SIZE_3X3 14
#define OUT_COL_TILE_SIZE_3X3 14
#define OUT_DEPTH_TILE_SIZE_3X3 4
#define IN_ROW_TILE_SIZE_3X3 OUT_ROW_TILE_SIZE_3X3+2
#define IN_COL_TILE_SIZE_3X3 OUT_COL_TILE_SIZE_3X3+2
#define IN_DEPTH_TILE_SIZE_3X3 4

#define KERNEL_SIZE_3X3 3
#define PAD_3X3 1
#define STRIDE_3X3 1

#define WEIGHTS_DATA_LENGTH 9
#define INPUT_DATA_LENGTH 256
#define OUTPUT_DATA_LENGTH 196

void test()
{
	int input_buffer[INPUT_DATA_LENGTH];
	int weights_buffer[WEIGHTS_DATA_LENGTH];
	int output_buffer[OUTPUT_DATA_LENGTH];

	for(int i = 0; i < WEIGHTS_DATA_LENGTH; i++)
	{
		weights_buffer[i] = 1;
	}

	for(int i = 0; i < INPUT_DATA_LENGTH; i++)
	{
		input_buffer[i] = 1;
	}

	for(int i = 0; i < OUTPUT_DATA_LENGTH; i++)
	{
		output_buffer[i] = 0;
	}

	int inputSize = 14;
	int outputSize  = 14;
	int inputDepth = 1;
	int outRowStart = 0;
	int outRowEnd = 14; 		// outRow limits
	int outColStart = 0;
	int outColEnd = 14; 		// outCol limits
	int outDepthStart = 0;
	int outDepthEnd = 1; 	// outDepth limits
	int inDepthStart = 0;
	int inDepthEnd	= 1; 	// inDepth limits


	int outRowTileSize = outRowEnd - outRowStart;
	int outColTileSize = outColEnd - outColStart;
	int outDepthTileSize = outDepthEnd - outDepthStart;
	int inDepthTileSize = inDepthEnd - inDepthStart;

	int inRowTileSize = outRowTileSize + 2;
	int inColTileSize = outColTileSize + 2;

	int paddedRow, paddedCol;
	for(int outRowIndex = outRowStart; outRowIndex < outRowEnd; outRowIndex++)
	{
		for(int outColIndex = outColStart; outColIndex < outColEnd; outColIndex++)
		{
			for(int outDepthIndex = outDepthStart; outDepthIndex < outDepthEnd; outDepthIndex++)
			{
				int result = 0;
				for(int inDepthIndex = inDepthStart; inDepthIndex < inDepthEnd; inDepthIndex++)
				{
					for(int kernelRow = 0; kernelRow < KERNEL_SIZE_3X3; kernelRow++)
					{
						paddedRow = STRIDE_3X3 * outRowIndex + kernelRow - PAD_3X3;

						for(int kernelCol = 0; kernelCol < KERNEL_SIZE_3X3; kernelCol++)
						{
							paddedCol = STRIDE_3X3 * outColIndex + kernelCol - PAD_3X3;

							if(!(paddedCol < 0 || paddedCol >= inputSize || paddedRow < 0 || paddedRow >= inputSize))
							{
								//outputTile[(outDepthIndex * outRowTileSize + outRowIndex) * outColTileSize + outColIndex] +=

								result +=
										//								output[outDepthIndex][rowIndex][colIndex] +=
										weights_buffer[((outDepthIndex * inDepthTileSize + inDepthIndex) * KERNEL_SIZE_3X3 + kernelRow) * KERNEL_SIZE_3X3 + kernelCol] *
										//									weights[outDepthIndex][inDepthIndex][kernelRow][kernelCol] *
										input_buffer[(inDepthIndex * inRowTileSize + paddedRow) * inColTileSize + paddedCol];
								//									input[inDepthIndex][inputRow][inputCol];

								//std::cout << (outDepthIndex * outRowTileSize + outRowIndex) * outColTileSize + outColIndex << " " << outputTile[(outDepthIndex * outRowTileSize + outRowIndex) * outColTileSize + outColIndex] << std::endl;
							}
						}
					}
				}
				output_buffer[(outDepthIndex * outRowTileSize + outRowIndex) * outColTileSize + outColIndex] = result;
//				std::cout << output_buffer[(outDepthIndex * outRowTileSize + outRowIndex) * outColTileSize + outColIndex] << std::endl;
			}
		}
	}

	std::cout << "output: \n\t";
	for(int hIndex = 0; hIndex < 14; hIndex ++)
	{
			for(int wIndex = 0; wIndex < 14; wIndex ++)
			{
				std::cout << output_buffer[hIndex * 14 + wIndex];
			}
			std::cout << ("\n\t");
	}
	std::cout << ("\n");
}




}
