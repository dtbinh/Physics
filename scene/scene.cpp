#include "scene.h"
#include "object.h"
#include "cube.h"
#include "interface/glwidget.h"
#include "extra/material.h"
#include "character.h"
#include "joint.h"
#include <time.h>
#include <stdlib.h>
#include "graphics/draw.h"
bool apply = true;


Scene::Scene(GLWidget *parent)
{
    //loadFile(":/test.xml");
this->sim_status = true;
    this->parent = parent;
    //select = new Object(this);
Physics::initScene(this);
this->sim_step = 67;
this->frame_step = 1;
this->status_motion = false;
this->enableGravity = false;
this->externalForce = Vec4();
this->propKs = Vec4(1,1,1);
this->propKd = Vec4(1,1,1);
//Character *chara = new Character(this);
//this->characters.push_back(chara);

//Object *botpiece = addObject(
//                             Vec4(1,3,1),
//                             Vec4(0,4,0),
//                             Quaternion(),TYPE_CUBE,2.0,chara
//                             );
//Object *toppiece = addObject(Vec4(1,3,1),
//                             Vec4(0,7,0),
//                             Quaternion(1,0,0,0),TYPE_CUBE,2.0,chara
//                             );
////select = toppiece;
////asdfg = botpiece;
////selectedObjects.push_back(toppiece);

//Joint *joint = addJointBall(Vec4(0,5.5,0), botpiece,toppiece, chara);
//Joint *joint2 = addJointFixed(botpiece, chara);
//ControlPD *pd = new ControlPD(joint,Quaternion(Vec4(90,45,45)),Vec4(5,5,5),Vec4(0.5,0.5,0.5));
//chara->controllers.push_back(pd);
//controllers.push_back(pd);
//botpiece->appForce(5,5,0);
//Physics::bodyAddForce(botpiece->getBody(),2,2,2);
//objects.push_back(addObject(Vec4(5,2,5),Vec4(0,20,0),Quaternion(),TYPE_CUBE));

//    externalForce = new Vector3f(0,0,0);
//    externalTorque = new Vector3f(0,0,0);

//    Physics::initScene(this);
//    camera = new Camera();

//    floor = new Prop(this,PROP_PLANE);

//    camera->moveForward(-20.0);
//    camera->moveUp(2.0);

//    Character *chara = new Character(this);
//    this->characters.push_back(chara);


//    Object *botpiece = addObject(OBJ_BOX, MAT_ORANGE, chara,
//                                 Vector3f(1,3,1),
//                                 Vector3f(0,4,0),
//                                 Quaternion4f()
//                                 );
//    Object *toppiece = addObject(OBJ_BOX, MAT_YELLOW, chara,
//                                 Vector3f(1,3,1),
//                                 Vector3f(0,7,0),
//                                 Quaternion4f()
//                                 );
//    asdfg = botpiece;
//    selectedObjects.push_back(toppiece);

//    interestJoint = addJointBall(Vector3f(0,5.5,0), toppiece, botpiece, chara);

    //*/

    /*
    Vector3f footDimension(0.5,0.5,1.0);
    Vector3f legDimension (0.4,0.8,0.4);
    Vector3f armDimension (0.4,0.8,0.4);
    Vector3f bodyDimension(1.0,1.6,0.5);
    Vector3f headDimension(0.6,0.8,0.6);
    Vector3f handDimension(0.5,0.5,0.5);

    Object *foot_right = addObject(OBJ_BOX, MAT_DARK_ORANGE, chara,
                                   footDimension,
                                   Vector3f(-footDimension.getX()/2, footDimension.getY()/2, (footDimension.getZ()-legDimension.getZ())/2)
                                   );
    Object *leg1_right = addObject(OBJ_BOX, MAT_ORANGE, chara,
                                   legDimension,
                                   Vector3f(-bodyDimension.getX()/4, (legDimension.getY()/2) + footDimension.getY(), 0)
                                   );
    Object *leg2_right = addObject(OBJ_BOX, MAT_ORANGE, chara,
                                   legDimension,
                                   Vector3f(-bodyDimension.getX()/4, (legDimension.getY()/2) + legDimension.getY() + footDimension.getY(), 0)
                                   );

    Object *foot_left = addObject(OBJ_BOX, MAT_DARK_YELLOW, chara,
                                  footDimension,
                                  Vector3f(footDimension.getX()/2, footDimension.getY()/2, (footDimension.getZ()-legDimension.getZ())/2)
                                  );
    Object *leg1_left = addObject(OBJ_BOX, MAT_YELLOW, chara,
                                  legDimension,
                                  Vector3f(bodyDimension.getX()/4, (legDimension.getY()/2) + footDimension.getY(), 0)
                                  );
    Object *leg2_left = addObject(OBJ_BOX, MAT_YELLOW, chara,
                                  legDimension,
                                  Vector3f(bodyDimension.getX()/4, (legDimension.getY()/2) + legDimension.getY() + footDimension.getY(), 0)
                                  );


    Object *body_bot = addObject(OBJ_BOX, MAT_WHITE, chara,
                                 bodyDimension.addY(-bodyDimension.getY()/2),
                                 Vector3f(0, (bodyDimension.getY()/4) + footDimension.getY() + (2*legDimension.getY()), 0)
                                 );

    Object *body_top = addObject(OBJ_BOX, MAT_WHITE, chara,
                                 bodyDimension.addY(-bodyDimension.getY()/2),
                                 Vector3f(0, (bodyDimension.getY()*3/4) + footDimension.getY() + (2*legDimension.getY()), 0)
                                 );

    Object *arm2_left = addObject(OBJ_BOX, MAT_YELLOW, chara,
                                  armDimension,
                                  Vector3f((bodyDimension.getX()+armDimension.getX())/2, bodyDimension.getY() + footDimension.getY() + (2*legDimension.getY()) - armDimension.getY()/2, 0)
                                  );
    Object *arm1_left = addObject(OBJ_BOX, MAT_YELLOW, chara,
                                  armDimension,
                                  Vector3f((bodyDimension.getX()+armDimension.getX())/2, bodyDimension.getY() + footDimension.getY() + (2*legDimension.getY()) - armDimension.getY()*3/2, 0)
                                  );
    Object *arm2_right = addObject(OBJ_BOX, MAT_ORANGE, chara,
                                   armDimension,
                                   Vector3f(-(bodyDimension.getX()+armDimension.getX())/2, bodyDimension.getY() + footDimension.getY() + (2*legDimension.getY()) - armDimension.getY()/2, 0)
                                   );
    Object *arm1_right = addObject(OBJ_BOX, MAT_ORANGE, chara,
                                   armDimension,
                                   Vector3f(-(bodyDimension.getX()+armDimension.getX())/2, bodyDimension.getY() + footDimension.getY() + (2*legDimension.getY()) - armDimension.getY()*3/2, 0)
                                   );

    Object *hand_left = addObject(OBJ_BOX, MAT_DARK_YELLOW, chara,
                                  handDimension,
                                  Vector3f((bodyDimension.getX()+armDimension.getX())/2, bodyDimension.getY() + footDimension.getY() + (2*legDimension.getY()) - (armDimension.getY()*2) - handDimension.getY()/2 , 0)
                                  );

    Object *hand_right = addObject(OBJ_BOX, MAT_DARK_ORANGE, chara,
                                   handDimension,
                                   Vector3f(-(bodyDimension.getX()+armDimension.getX())/2, bodyDimension.getY() + footDimension.getY() + (2*legDimension.getY()) - (armDimension.getY()*2) - handDimension.getY()/2 , 0)
                                   );



    Object *head = addObject(OBJ_BOX, MAT_DARK_BLUE, chara,
                             headDimension,
                             Vector3f(0, footDimension.getY()+2*legDimension.getY()+bodyDimension.getY()+(headDimension.getY()/2), 0)
                             );

    selectedObjects.push_back(head);
    //selectedObjects.push_back(hand_left);
    //    selectedObjects.push_back(hand_right);
    //    selectedObjects.push_back(foot_right);
    //    selectedObjects.push_back(foot_left);

    Joint *joint;

    joint = addJointBall(leg2_left->initialPosition->addY( legDimension.getY()/2 ), body_bot, leg2_left, chara);
    joint->setColor(MAT_GREEN);
    joint = addJointBall(leg1_left->initialPosition->addY( legDimension.getY()/2 ), leg2_left, leg1_left, chara);
    joint->setColor(MAT_GREEN);
    joint = addJointBall(foot_left->initialPosition->addY( footDimension.getY()/2 ).addZ((legDimension.getZ()-footDimension.getZ())/2), leg1_left, foot_left, chara);
    joint->setColor(MAT_GREEN);

    joint = addJointBall(arm2_left->initialPosition->addY( armDimension.getY()/2 ).addX( -armDimension.getX()/2 ), body_top, arm2_left, chara);
    joint->setColor(MAT_GREEN);
    joint = addJointBall(arm1_left->initialPosition->addY( armDimension.getY()/2 ), arm2_left, arm1_left, chara);
    joint->setColor(MAT_GREEN);
    //interestJoint = joint;
    joint = addJointBall(hand_left->initialPosition->addY( handDimension.getY()/2 ), arm1_left, hand_left, chara);
    joint->setColor(MAT_GREEN);


    joint = addJointBall(leg2_right->initialPosition->addY( legDimension.getY()/2 ), body_bot, leg2_right, chara);
    joint = addJointBall(leg1_right->initialPosition->addY( legDimension.getY()/2 ), leg2_right, leg1_right, chara);
    joint = addJointBall(foot_right->initialPosition->addY( footDimension.getY()/2 ).addZ((legDimension.getZ()-footDimension.getZ())/2), leg1_right, foot_right, chara);

    joint = addJointBall(arm2_right->initialPosition->addY( armDimension.getY()/2 ).addX( armDimension.getX()/2 ), body_top, arm2_right, chara);
    joint = addJointBall(arm1_right->initialPosition->addY( armDimension.getY()/2 ), arm2_right, arm1_right, chara);
    joint = addJointBall(hand_right->initialPosition->addY( handDimension.getY()/2 ), arm1_right, hand_right, chara);

    joint = addJointBall(head->initialPosition->addY( -headDimension.getY()/2 ), body_top, head, chara);
    joint->setColor(MAT_CYAN);
    joint = addJointBall(body_bot->initialPosition->addY( +bodyDimension.getY()/4 ), body_bot, body_top, chara);
    joint->setColor(MAT_CYAN);

    //*/

    //saveFile("");
}

