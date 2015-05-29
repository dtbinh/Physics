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


MatrixF::MatrixF()

{
    // This function allocates memory for a matrix of size 6 rows by 6 columns and
    // initializes all the elements of the matrix to zero.
    //data = gsl_matrix_calloc(6, 6);
    data_eigen = MatrixXd::Identity(6,6);
    //data_eigen = new MatrixXf(6,6);
    //data_eigen->resize(6,6);
}

MatrixF::MatrixF(unsigned int m, unsigned int n)

{
    // This function allocates memory for a matrix of size m rows by n columns and
    // initializes all the elements of the matrix to zero
    //data_eigen.resize(m,n);
    data_eigen = MatrixXd::Zero(m,n);
    //data = gsl_matrix_calloc(m, n);
    //data_eigen->resize(m,n);
}

MatrixF::MatrixF(const MatrixF& N)
{
//    const gsl_matrix* ndata = N.data;

//    data = gsl_matrix_alloc(ndata->size1, ndata->size2);
//    gsl_matrix_memcpy(data, ndata);

    data_eigen = N.data_eigen;
}

MatrixF::~MatrixF()
{
    //gsl_matrix_free(data);
}

void MatrixF::setRow(unsigned int i, Vec& u)
{
    if (data_eigen.rows() != u.data_eigen.rows()) {
        cout << "[MatrixF::set_row] - vector size does not match matrix rows" << endl;
        exit(0);
    }

    this->data_eigen.row(i) = u.data_eigen;


//    gsl_matrix_set_row(data, i, u.data);
}

void MatrixF::getRow(unsigned int i, Vec& u)
{
    if (data_eigen.rows() != u.data_eigen.rows()) {
        cout << "[MatrixF::get_row] - vector size does not match matrix rows" << endl;
        exit(0);
    }

    u.data_eigen = data_eigen.row(i);

//    gsl_matrix_get_row(u.data, data, i);
}

void MatrixF::setColumn(unsigned int j, Vec& u)
{
    if (data_eigen.cols() != u.data_eigen.cols()) {
        cout << "[MatrixF::set_column] - vector size does not match matrix cols" << endl;
        exit(0);
    }

    this->data_eigen.col(j) = u.data_eigen;

//    gsl_matrix_set_col(data, j, u.data);
}

void MatrixF::getColumn(unsigned int j, Vec& u)
{
    if (data_eigen.cols() != u.data_eigen.cols()) {
        cout << "[MatrixF::get_column] - vector size does not match matrix cols" << endl;
        exit(0);
    }

    u.data_eigen = data_eigen.col(j);

    //gsl_matrix_get_col(u.data, data, j);
}

void MatrixF::set(MatrixF &N)
{
//    if (getNumRows() != N.getNumRows() || getNumColumns() != N.getNumColumns()) {
//        gsl_matrix_free( data );
//        data = gsl_matrix_alloc(N.getNumRows(), N.getNumColumns());
//    }

    this->data_eigen = N.data_eigen;

    //gsl_matrix_memcpy(data, N.data);
}

void MatrixF::setDiag(Vec& u)
{
    if (data_eigen.cols() != u.size()) {
        cout << "[MatrixF::set_diag] - matrix is not square" << endl;
        exit(0);
    }

    if (data_eigen.rows() != u.size()) {
        cout << "[MatrixF::set_diag] - diag size does not match matrix size" << endl;
        exit(0);
    }
    for (unsigned int i=0; i < u.size(); i++)
        this->data_eigen(i,i) = u.data_eigen(i);


//    for (unsigned int i=0; i < u.size(); i++)
//        data->data[i + i * data->size2] = u[i];
}

void MatrixF::setSubmatrix(unsigned int i, unsigned int j, MatrixF& sub)
{
    unsigned int nrows = getNumRows();
    unsigned int ncols = getNumColumns();
    unsigned int nrows_sub = sub.getNumRows();
    unsigned int ncols_sub = sub.getNumColumns();

    if (i + nrows_sub > nrows) {
        cout << "[MatrixF::setSubmatrix] - submatrix out of bounds" << endl;
        exit(0);
    }

    if (j + ncols_sub > ncols) {
        cout << "[MatrixF::setSubmatrix] - submatrix out of bounds" << endl;
        exit(0);
    }

    //qDebug() << "teste!";
    for (unsigned int k=0; k < nrows_sub; k++)
        for (unsigned int l=0; l < ncols_sub; l++)
            this->data_eigen(i+k,j+l) = sub(k,l);
    //qDebug() << "passou!";

//    for (unsigned int k=0; k < nrows_sub; k++)
//        for (unsigned int l=0; l < ncols_sub; l++)
//            data->data[ (j+l) + (i+k)*ncols ] = sub(k, l);
}

void MatrixF::getSubmatrix(unsigned int i, unsigned int j, MatrixF &sub)
{
    unsigned int nrows = getNumRows();
    unsigned int ncols = getNumColumns();
    unsigned int nrows_sub = sub.getNumRows();
    unsigned int ncols_sub = sub.getNumColumns();

    if (i + nrows_sub > nrows) {
        cout << "[MatrixF::getSubmatrix] - submatrix out of bounds" << endl;
        exit(0);
    }

    if (j + ncols_sub > ncols) {
        cout << "[MatrixF::getSubmatrix] - submatrix out of bounds" << endl;
        exit(0);
    }


    for (unsigned int k=0; k < nrows_sub; k++)
        for (unsigned int l=0; l < ncols_sub; l++)
            sub(k,l) = this->data_eigen(i+k,j+l);


            //sub(k, l) = data->data[ (j+l) + (i+k)*ncols ];
}

