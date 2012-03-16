#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cassert>
#include <string>

template <class Tp>
class Matrix
{
public:
    Matrix(int r, int c) : rows(r), cols(c), data(new Tp[c*r])
        {}

    Matrix(const Matrix<Tp> &other) : rows(other.rows), cols(other.cols), data(new Tp[other.rows * other.cols])
    {
        CopyData(other);
    }

    ~Matrix()
    {
        delete[] data;
        data = 0;
    }

    Tp &operator() (int i, int j)
    {
        assert(i >= 0 && i < rows && j >= 0 && j < cols);
        return data[cols * i + j];
    }

    Tp operator() (int i, int j) const
    {
        assert(i >= 0 && i < rows && j >= 0 && j < cols);
        return data[cols * i + j];
    }

    Matrix &operator= (const Matrix<Tp> &other)
    {
        if (&other != this) {
            if (other.rows != rows || other.cols != cols) {
                delete[] data;
                rows = other.rows;
                cols = other.cols;
                data = new Tp[rows * cols];
            }

            CopyData(other);
        }
        return *this;
    }

    int Rows() const
        { return rows; }

    int Cols() const
        { return cols; }

    bool CheckBounds(int row, int col) const
        { return row >= 0 && col >= 0 && row < Rows() && col < Cols(); }

private:
    int rows, cols;
    Tp *data;

    void CopyData(const Matrix &from)
    {
        assert(rows == from.rows && cols == from.cols);
        memcpy(data, from.data, sizeof(Tp) * rows * cols);
    }
};

#endif