Scene::~Scene(){
    Physics::closeScene(this);

//    if(externalForce != NULL){
//        delete externalForce;
//    }
//    if(externalTorque != NULL){
//        delete externalTorque;
//    }

//    if(camera!=NULL){
//        delete camera;
//    }

//    std::vector<Character*> characters;
//    //vector<Contact*> contacts;

//    while(!objects.empty()){
//        delete objects.back();
//        objects.pop_back();
//    }

//    while(!particles.empty()){
//        delete particles.back();
//        particles.pop_back();
//    }

//    while(!particleEngines.empty()){
//        delete particleEngines.back();
//        particleEngines.pop_back();
//    }

//    while(!characters.empty()){
//        delete characters.back();
//        characters.pop_back();
//    }
}

Object* Scene::addObject(Vec4 properties, Vec4 position, Quaternion rotation,int type,float mass,Character *character)
{
    srand(time(NULL));
    int r = rand() %22;
    Object *obj = new Object(this);//position,rotation,properties,type,this);
    obj->setMaterial(r);
    obj->setType(type);
    obj->setPosition(position);
    obj->setRotation(rotation);
    obj->setProperties(Vec4(properties.x(),properties.y(),properties.z()));
    obj->setFMass(mass);
    //obj->setScene(this);

    if(character == 0){
        objects.push_back(obj);
        Physics::createObject(obj,this->getSpace(), mass, position, rotation);
    }else{
        character->objects.push_back(obj);
        Physics::createObject(obj, character->getSpace(), mass, position, rotation);
    }

    return obj;
}

