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
    QuaternionQ qwanted;
    Vec4       ks;             //constante linear
    Vec4       kd;             //constante derivada
    Vec4       ks_fall;        //constante ks ao cair
    Vec4       kd_fall;        //constante kd ao cair
    Vec4       propKs;         //constante linear proporcional setada via classe Scene
    Vec4       propKd;         //constante derivada proporcional setada via classe Scene
    bool       enabled;        //habilita o controle de PD da junta
    MatrixF     inertia;        //matriz de inercia
    Vec4       velDesired;     //velocidade linear desejada
    bool       enable_inertia; //habilita ou não o fator de inercia da junta
    bool       is_fall; //personagem esta caindo
    Vec4       euler;

public:
    void setFall(bool b);
    bool isFall();
    void setKsFall(Vec4 ks_f);
    Vec4 getKsFall();
    void setKdFall(Vec4 kd_f);
    Vec4 getKdFall();

    void setInertiaFactors(MatrixF i);
    void resetInertiaFactors();
    ControlPD(Joint *joint,QuaternionQ qwanted,Vec4 ks,Vec4 kd);
    void setJoint(Joint *joint);
    Joint* getJoint();
    void setQuaternionWanted(QuaternionQ qwanted);
    QuaternionQ getQuaternionWanted();
    void setKs(Vec4 ks);
    Vec4 getKs();
    void setKd(Vec4 kd);
    Vec4 getKd();
    void setEnabled(bool enabled);
    bool isEnabled();
    void setEnabledInertia(bool enabled_inertia);
    bool isEnabledInertia();
    //algulos de euler
    void setEuler(Vec4 e);
    Vec4 getEuler();


    void evaluate();
    void updateKsKdCoros(float MassTotal);
    void setProportionalKs(Vec4 pks);
    void setProportionalKd(Vec4 pkd);
    void setVelocityDesired(Vec4 vel);
    QuaternionQ getOrientation();
    Vec4 getProportionalKs();
    Vec4 getProportionalKd();
    Vec4 limitingTorquePD(Vec4 tq);
    static Vec4 limitingTorquePD(Vec4 tq,Joint* j);

    static Vec4 getTorquePD(Joint* joint,Vec4 ks, Vec4 kd,QuaternionQ qDesired=QuaternionQ());
    static Vec4 getTorquePDCOM(Joint* joint,Vec4 ks, Vec4 kd,QuaternionQ qDesired=QuaternionQ(Vec4(0,0,90)),Vec4 velocity=Vec4());

    static float grauToRad2(float angle);
    static dReal limitaValor(dReal lim, dReal valor);

};

#endif // CONTROL_H
