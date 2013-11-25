#ifndef BALANCE_H
#define BALANCE_H
#include "math/matrix.h"
#include "math/quaternion.h"
class Character;
class Joint;
class Object;
class Vec;


class Balance
{
    Character* chara;
    Matrix     relationJoints;
    Matrix     relationJointsB;
    Vec4       ksTorque;
    Vec4       kdTorque;
    Vec4       ksForce;
    Vec4       kdForce;
    Vec4       kDist;    //parâmetros da distância na locomoção
    Vec4       kVel;     //parâmetros da velocidade de locomoção

    Vec4       kmomlin;
    Vec4       kmomang;
    Quaternion bdesired;
    float      compensation;
    bool       enable_force;
    bool       enable_torque;
    bool       enable_momentum;
    bool       enable_balance;
    int        useHierarchy;
    //estratégia de cone de fricção
    float angle;
    float height;
    float radius;
    float m;
    float limit;

public:
    Balance(Character* chara);
    void contructRelationJointsBodies(); //esta função constrói uma matrix de relacionamento entre as juntas e os corpos da
                                         //hierarquia, Mat(n,m), onde n é o numero de juntas e m o número de corpos, considerando
                                         //os corpos filhos.
    void contructRelationJoints();       //esta função constrói uma matrix de relacionamento entre as juntas e sua hierarquia
                                         //hierarquia, Mat(n,n), onde n é o numero de juntas.

    Matrix getJacobianSum(Object* obj);  //calcula o somatório de todas as jacobianas
    Matrix getInertiaFactors(Joint* joint);
    Vec getTwistWrenchTotal(Vec twist); //retorna os valores que serão aplicados no character com forças e torques virtuais
    Vec getJacobianLocomotion(std::vector<Joint*> joints, Object* effector, Vec twist);
    Vec4 getKsTorque();
    void setKsTorque(Vec4 kst);
    Vec4 getKdTorque();
    void setKdTorque(Vec4 kdt);
    Vec4 getKsForce();
    void setKsForce(Vec4 ksf);
    Vec4 getKdForce();
    void setKdForce(Vec4 kdf);
    Vec4 getKVelocityLocomotion();
    void setKVelocityLocomotion(Vec4 k);
    Vec4 getKDistanceLocomotion();
    void setKDistanceLocomotion(Vec4 k);
    Vec4 getKMomentumLinear();
    Vec4 getKMomentumAngular();
    Quaternion getDesiredQuaternion();

    void setDeriredQuaternion(Vec4 euler);
    void setDeriredQuaternion(Quaternion quat);
    void setKMomentumLinear(Vec4 k);
    void setKMomentumAngular(Vec4 k);
    float getCompensation();
    void setCompensation(float f);
    //cone de ficção
    void setLimitCone(float v);
    float getLimitCone();
    float getMCone();
    void setMCone(float val);
    float getRadiusCone();
    void setRadiusCone(float val);
    float getHeightCone();
    void setHeightCone(float val);
    float getAngleCone();
    void setAngleCone(float val);


    float getTorqueMaxCompensable(Object* foot,Vec4 torque);

    void setEnableTorque(bool b);
    bool getEnableTorque();
    void setEnableForce(bool b);
    bool getEnableForce();
    void setEnableMomentum(bool b);
    bool getEnableMomentum();
    Vec4 limitingTorque(Vec4 lim_inf,Vec4 lim_sup, Vec4 torque);
    void evaluate(); //executa a rotina de tratamento do equilíbrio
    void setEnableBalance(bool b);
};

#endif // BALANCE_H
