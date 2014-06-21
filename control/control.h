#ifndef CONTROL_H
#define CONTROL_H

#include "math/vec4.h"
#include "math/quaternion.h"
#include "scene/joint.h"
#include "math/matrix.h"
/*
  Essa classe está anexada a cada junta presente no personagem, ou pode ser
  chamada externamente. Tem como função fazer com que a configuração de
  orientação entre dois corpos seja alcançada.


  */
class ControlPD
{
private:
    Joint*     joint;
    Quaternion qwanted;
    Vec4       ks;             //constante linear
    Vec4       kd;             //constante derivada
    Vec4       propKs;         //constante linear proporcional setada via classe Scene
    Vec4       propKd;         //constante derivada proporcional setada via classe Scene
    bool       enabled;        //habilita o controle de PD da junta
    Matrix     inertia;        //matriz de inercia
    Vec4       velDesired;     //velocidade linear desejada
    bool       enable_inertia; //habilita ou não o fator de inercia da junta

public:
    void setInertiaFactors(Matrix i);
    void resetInertiaFactors();
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
    void setEnabledInertia(bool enabled_inertia);
    bool isEnabledInertia();
    void evaluate();
    void updateKsKdCoros(float MassTotal);
    void setProportionalKs(Vec4 pks);
    void setProportionalKd(Vec4 pkd);
    void setVelocityDesired(Vec4 vel);
    Quaternion getOrientation();
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
