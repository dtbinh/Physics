#ifndef CHARACTER_H
#define CHARACTER_H
#include "physics/physics.h"
#include "control/balance.h"
#include "mocap/mocap.h"

#include <vector>
#include <QString>
#include "control/sensor.h"

class Object;
class Joint;
class Scene;
class ControlPD;
class Character
{
public:
    Character(Scene *scene);
    ~Character();
    Scene *scene;
    SpaceID space;
    JointGroupID jointGroup;
    std::vector<Object*> objects;
    std::vector<Joint*> joints;
    //control
    std::vector<ControlPD*> controllers;
    Balance* balance;
    MoCap* capMotion;
    float alpha;
    bool wire;
    bool shadow_motion;
public:
    //variaveis publicas
    std::vector<bool**> hierarchy; //variavel q guarda os relacionamentos das juntas de acordo com os corpos que
                                         //estão em contato com o solo

    void           setSpace(SpaceID space);
    SpaceID        getSpace();
    void           setJointGroup(JointGroupID jointgroup);
    JointGroupID   getJointGroup();
    Scene*         getScene();
    std::vector<Joint*> getJoints();
    std::vector<ControlPD*> getControllersPD();


    void           restartPhysics();  //reinicializa atributos físicos do personagem
    Object*        getObject(QString name); //extrai o nome do character
    Vec4           getPosCOM();                //extraí a posição do centro de massa
    Vec4           getVelCOM();                //extraí a velocidade linear no centro de massa
    Vec4           getLinearMomentum();        //extraí momento linear do character
    Vec4           getAngularMomentum();       //extraí momento angular do character
    Vec4           getVelAngular();            //extraí a velocidade angular do character
    float          getMassTotal();          //retorna a massa total do character
    int            getNumJoints();          //retorna a quantidade de juntas
    int            getNumBodies();          //retorna a quantidade de corpos
    int            getIdObject(Object *obj);
    Joint*         getParentMoreMass();
    Joint*         getJointParentBalance();
    Joint*         getJoint(int i);
    Object*         getBody(int i);
    int            getPositionBody(Object* obj);
    std::vector<Joint*> getJointChilds(Joint* exclude,Object* obj);
    std::vector<Object*> getBodiesFoot();
    bool           isBodyHierarchy(Joint* joint,Object* obj);
    bool           isJointHierarchy(Joint* joint,Joint *child);
    void           setBalance(Balance *balance); //adicionar controle de equilibrio
    void           updateKsKdControlPDCoros();
    Balance*       getBalance();
    Vec4           getSumTorqueGRF2COM();
    Vec4           getSumForceGRF2COM();
    void           checkContactFoot(bool b=false);
    MoCap*         getMoCap();
    void           contructHierarchyBodies();
    void           setHierarchyMap(int i);
    void           showHierarchies();
    void           loadMotionFrames();



    //desenho
    void           draw();
    void           drawCOM(); //desenha a posição do centro de massa
    void           setAlpha(float v);
    void           setWireframe(bool b);
    void           drawFootProjected();
    void           drawCOMProjected();
    void           drawShadowMotion(int frame);


};

#endif // CHARACTER_H