void Scene::restartPhysics()
{
    //    for (unsigned int i=0;i<objects.size();i++){
    //        Physics::createObject(objects.at(i), this->space, objects.at(i)->getFMass(), objects.at(i)->getPosition(), objects.at(i)->getRotation());
    //    }
//    for (unsigned int i=0;i<characters.size();i++){
//          Physics::closeCharacter(characters.at(i));
//    }
    Physics::closeScene(this);
    Physics::initScene(this);
    for (unsigned int i=0;i<objects.size();i++){
        Physics::createObject(objects.at(i), this->space, objects.at(i)->getFMass(), objects.at(i)->getPosition(), objects.at(i)->getRotation());
    }
    for (unsigned int i=0;i<characters.size();i++){
        characters.at(i)->restartPhysics();
        characters.at(i)->checkContactFoot(true);
    }
    if(enableGravity) Physics::setGravity(this,this->gravity);
    else Physics::setGravity(this,Vec4());

}

void Scene::initPhysics()
{
    Physics::initScene(this);
}

void Scene::stopPhysics()
{
    sim_status = false;
}

void Scene::startPhysics()
{
    sim_status = true;
}

void Scene::simulationStep()
{
  //  dBodyEnable(asdfg->body);
//    for(int i=0;i<50;i++){
//        for(std::vector<Object*>::iterator it = objects.begin(); it!=objects.end(); it++){
//            dBodyEnable((*it)->getBody());
//            //(*it)->appForce(externalForce);
//            //(*it)->appTorque(externalTorque);
//        }

////        if(interestJoint!=NULL){
////            Physics::ControlPDBallRubens(interestJoint,tarQ,ks,kd);
////        }

//        Physics::simSingleStep(this);
//    }
//Physics::simSingleStep(this);
    //Physics::simSingleStep(this);
    if(!sim_status) return;
    if(characters.size()>0)
        if (status_motion)
            if(characters.at(0)->getMoCap()->sizeFrames()>0){
                if (frame_step>characters.at(0)->getMoCap()->sizeFrames()){
                    frame_step = 1;
                    characters.at(0)->getMoCap()->initializePosesModel(0);
                }
                //float percent = frame_step/(characters.at(0)->getMoCap()->sizeFrames()*1.);
                characters.at(0)->getMoCap()->stepFrame(frame_step);
                frame_step+=2;
            }
    std::vector<Object*> objs = objectsScene();
    std::vector<Joint*> jts = jointsScene();
    for(int i=0;i<this->sim_step;i++){
        if(characters.size()>0)
//            if (status_motion)
//                if(characters.at(0)->getMoCap()->sizeFrames()>0){
//                    characters.at(0)->getMoCap()->physicsFootStep(Vec4(-1.0,0,0));
//                    characters.at(0)->getMoCap()->enableFoots();
//                }
           for(int i=0;i<objs.size();i++){
               Physics::setEnableObject(objs.at(i));
               if ((objs.at(i)->isSelected()) && !apply){
                   objs.at(i)->addForce(this->externalForce);
               }
           }
           for(unsigned int k=0;k<jts.size();k++) Physics::setEnableJoint(jts.at(k));
           for(unsigned int j=0;j<characters.size();j++){

               for(std::vector<ControlPD*>::iterator it = characters.at(j)->controllers.begin(); it!=characters.at(j)->controllers.end(); it++){
                   (*it)->evaluate();
               }
               if(characters.at(j)->balance!=NULL && enableGravity){
                   characters.at(j)->balance->evaluate();
                   characters.at(j)->checkContactFoot();
               }

           }
           Physics::simSingleStep(this);
        }
        apply = true;


}

