#include "vecn.h"

Vec::Vec()
{
    data_eigen = VectorXd(3);
//    data = gsl_vector_calloc(3);
}

Vec::Vec(unsigned int len)
{
    data_eigen = VectorXd(len);
//    data = gsl_vector_calloc(len);
}

Vec::Vec(double v1, double v2, double v3)
{
    data_eigen = VectorXd(3);

    data_eigen(0) = v1;
    data_eigen(1) = v2;
    data_eigen(2) = v3;


//    data = gsl_vector_calloc(3);

//    gsl_vector_set(data, 0, v1);
//    gsl_vector_set(data, 1, v2);
//    gsl_vector_set(data, 2, v3);
}

Vec::Vec(Vec4 v)
{
    data_eigen = VectorXd(3);
    data_eigen(0) = v.x();
    data_eigen(1) = v.y();
    data_eigen(2) = v.z();


//    data = gsl_vector_calloc(3);

//    gsl_vector_set(data, 0, v.x());
//    gsl_vector_set(data, 1, v.y());
//    gsl_vector_set(data, 2, v.z());
}

Vec::Vec(Vec4 v1,Vec4 v2)
{
    data_eigen = VectorXd(6);
    data_eigen(0) = v1.x();
    data_eigen(1) = v1.y();
    data_eigen(2) = v1.z();

    data_eigen(3) = v2.x();
    data_eigen(4) = v2.y();
    data_eigen(5) = v2.z();

//    data = gsl_vector_calloc(6);

//    gsl_vector_set(data, 0, v1.x());
//    gsl_vector_set(data, 1, v1.y());
//    gsl_vector_set(data, 2, v1.z());

//    gsl_vector_set(data, 3, v2.x());
//    gsl_vector_set(data, 4, v2.y());
//    gsl_vector_set(data, 5, v2.z());
}



Vec::Vec(const Vec& u)
{
    data_eigen = u.data_eigen;
//    data = gsl_vector_alloc(u.data->size);
//    gsl_vector_memcpy(data, u.data);
}

Vec::~Vec()
{
   // gsl_vector_free(data);
}

void Vec::set(Vec& u)
{
//    if (u.size() != size()) {
//        //gsl_vector_free(data);
//        //data = gsl_vector_calloc(u.size());
//    }

    data_eigen = u.data_eigen;

    //gsl_vector_memcpy(data, u.data);
}

void Vec::print(int n_cols_block)
{
    cout.precision(4);

    // num rows and columns
    int n = this->size();
    //int n_cols_block = 10; //number of columns of the blocks

    printf("Vector: ");

    //for (unsigned int i=0; i < this->size(); i++) {

    //for each block
    for (int i=0;i<n;i++){
        printf(" %.3f",data_eigen(i));
    }

    printf("\n");


//    for (int b=0; b < (n/n_cols_block)+1; b++) {
//      if (b!=0) cout << "   Continuing...\n";
//      //for each [block b]'s column
//      for (int i=b*n_cols_block; i < min((b+1)*n_cols_block,n); i++) {
//        if ( (*this)[i]>ZERO || (*this)[i]<-ZERO ) {
//          cout << " ";
//          cout.width(10);
//          //cout.fill('#');
//          cout << (*this)[i];
//          cout << " | ";
//        } else  cout << " " << "       0.0" << " | ";
//      }
//      cout << endl;
//    }

//    cout.precision(6);
}

Vec Vec::operator*(double s)
{


    this->data_eigen = s*this->data_eigen;
    Vec v( *this );

//    gsl_vector_scale(v.data, s);

    return v;
}

Vec Vec::operator/(double s)
{


    this->data_eigen = this->data_eigen*(1.0/s);
    Vec v( *this );

//    gsl_vector_scale(v.data, 1.0/s);

    return v;
}

Vec Vec::operator+(Vec u)
{


    if (u.size() != size()) {
        std::cout << "[Vec::operator+] - dimension does not match\n";
        exit(0);
    }

    this->data_eigen = this->data_eigen+u.data_eigen;
    Vec v( *this );

    //gsl_vector_add(v.data, u.data);

    return v;
}

Vec Vec::operator-(Vec u)
{


    if (u.size() != size()) {
        std::cout << "[Vec::operator-] - dimension doest not match\n";
        exit(0);
    }
    this->data_eigen = this->data_eigen - u.data_eigen;
    Vec v( *this );

    //gsl_vector_sub(v.data, u.data);

    return v;
}

double Vec::dot(Vec& u)
{
    double sum = 0.0;

    if (u.size() != size()) {
        std::cout << "[Vec::dot] - dimension does not match\n";
        exit(0);
    }

    for (unsigned int i = 0; i < size(); ++i)
        sum += data_eigen(i) * u.data_eigen(i);

//    for (unsigned int i = 0; i < size(); ++i)
//        sum += data->data[i] * u.data->data[i];

    return sum;
}

double Vec::length() {
  double length = 0.0;

  length = sqrtf( this->dot(*this) );

  return length;
}

void Vec::normalize() {
  double length = this->length();

  if ( fabsf(length) < ZERO ) {
    //std::cout << "\n[Vec::normalize] - length very small (almost equal to zero)\n";
    return;
  }

  (*this) = (*this) * (1.0/length);
}
