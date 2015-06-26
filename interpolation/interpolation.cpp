#include "interpolation.h"


Interpolation::Interpolation()
{
}

QMatrix4x4 Interpolation::LinearEuler(Vec4 p, Vec4 q, double t)
{
    QMatrix4x4 mat;
    Vec4 result = p*(1 - t) + q*t;
    mat.rotate(result.x3, 0.0, 0.0, 1.0);

    mat.rotate(result.x2, 0.0, 1.0, 0.0);

    mat.rotate(result.x1, 1.0, 0.0, 0.0);

    return mat;
}

QuaternionQ Interpolation::Lerp(QuaternionQ p, QuaternionQ q, double t)
{
    QuaternionQ result =  p*(1.0 - t)+ q*t;
    return result;
}

QuaternionQ Interpolation::Slerp(QuaternionQ q1, QuaternionQ q2, double t)
{
    double cosomega;
    double sinomega;
    double omega;
    double s1, s2;

    if((q1-q2).normal() > (q1+q2).normal()) q2 = q2*(-1);
    cosomega = QuaternionQ::dot(q1,q2);
    if((1.0 - cosomega) < 0.000001 )
    {
        return Lerp(q1, q2, t);
    }
    if((1.0 + cosomega) < 0.000001)
    {
        QuaternionQ q2a;
        q2a.setScalar(-q2.getPosX());
        q2a.setPosX(q2.getScalar());
        q2a.setPosY(-q2.getPosZ());
        q2a.setPosZ(q2.getPosY());
        s1 = sin((1.0 - t) * M_PI_2);
        s2 = sin(t * M_PI_2);
        q2a.normalize();
        QuaternionQ result =  (q1*s1 + q2a*s2);
        result.normalize();
        return result;

    }
    omega = acos(cosomega);
    sinomega = sin(omega);
    s1 = sin((1.0 - t) * omega) / sinomega;
    s2 = sin(t * omega) / sinomega;

    QuaternionQ result = (q1*s1 + q2*s2);
    result.normalize();
    return result;

}

QuaternionQ Interpolation::LerpQ(QuaternionQ p, QuaternionQ q, double t)
{
    QuaternionQ result =  p*(1.0 - t)+ q*t;
    result.normalize();
    return result;
}

QuaternionQ Interpolation::SlerpQ(QuaternionQ p, QuaternionQ q, double t)
{


    double cosomega;
    double sinomega;
    double omega;
    double s1, s2;

    if((p-q).normal() > (p+q).normal()) q = q*(-1);
    cosomega = QuaternionQ::dot(p,q);
    if((1.0 - cosomega) < 0.000001 )
    {
        return LerpQ(p, q, t);
    }
    if((1.0 + cosomega) < 0.000001)
    {
        QuaternionQ q2a;
        q2a.setScalar(-q.getPosX());
        q2a.setPosX(q.getScalar());
        q2a.setPosY(-q.getPosZ());
        q2a.setPosZ(q.getPosY());
        s1 = sin((1.0 - t) * M_PI_2);
        s2 = sin(t * M_PI_2);
        q2a.normalize();
        QuaternionQ result =  (p*s1 + q2a*s2);
        result.normalize();
        return result;

    }
    omega = acos(cosomega);
    sinomega = sin(omega);
    s1 = sin((1.0 - t) * omega) / sinomega;
    s2 = sin(t * omega) / sinomega;

    QuaternionQ result = (p*s1 + q*s2);
    result.normalize();

    return result;
}

QuaternionQ Interpolation::Squad(QuaternionQ q1, QuaternionQ q2, QuaternionQ s1, QuaternionQ s2, double t)
{

    QuaternionQ q11= SlerpQ(q1,s1,t),
               q12= SlerpQ(s1,s2,t),
               q13= SlerpQ(s2,q2,t);

    return Slerp(SlerpQ(q11,q12,t), SlerpQ(q12,q13,t),t);
}

QuaternionQ Double(QuaternionQ p,QuaternionQ q)
{
    return q*QuaternionQ::dot(p,q)*2 - p;
}

QuaternionQ Bissect(QuaternionQ p,QuaternionQ q)
{
    return (p+q)/(p+q).normal();
}


QuaternionQ Interpolation::KeyFramestoSquad(QList<Vec4> keys,int frame, double t)
{
    if(frame<=1){
        QuaternionQ q1(keys[frame-1].x1,keys[frame-1].x2,keys[frame-1].x3);
        QuaternionQ q2(keys[frame].x1,keys[frame].x2,keys[frame].x3);
        QuaternionQ q3(keys[frame+1].x1,keys[frame+1].x2,keys[frame+1].x3);
        QuaternionQ s1 = q1;
        QuaternionQ s11 = Bissect(Double(q1,q2),q3);
        QuaternionQ s2 = Double(s11,q2);
        return Squad(q1,q2,s1,s2,t);
    }
    if(frame>1 &&frame<=keys.size()-2){
    QuaternionQ q0(keys[frame-2].x1,keys[frame-2].x2,keys[frame-2].x3);
    QuaternionQ q1(keys[frame-1].x1,keys[frame-1].x2,keys[frame-1].x3);
    QuaternionQ q2(keys[frame].x1,keys[frame].x2,keys[frame].x3);
    QuaternionQ q3(keys[frame+1].x1,keys[frame+1].x2,keys[frame+1].x3);
    QuaternionQ s1 = Bissect(Double(q0,q1),q2);
    QuaternionQ s11 = Bissect(Double(q1,q2),q3);
    QuaternionQ s2 = Double(s11,q2);
    return Squad(q1,q2,s1,s2,t);
    }
    if(frame==keys.size()-1){
        QuaternionQ q0(keys[frame-2].x1,keys[frame-2].x2,keys[frame-2].x3);
        QuaternionQ q1(keys[frame-1].x1,keys[frame-1].x2,keys[frame-1].x3);
        QuaternionQ q2(keys[frame].x1,keys[frame].x2,keys[frame].x3);
        QuaternionQ s1 = Bissect(Double(q0,q1),q2);
        QuaternionQ s2 = q2;
        return Squad(q1,q2,s1,s2,t);
    }
}
