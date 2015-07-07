#ifndef PHYSICS_H
#define PHYSICS_H
//#define dDOUBLE
#include <ode/ode.h>
#include <ode/precision.h>
#include <ode/odecpp_collision.h>
#include "functions.h"

#define BodyID dBodyID
#define GeomID dGeomID
#define WorldID dWorldID
#define SpaceID dSpaceID
#define Mass dMass
#define JointID dJointID
#define JointGroupID dJointGroupID
//#define JointFeedback dJointFeedback

class Matrix4x4;
class Vec4;
class Quaternion;
class Scene;
class Object;
class Character;
class Joint;
class MatrixF;

namespace Physics{
    //inicializadores do ode
    void nearCallback(void *data, GeomID o1, GeomID o2);
    void simSingleStep(Scene *scene);
    void worldStep(WorldID world, float stepSize);
    void setGravity(Scene *scene, Vec4 g);



    //Body Manipulation
    void                  getGeomTransform(dGeomID geom, Matrix4x4* transform); //extrai a matriz de transformação do objeto (Rotation x Translate)
    void                  createObject(Object *obj, dSpaceID space, float mass, Vec4 position, QuaternionQ rotation); //cria um objeto físico
    void                  createObject(Object *obj, dSpaceID space, float mass, Vec4 position, Vec4 Velocity); //cria um objeto físico com velocidade
    QuaternionQ            getRotationBody(Object *obj);               //extrai o quaternion de rotação do objeto
    Vec4                  getPositionBody(GeomID g);               //extrai a posição do objeto

    void                  setEnableObject(Object *obj);               //habilita o objeto para ser manipulado no mundo
    void                  setDisableObject(Object *obj);              //desabilita o objeto para ser manipulado no mundo
    void                  updateObject(Object *obj); //decrepted
    MatrixF getMatrixRotation(Object *obj); //extrai a matriz de rotação como instancia da classe Matrix


    void                  bodySetTorque(BodyID body, float x, float y, float z);
    void                  bodyAddTorque(BodyID body, float x, float y, float z);
    void                  bodySetForce(BodyID body, float x, float y, float z);
    void                  bodyAddForce(BodyID body, float x, float y, float z);
    void                  setPositionBody(Object *body,Vec4 pos);
    void                  setRotationBody(Object *body, QuaternionQ quat);
    Vec4                  getAngularVelBody(Object *obj);            //extrai a velocidade angular do objeto em coordenadas globais
    Vec4                  getAngularMomentumBody(Object *obj);            //extrai a velocidade angular do objeto em coordenadas globais
    Vec4                  getAngularMomentumMoCap(Object* obj,Vec4 vel,QuaternionQ q);
    Vec4                  getLinearVelBody(Object *obj);            //extrai a velocidade angular do objeto em coordenadas globais
    Vec4                  getRelVelocityBody(Object *obj);           //extrai a velocidade do objeto em coordenadas globais
    Vec4                  getRelPositionBody(Object *obj);           //extraí a posição do objeto em coordenadas globais

    //Joint Manipulation
    void                  initJointBall(Joint* joint, Vec4 anchor);  //criar junta ball
    Vec4                  getAnchorJoint(Joint *joint);
    void                  initJointFixed(Joint*joint);               //criar junta fixed
    void                  initJointHinge(Joint* joint, Vec4 anchor, Vec4 axis); //criar junta hinge
    Vec4                  getAxisHingeJoint(Joint *joint);           //retorna o eixo da junta hinge
    QuaternionQ getRotationJoint(Joint* joint);            //retorna o quaternion entre os corpos da junta
    QuaternionQ           getRotationJointInit(Joint* joint);            //retorna o quaternion entre os corpos da junta
    void                  closeJoint(Joint* joint);                  //fecha a junta
    void                  setEnableJoint(Joint *joint);              //habilita a junta para ser manipulada no mundo
    void                  setDisableJoint(Joint *joint);             //desabilita a junta para ser manipulada no mundo
    Vec4                  getJointBallAnchor( Joint* joint );        //retorna a posição atual onde está ancorada a junta


    //Must use
    void setSceneInUse(Scene *scene);
    void initScene(Scene *scene);
    void closeScene(Scene *scene);
    void initCharacter(Character *chara);
    void closeCharacter(Character *chara);
    void closeObject(Object *obj);
    void closePlane(dGeomID plane);
    dGeomID initPlane(Vec4 plane,Scene* scene);

}

#endif // PHYSICS_H
