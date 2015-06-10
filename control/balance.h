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
    MatrixF     relationJoints;
    MatrixF     relationJointsB;
    Vec4       ksTorque;
    Vec4       kdTorque;
    Vec4       ksForce;
    Vec4       kdForce;
    Vec4       kDist;    //parâmetros da distância na locomoção
    Vec4       kVel;     //parâmetros da velocidade de locomoção
    Vec4       velAnt;

    Vec4       kmomlin;
    Vec4       kmomang;
    QuaternionQ bdesired;

    float      compensation;
    bool       enable_force;
    bool       enable_torque;
    bool       enable_momentum;
    bool       enable_gravitycomp;
    bool       enable_balance;
    int        useHierarchy;
    int        steps;
    int        limitsteps;
    //estratégia de cone de fricção

    float angle;     //angulo do cone
    float height;    //altura do pé
    float radius;    //raio do pé
    float m;         //modulo da força
    float limit;     //limite de angulo
    bool jump;
    float init_jump;
    float min_jump;
    float max_jump;
    float sensor_tolerance;
    float grav_comp;



public:
    double sim_dist;
    double mocap_dist;
    Balance(Character* chara);
    void contructRelationJointsBodies(); //esta função constrói uma matrix de relacionamento entre as juntas e os corpos da
                                         //hierarquia, Mat(n,m), onde n é o numero de juntas e m o número de corpos, considerando
                                         //os corpos filhos.
    void contructRelationJoints();       //esta função constrói uma matrix de relacionamento entre as juntas e sua hierarquia
                                         //hierarquia, Mat(n,n), onde n é o numero de juntas.

    MatrixF getJacobianSum(Object* obj);  //calcula o somatório de todas as jacobianas
    MatrixF getInertiaFactors(Joint* joint);
    Vec getTwistWrenchTotal(Vec twist,Vec4 com); //retorna os valores que serão aplicados no character com forças e torques virtuais
    Vec getJacobianLocomotion(std::vector<Joint*> joints, Object* effector, Vec twist);
    Vec4 getKsTorque();
    void setKsTorque(Vec4 kst);
    Vec4 getKdTorque();
    void setKdTorque(Vec4 kdt);
    Vec4 getKsForce();
    void setKsForce(Vec4 ksf);
    Vec4 getKdForce();
    void setKdForce(Vec4 kdf);
    void habiliteJump(bool b);

    Vec4 getKMomentumLinear();
    Vec4 getKMomentumAngular();

    QuaternionQ getDesiredQuaternion();

    void setDeriredQuaternion(Vec4 euler);
    void setDeriredQuaternion(QuaternionQ quat);
    void setKMomentumLinear(Vec4 k);
    void setKMomentumAngular(Vec4 k);
    float getCompensation();
    void setCompensation(float f);
    //suavidade
    void  setStepsInterpolation(float limit);
    float getStepsInterpolation();

    //compensação da gravidade
    void  setCompensationGravity(double val); //val [0,1]
    float getCompensationGravity(); //val [0,1]

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
    //discretização do deslocamento do com
    void setLimitSteps(int value);
    int getLimitSteps();
    //tolerancia do sensor
    void setSensorTolerance(float value);
    float getSensorTolerance();

    //constantes do Simbicon
    Vec4 getKVelocityLocomotion();
    void setKVelocityLocomotion(Vec4 k);
    Vec4 getKDistanceLocomotion();
    void setKDistanceLocomotion(Vec4 k);


    float getTorqueMaxCompensable(Object* foot,Vec4 torque);

    void setEnableTorque(bool b);
    bool getEnableTorque();
    void setEnableForce(bool b);
    bool getEnableForce();
    void setEnableGravityCompensation(bool b);
    bool getEnableGravityCompensation();
    void setEnableMomentum(bool b);
    bool getEnableMomentum();
    Vec4 limitingTorque(Vec4 lim_inf,Vec4 lim_sup, Vec4 torque);
    Vec4 limitingTorque(float x, Vec4 torque);
    void evaluate(Joint* jDes,float mass_total,int frame = -1,QuaternionQ qdesired=QuaternionQ(),Vec4 vel_ang_des=Vec4(), Vec4 velCOM_moCap=Vec4(),Vec4 mom_lin_des=Vec4(),Vec4 mom_ang_des=Vec4()); //executa a rotina de tratamento do equilíbrio
    void evaluateSIMBICON();
    void setEnableBalance(bool b);
};

#endif // BALANCE_H
