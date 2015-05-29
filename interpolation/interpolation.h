#ifndef ITERPOLATION_H
#define ITERPOLATION_H
#include <QMatrix3x4>
#include <math/vec4.h>
#include <math/quaternion.h>
class Interpolation
{
public:
    Interpolation();
    static QMatrix4x4 LinearEuler(Vec4 p,Vec4 q,double t);
    static QuaternionQ Lerp(QuaternionQ p,QuaternionQ q,double t);
    static QuaternionQ Slerp(QuaternionQ p,QuaternionQ q,double t);
    static QuaternionQ LerpQ(QuaternionQ p,QuaternionQ q,double t);
    static QuaternionQ SlerpQ(QuaternionQ p,QuaternionQ q,double t);
    static QuaternionQ KeyFramestoSquad(QList<Vec4> keys,int frame,double t);
    static QuaternionQ Squad(QuaternionQ q1,QuaternionQ q2,QuaternionQ s1,QuaternionQ s2,double t);
};

#endif // ITERPOLATION_H
