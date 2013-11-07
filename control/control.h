#ifndef CONTROL_H
#define CONTROL_H

#include "math/vec4.h"
#include "math/quaternion.h"
#include "scene/joint.h"

class ControlPD
{
private:
    Joint*     joint;
    Quaternion qwanted;
    Vec4       ks;         //constante linear
    Vec4       kd;         //constante derivada
    Vec4       propKs;
    Vec4       propKd;
    bool       enabled;

public:
    ControlPD(Joint *joint,Quaternion qwanted,Vec4 ks,Vec4 kd);
    void setJoint(Joint *joint);
    Joint* getJoint();
    void setQuaternionWanted(Quaternion qwanted);
    Quaternion getQuaternionWanted();
    void setKs(Vec4 ks);
    Vec4 getKs();
    void setKd(Vec4 kd);
    Vec4 getKd();
    void setEnabled(bool enabled);
    bool isEnabled();
    void evaluate();
    void updateKsKdCoros(float MassTotal);
    void setProportionalKs(Vec4 pks);
    void setProportionalKd(Vec4 pkd);

    Vec4 getProportionalKs();
    Vec4 getProportionalKd();
    Vec4 limitingTorquePD(Vec4 tq);
    static Vec4 limitingTorquePD(Vec4 tq,Joint* j);

    static Vec4 getTorquePD(Joint* joint,Vec4 ks, Vec4 kd,Quaternion qDesired=Quaternion());
    static Vec4 getTorquePDCOM(Joint* joint,Vec4 ks, Vec4 kd,Quaternion qDesired=Quaternion(Vec4(0,0,90)),Vec4 velocity=Vec4());

    static float grauToRad2(float angle);
    static dReal limitaValor(dReal lim, dReal valor);

};

#endif // CONTROL_H
