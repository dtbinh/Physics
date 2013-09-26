#ifndef MATRIX_H
#define MATRIX_H

#include <gsl/gsl_matrix.h>
#include "vecn.h"

class Matrix
{
    public:
        Matrix();
        Matrix(unsigned int m, unsigned int n);
        Matrix(const Matrix& N); // copy constructor
        ~Matrix();

        inline unsigned int getNumRows() { return data->size1; }
        inline unsigned int getNumColumns() { return data->size2; }

        void setRow(unsigned int i, Vec& u);
        void getRow(unsigned int i, Vec& u);
        void setColumn(unsigned int j, Vec& u);
        void getColumn(unsigned int j, Vec& u);
        void set(Matrix& N);
        void setDiag(Vec& u);
        void setSubmatrix(unsigned int i, unsigned int j, Matrix& sub);
        void getSubmatrix(unsigned int i, unsigned int j, Matrix& sub);

        Matrix transpose();
        Matrix identity();
        Matrix inverse();

        static Matrix crossProductMatrix(Vec u);

        void print(int n_cols_block = 10);

        Matrix operator=(Matrix N) { set(N); return *this; }
        Matrix operator*(Matrix& N);
        Matrix operator*(double s);
        Vec operator*(Vec u);
        Matrix operator+(Matrix& N);
        Matrix operator-(Matrix& N);
        inline double& operator()(int i, int j) { return data->data[j + i * data->size2]; }


        gsl_matrix* data;
};

#endif // MATRIX_H
