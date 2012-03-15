#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <assert.h>

template <class Tp>
class Matrix
{
public:
    Matrix(int r, int c)
        : rows(r), cols(c), data(new Tp[c*r]) {}

    ~Matrix()
    {
        delete[] data;
        data = 0;
    }

    Tp &operator() (int i, int j)
    {
        assert(i >= 0 && i < rows);
        assert(j >= 0 && j < cols);
        return data[cols * i + j];
    }

    int Rows() const
        { return rows; }

    int Cols() const
        { return cols; }

private:
    int rows, cols;
    Tp *data;
};

#endif
