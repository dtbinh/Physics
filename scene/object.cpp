#include "object.h"
#include "graphics/draw.h"
#include "math/matrix4x4.h"
#include "graphics/mesh.h"
#include "extra/material.h"
#include "scene.h"
#include "math/matrix.h"
#include "control/sensor.h"
#include "math/ray.h"


//calculo para interseção ray-plane limitado
bool tryInterceptionPointFace(Face face, Vec4 point)
{
        Vec4 a,b,c,n; //coordenadas do triangulo
        for(int i=0;i<face.vertexs.size()-2;i++){
            n.setVec4(face.normals.at(0)->x1,face.normals.at(0)->x2,face.normals.at(0)->x3);
            a.setVec4(face.vertexs.at(0)->x1,face.vertexs.at(0)->x2,face.vertexs.at(0)->x3);
            b.setVec4(face.vertexs.at(i+1)->x1,face.vertexs.at(i+1)->x2,face.vertexs.at(i+1)->x3);
            c.setVec4(face.vertexs.at(i+2)->x1,face.vertexs.at(i+2)->x2,face.vertexs.at(i+2)->x3);
            if (Vec4::crossProduct((b-a),(point-a))*n >= 0 && Vec4::crossProduct((c-b),(point-b))*n >= 0 && Vec4::crossProduct((a-c),(point-c))*n >= 0) return true;

        }
        return false;
}

//----------------------Construtores & Destrutores

Object::Object()
{
    this->position   = Vec4();
    this->rotation   = QuaternionQ();
    this->selected   = false;
    this->properties = Vec4();
    this->type       = -1;
    Material::setMaterial(this->material,0);
    this->id_material = -1;
    this->mass  = new Mass();
    this->material = new Material();
    this->isFoot = false;
    this->bodyBalance = false;
    this->compensable = 0.0;
    this->show_effector = false;
    this->enabled_cpdp = false;
    this->show_target = false;
    this->has_cup = false;
    this->target = Vec4();
    this->ks = Vec4();
    this->kd = Vec4();
    this->chara = NULL;
    this->objFile = "";
    this->rendermesh = false;
    this->mirror_obj = NULL;


}

Object::Object(Scene *scene)
{
    this->id_material = -1;
    this->scene = scene;
    this->mass  = new Mass();
    this->material = new Material();
    this->properties = Vec4();
    this->selected = false;
    this->isFoot = false;
    this->bodyBalance = false;
    this->compensable = 0.0;
    this->show_effector = false;
    this->enabled_cpdp = false;
    this->show_target = false;
    this->has_cup = false;
    this->target = Vec4();
    this->ks = Vec4();
    this->kd = Vec4();
    this->chara = NULL;
    this->objFile = "";
    this->rendermesh = false;
    this->mirror_obj = NULL;
}

Object::Object(Vec4 position, QuaternionQ rotation, Vec4 properties, int type, Scene *scene, QString name)
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
    this->compensable = 0.0;
    this->show_effector = false;
    this->enabled_cpdp = false;
    this->show_target = false;
    this->has_cup = false;
    this->target = Vec4();
    this->ks = Vec4();
    this->kd = Vec4();
    this->chara = NULL;
    this->rendermesh = false;
    this->mirror_obj = NULL;
}

void Object::setCharacter(Character *chara)
{
    this->chara = chara;
}

Character* Object::getCharacter()
{
    return chara;
}

Object::~Object()
{
    delete this->scene;
    delete this->mass;
    delete this->material;
    delete this->objMesh;
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
    return out;
}

void Object::setObjFile(QString obj)
{

    this->objFile = obj;
    objMesh = new ObjMesh(obj.toStdString());
}

