#include <iostream>
#include "Matrix.h"
#include <cstring>

#define INDEX_ERR_MSG "Index out of range.\n"
#define DIVISION_ERR_MSG "Division by zero.\n"
#define DIMENSIONS_ERR_MSG "Invalid Matrix dimensions.\n"
#define STREAM_ERR_MSG "Error loading from input stream.\n"


/**
 * constructs a new matrix
 * @param rows the rows number of the matrix
 * @param cols the columns number of the matrix
 */
Matrix::Matrix(int rows, int cols)
{
    if (rows >= 0 && cols >= 0)
    {
        _rows = rows;
        _cols = cols;
    }
    else
    {
        std::cerr << INDEX_ERR_MSG;
        exit(1);
    }
    this->_data = new float[_cols*_rows];
    for (int i = 0; i < rows*cols; ++ i)
    {
        this->_data[i] = 0;
    }
}

/**
 * default constructor
 */
Matrix::Matrix()
{
    _rows = 1;
    _cols = 1;
    this->_data = new float[_cols*_rows];
    this->_data[0] = 1;
}

/**
 * constructs a new matrix using an existing one
 * @param m the matrix to copy
 */
Matrix::Matrix(Matrix const &m)
{
    if (m.getRows() < 0 || m.getCols() < 0)
    {
        std::cerr << INDEX_ERR_MSG;
        exit(1);
    }
    _rows = m.getRows();
    _cols = m.getCols();
    this->_data = new float[_cols*_rows];
    for (int i = 0; i < _rows*_cols; ++ i)
    {
        this->_data[i] = m[i];
    }
}

/**
 * destructor
 */
Matrix::~Matrix()
{
    delete [] _data;
    _data = nullptr;
}

/**
 *
 * @return matrix's number of rows
 */
int Matrix::getRows() const
{
    return this->_rows;
}

/**
 *
 * @return matrix's number of columns
 */

int Matrix::getCols() const
{
    return this->_cols;
}


/**
 * transforms the matrix into a vector by changing it's dimensions
 * @return a vectorized version of the matrix
 */
Matrix& Matrix::vectorize()
{
    this->_rows = this->_cols * this->_rows;
    this->_cols = 1;
    return (*this);
}

/**
 * prints matrix's elements
 */
void Matrix::print() const
{
    for (int i = 0; i < _rows * _cols; ++ i)
    {
        if ( i == _cols*_rows - 1)
        {
            std::cout << _data[i];
        }
        if (i % _cols == _cols - 1 )
        {
            std::cout << _data[i] << std::endl;
        }
        else
        {
            std::cout << _data[i] << " " ;
        }
    }
}


/**
 * compares two matrices
 * @param other the matrix to compare to
 * @return true if matrix's have same number of rows, columns and same data array, false
 * otherwise
 */
bool Matrix::operator==(const Matrix &other) const
{
    if (this->_cols == other.getCols() && this->_rows == other.getRows())
    {
        for (int i = 0; i < _rows*_cols; ++ i)
        {
            if (this->_data[i] != other[i])
            {
                return false;
            }
        }
        return true;
    }
    return false;
}


/**
 * compares two matrices
 * @param other the matrix to compare to
 * @return true if matrix's are not equal as determined in the == operator
 */
bool Matrix::operator!=(const Matrix &other) const
{
    return (!(*this == other));
}

/**
 * gives the matrix the attributes of other matrix
 * @param b gives the matrix the attributes of 'b'
 * @return the matrix with b's attributes
 */
Matrix& Matrix::operator=(const Matrix &b)
{
    if (this == &b)
    {
        return *this;
    }
    this->_cols = b._cols;
    this->_rows = b._rows;
    delete [] this->_data;
    this->_data = new float[_cols*_rows];
    for (int i = 0; i < _rows*_cols; ++ i)
    {
        this->_data[i] = b._data[i];
    }
    return *this;
}


