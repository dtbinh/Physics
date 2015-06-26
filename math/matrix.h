#ifndef MATRIX_H
#define MATRIX_H

#include <gsl/gsl_matrix.h>

#include "vecn.h"
#include <eigen3/Eigen/Dense>
using namespace Eigen;


class MatrixF
{
    public:
        MatrixF();
        MatrixF(unsigned int m, unsigned int n);
        MatrixF(const MatrixF& N); // copy constructor
        ~MatrixF();

        inline unsigned int getNumRows() { return data_eigen.rows(); /*return data->size1;*/ }
        inline unsigned int getNumColumns() { return data_eigen.cols(); /*return data->size2;*/ }

        void setRow(unsigned int i, Vec& u);
        void getRow(unsigned int i, Vec& u);
        void setColumn(unsigned int j, Vec& u);
        void getColumn(unsigned int j, Vec& u);
        void set(MatrixF& N);
        void setDiag(Vec& u);
        void setSubmatrix(unsigned int i, unsigned int j, MatrixF &sub);
        void getSubmatrix(unsigned int i, unsigned int j, MatrixF& sub);

        MatrixF transpose();
        MatrixF identity();
        MatrixF inverse();

        static MatrixF crossProductMatrix(Vec u);

        void print(int n_cols_block = 10);


        MatrixF operator=(MatrixF N) { set(N); return *this; }
        MatrixF operator*(MatrixF& N);
        MatrixF operator*(double s);
        Vec operator*(Vec u);
        MatrixF operator+(MatrixF& N);
        MatrixF operator-(MatrixF& N);
        double& operator()(int i, int j);


        MatrixXd data_eigen;


        //gsl_matrix* data;
};

#endif // MATRIX_H
