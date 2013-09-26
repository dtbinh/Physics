#include "matrix.h"
using namespace std;
#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <stdio.h>
#include <iostream>
#include <fstream>


#ifndef ZERO
  #define ZERO 0.00000000001
#endif

Matrix::Matrix()
    : data(NULL)
{
    // This function allocates memory for a matrix of size 6 rows by 6 columns and
    // initializes all the elements of the matrix to zero.
    data = gsl_matrix_calloc(6, 6);
}

Matrix::Matrix(unsigned int m, unsigned int n)
    : data(NULL)
{
    // This function allocates memory for a matrix of size m rows by n columns and
    // initializes all the elements of the matrix to zero
    data = gsl_matrix_calloc(m, n);
}

Matrix::Matrix(const Matrix& N)
{
    const gsl_matrix* ndata = N.data;

    data = gsl_matrix_alloc(ndata->size1, ndata->size2);
    gsl_matrix_memcpy(data, ndata);
}

Matrix::~Matrix()
{
    gsl_matrix_free(data);
}

void Matrix::setRow(unsigned int i, Vec& u)
{
    if (data->size2 != u.size()) {
        cout << "[matrix::set_row] - vector size does not match matrix size2" << endl;
        exit(0);
    }

    gsl_matrix_set_row(data, i, u.data);
}

void Matrix::getRow(unsigned int i, Vec& u)
{
    if (data->size2 != u.size()) {
        cout << "[matrix::get_row] - vector size does not match matrix size2" << endl;
        exit(0);
    }

    gsl_matrix_get_row(u.data, data, i);
}

void Matrix::setColumn(unsigned int j, Vec& u)
{
    if (data->size1 != u.size()) {
        cout << "[matrix::set_column] - vector size does not match matrix size1" << endl;
        exit(0);
    }

    gsl_matrix_set_col(data, j, u.data);
}

void Matrix::getColumn(unsigned int j, Vec& u)
{
    if (data->size1 != u.size()) {
        cout << "[matrix::get_column] - vector size does not match matrix size1" << endl;
        exit(0);
    }

    gsl_matrix_get_col(u.data, data, j);
}

void Matrix::set(Matrix& N)
{
    if (getNumRows() != N.getNumRows() || getNumColumns() != N.getNumColumns()) {
        gsl_matrix_free( data );
        data = gsl_matrix_alloc(N.getNumRows(), N.getNumColumns());
    }

    gsl_matrix_memcpy(data, N.data);
}

void Matrix::setDiag(Vec& u)
{
    if (data->size1 != data->size2) {
        cout << "[matrix::set_diag] - matrix is not square" << endl;
        exit(0);
    }

    if (data->size1 != u.size()) {
        cout << "[matrix::set_diag] - diag size does not match matrix size" << endl;
        exit(0);
    }

    for (unsigned int i=0; i < u.size(); i++)
        data->data[i + i * data->size2] = u[i];
}

void Matrix::setSubmatrix(unsigned int i, unsigned int j, Matrix& sub)
{
    unsigned int nrows = getNumRows();
    unsigned int ncols = getNumColumns();
    unsigned int nrows_sub = sub.getNumRows();
    unsigned int ncols_sub = sub.getNumColumns();

    if (i + nrows_sub > nrows) {
        cout << "[matrix::setSubmatrix] - submatrix out of bounds" << endl;
        exit(0);
    }

    if (j + ncols_sub > ncols) {
        cout << "[matrix::setSubmatrix] - submatrix out of bounds" << endl;
        exit(0);
    }

    for (unsigned int k=0; k < nrows_sub; k++)
        for (unsigned int l=0; l < ncols_sub; l++)
            data->data[ (j+l) + (i+k)*ncols ] = sub(k, l);
}

void Matrix::getSubmatrix(unsigned int i, unsigned int j, Matrix& sub)
{
    unsigned int nrows = getNumRows();
    unsigned int ncols = getNumColumns();
    unsigned int nrows_sub = sub.getNumRows();
    unsigned int ncols_sub = sub.getNumColumns();

    if (i + nrows_sub > nrows) {
        cout << "[matrix::getSubmatrix] - submatrix out of bounds" << endl;
        exit(0);
    }

    if (j + ncols_sub > ncols) {
        cout << "[matrix::getSubmatrix] - submatrix out of bounds" << endl;
        exit(0);
    }

    for (unsigned int k=0; k < nrows_sub; k++)
        for (unsigned int l=0; l < ncols_sub; l++)
            sub(k, l) = data->data[ (j+l) + (i+k)*ncols ];
}

Matrix Matrix::transpose()
{
    int n, m;
    Matrix T(*this);

    // new rows and columns after transpose
    m = getNumRows();
    n = getNumColumns();

    if (n != m) {
        Matrix newmat(n, m);

        for (int i=0; i < m; i++)
            for (int j=0; j < n; j++)
                newmat(j, i) = (*this)(i, j);

        return newmat;
    }
    else
        gsl_matrix_transpose( T.data );

    return T;
}

