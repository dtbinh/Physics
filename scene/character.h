#ifndef CHARACTER_H
#define CHARACTER_H
#include "physics/physics.h"
#include <vector>

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

public:

    void           setSpace(SpaceID space);
    SpaceID        getSpace();
    void           setJointGroup(JointGroupID jointgroup);
    JointGroupID   getJointGroup();
    Scene*         getScene();
    std::vector<Joint*> getJoints();
    std::vector<ControlPD*> getControllersPD();

    void           draw();
    void           restartPhysics();  //reinicializa atributos físicos do personagem
};

#endif // CHARACTER_H
