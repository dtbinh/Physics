#include "functions.h"
#include "math/quaternion.h"
#include "math/matrix.h"

void to_dQuaternion(Quaternion q,dQuaternion qi)
{
    qi[0] = q.qw();
    qi[1] = q.qx();
    qi[2] = q.qy();
    qi[3] = q.qz();
}

Matrix getMatrix2dMatrix3(const dReal* R)
{
    Matrix mat = Matrix(3,3);
    mat(0,0) = R[0];
    mat(0,1) = R[1];
    mat(0,2) = R[2];

    mat(1,0) = R[4];
    mat(1,1) = R[5];
    mat(1,2) = R[6];

    mat(2,0) = R[8];
    mat(2,1) = R[9];
    mat(2,2) = R[10];

    return mat;
}

const dReal* getVec42dReal(Vec4 v)
{
//    const dReal* r;
//    r = malloc(4 * sizeof(dReal) );
//    &r[0] = v.x();
//    &r[1] = v.y();
//    &r[2] = v.z();
//    &r[3] = 1;
    dVector3 r;
    r[0] = v.x();
    r[1] = v.y();
    r[2] = v.z();
    return r;

}