Matrix Matrix::identity()
{
    Matrix I(*this);

    gsl_matrix_set_identity( I.data );

    return I;
}

Matrix Matrix::inverse()
{
    int s;
    int n = getNumRows();
    int m = getNumColumns();

    if (m != n) {
        cout << "[matrix::inverse] - matrix is not a square" << endl;
        exit(0);
    }

    Matrix inv(*this);

    gsl_permutation* p = gsl_permutation_alloc(n);
    gsl_matrix* LU = gsl_matrix_alloc(n, n);

    gsl_matrix_memcpy(LU, inv.data);

    gsl_linalg_LU_decomp(LU, p, &s);
    gsl_linalg_LU_invert(LU, p, inv.data);

    gsl_permutation_free(p);
    gsl_matrix_free(LU);

    return inv;
}

Matrix Matrix::crossProductMatrix(Vec u)
{
    if ( u.size() != 3 ) {
        cout << "[matrix::cross] - vector is not 3x1" << endl;
        exit(0);
    }

    Matrix cross = Matrix(3,3);
        cross(0,0) = 0;
        cross(0,1) = -u[2];
        cross(0,2) = u[1];

        cross(1,0) = u[2];
        cross(1,1) = 0;
        cross(1,2) = -u[0];

        cross(2,0) = -u[1];
        cross(2,1) = u[0];
        cross(2,2) = 0;

    return cross;
}

void Matrix::print(int n_cols_block)
{
    for(int i=0;i<this->getNumRows();i++){
        for(int j=0;j<this->getNumColumns();j++) printf(" %.2f",(*this)(i,j));
        printf("\n");
    }

//    cout.precision(4);

//    // num rows and columns
//    int m = this->getNumRows();
//    int n = this->getNumColumns();
//    //int n_cols_block = 10; //number of columns of the blocks

//    cout << "Matrix:" << endl;

//    //for each block
//    for (int b=0; b < (n/n_cols_block)+1; b++) {
//      if ( !(b*n_cols_block<n) ) break;
//      if (b!=0) cout << "   _________________________________\n\n   Continuing...\n\n";
//      //for each row
//      for (int i=0; i < m; i++) {
//        //putting ------ in order to separate the rows 3 by 3
//        if ( (i%3==0)&&(i!=0) ) {
//          int uMax = min( n_cols_block,n-(b*n_cols_block) );
//          for (int u=0; u < uMax; u++) cout << "     ------   ";
//          cout << endl;
//        }
//        //for each [block b]'s column
//        for (int j=b*n_cols_block; j < min((b+1)*n_cols_block,n); j++) {
//          if ( (*this)(i, j)>ZERO || (*this)(i, j)<-ZERO ) {
//            cout << " ";
//            cout.width(10);
//            //cout.fill('#');
//            cout << (*this)(i, j);
//            cout << " | ";
//          } else  cout << " " << "       0.0" << " | ";
//        }
//        cout << endl;
//      }
//    }

//    cout.precision(6);
}

Matrix Matrix::operator*(Matrix& N)
{
    if (this->getNumColumns() != N.getNumRows()) {
        cout << "[matrix::operator* matrix] - dimension does not match" << endl;
        exit(0);
    }

    Matrix MN_temp(getNumRows(), N.getNumColumns());

    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, data,
                   N.data, 0.0, MN_temp.data);

    return MN_temp;
}

Matrix Matrix::operator*(double s)
{
    Matrix M( *this );

    gsl_matrix_scale(M.data, s);

    return M;
}

Vec Matrix::operator*(Vec u)
{
    if (this->getNumColumns() != u.size()) {
        cout << "[matrix::operator* fs::vector] - dimension does not match" << endl;
        exit(0);
    }

    //fs::vector Mu( u.size() );
    Vec Mu( getNumRows() );    // NOTE NOTE

    gsl_blas_dgemv(CblasNoTrans, 1.0, data, u.data, 0.0, Mu.data);

    return Mu;
}

Matrix Matrix::operator+(Matrix& N)
{
    Matrix M( *this );

    if (M.getNumRows() != N.getNumRows() ||
        M.getNumColumns() != N.getNumColumns())
    {
        cout << "[matrix::opertor+] - dimension does not match" << endl;
        exit(0);
    }

    gsl_matrix_add(M.data, N.data);

    return M;
}

Matrix Matrix::operator-(Matrix& N)
{
    Matrix M( * this );

    if (M.getNumRows() != N.getNumRows() ||
        M.getNumColumns() != N.getNumColumns())
    {
        cout << "[matrix::operator-] - dimension does not match" << endl;
        exit(0);
    }

    gsl_matrix_sub(M.data, N.data);
    return M;
}
