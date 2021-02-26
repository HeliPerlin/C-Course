#ifndef EX5_MATRIX_H
#define EX5_MATRIX_H

#include <ostream>
#include <istream>
#include <iostream>

/**
 * represents a single matrix with its dimensions and data
 */
class Matrix
{
private:
    int _rows;
    int _cols;
    float *_data;

public:
    /**
     * constructs a new matrix
     * @param rows the rows number of the matrix
     * @param cols the columns number of the matrix
     */
    Matrix(int rows, int cols);


    /**
     * default constructor
     */
    Matrix();

    /**
     * constructs a new matrix using an existing one
     * @param m the matrix to copy
     */
    Matrix(Matrix const &m);

    /**
     * destructor
     */
    ~Matrix();

    /**
     *
     * @return matrix's number of rows
     */
    int getRows() const;

    /**
     *
     * @return matrix's columns number
     */
    int getCols() const;

    /**
     * transforms the matrix into a vector by changing it's dimensions
     * @return a vectorized version of the matrix
     */
    Matrix& vectorize();

    /**
     * prints matrix's elements
     */
    void print() const;

    /**
     * compares two matrices
     * @param other the matrix to compare to
     * @return true if matrix's have same number of rows, columns and same data array, false
     * otherwise
     */
    bool operator==(const Matrix &other) const;

    /**
     * compares two matrices
     * @param other the matrix to compare to
     * @return true if matrix's are not equal as determined in the == operator
     */
    bool operator!=(const Matrix &other) const;

    /**
     * gives the matrix the attributes of other matrix
     * @param b gives the matrix the attributes of 'b'
     * @return the matrix with b's attributes
     */
    Matrix &operator=(const Matrix&b);

    /**
     * accesses to index in the data array
     * @param i rows index
     * @param j columns index
     * @return the element in the [i][j] index in the data array of the matrix
     */
     float &operator()(int i, int j);

    /**
     * accesses to index in the data array
     * @param i rows index
     * @param j columns index
     * @return the element in the [i][j] index in the data array of the matrix
     */
    float operator()(int i, int j) const;

    /**
     * accesses to index in the data array
     * @param i the wanted index
     * @return the i'th element in the data array
     */
    float &operator[](int i);

    /**
     * accesses to index in the data array
     * @param i the wanted index
     * @return the i'th element in the data array
     */
    float operator[](int i) const;

    /**
     * multiplies two matrices
     * @param other the matrix to multiply by
     * @return a new matrix that is the result of the multiplication
     */
    friend Matrix operator*(const Matrix &matrix, const Matrix &other) ;

    /**
     * multiplies a matrix by a scalar
     * @param c a scalar to multiply by
     * @return a new matrix that is the result of the multiplication
     */
    friend Matrix operator*(Matrix &matrix, float c);

    /**
     * multiplies a matrix by a scalar
     * @param c a scalar to multiply bu
     * @param matrix the matrix to multiple
     * @return a new matrix that is the multiplication result
     */
    friend Matrix operator*(float c, Matrix &matrix);

    /**
     * multiplies a matrix by a scalar
     * @param c a scalar to multiply by
     * @return the original matrix after multiplication
     */
    Matrix &operator*=(float c);

    /**
     * multiplies two matrices
     * @param otherMat the matrix to multiply by
     * @return the original matrix after multiplication
     */
    Matrix &operator*=(Matrix &otherMat);

    /**
     * divides a matrix by a scalar
     * @param c a scalar to divide by
     * @return a new matrix that is the result of the division
     */
    friend Matrix operator/(Matrix &matrix, float c);

    /**
     * divides a matrix by a scalar
     * @param c a scalar to divide by
     * @return the original matrix divided by c
     */
    Matrix &operator/=(float c);

    /**
     * sums two matrices
     * @param B the matrix to add
     * @return a new matrix that is the result of the addition
     */
    friend Matrix operator+(const Matrix &matrix, const Matrix &other);


    /**
     * sums two matrices
     * @param other the matrix to add
     * @return the original matrix after the addition
     */
    Matrix &operator+=(Matrix &other);

    /**
     * sums a matrix and a scalar
     * @param c the scalar to add to the matrix
     * @return the original matrix after the addition
     */
    Matrix &operator+=(float c);

    /**
     * writes the matrix's data into an output stream
     * @param ostream the stream to pass the output to
     * @param matrix the matrix to pass to the stream
     * @return the output stream
     */
    friend std::ostream& operator<<(std::ostream &ostream, const Matrix& matrix);

    /**
     * writes into a matrix from a given input stream
     * @param istream the stream from which receive the data
     * @param matrix the matrix to write the data into
     * @return the input stream
     */
    friend std::istream& operator>>(std::istream &istream, Matrix& matrix);
};


#endif //EX5_MATRIX_H
