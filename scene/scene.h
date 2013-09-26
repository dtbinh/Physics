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

#ifndef FEEDBACKCONTACT_H
#define FEEDBACKCONTACT_H



#endif


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
    //interface
    GLWidget *parent;
    //graphics
    std::vector<Object*> objects;
    std::vector<Object*> objects_shoot;
    std::vector<Character*> characters;
    //manipulators
    Vec4 externalForce;
    Vec4 propKs;
    Vec4 propKd;
    std::vector<GRF> groundForces;




public:

    Scene(GLWidget *parent);
    ~Scene();

    //Physics
    void           simulationStep();                         //executa um passo da simulação
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
    void           setSimStep(int sim_step);                 //adiciona a quantidade de passos da simulação
    int            getSimStep();                             //extrai o valor do passo da simulação
    std::vector<GRF> getGroundForces();
    void          addGroundForce(GRF grf);
    void          clearGroundForces();



    //Manipulação dos objetos do cenário
    Object*               addObject(Vec4 properties, Vec4 position, Quaternion rotation,int type,float mass=1.0, Character *character=0);//adiciona um objeto ao cenário
    Joint *               addJointBall(Vec4 anchor, Object *parent, Object *child, Character *chara,Vec4 limSup=Vec4(),Vec4 limInf=Vec4());//cria uma junta ball
    Joint *               addJointFixed(Object *parent,Character *chara); //cria uma junta fixa
    std::vector<Object*>  objectsScene(); //retorna os objetos do cenário e dos characteres
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
    //extra simulation
    void                  shootObject(Object* obj,int type, Vec4 begin);
    void                  clearObjectShooted();


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
    Vec4                  getKMomBalance();
    void                  setKMomBalance(Vec4 kmom);
    void                  setCompensacao(int value);

    void                  setAlphaCharacter(float val); //nao utilizado
    void                  setWireCharacter(bool b); //nao utilizado
    void                  setAngleBodyBalance(Vec4 v);
    //motion capture
    void                  restartMotionCapture();
    void                  statusMotionCapture(bool b);




    //Graphics
    void           draw(); //desenha os objetos e/ou juntas com OpenGL

};

#endif // SCENE_H
