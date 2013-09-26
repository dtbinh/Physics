#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#define dDOUBLE
#include "ode/ode.h"
class Vec4;
class Quaternion;
class Matrix;

void to_dQuaternion(Quaternion q,dQuaternion qi);
Matrix getMatrix2dMatrix3(const dReal* R);
const dReal *getVec42dReal(Vec4 v);




#endif // FUNCTIONS_H
