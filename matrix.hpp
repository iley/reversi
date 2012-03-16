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
        {}

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

    int Rows() const
        { return rows; }

    int Cols() const
        { return cols; }

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
