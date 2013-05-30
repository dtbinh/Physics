#ifndef SCENE_H
#define SCENE_H
class Objects;
class GLWidget;
#include <vector>
#include "physics/physics.h"
#include "math/vec4.h"
#include "control/control.h"
#include "character.h"
class Scene
{
private:
    //physics
    WorldID        world;
    SpaceID        space;
    JointGroupID   contactGroup;
    int            sim_step;       //passo da simulação
    //interface
    GLWidget *parent;
    //graphics
    std::vector<Object*> objects;
    std::vector<Character*> characters;
    //manipulators
    Vec4 externalForce;


public:

    Scene(GLWidget *parent);
    ~Scene();

    //Physics
    void           simulationStep();                         //executa um passo da simulação
    void           restartPhysics();                                //reinicializa a simulação
    SpaceID        getSpace();                               //retorna o espaço veinculado ao cenário
    void           setSpace(SpaceID space);                  //seta um espaço ao cenário
    WorldID        getWorld();                               //retorna o mundo veinculado ao cenário
    void           setWorld(WorldID world);                  //seta um mundo ao cenário
    JointGroupID   getJointGroup();                          //retorna o grupo de juntas veinculado ao cenário
    void           setJointGroup(JointGroupID contactGroup); //seta um grupo de juntas ao cenário

    //Parâmetros da Simulação
    void           setSimStep(int sim_step);                 //adiciona a quantidade de passos da simulação
    int            getSimStep();                             //extrai o valor do passo da simulação



    //Manipulação dos objetos do cenário
    Object*               addObject(Vec4 properties, Vec4 position, Quaternion rotation,int type,float mass=1.0, Character *character=0);//adiciona um objeto ao cenário
    Joint *               addJointBall(Vec4 anchor, Object *parent, Object *child, Character *chara,Vec4 limSup=Vec4(),Vec4 limInf=Vec4());//cria uma junta ball
    Joint *               addJointFixed(Object *parent,Character *chara); //cria uma junta fixa
    std::vector<Object*>  objectsScene(); //retorna os objetos do cenário e dos characteres
    Object*               selectedObject();
    std::vector<Joint*>   jointsScene(); //retorna as juntas do cenário
    Joint*                selectedJoint();
    void                  applyForce(Vec4 force);


    //Graphics
    void           draw(); //desenha os objetos e/ou juntas com OpenGL

};

#endif // SCENE_H