MatrixF MatrixF::transpose()
{
    //int n, m;


    // new rows and columns after transpose
//    m = getNumRows();
//    n = getNumColumns();

//    if (n != m) {
//        MatrixF newmat(n, m);

//        for (int i=0; i < m; i++)
//            for (int j=0; j < n; j++)
//                newmat(j, i) = (*this)(i, j);

//        return newmat;
//    }
//    else{
        //gsl_matrix_transpose( T.data );
    MatrixF T(*this);
    T.data_eigen = data_eigen.transpose();
    //}


    return T;
}

MatrixF MatrixF::identity()
{

    MatrixF I(*this);

    //gsl_matrix_set_identity( I.data );
    I.data_eigen = MatrixXd::Identity(this->getNumRows(),this->getNumColumns());


    return I;
}

MatrixF MatrixF::inverse()
{
    int s;
    int n = getNumRows();
    int m = getNumColumns();

    if (m != n) {
        cout << "[MatrixF::inverse] - matrix is not a square" << endl;
        exit(0);
    }



    MatrixF inv(*this);
    inv.data_eigen = data_eigen.inverse();


//    gsl_permutation* p = gsl_permutation_alloc(n);
//    gsl_matrix* LU = gsl_matrix_alloc(n, n);

//    gsl_matrix_memcpy(LU, inv.data);

//    gsl_linalg_LU_decomp(LU, p, &s);
//    gsl_linalg_LU_invert(LU, p, inv.data);

//    gsl_permutation_free(p);
//    gsl_matrix_free(LU);

    return inv;
}

MatrixF MatrixF::crossProductMatrix(Vec u)
{
    if ( u.size() != 3 ) {
        cout << "[MatrixF::cross] - vector is not 3x1" << endl;
        exit(0);
    }

    MatrixF cross_e = MatrixF(3,3);
        cross_e.data_eigen(0,0) = 0;
        cross_e.data_eigen(0,1) = -u[2];
        cross_e.data_eigen(0,2) = u[1];

        cross_e.data_eigen(1,0) = u[2];
        cross_e.data_eigen(1,1) = 0;
        cross_e.data_eigen(1,2) = -u[0];

        cross_e.data_eigen(2,0) = -u[1];
        cross_e.data_eigen(2,1) = u[0];
        cross_e.data_eigen(2,2) = 0;

//    MatrixF cross = MatrixF(3,3);
//        cross(0,0) = 0;
//        cross(0,1) = -u[2];
//        cross(0,2) = u[1];

//        cross(1,0) = u[2];
//        cross(1,1) = 0;
//        cross(1,2) = -u[0];

//        cross(2,0) = -u[1];
//        cross(2,1) = u[0];
//        cross(2,2) = 0;

    return cross_e;
}

void MatrixF::print(int /*n_cols_block*/)
{
    int elements = 0;

    printf("Matrix: \n");
    for(unsigned int i=0;i<this->getNumRows();i++){
        for(unsigned int j=0;j<this->getNumColumns();j++){
            printf(" %.2f",(*this)(i,j));
            if(j==this->getNumColumns()-1) printf("\n");
            elements++;
        }

    }
    printf("-------------------------------\n");
    //printf("Elementos: %d\n",elements);

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

MatrixF MatrixF::operator*(MatrixF& N)
{
    if (this->getNumColumns() != N.getNumRows()) {
        cout << "[MatrixF::operator* matrix] - dimension does not match" << endl;
        exit(0);
    }

    MatrixF MN_temp(getNumRows(), N.getNumColumns());

//    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, data,
//                   N.data, 0.0, MN_temp.data);
    MN_temp.data_eigen = this->data_eigen * N.data_eigen;

    return MN_temp;
}

MatrixF MatrixF::operator*(double s)
{

    MatrixF M( *this );
    M.data_eigen = s * data_eigen ;


//    gsl_matrix_scale(M.data, s);

    return M;
}

Vec MatrixF::operator*(Vec u)
{
    if (this->getNumColumns() != u.size()) {
        cout << "[MatrixF::operator* fs::vector] - dimension does not match" << endl;
        exit(0);
    }

    //fs::vector Mu( u.size() );
    Vec Mu( getNumRows() );    // NOTE NOTE

    //this->data_eigen = this->data_eigen * u.data_eigen;

    Mu.data_eigen = this->data_eigen * u.data_eigen;

//    gsl_blas_dgemv(CblasNoTrans, 1.0, data, u.data, 0.0, Mu.data);

    return Mu;
}

MatrixF MatrixF::operator+(MatrixF& N)
{


    if (this->getNumRows() != N.getNumRows() ||
        this->getNumColumns() != N.getNumColumns())
    {
        cout << "[MatrixF::opertor+] - dimension does not match" << endl;
        exit(0);
    }

//    gsl_matrix_add(M.data, N.data);
    MatrixF M( *this );
    M.data_eigen = M.data_eigen + N.data_eigen;


    return M;
}

MatrixF MatrixF::operator-(MatrixF& N)
{
    if (this->getNumRows() != N.getNumRows() ||
        this->getNumColumns() != N.getNumColumns())
    {
        cout << "[MatrixF::operator-] - dimension does not match" << endl;
        exit(0);
    }
    MatrixF M( * this );
    M.data_eigen = M.data_eigen - N.data_eigen;


//    gsl_matrix_sub(M.data, N.data);
    return M;
}

double &MatrixF::operator()(int i, int j)
{
    return data_eigen(i,j);
    //return data->data[j + i * data->size2];
}