/**
 * accesses to index in the data array
 * @param i rows index
 * @param j columns index
 * @return the element in the [i][j] index in the data array of the matrix
 */
float &Matrix::operator()(int i, int j)
{
    if (_cols <= j || _rows <= i || i < 0 || j < 0)
    {
        std::cerr << INDEX_ERR_MSG;
        exit(1);
    }
    return this->_data[i*_cols + j];
}

/**
 * accesses to index in the data array
 * @param i rows index
 * @param j columns index
 * @return the element in the [i][j] index in the data array of the matrix
 */
float Matrix::operator()(int i, int j) const
{
    if (_cols <= j || _rows <= i || i < 0 || j < 0)
    {
        std::cerr << INDEX_ERR_MSG;
        exit(1);
    }
    return this->_data[i*_cols + j];
}


/**
 * accesses to index in the data array
 * @param i the wanted index
 * @return the i'th element in the data array
 */
float &Matrix::operator[](int i)
{
    if (_cols*_rows <= i || i < 0)
    {
        std::cerr << INDEX_ERR_MSG;
        exit(1);
    }
    return this->_data[i];
}

/**
 * accesses to index in the data array
 * @param i the wanted index
 * @return the i'th element in the data array
 */
float Matrix::operator[](int i) const
    {
        if (_cols*_rows <= i || i < 0)
        {
            std::cerr << INDEX_ERR_MSG;
            exit(1);
        }
        return this->_data[i];
    }

/**
 * multiplies two matrices
 * @param other the matrix to multiply by
 * @return a new matrix that is the result of the multiplication
 */
Matrix operator*(const Matrix &matrix, const Matrix &other )
{
    if (matrix.getCols() != other.getRows())
    {
        std::cerr << DIMENSIONS_ERR_MSG ; // VERIFY
        exit(1);
    }
    Matrix newMatrix(matrix.getRows(), other.getCols());
    for (int i = 0; i < matrix.getRows(); ++ i)
    {
        for (int j = 0; j < other.getCols(); ++ j)
        {
            float value = 0;
            for (int k = 0; k < matrix.getCols(); ++ k)
            {
                value += (matrix)(i, k) * other(k, j);
            }
            newMatrix(i, j) = value;
        }
    }
    return newMatrix;
}


/**
 * multiplies a matrix by a scalar
 * @param c a scalar to multiply by
 * @return a new matrix that is the result of the multiplication
 */
Matrix operator*(Matrix &matrix, float c)
{
    Matrix newMatrix(matrix);
    for (int i = 0; i < matrix.getRows() * matrix.getCols(); ++ i)
    {
        newMatrix._data[i] *= c;
    }
    return newMatrix;
}

/**
 * multiplies a matrix by a scalar
 * @param c a scalar to multiply bu
 * @param matrix the matrix to multiple
 * @return a new matrix that is the multiplication result
 */
Matrix operator*(float c, Matrix &matrix)
{
    Matrix newMatrix(matrix);
    for (int i = 0; i < matrix.getCols() * matrix.getRows(); ++ i)
    {
        newMatrix[i] *= c;
    }
    return newMatrix;
}

/**
 * multiplies a matrix by a scalar
 * @param c a scalar to multiply by
 * @return the original matrix after multiplication
 */
Matrix& Matrix::operator*=(float c)
{
    for (int i = 0; i < _rows*_cols; ++ i)
    {
        _data[i] *= c;
    }
    return *this;
}

/**
 * multiplies two matrices
 * @param otherMat the matrix to multiply by
 * @return the original matrix after multiplication
 */
Matrix& Matrix::operator*=(Matrix &otherMat)
{
    if (this->_cols != otherMat._rows)
    {
        std::cerr << DIMENSIONS_ERR_MSG ; // VERIFY
        exit(1);
    }
    Matrix newMatrix(*this);
    this->_cols = otherMat.getCols();
    for (int i = 0; i < this->_rows; ++ i)
    {
        for (int j = 0; j < otherMat.getCols(); ++ j)
        {
            float value = 0;
            for (int k = 0; k < newMatrix.getCols(); ++ k)
            {
                value += newMatrix(i, k) * otherMat(k, j);
            }
            (*this)(i, j) = value;
        }
    }
    return *this;
}

