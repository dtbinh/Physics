#include "object.h"
#include "graphics/draw.h"
#include "math/matrix4x4.h"
#include "graphics/mesh.h"
#include "extra/material.h"
#include "scene.h"
#include "math/matrix.h"

//----------------------Construtores & Destrutores

Object::Object()
{
    this->position   = Vec4();
    this->rotation   = Quaternion();
    this->selected   = false;
    this->properties = Vec4();
    this->type       = -1;
    Material::setMaterial(this->material,0);
    this->mass  = new Mass();
    this->material = new Material();
    this->isFoot = false;
    this->bodyBalance = false;
}

Object::Object(Scene *scene)
{
    this->scene = scene;
    this->mass  = new Mass();
    this->material = new Material();
    this->selected = false;
    this->isFoot = false;
    this->bodyBalance = false;
}

Object::Object(Vec4 position, Quaternion rotation, Vec4 properties, int type, Scene *scene,QString name)
{
    this->position   = position;
    this->rotation   = rotation;
    this->selected   = false;
    this->properties = properties;
    this->type       = type;
    this->mass  = new Mass();
    this->material = new Material();
    Material::setMaterial(this->material,0);
    this->scene      = scene;
    this->name       = name;
    this->isFoot = false;
    this->bodyBalance = false;

}

Object::~Object()
{
    delete this->scene;
    delete this->mass;
    delete this->material;
}

//---------------------Scene

void Object::setSelected(bool b)
{
    this->selected = b;
}

bool Object::isSelected()
{
    return this->selected;
}

void Object::setScene(Scene *scene)
{
    this->scene = scene;
}

Scene *Object::getScene()
{
    return this->scene;
}

QString Object::getName()
{
    return this->name;
}

void Object::setName(QString name)
{
    this->name = name;
}

QString Object::saveObject() //função a ser pensada...
{
    QString out;
    out = ""; //depois configurar formato de saída
}

//---------------------Physics

dBodyID Object::getBody()
{
    return this->body;
}

void Object::setBody(dBodyID body)
{
    this->body = body;
}

GeomID Object::getGeometry()
{
    return this->geometry;
}

void Object::setGeometry(GeomID geom)
{
    this->geometry = geom;
}

Mass *Object::getMass()
{
    return this->mass;
}

void Object::setMass(Mass *mass)
{
    this->mass = mass;
}

void Object::appTorque(float x, float y, float z)
{
    Physics::bodySetTorque(this->body,x,y,z);
}

void Object::appTorque(Vec4 *torque)
{
    Physics::bodySetTorque(this->body,torque->x(),torque->y(),torque->z());
}

void Object::appTorque(Vec4 torque)
{
    Physics::bodySetTorque(this->body,torque.x(),torque.y(),torque.z());
}

void Object::addTorque(Vec4 torque)
{
    Physics::bodyAddTorque(this->body,torque.x(),torque.y(),torque.z());
}

void Object::addTorque(float x, float y, float z)
{
    Physics::bodyAddTorque(this->body,x,y,z);
}

void Object::addTorque(Vec4 *torque)
{
    Physics::bodyAddTorque(this->body,torque->x(),torque->y(),torque->z());
}

void Object::appForce(float x, float y, float z)
{
    Physics::bodySetForce(this->body,x,y,z);
}

void Object::appForce(Vec4 *force)
{
    Physics::bodySetForce(this->body,force->x(),force->y(),force->z());
}

void Object::appForce(Vec4 force)
{
    Physics::bodySetForce(this->body,force.x(),force.y(),force.z());
}

void Object::addForce(Vec4 force)
{
    Physics::bodyAddForce(this->body,force.x(),force.y(),force.z());
}

void Object::updatePhysics()
{
    Physics::updateObject(this);
}

Vec4 Object::getRelVelAngular()
{
    return Physics::getAngularVelBody(this);
}

Vec4 Object::getRelVelLinear()
{
    return Physics::getLinearVelBody(this);
}