void Scene::draw()
{

//    for(std::vector<Object*>::iterator it = objects.begin(); it!= objects.end(); it++){
//        (*it)->draw();
//    }
    //select->getPositionCurrent().showVec4();
    for(std::vector<Character*>::iterator it = characters.begin(); it!= characters.end(); it++){
        (*it)->draw();
    }
    for(std::vector<Object*>::iterator it = objects.begin(); it!= objects.end(); it++){
        (*it)->draw();
    }
    if(externalForce.module()!=0){
            std::vector<Object*> objs = objectsScene();
        for(int i=0;i<objs.size();i++)
            if (objs.at(i)->isSelected()){
                Draw::drawArrow(objs.at(i)->getPositionCurrent(),this->externalForce.unitary(),0.5);
            }

    }
    //Draw::drawPoint(Vec4(0,7,0));

//    glDisable(GL_DEPTH_TEST);
//    for(std::list<Object*>::iterator it = selectedObjects.begin(); it!=selectedObjects.end(); it++){
//        (*it)->drawSelected();
//    }
//    glEnable(GL_DEPTH_TEST);
}

SpaceID Scene::getSpace()
{
    return this->space;
}

void Scene::setSpace(dSpaceID space)
{
    this->space = space;
}

WorldID Scene::getWorld()
{
    return this->world;
}

