#include "joint.h"
#include "math/vec4.h"
#include "math/quaternion.h"
#include "extra/material.h"
#include "object.h"
#include "character.h"
#include "graphics/draw.h"

Joint::Joint(Character *chara,int type)
{
    this->character = chara;
    chara->joints.push_back(this);
    this->type = type;
    material = new Material();
    this->selected = false;
    //this->parent = new Object();
    //this->child = new Object();
    //transform = new Matrix4f();
    //transform->setIdentity();
    initialAnchor = new Vec4();
}

Joint::Joint(dJointID joint, Character *chara, Object *parent, Object *child, int type, Vec4 limSup, Vec4 limInf)
{
    this->joint = joint;
    this->parent = parent;
    this->child  = child;
    this->type   = type;
    this->limSup = limSup;
    this->limInf = limInf;
    this->character = chara;
    this->initialAnchor = new Vec4();
    this->selected = false;
    chara->joints.push_back(this);
}

Joint::~Joint()
{
    delete this->material;
    delete this->initialAnchor;
    Physics::closeJoint(this);
}

void Joint::initJoint(Vec4 anchor)
{
    this->initialAnchor->setVec4(anchor);
    switch (this->type){
        case JOINT_FIXED:{
        Physics::initJointFixed(this);
        this->name = "Fixed";
        break;
    }
    case JOINT_HINGE:{

        break;
    }
    case JOINT_BALL:{
        Physics::initJointBall(this,anchor);
        this->name = "Ball";
        break;
    }
    }


}

void Joint::setJoint(JointID joint)
{
    this->joint = joint;
}

JointID Joint::getJoint()
{
    return this->joint;
}

void Joint::setParent(Object *parent)
{
    this->parent = parent;
}

Object* Joint::getParent()
{
    return this->parent;
}

void Joint::setChild(Object *child)
{
    this->child = child;
}

Object* Joint::getChild()
{
    return this->child;
}

Character *Joint::getCharacter()
{
    return this->character;
}

void Joint::restartJoint()
{
    //Physics::closeJoint(this);
    //this->initialAnchor->setVec4(anchor);
    Vec4 anchor = Vec4(this->initialAnchor->x(),this->initialAnchor->y(),this->initialAnchor->z());
    switch (this->type){
        case JOINT_FIXED:{
        Physics::initJointFixed(this);
        break;
    }
    case JOINT_HINGE:{

        break;
    }
    case JOINT_BALL:{
        Physics::initJointBall(this,anchor);
        break;
    }
    }
}

void Joint::setSelected(bool selected)
{
    this->selected = selected;
}

bool Joint::isSelected()
{
    return this->selected;
}

void Joint::setName(QString name)
{
    this->name = name;
}

QString Joint::getName()
{
    return this->name;
}

void Joint::draw()
{
    //if(this->initialAnchor==Vec4()) return;
    switch (this->type){
    case (JOINT_HINGE):{
        break;
    }
    case (JOINT_BALL):{
        Vec4 position = Physics::getJointBallAnchor(this);
        Draw::drawSphere(position);
        break;
    }
    case (JOINT_FIXED):{
        break;
    }
    }
}
