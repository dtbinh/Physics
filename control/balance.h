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
    Vec4       kmom;
    Quaternion bdesired;
    float      compensation;
    bool       enable_force;
    bool       enable_torque;
    bool       enable_momentum;
    bool       enable_balance;
    int        useHierarchy;

public:
    Balance(Character* chara);
    void contructRelationJointsBodies(); //esta função constrói uma matrix de relacionamento entre as juntas e os corpos da
                                         //hierarquia, Mat(n,m), onde n é o numero de juntas e m o número de corpos, considerando
                                         //os corpos filhos.
    void contructRelationJoints();       //esta função constrói uma matrix de relacionamento entre as juntas e sua hierarquia
                                         //hierarquia, Mat(n,n), onde n é o numero de juntas.

    Matrix getJacobianSum(Object* obj);  //calcula o somatório de todas as jacobianas
    Vec getTwistWrenchTotal(Vec twist); //retorna os valores que serão aplicados no character com forças e torques virtuais
    Vec4 getKsTorque();
    void setKsTorque(Vec4 kst);
    Vec4 getKdTorque();
    void setKdTorque(Vec4 kdt);
    Vec4 getKsForce();
    void setKsForce(Vec4 ksf);
    Vec4 getKdForce();
    void setKdForce(Vec4 kdf);
    Vec4 getKMomentum();
    Vec4 getDesiredQuaternion();
    void setDeriredQuaternion(Vec4 euler);
    void setKMomentum(Vec4 k);
    float getCompensation();
    void setCompensation(float f);

    void setEnableTorque(bool b);
    void setEnableForce(bool b);
    void setEnableMomentum(bool b);
    Vec4 limitingTorque(Vec4 lim_inf,Vec4 lim_sup, Vec4 torque);
    void evaluate(); //executa a rotina de tratamento do equilíbrio
    void setEnableBalance(bool b);
};

#endif // BALANCE_H