void Scene::setWorld(dWorldID world)
{
    this->world = world;

}

dJointGroupID Scene::getJointGroup()
{
    return this->contactGroup;
}

void Scene::setJointGroup(dJointGroupID contactGroup)
{
    this->contactGroup = contactGroup;
}

void Scene::setGravityParameters(Vec4 g)
{
    this->gravity = g;
}

Vec4 Scene::getGravity()
{
    return this->gravity;
}


void Scene::setGravity(bool b)
{
    this->enableGravity = b;
    if(enableGravity) Physics::setGravity(this,this->gravity);
    else Physics::setGravity(this,Vec4());
}

void Scene::setSimStep(int sim_step)
{
    this->sim_step = sim_step;
}

int Scene::getSimStep()
{
    return this->sim_step;
}

std::vector<GRF> Scene::getGroundForces()
{
    return groundForces;
}

void Scene::addGroundForce(GRF grf)
{
    groundForces.push_back(grf);
}

void Scene::clearGroundForces()
{
    for (unsigned int i=0;i<groundForces.size();i++) {
        delete groundForces[i].jtFb;
    }
    groundForces.clear();
}

Joint* Scene::addJointBall(Vec4 anchor, Object *parent, Object *child, Character *chara, Vec4 limSup, Vec4 limInf)
{
    Joint *joint = NULL;
    if(chara != NULL){
        joint = new Joint(chara,JOINT_BALL);
        joint->setParent(parent);
        joint->setChild(child);
        joint->initJoint(anchor);

    }
    return joint;
}

Joint *Scene::addJointFixed(Object *parent,Character *chara)
{
    Joint *joint = NULL;
    if(chara != NULL){
        joint = new Joint(chara,JOINT_FIXED);
        joint->setParent(parent);
        joint->initJoint();
    }
    return joint;
}

std::vector<Object*> Scene::objectsScene()
{
    std::vector<Object*> allobjetcs;
    for(int i=0;i<characters.size();i++) for(int j=0;j<characters.at(i)->objects.size();j++) allobjetcs.push_back(characters.at(i)->objects.at(j));
    for(int i=0;i<objects.size();i++) allobjetcs.push_back(objects.at(i));
    return allobjetcs;
}

