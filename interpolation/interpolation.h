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
    static Quaternion Lerp(Quaternion p,Quaternion q,double t);
    static Quaternion Slerp(Quaternion p,Quaternion q,double t);
    static Quaternion LerpQ(Quaternion p,Quaternion q,double t);
    static Quaternion SlerpQ(Quaternion p,Quaternion q,double t);
    static Quaternion KeyFramestoSquad(QList<Vec4> keys,int frame,double t);
    static Quaternion Squad(Quaternion q1,Quaternion q2,Quaternion s1,Quaternion s2,double t);
};

#endif // ITERPOLATION_H
