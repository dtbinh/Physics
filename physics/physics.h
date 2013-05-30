#ifndef PHYSICS_H
#define PHYSICS_H
#define dDOUBLE
#include <ode/ode.h>

#define BodyID dBodyID
#define GeomID dGeomID
#define WorldID dWorldID
#define SpaceID dSpaceID
#define Mass dMass
#define JointID dJointID
#define JointGroupID dJointGroupID

class Matrix4x4;
class Vec4;
class Quaternion;
class Scene;
class Object;
class Character;
class Joint;

namespace Physics{

    //inicializadores do ode
    void nearCallback(void *data, dGeomID o1, dGeomID o2);
    void simSingleStep(Scene *scene);
    void worldStep(WorldID world, float stepSize);



    //Body Manipulation
    void                  getGeomTransform(GeomID geom, Matrix4x4* transform); //extrai a matriz de transformação do objeto (Rotation x Translate)
    void                  createObject(Object *obj, dSpaceID space, float mass, Vec4 position, Quaternion rotation); //cria um objeto físico
    Quaternion            getRotationBody(GeomID geom);               //extrai o quaternion de rotação do objeto
    Vec4                  getPositionBody(GeomID geom);               //extrai a posição do objeto

    void                  setEnableObject(Object *obj);               //habilita o objeto para ser manipulado no mundo
    void                  setDisableObject(Object *obj);              //desabilita o objeto para ser manipulado no mundo


    void                  bodySetTorque(BodyID body, float x, float y, float z);
    void                  bodyAddTorque(BodyID body, float x, float y, float z);
    void                  bodySetForce(BodyID body, float x, float y, float z);
    void                  bodyAddForce(BodyID body, float x, float y, float z);
    Vec4                  getAngularVelBody(Object *obj);            //extrai a velocidade angular do corpo

    //Joint Manipulation
    void                  initJointBall(Joint* joint, Vec4 anchor);  //criar junta ball
    void                  initJointFixed(Joint*joint);               //criar junta fixed
    Quaternion            getRotationJoint(Joint* joint);            //retorna o quaternion entre os corpos da junta
    void                  closeJoint(Joint* joint);                  //fecha a junta
    void                  setEnableJoint(Joint *joint);              //habilita a junta para ser manipulada no mundo
    void                  setDisableJoint(Joint *joint);             //desabilita a junta para ser manipulada no mundo
    Vec4                  getJointBallAnchor( Joint* joint );        //retorna a posição atual onde está ancorada a junta


    //Must use
    void initScene(Scene *scene);
    void closeScene(Scene *scene);
    void initCharacter(Character *chara);
    void closeCharacter(Character *chara);

}

#endif // PHYSICS_H