/**
 * divides a matrix by a scalar
 * @param c a scalar to divide by
 * @return a new matrix that is the result of the division
 */
Matrix operator/(Matrix &matrix, float c)
{
    if (c == 0)
    {
        std::cerr << DIVISION_ERR_MSG ; // VERIFY
        exit(1);
    }
    Matrix newMatrix(matrix);
    for (int i = 0; i < matrix.getRows() * matrix.getCols(); ++ i)
    {
        newMatrix[i] /= c;
    }
    return newMatrix;
}

/**
 * divides a matrix by a scalar
 * @param c a scalar to divide by
 * @return the original matrix divided by c
 */
Matrix& Matrix::operator/=(float c)
{
    if (c == 0)
    {
        std::cerr << DIVISION_ERR_MSG ;
        exit(1);
    }
    for (int i = 0; i < _rows*_cols; ++ i)
    {
        this->_data[i] /= c;
    }
    return *this;
}


/**
 * sums two matrices
 * @param B the matrix to add
 * @return a new matrix that is the result of the addition
 */
Matrix operator+(const Matrix &matrix, const Matrix &other)
{
    if (matrix.getRows() != other.getRows() || matrix.getCols() != other.getCols())
    {
        std::cerr << DIMENSIONS_ERR_MSG ;
        exit(1);
    }
    Matrix newMatrix(matrix.getRows(), matrix.getCols());
    for (int i = 0; i < matrix.getRows() * other.getCols(); ++ i)
    {
        newMatrix[i] = matrix[i] + other[i];
    }
    return newMatrix;
}

/**
 * sums two matrices
 * @param other the matrix to add
 * @return the original matrix after the addition
 */
Matrix& Matrix::operator+=(Matrix &other)
{
    if (this->_rows != other._rows || this->_cols != other._cols)
    {
        std::cerr << DIMENSIONS_ERR_MSG; // VERIFY
        exit(1);
    }
    for (int i = 0; i < this->_rows * this->_cols; ++ i)
    {
        this->_data[i] += other._data[i];
    }
    return *this;
}

/**
 * sums a matrix and a scalar
 * @param c the scalar to add to the matrix
 * @return the original matrix after the addition
 */
Matrix& Matrix::operator+=(float c)
{
    for (int i = 0; i < _rows*_cols; ++ i)
    {
        this->_data[i] += c;
    }
    return *this;
}

/**
 * writes the matrix's data into an output stream
 * @param ostream the stream to pass the output to
 * @param matrix the matrix to pass to the stream
 * @return the output stream
 */
std::istream& operator>>(std::istream &istream, Matrix& matrix)
{
    if (istream)
    {
        for (int i = 0; i < matrix.getRows() * matrix.getCols(); ++ i)
        {
            istream >> matrix[i];
        }
    }
    else
    {
        std::cerr << STREAM_ERR_MSG;
        exit(1);
    }

    return istream;
}

/**
 * writes into a matrix from a given input stream
 * @param istream the stream from which receive the data
 * @param matrix the matrix to write the data into
 * @return the input stream
 */
std::ostream& operator<<(std::ostream &ostream, const Matrix& matrix)
{
    for (int i = 0; i < matrix.getRows() * matrix.getCols(); ++ i)
    {
        if (i == matrix.getCols() * matrix.getRows() - 1)
        {
            ostream << matrix[i];
            return ostream;
        }
        if (i % matrix.getCols() == matrix.getCols() - 1)
        {
            ostream << matrix[i] << std::endl;
        }
        else
        {
            ostream << matrix[i] << " ";
        }
    }
return ostream;
}
