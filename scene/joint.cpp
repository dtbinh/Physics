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

Joint::Joint(int type)
{
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
    this->limSupA = limSup;
    this->limInfA = limInf;
    this->character = chara;
    this->initialAnchor = new Vec4();
    this->selected = false;
    chara->joints.push_back(this);
}

void Joint::setLimitsAngularSuperior(Vec4 sup)
{
    this->limSupA = sup;
}

void Joint::setLimitsAngularInferior(Vec4 inf)
{
    this->limInfA = inf;
}

Vec4 Joint::getTorqueMax()
{
    return this->tqMax;
}

Vec4 Joint::getLimitAngularMax()
{
    return this->limSupA;
}

Vec4 Joint::getLimitAngularMin()
{
    return this->limInfA;
}

Vec4 Joint::getPositionAnchorInit()
{
    return Vec4(initialAnchor->x(),initialAnchor->y(),initialAnchor->z());
}

Vec4 Joint::getPositionAnchor()
{
    return Physics::getAnchorJoint(this);
}



Joint::~Joint()
{
    delete this->material;
    delete this->initialAnchor;
    Physics::closeJoint(this);
}

Matrix Joint::getAd()
{
    Matrix Ad(6,6);
    //R
    Matrix R(3,3);
    //matriz de rotacao da junta em relacao ao frame global (assume-se que o frame da junta eh o mesmo que o frame global - em relacao a rotacao)
    R = R.identity();
    Vec p = Vec(getPositionCurrent()) - Vec(Vec4());
    //[p]R
    Matrix pR;
    Matrix cross_p = Matrix::crossProductMatrix(p);
    pR = cross_p * R;
    //Ad
    Ad.setSubmatrix(0,0,R);
    Ad.setSubmatrix(3,0,pR);
    Ad.setSubmatrix(3,3,R);

    return Ad;
}

Matrix Joint::getAd(Vec4 pos)
{
    Matrix Ad(6,6);
    //R
    Matrix R(3,3);
    //matriz de rotacao da junta em relacao ao frame global (assume-se que o frame da junta eh o mesmo que o frame global - em relacao a rotacao)
    R = R.identity();
    Vec p = Vec(getPositionCurrent()) - Vec(pos);
    //[p]R
    Matrix pR;
    Matrix cross_p = Matrix::crossProductMatrix(p);
    pR = cross_p * R;
    //Ad
    Ad.setSubmatrix(0,0,R);
    Ad.setSubmatrix(3,0,pR);
    Ad.setSubmatrix(3,3,R);
    return Ad;
}

Matrix Joint::getAd(Object *obj)
{
    // | R p |^-1   | R^t  R^t.(-p) |
    // | 0 1 |    = |  0      1     |
    Matrix Ad(6,6);
    //R
    Matrix R = Physics::getMatrixRotation(obj);
    //matriz inversa de R
    Matrix invR = R.transpose(); //R^-1 = R^t, pois R eh uma matriz de rotacao
    R = invR;
    Vec posJoint = Vec(getPositionCurrent()),     //posição do mundo
         posBody = Vec(obj->getPositionCurrent()), //posição do corpo
         p;                                   //deslocamento

    p = posBody - posJoint;
    p = R*p;
    //[p]R
    Matrix pR;
    Matrix cross_p = Matrix::crossProductMatrix(p);
    pR = cross_p * R;
    //Ad
    Ad.setSubmatrix(0,0,R);
    Ad.setSubmatrix(3,0,pR);
    Ad.setSubmatrix(3,3,R);
    return Ad;
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

int Joint::getType()
{
    return this->type;
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

Vec4 Joint::getPositionCurrent()
{
    return Physics::getJointBallAnchor(this);
}

QString Joint::showInfo()
{
    QString jo;
    QString aux;
    jo += "Properties Joint------\n";
    jo += "Name: "+getName()+"\n";
    jo += "Parent: "+parent->getName()+"\n";
    jo += "Child: "+child->getName()+"\n";
    Vec4 p = getPositionAnchor();
    jo += aux.sprintf("Anchor: %.3f %.3f %.3f \n",p.x(),p.y(),p.z());
    return jo;

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

        if (selected) Draw::drawSphereSelected(position);
        else Draw::drawSphere(position,MATERIAL_SILVER);
        break;
    }
    case (JOINT_FIXED):{
        break;
    }
    }
}

void Joint::setTorqueMax(Vec4 tq)
{
    this->tqMax = tq;
}