QString Object::getObjFile()
{
    QDir qfile;
    QString relative = qfile.relativeFilePath(objFile);
    return relative;
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

void Object::clearPhysics()
{
    Physics::closeObject(this);
}

MatrixF Object::getAd()
{
    // | R p |^-1   | R^t  R^t.(-p) |
    // | 0 1 |    = |  0      1     |
    MatrixF Ad(6,6);
    //R
    MatrixF R = Physics::getMatrixRotation(this);
    //matriz inversa de R
    MatrixF invR = R.transpose(); //R^-1 = R^t, pois R eh uma matriz de rotacao
    R = invR;
    Vec posWorld, //posição do mundo
        posBody,           //posição do corpo
        p;                 //deslocamento
    posBody = Vec(getPositionCurrent());
    posWorld = Vec(Vec4());
    p = posWorld - posBody;
    p = R*p;
    //[p]R
    MatrixF pR;
    MatrixF cross_p = MatrixF::crossProductMatrix(p);
    pR = cross_p * R;
    //Ad
    Ad.setSubmatrix(0,0,R);
    Ad.setSubmatrix(3,0,pR);
    Ad.setSubmatrix(3,3,R);
    return Ad;
}

MatrixF Object::getAd(Character* chara)
{
    // | R p |^-1   | R^t  R^t.(-p) |
    // | 0 1 |    = |  0      1     |
    MatrixF Ad(6,6);
    //R
    MatrixF R = Physics::getMatrixRotation(this);
    //matriz inversa de R
    MatrixF invR = R.transpose(); //R^-1 = R^t, pois R eh uma matriz de rotacao
    R = invR;
    Vec posCOM = Vec(chara->getPosCOM()), //posição do mundo
        posBody,           //posição do corpo
        p;                 //deslocamento
    posBody = Vec(getPositionCurrent());
    p = posCOM- posBody;
    p = R*p;
    //[p]R
    MatrixF pR;
    MatrixF cross_p = MatrixF::crossProductMatrix(p);
    pR = cross_p * R;
    //Ad
    Ad.setSubmatrix(0,0,R);
    Ad.setSubmatrix(3,0,pR);
    Ad.setSubmatrix(3,3,R);
    return Ad;
}

MatrixF Object::getAd(Vec4 pos)
{
    // | R p |^-1   | R^t  R^t.(-p) |
    // | 0 1 |    = |  0      1     |
    MatrixF Ad(6,6);
    //R
    MatrixF R = Physics::getMatrixRotation(this);
    //matriz inversa de R
    MatrixF invR = R.transpose(); //R^-1 = R^t, pois R eh uma matriz de rotacao
    R = invR;
    Vec posCOM = pos, //posição do mundo
        posBody,           //posição do corpo
        p;                 //deslocamento
    posBody = Vec(getPositionCurrent());
    p = posCOM- posBody;
    p = R*p;
    //[p]R
    MatrixF pR;
    MatrixF cross_p = MatrixF::crossProductMatrix(p);
    pR = cross_p * R;
    //Ad
    Ad.setSubmatrix(0,0,R);
    Ad.setSubmatrix(3,0,pR);
    Ad.setSubmatrix(3,3,R);
    return Ad;
}

MatrixF Object::getIM()
{
    //tratando o caso da geometria tipo ccylinder
    /* ---- Ainda não considerado no modelo
      if ( dGeomGetClass( b->geom ) == dCCylinderClass ) {
        //correspondente a rotacionar a matriz de inercia 90 graus no eixo X
        diagI[1] = massBody.I[10];
        diagI[2] = massBody.I[5];
      }
      */
    MatrixF Ibody = MatrixF(6,6);
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

void Object::setCollideWithGround(bool b)
{
    collide_ground = b;
}

bool Object::isCollideWithGround()
{
    return collide_ground;
}

//---------------------Geometry

void Object::setMaterial(int m)
{
    Material::setMaterial(this->material,m);
    id_material = m;
}

int Object::getIntMaterial()
{
    return id_material;
}

Mesh *Object::getMesh()
{
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

Matrix4x4 Object::getMatrixTransformationODE()
{
    Matrix4x4 *transform = new Matrix4x4();
    //Vec4 currentPos = this->getPositionCurrent();
    Physics::getGeomTransform(this->geometry,transform);
    Matrix4x4 returns;
    returns.set(0,transform->get(0));
    returns.set(1,transform->get(1));
    returns.set(2,transform->get(2));
    returns.set(3,transform->get(3));
    returns.set(4,transform->get(4));
    returns.set(5,transform->get(5));
    returns.set(6,transform->get(6));
    returns.set(7,transform->get(7));
    returns.set(8,transform->get(8));
    returns.set(9,transform->get(9));
    returns.set(10,transform->get(10));
    returns.set(11,transform->get(11));
    returns.set(12,transform->get(12));
    returns.set(13,transform->get(13));
    returns.set(14,transform->get(14));
    returns.set(15,transform->get(15));
    delete transform;
    return returns;
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

    //Draw::drawSphere(posEffectorBackward(),MATERIAL_CHROME,0.09);
    if(has_cup) Draw::drawCoffeeCup(getPositionCurrent(),MATERIAL_WHITE_PLASTIC,QuaternionQ(Vec4(-90,0,0))*getRotationCurrent().conjugate());
    if(show_target) Draw::drawSphere(target,MATERIAL_GOLD,0.05);
    if(show_effector) Draw::drawSphere(getPositionCurrent(),MATERIAL_PEARL,0.02);
    if (show_effector&&show_target){
        if(enabled_cpdp) Draw::drawLine(target,getPositionCurrent(),Vec4(0,.9,0),1.4);
        else Draw::drawLine(target,getPositionCurrent(),Vec4(0.9,0,0),1.4);
    }
    if (this->geometry==0) return;
    switch (this->type){
    case TYPE_CUBE:{
            if (wire){
                Draw::drawWireframe(getMatrixTransformation(),this->properties,Vec4(0.3,0.3,0.3));
                if(objFile.isEmpty() || !rendermesh){
                    Draw::drawCube(getMatrixTransformation(),this->properties,this->material);
                }else{
                    glEnable(GL_BLEND);
                    Material *m = new Material();
                    Material::setMaterial(m,MATERIAL_ICE);
                    Draw::drawObj(getPositionCurrent(),this->id_material,getRotationCurrent().conjugate(),this->objFile,objMesh);
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                    Draw::drawCube(getMatrixTransformation(),this->properties,m,0.4);
                    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
                    glDisable(GL_BLEND);
                    delete m;
                    //glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
                }
            }
            else{
//                if(isFoot && !Sensor::isSwingFoot(this)){
//                    Material *mat = new Material();
//                    mat->setMaterial(mat,MATERIAL_CYAN_PLASTIC);
//                    Draw::drawCube(getMatrixTransformation(),this->properties,mat);
//                }else{

                if(objFile.isEmpty() || !rendermesh){
                    Draw::drawCube(getMatrixTransformation(),this->properties,this->material);
                }else{
                    Draw::drawObj(getPositionCurrent(),this->id_material,getRotationCurrent().conjugate(),this->objFile,objMesh);
                }

//                }
            }
            //if(this->selected) Draw::drawSelection(getMatrixTransformation(),this->properties);
            //if(this->isFoot) Draw::drawSelection(getMatrixTransformation(),this->properties,Vec4(0,0,1));
            break;
        }
    case TYPE_CYLINDER:{
        glPushMatrix();
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,material->ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material->diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material->specular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material->shininess*128);
        const dReal* pos;
        const dReal* R;
        pos = dGeomGetPosition (geometry);
        R = dGeomGetRotation (geometry);
        Draw::setTransformODE(pos,R);
        glTranslatef(0,0,-properties.x()*2);
        GLUquadricObj *quadrico = gluNewQuadric();
        Draw::gluClosedCylinder(quadrico,properties.x(),properties.x(),properties.y(),20,20);
        gluDeleteQuadric( quadrico );
        //Draw::drawCylinderClosed(this->getPositionCurrent(),Vec4(0,0,1),properties.x(),properties.y(),this->id_material);
        //Draw::drawCylinder(getMatrixTransformation(),this->material,this->properties);
        glPopMatrix();
        break;
        }
    case TYPE_SPHERE:{
            Draw::drawSphere(getMatrixTransformation(),this->material,properties.x());
            break;
        }
    }
}

void Object::drawShadow()
{
    if (this->geometry==0) return;
    if(has_cup) Draw::drawCoffeeCup(getPositionCurrent(),MATERIAL_WHITE_PLASTIC,QuaternionQ(Vec4(-90,0,0))*getRotationCurrent().conjugate());
    if(show_target) Draw::drawSphere(target,MATERIAL_GOLD,0.05);
//    if (show_effector && show_target){
//        if(enabled_cpdp) Draw::drawLine(target,getPositionCurrent(),Vec4(0,.9,0),1.4);
//        else Draw::drawLine(target,getPositionCurrent(),Vec4(0.9,0,0),1.4);
//    }

    switch (this->type){
    case TYPE_CUBE:{
        if(objFile.isEmpty() || !rendermesh){
            Draw::drawCube(getMatrixTransformation(),this->properties,this->material);
        }else{
            Draw::drawObj(getPositionCurrent(),this->id_material,getRotationCurrent().conjugate(),this->objFile,objMesh);
        }
            break;
        }
    case TYPE_CYLINDER:{
            Draw::drawCylinder(getMatrixTransformation(),this->material,this->properties);
            break;
        }
    case TYPE_SPHERE:{
            Draw::drawSphere(getMatrixTransformation(),this->material,properties.x());
            break;
        }
    }
}

void Object::draw(Vec4 position, QuaternionQ q, int mat, bool wire)
{
   // Draw::drawPoint(posEffectorForward(),0.2,Vec4(0.5,0.5,0.5));
    //if (this->geometry==0) return;
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
        if(mat==-1){
            if(wire){
            Draw::drawWireframe(getMatrixTransformation(),this->properties,Vec4(0.3,0.3,0.3));
            if(objFile.isEmpty() || !rendermesh){
                Draw::drawCube(getMatrixTransformation(),this->properties,mat);
            }else{
                glEnable(GL_BLEND);
                Material *m = new Material();
                Material::setMaterial(m,MATERIAL_ICE);
                Draw::drawObj(transform,this->id_material,objMesh);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                 Draw::drawCube(getMatrixTransformation(),this->properties,m,0.4);
                 delete m;
                glDisable(GL_BLEND);
            }
            }else{
            if(objFile.isEmpty() || !rendermesh)
              Draw::drawCube(transform,this->properties,mat);
            else{
              Draw::drawObj(transform,this->id_material,objMesh);
             }
            }
        }else{
            if(wire){
            Draw::drawWireframe(getMatrixTransformation(),this->properties,Vec4(0.3,0.3,0.3));
            if(objFile.isEmpty() || !rendermesh){
                Draw::drawCube(getMatrixTransformation(),this->properties,mat);
            }else{
                glEnable(GL_BLEND);
                Material *m = new Material();
                Material::setMaterial(m,MATERIAL_ICE);
                Draw::drawObj(transform,mat,objMesh);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                Draw::drawCube(getMatrixTransformation(),this->properties,m,0.4);
                //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
                glDisable(GL_BLEND);
                delete m;
                //glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            }
            }else{

            if(objFile.isEmpty() || !rendermesh)
                Draw::drawCube(transform,this->properties,mat);
            else{
                Draw::drawObj(transform,mat,objMesh);
            }
            }
            break;
        }
        }
    case TYPE_CYLINDER:{
            Draw::drawCylinder(getMatrixTransformation(),this->material,this->properties);
            break;
        }
    case TYPE_SPHERE:{
            Draw::drawSphere(getMatrixTransformation(),this->material);
            break;
        }
    }
    delete transform;
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

void Object::setRotation(QuaternionQ rotation)
{
    this->rotation = rotation;
}

void Object::setRotationCurrent(QuaternionQ rotation)
{
    Physics::setRotationBody(this,rotation);
}

QuaternionQ Object::getRotation()
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

QuaternionQ Object::getRotationCurrent()
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
        posf =  Vec4(properties.x()/2.0,0,0);
    else if(properties.y()>properties.x() && properties.y()>properties.z())
        posf =  Vec4(0,properties.y()/2.0,0);
    else
        posf =  Vec4(0,0,properties.z()/2.0);
    return pos + QuaternionQ::getVecRotation(this->getRotationCurrent(),posf);
}

Vec4 Object::posEffectorBackward()
{
    Vec4 pos = this->getPositionCurrent();
    Vec4 posf;
    if(properties.x()>properties.y() && properties.x()>properties.z())
        posf =  Vec4(-properties.x()/2.0,0,0);
    else if(properties.y()>properties.x() && properties.y()>properties.z())
        posf =  Vec4(0,-properties.y()/2.0,0);
    else
        posf =  Vec4(0,0,-properties.z()/2.0);
    return pos + QuaternionQ::getVecRotation(this->getRotationCurrent(),posf);
}

Vec4 Object::posEffectorForward(Vec4 pos, QuaternionQ rot, Object *obj)
{
    Vec4 properties = obj->getProperties();
    Vec4 posf;
    if(properties.x()>properties.y() && properties.x()>properties.z())
        posf = Vec4(properties.x()/2.0,0,0);
    else if(properties.y()>properties.x() && properties.y()>properties.z())
        posf = Vec4(0,properties.y()/2.0,0);
    else
        posf = Vec4(0,0,properties.z()/2.0);
    return pos+QuaternionQ::getVecRotation(rot,posf);
}

Vec4 Object::posEffectorBackward(Vec4 pos, QuaternionQ rot, Object *obj)
{
    Vec4 posf;
    Vec4 properties = obj->getProperties();
    if(properties.x()>properties.y() && properties.x()>properties.z())
        posf = Vec4(-properties.x()/2.0,0,0);
    else if(properties.y()>properties.x() && properties.y()>properties.z())
        posf = Vec4(0,-properties.y()/2.0,0);
    else
        posf = Vec4(0,0,-properties.z()/2.0);
    return pos + QuaternionQ::getVecRotation(rot,posf);
}

QString Object::showInfo()
{
    QString obj;
    QString aux;
    obj += "Info Properties Body---------\n";
    obj += aux.sprintf("Geometry: %d\n",getType());
    obj += "Name: "+getName()+"\n";
    obj += aux.sprintf("Mass: %.3f\n",getMass());
    Vec4 p = getPosition();
    obj += aux.sprintf("Position: %.3f %.3f %.3f \n",p.x(),p.y(),p.z());
    p = getProperties();
    obj += aux.sprintf("Properties: %.3f %.3f %.3f \n",p.x(),p.y(),p.z());
    QuaternionQ q = getRotation();
    obj += aux.sprintf("Quaternion: %.3f %.3f %.3f %.3f \n",q.getScalar(),q.getPosX(),q.getPosY(),q.getPosZ());
    if(getFoot())
        obj +="Is Foot: true\n";
    else
        obj +="Is Foot: false\n";
    if(getBodyBalance())
        obj +="Is Body Balance: true\n";
    else
        obj +="Is Body Balance: false\n";
    obj += "Info Cases Use Body---------\n";
    obj += "Positional Control PD\n";
    if (enabled_cpdp)
        obj +="Enabled: true\n";
    else
        obj +="Enabled: false\n";
    if (show_target)
        obj +="Show Target: true\n";
    else
        obj +="Show Target: false\n";
    if (show_effector)
        obj +="Show Effector: true\n";
    else
        obj +="Show Effector: false\n";
    p = getTarget();
    obj += aux.sprintf("Target: %.3f %.3f %.3f \n",p.x(),p.y(),p.z());
    p = getKs();
    obj += aux.sprintf("Ks: %.3f %.3f %.3f \n",p.x(),p.y(),p.z());
    p = getKd();
    obj += aux.sprintf("Kd: %.3f %.3f %.3f \n",p.x(),p.y(),p.z());
    obj += "Cup Coffee Control PD\n";
    if (has_cup)
        obj +="Enabled: true\n";
    else
        obj +="Enabled: false\n";
    p = getKsCup();
    obj += aux.sprintf("Ks: %.3f %.3f %.3f \n",p.x(),p.y(),p.z());
    p = getKdCup();
    obj += aux.sprintf("Kd: %.3f %.3f %.3f \n",p.x(),p.y(),p.z());
    return obj;

}

void Object::setRenderMesh(bool b)
{
    this->rendermesh = b;
}

float Object::intersectionRay(Ray ray, float t)
{
    //considerando que cada objeto é uma caixa
//    Vec4 bounds[2];
//    bounds[0] = Vec4(-0.5,-0.5,-0.5);
//    bounds[1] = Vec4(0.5,0.5,0.5);
//    Ray copy;
//    Matrix4x4 transform = getMatrixTransformationODE();
//    copy.setOrigin(transform.transform_origin_ray(transform,ray.origin));
//    copy.setDirection(transform.transform_direction_ray(transform,ray.direction));
//    float tmin, tmax, tymin, tymax, tzmin, tzmax;
//    tmin = (bounds[copy.sign[0]].x() - copy.origin.x()) * copy.inverse_direction.x();
//    tmax = (bounds[1-copy.sign[0]].x() - copy.origin.x()) * copy.inverse_direction.x();
//    tymin = (bounds[copy.sign[1]].y() - copy.origin.y()) * copy.inverse_direction.y();
//    tymax = (bounds[1-copy.sign[1]].y() - copy.origin.y()) * copy.inverse_direction.y();
//    if ( (tmin > tymax) || (tymin > tmax) )
//        return -1;
//    if (tymin > tmin)
//        tmin = tymin;
//    if (tymax < tmax)
//        tmax = tymax;
//    tzmin = (bounds[copy.sign[2]].z() - copy.origin.z()) * copy.inverse_direction.z();
//    tzmax = (bounds[1-copy.sign[2]].z() - copy.origin.z()) * copy.inverse_direction.z();
//    if ( (tmin > tzmax) || (tzmin > tmax) )
//        return -1;
//    if (tzmin > tmin)
//        tmin = tzmin;
//    if (tzmax < tmax)
//        tmax = tzmax;
//    if (!( (tmin < t) && (tmax > tmin) )) return -1;

    //
    mesh = new Mesh();
    mesh = Draw::getMeshCube(getMatrixTransformation(),getProperties(),mesh);


    for (int i=0;i<mesh->faces.size();i++){
        Vec4 p1,n;
        p1.setVec4(mesh->faces.at(i).vertexs[0]->x1,mesh->faces.at(i).vertexs[0]->x2,mesh->faces.at(i).vertexs[0]->x3);
        n.setVec4(mesh->faces.at(i).normals[0]->x(),mesh->faces.at(i).normals[0]->y(),mesh->faces.at(i).normals[0]->z());
        if ((!(fabs(n*(ray.direction))<0.0001))){
            float t_ = (n*p1 - ray.origin*n)/(n*(ray.direction));
            if((t_<t) && t_>0){
                if (tryInterceptionPointFace(mesh->faces.at(i),ray.positionRay(t_))){
                    t = t_;
                }
            }
        }


    }
    delete mesh;
    return t;
}

float Object::getCompensableFactor()
{
    return compensable;
}

void Object::setCompensableFactor(float val)
{
    compensable = val;
}

void Object::setShowEffector(bool b)
{
    show_effector = b;
}

bool Object::isShowEffector()
{
    return show_effector;
}

void Object::setShowTarget(bool b)
{
    show_target = b;
}

bool Object::isShowTarget()
{
    return show_target;
}

void Object::setEnableCPDP(bool b)
{
    enabled_cpdp = b;
}

bool Object::isEnableCPDP()
{
    return enabled_cpdp;
}

void Object::setTarget(Vec4 pos)
{
    if(this->mirror_obj!=NULL){
        Vec4 add = pos - target;
        this->mirror_obj->addMirrorPos(add);
        this->addMirrorPos(add);
    }else{
        target = pos;
    }
}

Vec4 Object::getTarget()
{
    return target;
}

void Object::addMirrorPos(Vec4 pos)
{
    this->target +=pos;
}

void Object::setKs(Vec4 pos)
{
    ks = pos;
}

Vec4 Object::getKs()
{
    return ks;
}

void Object::setKd(Vec4 pos)
{
    kd = pos;
}

Vec4 Object::getKd()
{
    return kd;
}

void Object::evaluate(int val)
{
    if(has_cup){
        Vec4 v = QuaternionQ::getVecRotation(getRotationCurrent()*getRotation().conjugate(),Vec4(0,0,1));
        //Vec4 axis = q.getVector();
        Vec4 newaxis = Vec4(0,0,1)^v;
        float angle = Vec4(0,0,1)*v;
        Vec4 ax;
        dReal ang;
        QuaternionQ ql = QuaternionQ(angle,newaxis);
        QuaternionQ qDelta = QuaternionQ::deltaQuat(QuaternionQ(Vec4(-180,0,0)), ql );
          //indo pelo caminho mais curto
        //qDelta = Quaternion().lessArc(qDelta);
        qDelta.toAxisAngle( &ax, &ang );
        Vec4 torque = ks.mult(ax*ang) - kd.mult(getRelVelAngular());
        Physics::bodyAddTorque(this->body,torque.x(),torque.y(),torque.z());
    }
    if (!(enabled_cpdp)) return;
    for(int i=0;i<val;i++){
        Vec4 effector = getPositionCurrent();
        Vec4 force = ks.mult(target - effector) - kd.mult(getRelVelLinear());
        Physics::bodyAddForce(this->body,force.x(),force.y(),force.z());
    }
}

void Object::setMirror(Object *m)
{
    this->mirror_obj = m;
}

Object *Object::getMirror()
{
    return this->mirror_obj;
}

void Object::setCoffeeCup(bool b)
{
    has_cup = b;
}

bool Object::hasCoffeeCup()
{
    return has_cup;
}

void Object::setKsCup(Vec4 pos)
{
    kscup = pos;
}

Vec4 Object::getKsCup()
{
    return kscup;
}

void Object::setKdCup(Vec4 pos)
{
    kdcup = pos;
}

Vec4 Object::getKdCup()
{
    return kdcup;
}

