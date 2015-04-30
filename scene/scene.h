#ifndef SCENE_H
#define SCENE_H
class Objects;
class GLWidget;
#include <vector>
#include "physics/physics.h"
#include "math/vec4.h"
#include "control/control.h"
#include "character.h"
#include "grf.h"
#include "extra/material.h"

#ifndef FEEDBACKCONTACT_H
#define FEEDBACKCONTACT_H



#endif

class Ray;
class Scene
{
private:
    //physics
    WorldID        world;
    SpaceID        space;
    JointGroupID   contactGroup;
    Vec4           gravity;        //gravidade
    bool           enableGravity;  //habilita o uso da gravidade
    int            sim_step;       //passo da simulação
    bool           sim_status;     //retorna se a simulação esta ativa
    int            frame_step;
    bool           status_motion;
    bool           show_grf;
    Vec4           viewer[3];  //guarda as posições da camera, eye, at, up
    Vec4           projection;

    //interface
    GLWidget *parent;
    //graphics
    std::vector<Object*> objects;
    std::vector<Object*> objects_shoot;
    std::vector<Joint*> joints;
    std::vector<Character*> characters;
    //manipulators
    Vec4 externalForce;
    Vec4 propKs;
    Vec4 propKd;
public:
    int            width,height;





public:
    std::vector<GRF> groundForces;
    Scene(GLWidget *parent);
    ~Scene();

    //Physics
    void           simulationStep(bool balance=true);                         //executa um passo da simulação
    void           restartPhysics();                                //reinicializa a simulação
    void           initPhysics();
    void           stopPhysics();
    void           startPhysics();
    SpaceID        getSpace();                               //retorna o espaço veinculado ao cenário
    void           setSpace(SpaceID space);                  //seta um espaço ao cenário
    WorldID        getWorld();                               //retorna o mundo veinculado ao cenário
    void           setWorld(WorldID world);                  //seta um mundo ao cenário
    JointGroupID   getJointGroup();                          //retorna o grupo de juntas veinculado ao cenário
    void           setJointGroup(JointGroupID contactGroup); //seta um grupo de juntas ao cenário

    //Parâmetros da Simulação
    void           setGravityParameters(Vec4 g);
    Vec4           getGravity();
    void           setGravity(bool b);
    bool           hasGravity();
    void           setSimStep(int sim_step);                 //adiciona a quantidade de passos da simulação
    int            getSimStep();                             //extrai o valor do passo da simulação
    std::vector<GRF> getGroundForces();
    void          addGroundForce(GRF grf);
    void          clearGroundForces();



    //Manipulação dos objetos do cenário
    void                  setViewer(Vec4 eye,Vec4 at,Vec4 up);
    void                  setProjection(Vec4 p);
    void                  setWindow(int width,int height);
    Object*               addObject(Vec4 properties, Vec4 position, Quaternion rotation,int type,float mass=1.0, Character *character=0,int material=MATERIAL_ZINN);//adiciona um objeto ao cenário
    Joint *               addJointBall(Vec4 anchor, Object *parent, Object *child, Character *chara,Vec4 limSup=Vec4(),Vec4 limInf=Vec4());//cria uma junta ball
    Joint *               addJointFixed(Object *parent,Object *child, Character *chara); //cria uma junta fixa
    std::vector<Object*>  objectsScene(); //retorna os objetos do cenário e dos characteres
    std::vector<Object*>  objectsSceneChara(); //retorna os objetos dos characteres
    Object*               selectedObject();
    std::vector<Joint*>   jointsScene(); //retorna as juntas do cenário
    Joint*                selectedJoint();
    void                  applyForce(Vec4 force);
    void                  clear();
    Object*               getObject(QString name);
    void                  addCharacter(Character* chara);
    void                  setExternalForce(Vec4 force);
    Vec4                  getExternalForce();
    void                  setProportionalKsPD(Vec4 ks);
    Vec4                  getProportionalKsPD();
    void                  setProportionalKdPD(Vec4 kd);
    Vec4                  getProportionalKdPD();
    Character*            getCharacter(int i);
    int                   getSizeCharacter();
    Object*               getObject(dBodyID id);
    //extra simulation
    void                  clearObjectShooted();
    void                  createRamp();
    void                  createCharacter();
    void                  startRecorder(bool b);


    //equilibrio do personagem
    void                  setEnableTorqueBalance(bool b);
    void                  setEnableForceBalance(bool b);
    void                  setEnableMomentumBalance(bool b);
    Vec4                  getKsForceBalance();
    void                  setKsForceBalance(Vec4 ks);
    Vec4                  getKdForceBalance();
    void                  setKdForceBalance(Vec4 kd);
    Vec4                  getKsTorqueBalance();
    void                  setKsTorqueBalance(Vec4 ks);
    Vec4                  getKdTorqueBalance();
    void                  setKdTorqueBalance(Vec4 kd);
    Vec4                  getKMomLinearBalance();
    void                  setKMomLinearBalance(Vec4 kmom);
    Vec4                  getKMomAngularBalance();
    void                  setKMomAngularBalance(Vec4 kmom);
    Vec4                  getKVelocityLocomotion();
    void                  setKVelocityLocomotion(Vec4 k);
    Vec4                  getKDistanceLocomotion();
    void                  setKDistanceLocomotion(Vec4 k);
    void                  setLimitSteps(int value);
    int                   getLimitSteps();

    void                  setCompensacao(int value);
    Object*               objectClicked(int width,int height);      //retorna o objeto selecionado no clique da tela
    Object*               getObject(Ray ray); //calcula a interseção do raio com o objeto


    void                  setAlphaCharacter(float val); //nao utilizado
    void                  setWireCharacter(bool b); //nao utilizado
    void                  setAngleBodyBalance(Vec4 v);
    //motion capture
    void                  restartMotionCapture();
    void                  statusMotionCapture(bool b);
    //testes de robustez
    void                  shotBallsCharacterRandom(Character *chara, int posPlevis, float den=100.); //atirar objetos no personagem de forma aleatória
    void                  shotBallsCharacterBody(Object* body, float velocity, float den=100.); //atirar objetos no personagem de acordo com a posição de um corpo, e com determinada velocidade
    void                  habiliteJump();
    //teste de contato
    bool                  isGeometryFootSwing(dGeomID geom);





    //Graphics
    void           draw(); //desenha os objetos e/ou juntas com OpenGL
    void           drawGRF(bool b); //desenha os objetos e/ou juntas com OpenGL
    void           drawShadows(); //desenha os objetos e/ou juntas com OpenGL
    void           loadSceneObjects();
    void           setRenderMesh(bool b);


};

#endif // SCENE_H