Object *Scene::selectedObject()
{
    std::vector<Object*> allobjetcs;
    for(int i=0;i<characters.size();i++) for(int j=0;j<characters.at(i)->objects.size();j++) allobjetcs.push_back(characters.at(i)->objects.at(j));
    for(int i=0;i<objects.size();i++) allobjetcs.push_back(objects.at(i));
    for(int i=0;i<allobjetcs.size();i++) if(allobjetcs.at(i)->isSelected()) return allobjetcs.at(i);
    return NULL;
}

std::vector<Joint*> Scene::jointsScene()
{
    std::vector<Joint*> alljoints;
    //QString s;
    //s.toLocal8Bit().data()
    //characters.at(0)->joints.at(0)->getChild();
    //printf("\num juntas: %s",characters.at(0)->joints.at(1)->getName().toLocal8Bit().data());
    for(unsigned int i=0;i<this->characters.size();i++) for(unsigned int j=0;j<this->characters.at(i)->joints.size();j++) alljoints.push_back(this->characters.at(i)->joints.at(j));
    return alljoints;
}

Joint *Scene::selectedJoint()
{
    std::vector<Joint*> alljoints;
    for(unsigned int i=0;i<characters.size();i++) for(unsigned int j=0;j<characters.at(i)->joints.size();j++) alljoints.push_back(characters.at(i)->joints.at(j));
    for(unsigned int i=0;i<alljoints.size();i++) if(alljoints.at(i)->isSelected()) return alljoints.at(i);
    return NULL;
}

void Scene::applyForce(Vec4 force)
{
    Object *object = selectedObject();
    if(object!=NULL){
        apply = false;
        object->addForce(force);
    }

}

void Scene::clear()
{
    objects.clear();
    characters.clear();
    Physics::closeScene(this);

}

Object *Scene::getObject(QString name)
{
    for(unsigned int i=0;i<objects.size();i++)
        if (objects.at(i)->getName() == name) return objects.at(i);
    return NULL;
}

void Scene::addCharacter(Character *chara)
{
    this->characters.push_back(chara);
}

void Scene::setExternalForce(Vec4 force)
{
    apply = false;
    this->externalForce = force;
}

Vec4 Scene::getExternalForce()
{
    return this->externalForce;
}

void Scene::setProportionalKsPD(Vec4 ks)
{
    this->propKs = ks;
    for(unsigned int i=0;i<characters.size();i++)
        for(unsigned int j=0;j<characters.at(i)->controllers.size();j++)
            characters.at(i)->controllers.at(j)->setProportionalKs(propKs);
}

Vec4 Scene::getProportionalKsPD()
{
    return propKs;
}

void Scene::setProportionalKdPD(Vec4 kd)
{
    this->propKd = kd;
    for(unsigned int i=0;i<characters.size();i++)
        for(unsigned int j=0;j<characters.at(i)->controllers.size();j++)
            characters.at(i)->controllers.at(j)->setProportionalKd(propKd);
}

Vec4 Scene::getProportionalKdPD()
{
    return propKd;
}

Character *Scene::getCharacter(int i)
{
    return this->characters.at(i);
}

int Scene::getSizeCharacter()
{
    return this->characters.size();
}

void Scene::shootObject(Object *obj, int type, Vec4 begin)
{

}

void Scene::setEnableTorqueBalance(bool b)
{
    if (characters.size()==0) return;
    if (characters.at(0)->getBalance()==NULL) return;
    return characters.at(0)->getBalance()->setEnableTorque(b);
}

void Scene::setEnableForceBalance(bool b)
{
    if (characters.size()==0) return;
    if (characters.at(0)->getBalance()==NULL) return;
    return characters.at(0)->getBalance()->setEnableForce(b);
}

void Scene::setEnableMomentumBalance(bool b)
{
    if (characters.size()==0) return;
    if (characters.at(0)->getBalance()==NULL) return;
    return characters.at(0)->getBalance()->setEnableMomentum(b);
}

Vec4 Scene::getKsForceBalance()
{
    //neste caso estou fazendo a chamada do objeto caractere de valor 0, depois deixaremos esta função mais geral
    if (characters.size()==0) return Vec4();
    if (characters.at(0)->getBalance()==NULL) return Vec4();
    return characters.at(0)->getBalance()->getKsForce();
}

