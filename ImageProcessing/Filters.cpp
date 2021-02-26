#include <iostream>
#include <fstream>
#include <cmath>
#include <ostream>
#include <istream>
#include "Matrix.h"

#define SHADES 256


/**
 * applies quantization filter on an image represented as a matrix
 * @param image a matrix representing the image to apply the filter on
 * @param levels the wanted level of shades
 * @return a matrix representing the image after quantization
 */
Matrix quantization(Matrix &image, int const levels)
{
    Matrix newMatrix(image.getRows(), image.getCols());
    newMatrix = image;
    int scaleRange = (SHADES / levels) ;
    for (int i = 0; i < newMatrix.getCols() * newMatrix.getRows(); ++ i)
    {
        float curVal = newMatrix[i];
        int j = 0;
        int destRange = 0;
        while ((j + scaleRange - 1) < SHADES)
        {
            if (curVal >= (float )j && curVal <= (float )j + (float )scaleRange-1)
            {
                destRange = j;
                break;
            }
            j += scaleRange ;
        }
        newMatrix[i] = floorf(((float )destRange + (float )(destRange + scaleRange - 1)) / 2);
    }
    return newMatrix;
}


/**
 * calculates the convolution of two given matrices
 * @param matrix the matrix to apply convolution on
 * @param convolutionMat the matrix to apply convolution with
 * @return the matrix after convolution
 */
Matrix convolution(Matrix const &matrix, Matrix &convolutionMat)
{
    Matrix newMat(matrix.getRows(), matrix.getCols());
    for (int i = 0; i < matrix.getCols() * matrix.getRows(); ++ i)
    {
        float val = 0;
        for (int j = 0; j < 3; ++ j)
        {
            for (int k = 0; k < 3; ++ k)
            {
                int index = i - (matrix.getCols() + 1) + (j * (matrix.getRows()) + k );
                int indCol = index % (matrix.getCols());
                int indRow = (index - indCol) / matrix.getCols();
                int iCol = i % (matrix.getCols());
                int iRow = (i - iCol) / matrix.getCols();
                if (index >= 0 && index < matrix.getCols() * matrix.getRows() && indCol >= iCol -
                    1 && indCol <= iCol + 1 && indRow >= iRow - 1 && indRow <= iRow + 1)
                {
                    val += (convolutionMat(j, k) * matrix[index]);
                }
                else
                {
                    val += 0;
                }
            }
        }
        (newMat)[i] = rintf(val);
    }
    return newMat;

}

/**
 * applies blurring affect on a given image represented as a matrix
 * @param image a matrix representing the image to apply the filter on
 * @return a matrix representing the image after blurring
 */
Matrix blur(const Matrix &image)
{
    Matrix convolutionMat(3, 3);
    // concatenate values using the >> operator and then multiple by 1/16 using *= operator
    // the values are: {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}} * 1/16
    convolutionMat[0] = 1.f;
    convolutionMat[1] = 2.f;
    convolutionMat[2] = 1.f;
    convolutionMat[3] = 2.f;
    convolutionMat[4] = 4.f;
    convolutionMat[5] = 2.f;
    convolutionMat[6] = 1.f;
    convolutionMat[7] = 2.f;
    convolutionMat[8] = 1.f;
    convolutionMat /= 16;
    Matrix newMatrix = convolution(image, convolutionMat); // HOW DOES T WORK IF "CONVOLUTION"
    // FUNCTION RETURNS A VALUE AND NOT A REFERENCE;
    for (int i = 0; i < newMatrix.getCols()*newMatrix.getRows(); ++ i)
    {
        if (newMatrix[i] > 255)
        {
            newMatrix[i] = 255;
        }
        if (newMatrix[i] < 0)
        {
            newMatrix[i] = 0;
        }
    }

    return newMatrix;
}

/**
 * applies sobel affect on a given image
 * @param image the matrix representing the image to apply the filter on
 * @return the matrix after the application of the filter
 */
Matrix sobel(const Matrix &image)
{
    Matrix Gx(3, 3);
    // concatenate values using the >> operator and then multiple by 1/8 using *= operator
    // the values are: {{1, 0, -1}, {2, 0, -2}, {1, 0, -1}} * 1/8
    Gx[0] = 1.f;
    Gx[1] = 0.f;
    Gx[2] = - 1.f;
    Gx[3] = 2.f;
    Gx[4] = 0.f;
    Gx[5] = - 2.f;
    Gx[6] = 1.f;
    Gx[7] = 0.f;
    Gx[8] = - 1.f;
    Gx /= 8;
    Matrix newMatrix1(image.getRows(), image.getCols());
    newMatrix1 = convolution(image, Gx);


    Matrix Gy(3, 3);
    // concatenate values using the >> operator and then multiple by 1/8 using *= operator
    // the values are: {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}} * 1/8
    Gy[0] = 1.f;
    Gy[1] = 2.f;
    Gy[2] = 1.f;
    Gy[3] = 0.f;
    Gy[4] = 0.f;
    Gy[5] = 0.f;
    Gy[6] = - 1.f;
    Gy[7] = - 2.f;
    Gy[8] = - 1.f;
    Gy /= 8;
    Matrix newMatrix2(image.getRows(), image.getCols());
    newMatrix2 = convolution(image, Gy);

    newMatrix1 += newMatrix2;
    for (int i = 0; i < newMatrix1.getCols()*newMatrix2.getRows(); ++ i)
    {
        if (newMatrix1[i] > 255)
        {
            newMatrix1[i] = 255;
        }
        if (newMatrix1[i] < 0)
        {
            newMatrix1[i] = 0;
        }
    }

    return newMatrix1;
}