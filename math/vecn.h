#ifndef VECN_H
#define VECN_H
//engloba um vetor de tamanho arbritário
#ifndef ZERO
  #define ZERO 0.00000000001
#endif

#include <math.h>
#include <iostream>
#include "vec4.h"
using namespace std;

#include <gsl/gsl_vector.h>
class Vec
{
public:
    Vec();
    Vec(unsigned int len);
    Vec(double v1, double v2, double v3);
    Vec(Vec4 v);
    Vec(Vec4 v1,Vec4 v2);
    Vec(const Vec& u);    // copy constructor
    ~Vec();

    inline unsigned size() { return data->size; }

    void set(Vec& u);

    void print(int n_cols_block = 10);

    Vec operator=(Vec u) { set(u); return *this; }
    Vec operator*(double s);
    Vec operator/(double s);
    Vec operator+(Vec u);
    Vec operator-(Vec u);
    inline double& operator[](int idx) { return data->data[idx]; }

    double dot(Vec& u);

    double length();
    void normalize();

    gsl_vector* data;
};

#endif // VECN_H
