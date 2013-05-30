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

};

#endif // CONTROL_H