Matrix Object::getAd()
{
    // | R p |^-1   | R^t  R^t.(-p) |
    // | 0 1 |    = |  0      1     |
    Matrix Ad(6,6);
    //R
    Matrix R = Physics::getMatrixRotation(this);
    //matriz inversa de R
    Matrix invR = R.transpose(); //R^-1 = R^t, pois R eh uma matriz de rotacao
    R = invR;
    Vec posWorld, //posição do mundo
        posBody,           //posição do corpo
        p;                 //deslocamento
    posBody = Vec(getPositionCurrent());
    posWorld = Vec(Vec4());
    p = posWorld - posBody;
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

Matrix Object::getAd(Character* chara)
{
    // | R p |^-1   | R^t  R^t.(-p) |
    // | 0 1 |    = |  0      1     |
    Matrix Ad(6,6);
    //R
    Matrix R = Physics::getMatrixRotation(this);
    //matriz inversa de R
    Matrix invR = R.transpose(); //R^-1 = R^t, pois R eh uma matriz de rotacao
    R = invR;
    Vec posCOM = Vec(chara->getPosCOM()), //posição do mundo
        posBody,           //posição do corpo
        p;                 //deslocamento
    posBody = Vec(getPositionCurrent());
    p = posCOM- posBody;
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

Matrix Object::getIM()
{
    //tratando o caso da geometria tipo ccylinder
    /* ---- Ainda não considerado no modelo
      if ( dGeomGetClass( b->geom ) == dCCylinderClass ) {
        //correspondente a rotacionar a matriz de inercia 90 graus no eixo X
        diagI[1] = massBody.I[10];
        diagI[2] = massBody.I[5];
      }
      */
    Matrix Ibody = Matrix(6,6);
    Ibody(0,0) = mass->I[0];
    Ibody(1,1) = mass->I[1];
    Ibody(2,2) = mass->I[2];
    Ibody(3,3) = mass->mass;
    Ibody(4,4) = mass->mass;
    Ibody(5,5) = mass->mass;
    return Ibody;
}

void Object::setBodyBalance(bool b)
{
    this->bodyBalance = b;
}

bool Object::getBodyBalance()
{
    return bodyBalance;
}

//---------------------Geometry

void Object::setMaterial(int m)
{
    Material::setMaterial(this->material,m);
}

Mesh *Object::getMesh() //função a ser pensada, se será necessária...
{
    Mesh *mesh = NULL;
    return mesh;
}

Matrix4x4* Object::getMatrixTransformation()
{
    Matrix4x4 *transform = new Matrix4x4();
    //Vec4 currentPos = this->getPositionCurrent();
    Physics::getGeomTransform(this->geometry,transform);
//    Vec4 currentPos = Vec4(transform->get(11),transform->get(12),transform->get(13));
//    transform->translate(-(currentPos).x(),-(currentPos).y(),-(currentPos).z());
//    transform->scale(this->properties.x(),this->properties.y(),this->properties.z());
//    transform->translate(currentPos.x(),currentPos.y(),currentPos.z());
    return transform;
}

void Object::setMaterial(Vec4 amb, Vec4 diff, Vec4 spe, float shininess)
{
    Material::setMaterial(this->material,amb,diff,spe,shininess);
}

void Object::wireframe()
{
    //a fazer
}

void Object::draw(bool wire)
{
    if (this->geometry==0) return;
//    Draw::drawPoint(posEffectorForward(),0.02,Vec4(0.5,0.5,0.5));
//    Draw::drawPoint(posEffectorBackward(),0.02,Vec4(0.5,0.0,0.5));
    switch (this->type){
    case TYPE_CUBE:{
            if (wire)
                Draw::drawWireframe(getMatrixTransformation(),this->properties,Vec4(1,0,0));
            else
                Draw::drawCube(getMatrixTransformation(),this->properties,this->material);
            if(this->selected) Draw::drawSelection(getMatrixTransformation(),this->properties);
            if(this->isFoot) Draw::drawSelection(getMatrixTransformation(),this->properties,Vec4(0,0,1));
            break;
        }
    case TYPE_CYLINDER:{
            Draw::drawCylinder(getMatrixTransformation(),this->material);
            break;
        }
    case TYPE_SPHERE:{
            Draw::drawSphere(getMatrixTransformation(),this->material);
            break;
        }
    }
}

void Object::draw(Vec4 position, Quaternion q,int mat)
{
   // Draw::drawPoint(posEffectorForward(),0.2,Vec4(0.5,0.5,0.5));
    if (this->geometry==0) return;
    Matrix4x4 *transform = new Matrix4x4();
    Matrix4x4 transformx = q.getMatrix();

    transform->set( 0, transformx.matrix[0] );
    transform->set( 1, transformx.matrix[4] );
    transform->set( 2, transformx.matrix[8] );
    transform->set( 3, 0 );
    transform->set( 4, transformx.matrix[1] );
    transform->set( 5, transformx.matrix[5] );
    transform->set( 6, transformx.matrix[9] );
    transform->set( 7, 0 );
    transform->set( 8, transformx.matrix[2] );
    transform->set( 9, transformx.matrix[6] );
    transform->set( 10, transformx.matrix[10]);
    transform->set( 11, 0 );

    transform->set( 15, 1.0 );

    transform->set(12,position.x());
    transform->set(13,position.y());
    transform->set(14,position.z());

    switch (this->type){
    case TYPE_CUBE:{
        if(mat==-1)
              Draw::drawCube(transform,this->properties,this->material);
        else
            Draw::drawCube(transform,this->properties,mat);
//            if(this->selected) Draw::drawSelection(getMatrixTransformation(),this->properties);
//            if(this->isFoot) Draw::drawSelection(getMatrixTransformation(),this->properties,Vec4(0,0,1));
            break;
        }
    case TYPE_CYLINDER:{
            Draw::drawCylinder(getMatrixTransformation(),this->material);
            break;
        }
    case TYPE_SPHERE:{
            Draw::drawSphere(getMatrixTransformation(),this->material);
            break;
        }
    }
}

Vec4 Object::getProperties()
{
    return this->properties;
}

void Object::setProperties(Vec4 properties)
{
    this->properties = properties;
}

void Object::setPosition(Vec4 position)
{
    this->position = position;
}

void Object::setPositionCurrent(Vec4 position)
{
    Physics::setPositionBody(this,position);
}

Vec4 Object::getPosition()
{
    return this->position;
}

void Object::setRotation(Quaternion rotation)
{
    this->rotation = rotation;
}

void Object::setRotationCurrent(Quaternion rotation)
{
    Physics::setRotationBody(this,rotation);
}

Quaternion Object::getRotation()
{
    return this->rotation;
}

int Object::getType()
{
    return this->type;
}

void Object::setType(int type)
{
    if(this->getName().isEmpty())
        switch (type){
        case (TYPE_CUBE):{
        this->name.append("Cube");
        break;

        }
        case (TYPE_SPHERE):{
        this->name.append("Sphere");
        break;
        }
        case (TYPE_CYLINDER):{
        this->name.append("Cylinder");
        break;
        }
        }

    this->type = type;
}

void Object::setFMass(float fmass)
{
    this->fmass = fmass;
}

float Object::getFMass()
{
    return fmass;
}

Vec4 Object::getPositionCurrent()
{
    return Physics::getPositionBody(this->geometry);
}

Quaternion Object::getRotationCurrent()
{
    return Physics::getRotationBody(this);
}

void Object::setFoot(bool b)
{
    this->isFoot = b;
}

bool Object::getFoot()
{
    return this->isFoot;
}

Vec4 Object::posEffectorForward()
{
    Vec4 pos = this->getPositionCurrent();
    Vec4 posf;
    if(properties.x()>properties.y() && properties.x()>properties.z())
        posf = pos + Vec4(properties.x()/2.0,0,0);
    else if(properties.y()>properties.x() && properties.y()>properties.z())
        posf = pos + Vec4(0,properties.y()/2.0,0);
    else
        posf = pos + Vec4(0,0,properties.z()/2.0);
    return Quaternion::getVecRotation(this->getRotationCurrent(),posf);
}

Vec4 Object::posEffectorBackward()
{
    Vec4 pos = this->getPositionCurrent();
    Vec4 posf;
    if(properties.x()>properties.y() && properties.x()>properties.z())
        posf = pos + Vec4(-properties.x()/2.0,0,0);
    else if(properties.y()>properties.x() && properties.y()>properties.z())
        posf = pos + Vec4(0,-properties.y()/2.0,0);
    else
        posf = pos + Vec4(0,0,-properties.z()/2.0);
    return Quaternion::getVecRotation(this->getRotationCurrent(),posf);
}

Vec4 Object::posEffectorForward(Vec4 pos, Quaternion rot, Object *obj)
{
    Vec4 properties = obj->getProperties();
    Vec4 posf;
    if(properties.x()>properties.y() && properties.x()>properties.z())
        posf = pos + Vec4(properties.x()/2.0,0,0);
    else if(properties.y()>properties.x() && properties.y()>properties.z())
        posf = pos + Vec4(0,properties.y()/2.0,0);
    else
        posf = pos + Vec4(0,0,properties.z()/2.0);
    return Quaternion::getVecRotation(rot,posf);
}

Vec4 Object::posEffectorBackward(Vec4 pos, Quaternion rot, Object *obj)
{
    Vec4 posf;
    Vec4 properties = obj->getProperties();
    if(properties.x()>properties.y() && properties.x()>properties.z())
        posf = pos + Vec4(-properties.x()/2.0,0,0);
    else if(properties.y()>properties.x() && properties.y()>properties.z())
        posf = pos + Vec4(0,-properties.y()/2.0,0);
    else
        posf = pos + Vec4(0,0,-properties.z()/2.0);
    return Quaternion::getVecRotation(rot,posf);
}
