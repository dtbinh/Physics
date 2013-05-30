#include "object.h"
#include "graphics/draw.h"
#include "math/matrix4x4.h"
#include "graphics/mesh.h"
#include "extra/material.h"
#include "scene.h"

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
}

Object::Object(Scene *scene)
{
    this->scene = scene;
    this->mass  = new Mass();
    this->material = new Material();
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

dGeomID Object::getGeometry()
{
    return this->geometry;
}

void Object::setGeometry(dGeomID geom)
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

void Object::appForce(float x, float y, float z)
{
    Physics::bodySetForce(this->body,x,y,z);
}

void Object::appForce(Vec4 *force)
{
    Physics::bodySetForce(this->body,force->x(),force->y(),force->z());
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

void Object::draw()
{
    if (this->geometry==0) return;
    switch (this->type){
    case TYPE_CUBE:{
            Draw::drawCube(getMatrixTransformation(),this->properties,this->material);
            if(this->selected) Draw::drawSelection(getMatrixTransformation(),this->properties);
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

Vec4 Object::getPosition()
{
    return this->position;
}

void Object::setRotation(Quaternion rotation)
{
    this->rotation = rotation;
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
    return Physics::getRotationBody(this->geometry);
}
