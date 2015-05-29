#ifndef FUNCTIONS_H
#define FUNCTIONS_H
//#define dDOUBLE
#include "ode/ode.h"
class Vec4;
class QuaternionQ;
class MatrixF;

void to_dQuaternion(QuaternionQ q,dQuaternion qi);
MatrixF getMatrix2dMatrix3(const dReal* R);
const dReal *getVec42dReal(Vec4 v);




#endif // FUNCTIONS_H
