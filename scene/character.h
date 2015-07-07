#ifndef CHARACTER_H
#define CHARACTER_H
#include "physics/physics.h"
#include "control/balance.h"
#include "mocap/mocap.h"

#include <vector>
#include <QString>
#include "control/sensor.h"
#include "scene/grf.h"

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
    Vec4 offset;
    std::vector<Object*> objects;
    std::vector<Joint*> joints;
    //control
    std::vector<ControlPD*> controllers;
    Balance* balance;
    MoCap* capMotion;
    float alpha;
    bool wire;
    bool shadow_motion;
    QString name;
    bool has_suitcase;

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
    Object*        getObject(QString name); //extrai um objeto a partir de um nome
    Vec4           getPosCOM();                //extraí a posição do centro de massa
    Vec4           getVelCOM();                //extraí a velocidade linear no centro de massa
    Vec4           getLinearMomentum();        //extraí momento linear do character
    Vec4           getAngularMomentum();       //extraí momento angular do character
    Vec4           getVelAngular();            //extraí a velocidade angular do character
    float          getMassTotal();          //retorna a massa total do character
    int            getNumJoints();          //retorna a quantidade de juntas
    int            getNumBodies();          //retorna a quantidade de corpos
    int            getIdObject(Object *obj);
    int            getIdJoint(Joint *joint);
    Joint*         getParentMoreMass();
    Joint*         getJointParentBalance();
    Joint*         getJoint(int i);
    Joint*         getJoint(Object *parent, Object *child);
    Joint*         getJoint2ObjectParent(Object* obj); //retorna a junta cujo o objeto passado como parâmetro seja seu parent
    Object*         getBody(int i);
    int            getPositionBody(Object* obj);
    std::vector<Joint*> getJointChilds(Joint* exclude,Object* obj);
    std::vector<Object*> getBodiesFoot(Object *nofoot=NULL);
    std::vector<Object*> getChildrens(Object* obj);
    bool           isBodyHierarchy(Joint* joint,Object* obj);
    //bool           isJointHierarchy(Joint* joint,Joint *child);
    void           setBalance(Balance *balance); //adicionar controle de equilibrio
    void           updateKsKdControlPDCoros();
    Balance*       getBalance();
    Vec4           getSumTorqueGRF2COM();
    Vec4           getSumForceGRF2COM();
    void           checkContactFoot(bool b=true);
    MoCap*         getMoCap();
    void           contructHierarchyBodies();
    void           setHierarchyMap(int i);
    void           showHierarchies();
    void           restartCollideWithGround();
    void           loadMotionFrames();
    void           clearVectorsGlobais();
    std::vector<Joint*> getHierarchyJoint(Object* begin, Object* end);
    bool           checkHierarchy(Joint *joint, Object* at);
    Vec4           getGRFSum(Object *obj);
    std::vector<GRF> getGRFsObject(Object *obj);
    void setKsRetaionshipKd();
    void setOffset(Vec4 offset);
    Object*        getObjectSelected();

    Vec4 getOffset();
    int             findJointIndex(Joint* searchJoint); //encontra o indice de uma junta caso ela exista no personagem
    ControlPD*      getController(int i);
    void            setSuitcase(int body,float mass);
    void            deleteSuitcase();
    void            isFall(bool b);



    QString         getName();
    void            setName(QString name);


    //desenho
    void           draw();
    void           drawPreShadows();
    void           drawShadows();
    void           drawCOM(); //desenha a posição do centro de massa
    void           drawMoCap(bool b);
    void           setAlpha(float v);
    void           setWireframe(bool b);
    void           drawFootProjected();
    void           drawCOMProjected();
    void           drawShadowMotion(int frame);

    //debug
    void           showHierarchyBodies(Object* begin, Object* end);


    bool           hasEffectorEnabled();
};

#endif // CHARACTER_H
