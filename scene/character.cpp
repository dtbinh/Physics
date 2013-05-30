#include "character.h"
#include "control/control.h"
#include "object.h"
#include "joint.h"
#include "scene.h"

Character::Character(Scene *parent)
{
    this->scene = parent;
    Physics::initCharacter(this);
}

Character::~Character(){


    while(!objects.empty()){
        delete objects.back();
        objects.pop_back();
    }

    while(!joints.empty()){
        delete joints.back();
        joints.pop_back();
    }

    Physics::closeCharacter(this);
}

void Character::draw()
{
    for(std::vector<Object*>::iterator it=objects.begin(); it!=objects.end(); it++){
        (*it)->draw();
    }
    for(std::vector<Joint*>::iterator it=joints.begin(); it!=joints.end(); it++){
        (*it)->draw();
    }
}

void Character::restartPhysics()
{
    Physics::initCharacter(this);
    for(int i=0;i<objects.size();i++){
        Physics::createObject(objects.at(i),this->getSpace(),objects.at(i)->getFMass(),objects.at(i)->getPosition(),objects.at(i)->getRotation());
    }
    for(int i=0;i<joints.size();i++){
        joints.at(i)->restartJoint();
    }
}

void Character::setSpace(SpaceID space)
{
    this->space = space;
}

void Character::setJointGroup(JointGroupID jointgroup)
{
    this->jointGroup = jointgroup;
}

Scene* Character::getScene()
{
    return this->scene;
}

std::vector<Joint*> Character::getJoints()
{
    return this->joints;
}

std::vector<ControlPD*> Character::getControllersPD()
{
    return this->controllers;
}

SpaceID Character::getSpace()
{
    return this->space;
}

JointGroupID Character::getJointGroup()
{
    return this->jointGroup;
}