void Scene::setKsForceBalance(Vec4 ks)
{
    //neste caso estou fazendo a chamada do objeto caractere de valor 0, depois deixaremos esta função mais geral
    if (characters.size()==0) return;
    if (characters.at(0)->getBalance()==NULL) return;
    characters.at(0)->getBalance()->setKsForce(ks);
}

Vec4 Scene::getKdForceBalance()
{
    //neste caso estou fazendo a chamada do objeto caractere de valor 0, depois deixaremos esta função mais geral
    if (characters.size()==0) return Vec4();
    if (characters.at(0)->getBalance()==NULL) return Vec4();
    return characters.at(0)->getBalance()->getKdForce();
}

void Scene::setKdForceBalance(Vec4 kd)
{
    //neste caso estou fazendo a chamada do objeto caractere de valor 0, depois deixaremos esta função mais geral
    if (characters.size()==0) return;
    if (characters.at(0)->getBalance()==NULL) return;
    characters.at(0)->getBalance()->setKdForce(kd);
}

Vec4 Scene::getKsTorqueBalance()
{
    //neste caso estou fazendo a chamada do objeto caractere de valor 0, depois deixaremos esta função mais geral
    if (characters.size()==0) return Vec4();
    if (characters.at(0)->getBalance()==NULL) return Vec4();
    return characters.at(0)->getBalance()->getKsTorque();
}

void Scene::setKsTorqueBalance(Vec4 ks)
{
    //neste caso estou fazendo a chamada do objeto caractere de valor 0, depois deixaremos esta função mais geral
    if (characters.size()==0) return;
    if (characters.at(0)->getBalance()==NULL) return;
    characters.at(0)->getBalance()->setKsTorque(ks);
}

Vec4 Scene::getKdTorqueBalance()
{
    //neste caso estou fazendo a chamada do objeto caractere de valor 0, depois deixaremos esta função mais geral
    if (characters.size()==0) return Vec4();
    if (characters.at(0)->getBalance()==NULL) return Vec4();
    return characters.at(0)->getBalance()->getKdTorque();
}

void Scene::setKdTorqueBalance(Vec4 kd)
{
    //neste caso estou fazendo a chamada do objeto caractere de valor 0, depois deixaremos esta função mais geral
    if (characters.size()==0) return;
    if (characters.at(0)->getBalance()==NULL) return;
    return characters.at(0)->getBalance()->setKdTorque(kd);
}

Vec4 Scene::getKMomBalance()
{
    if (characters.size()==0) return Vec4();
    if (characters.at(0)->getBalance()==NULL) return Vec4();
    return characters.at(0)->getBalance()->getKMomentum();
}

void Scene::setKMomBalance(Vec4 kmom)
{
    if (characters.size()==0) return;
    if (characters.at(0)->getBalance()==NULL) return;
    characters.at(0)->getBalance()->setKMomentum(kmom);
}

void Scene::setCompensacao(int value)
{
    if (characters.size()==0) return;
    if (characters.at(0)->getBalance()==NULL) return;
    characters.at(0)->getBalance()->setCompensation(value/100.);
}

void Scene::setAlphaCharacter(float val)
{
    if (characters.size()==0) return;
    characters.at(0)->setAlpha(val);
}

void Scene::setWireCharacter(bool b)
{
    if (characters.size()==0) return;
    characters.at(0)->setWireframe(b);
}

void Scene::setAngleBodyBalance(Vec4 v)
{
    if (characters.size()==0) return;
    if (characters.at(0)->getBalance()==NULL) return;
    characters.at(0)->getBalance()->setDeriredQuaternion(v);
}

void Scene::restartMotionCapture()
{
    frame_step = 1;
}

void Scene::statusMotionCapture(bool b)
{
    status_motion = b;
}


