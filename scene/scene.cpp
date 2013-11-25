#include "scene.h"
#include "object.h"
#include "cube.h"
#include "interface/glwidget.h"
#include "control/sensor.h"
#include "character.h"
#include "joint.h"
#include <time.h>
#include <stdlib.h>
#include "graphics/draw.h"
bool apply = true;


Scene::Scene(GLWidget *parent)
{
    this->sim_status = true;
    this->parent = parent;
    Physics::initScene(this);
    this->sim_step = 67;
    this->frame_step = 1;
    this->status_motion = false;
    this->enableGravity = false;
    this->externalForce = Vec4();
    this->propKs = Vec4(1,1,1);
    this->propKd = Vec4(1,1,1);
    this->show_grf = true;
}

Scene::~Scene(){
    Physics::closeScene(this);
}

Object* Scene::addObject(Vec4 properties, Vec4 position, Quaternion rotation,int type,float mass,Character *character,int material)
{
    //srand(time(NULL));
    //int r = rand() %22;
    Object *obj = new Object(this);//position,rotation,properties,type,this);
    obj->setMaterial(material);
    obj->setType(type);
    obj->setPosition(position);
    obj->setRotation(rotation);
    obj->setProperties(Vec4(properties.x(),properties.y(),properties.z()));
    obj->setFMass(mass);
    //obj->setScene(this);

    if(character == 0){
        objects.push_back(obj);
        Physics::createObject(obj,this->getSpace(), mass, position, rotation);
        obj->setScene(this);
    }else{
        character->objects.push_back(obj);
        obj->setCharacter(character);
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
    objects_shoot.clear();
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
    if(!sim_status) return;
    if(characters.size()>0)
        if (status_motion)
            if(characters.at(0)->getMoCap()->sizeFrames()>0 && getCharacter(0)->getMoCap()->status){
                if (frame_step<characters.at(0)->getMoCap()->getBeginClycle()) frame_step = characters.at(0)->getMoCap()->getBeginClycle();
                if (frame_step>=characters.at(0)->getMoCap()->getEndClycle()){
                    frame_step = 1+characters.at(0)->getMoCap()->getBeginClycle();
                    if(frame_step == 1){
                        if(this->getCharacter(0)->offset.module()==0) characters.at(0)->getMoCap()->initializePosesModel(0);
                        this->getCharacter(0)->getBalance()->setEnableBalance(true);
                    }
                }
                frame_step+=2;
                if (frame_step>=characters.at(0)->getMoCap()->getEndClycle()){
                    frame_step = 1+characters.at(0)->getMoCap()->getBeginClycle();
                    if(frame_step == 1){
                        if(this->getCharacter(0)->offset.module()==0) characters.at(0)->getMoCap()->initializePosesModel(0);
                        this->getCharacter(0)->getBalance()->setEnableBalance(true);
                    }
                }
                characters.at(0)->getMoCap()->stepFrame(frame_step);
            }
    std::vector<Object*> objs = objectsScene();
    std::vector<Joint*> jts = jointsScene();
    for(int i=0;i<this->sim_step;i++){
        if(characters.size()>0)
           for(unsigned int i=0;i<objs.size();i++){
               Physics::setEnableObject(objs.at(i));
               if ((objs.at(i)->isSelected()) && !apply){
                   objs.at(i)->addForce(this->externalForce);
               }
               objs.at(i)->evaluate(1);

           }
           for(unsigned int k=0;k<jts.size();k++) Physics::setEnableJoint(jts.at(k));
           for(unsigned int j=0;j<characters.size();j++){
               if(characters.at(j)->balance!=NULL && enableGravity){
                   characters.at(j)->balance->evaluate();
               }else{
                   for(std::vector<ControlPD*>::iterator it = characters.at(j)->controllers.begin(); it!=characters.at(j)->controllers.end(); it++){
                       (*it)->evaluate();
                   }
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
    for(std::vector<Object*>::iterator it = objects_shoot.begin(); it!= objects_shoot.end(); it++){
        (*it)->draw();
    }

    if(externalForce.module()!=0){
        std::vector<Object*> objs = objectsScene();
        for(unsigned int i=0;i<objs.size();i++)
            if (objs.at(i)->isSelected()){
                Draw::drawArrow(objs.at(i)->getPositionCurrent(),this->externalForce.unitary(),0.5);
            }
    }
    if (characters.size()>0)
       if (show_grf) GRF::drawGRF(groundForces,getCharacter(0)->getPosCOM());
    //Draw::drawPoint(Vec4(0,7,0));

//    glDisable(GL_DEPTH_TEST);
//    for(std::list<Object*>::iterator it = selectedObjects.begin(); it!=selectedObjects.end(); it++){
//        (*it)->drawSelected();
//    }
    //    glEnable(GL_DEPTH_TEST);
}

void Scene::drawGRF(bool b)
{
    show_grf = b;
}

void Scene::drawShadows()
{
    for(std::vector<Object*>::iterator it = objects_shoot.begin(); it!= objects_shoot.end(); it++){
        (*it)->draw();
    }
    for(std::vector<Character*>::iterator it = characters.begin(); it!= characters.end(); it++){
        (*it)->drawShadows();
    }



}

void Scene::loadSceneObjects()
{                                    //x   y    z

    //srand(time(NULL));
    addObject(Vec4(0.02,0.02,0.02),Vec4(0,0.020,0),Quaternion(),TYPE_SPHERE,2,0,MATERIAL_COPPER);
    addObject(Vec4(0.02,0.02,0.02),Vec4(0.2,0.020,0),Quaternion(),TYPE_SPHERE,2,0,MATERIAL_COPPER);
    addObject(Vec4(0.02,0.02,0.02),Vec4(0,0.020,-0.2),Quaternion(),TYPE_SPHERE,2,0,MATERIAL_COPPER);
    addObject(Vec4(0.02,0.02,0.02),Vec4(-0.5,0.020,0.5),Quaternion(),TYPE_SPHERE,2,0,MATERIAL_COPPER);
    addObject(Vec4(0.02,0.02,0.02),Vec4(1.0,0.020,-2.0),Quaternion(),TYPE_SPHERE,2,0,MATERIAL_COPPER);
    addObject(Vec4(0.02,0.02,0.02),Vec4(1.70,0.020,-0.70),Quaternion(),TYPE_SPHERE,2,0,MATERIAL_COPPER);
    addObject(Vec4(0.02,0.02,0.02),Vec4(-1.30,0.020,0.20),Quaternion(),TYPE_SPHERE,2,0,MATERIAL_COPPER);
    addObject(Vec4(0.02,0.02,0.02),Vec4(-2.0,0.020,-1.20),Quaternion(),TYPE_SPHERE,2,0,MATERIAL_COPPER);

    float z = -5;
    float y = 0.35;
    float x = 0;

//    addObject(Vec4(0.5,0.5,0.5),Vec4(2.34+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
//    addObject(Vec4(0.5,0.5,0.5),Vec4(2.87+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
//    addObject(Vec4(0.5,0.5,0.5),Vec4(3.38+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
//    addObject(Vec4(0.5,0.5,0.5),Vec4(3.91+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(0.78+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(1.31+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(1.81+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(0.25+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-0.27+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-0.78+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-1.31+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-1.81+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
//    addObject(Vec4(0.5,0.5,0.5),Vec4(-2.34+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
//    addObject(Vec4(0.5,0.5,0.5),Vec4(-2.87+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
//    addObject(Vec4(0.5,0.5,0.5),Vec4(-3.38+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
//    addObject(Vec4(0.5,0.5,0.5),Vec4(-3.91+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);

    z = -4.49;
    y = 0.35;
    x = 0.05;
    addObject(Vec4(0.5,0.5,0.5),Vec4(0.78+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(1.31+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(1.81+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(0.25+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-0.27+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-0.78+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-1.31+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-1.81+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    z = -3.94;

    x = -0.05;
    addObject(Vec4(0.5,0.5,0.5),Vec4(0.78+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(1.31+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(1.81+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(0.25+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-0.27+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-0.78+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-1.31+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-1.81+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    z = -3.38;

    x = -0.02;
    addObject(Vec4(0.5,0.5,0.5),Vec4(0.78+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(1.31+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(1.81+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(0.25+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-0.27+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-0.78+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-1.31+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-1.81+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    z = -2.85;

    x = -0.02;
    addObject(Vec4(0.5,0.5,0.5),Vec4(0.78+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(1.31+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(1.81+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(0.25+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-0.27+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-0.78+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-1.31+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-1.81+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    z = -2.31;

    x = -0.05;
    addObject(Vec4(0.5,0.5,0.5),Vec4(0.78+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(1.31+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(1.81+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(0.25+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-0.27+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-0.78+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-1.31+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-1.81+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    z = -1.79;

    x = 0.02;
    addObject(Vec4(0.5,0.5,0.5),Vec4(0.78+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(1.31+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(1.81+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(0.25+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-0.27+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-0.78+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-1.31+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-1.81+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    z = -1.25;

    x = 0.05;
    addObject(Vec4(0.5,0.5,0.5),Vec4(0.78+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(1.31+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(1.81+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(0.25+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-0.27+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-0.78+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-1.31+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-1.81+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    z = -0.74;

    x = -0.01;
    addObject(Vec4(0.5,0.5,0.5),Vec4(0.78+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(1.31+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(1.81+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(0.25+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-0.27+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-0.78+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-1.31+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-1.81+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);


    z = -0.2;

    x = 0.02;
    addObject(Vec4(0.5,0.5,0.5),Vec4(0.78+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(1.31+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(1.81+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(0.25+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-0.27+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-0.78+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-1.31+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-1.81+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    z = 0.34;

    x = 0.03;
    addObject(Vec4(0.5,0.5,0.5),Vec4(0.78+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(1.31+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(1.81+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(0.25+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-0.27+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-0.78+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-1.31+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-1.81+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    z = 0.85;

    x = -0.07;
    addObject(Vec4(0.5,0.5,0.5),Vec4(0.78+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(1.31+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(1.81+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(0.25+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-0.27+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-0.78+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-1.31+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-1.81+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    z = 1.37;

    x = 0.01;
    addObject(Vec4(0.5,0.5,0.5),Vec4(0.78+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(1.31+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(1.81+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(0.25+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-0.27+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-0.78+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-1.31+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-1.81+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    z = 1.88;

    x = 0.05;
    addObject(Vec4(0.5,0.5,0.5),Vec4(0.78+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(1.31+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(1.81+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(0.25+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-0.27+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-0.78+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-1.31+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-1.81+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    z = 2.4;

    x = -0.02;
    addObject(Vec4(0.5,0.5,0.5),Vec4(0.78+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(1.31+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(1.81+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(0.25+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-0.27+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-0.78+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-1.31+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);
    addObject(Vec4(0.5,0.5,0.5),Vec4(-1.81+x,y,z),Quaternion(),TYPE_CUBE,10,0,MATERIAL_TURQUOSIE);


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

bool Scene::hasGravity()
{
    return enableGravity;
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

Joint* Scene::addJointBall(Vec4 anchor, Object *parent, Object *child, Character *chara, Vec4 /*limSup*/, Vec4/* limInf*/)
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
    for(unsigned int i=0;i<characters.size();i++) for(unsigned int j=0;j<characters.at(i)->objects.size();j++) allobjetcs.push_back(characters.at(i)->objects.at(j));
    //for(unsigned int i=0;i<objects.size();i++) allobjetcs.push_back(objects.at(i));
    for(unsigned int i=0;i<objects_shoot.size();i++) allobjetcs.push_back(objects_shoot.at(i));
    return allobjetcs;
}

Object *Scene::selectedObject()
{
    std::vector<Object*> allobjetcs;
    for(unsigned int i=0;i<characters.size();i++) for(unsigned int j=0;j<characters.at(i)->objects.size();j++) allobjetcs.push_back(characters.at(i)->objects.at(j));
    for(unsigned int i=0;i<objects.size();i++) allobjetcs.push_back(objects.at(i));
    for(unsigned int i=0;i<allobjetcs.size();i++) if(allobjetcs.at(i)->isSelected()) return allobjetcs.at(i);
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

Object *Scene::getObject(dBodyID id)
{
    std::vector<Object*> objs = objectsScene();
    for(unsigned int i=0;i<objs.size();i++)
        if (id == objs.at(i)->getBody()) return objs.at(i);
    return NULL;
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

Vec4 Scene::getKMomLinearBalance()
{
    if (characters.size()==0) return Vec4();
    if (characters.at(0)->getBalance()==NULL) return Vec4();
    return characters.at(0)->getBalance()->getKMomentumLinear();
}

void Scene::setKMomLinearBalance(Vec4 kmom)
{
    if (characters.size()==0) return;
    if (characters.at(0)->getBalance()==NULL) return;
    characters.at(0)->getBalance()->setKMomentumLinear(kmom);
}

Vec4 Scene::getKMomAngularBalance()
{
    if (characters.size()==0) return Vec4();
    if (characters.at(0)->getBalance()==NULL) return Vec4();
    return characters.at(0)->getBalance()->getKMomentumAngular();
}

void Scene::setKMomAngularBalance(Vec4 kmom)
{
    if (characters.size()==0) return;
    if (characters.at(0)->getBalance()==NULL) return;
    characters.at(0)->getBalance()->setKMomentumAngular(kmom);

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

void Scene::shotBallsCharacterRandom(Character *chara,int posPelvis)
{
    //if ( inicio.modulo() == 0.0 ) {
    //srand(time(NULL));
    //int r = rand() % chara->getNumBodies();
    Vec4 posPelvisModel = chara->getBody(posPelvis)->getPositionCurrent();
    //float mass = chara->getBody(posPelvis)->getFMass();
    //this->objetos.push_back( new Objeto( Simulation::world, Simulation::space, posPelvisModelo, mass.mass, influExtSimFF, true, SPHERE ) ); //para a geometria do objeto ser aleatoria, basta nao definir o ultimo parametro
    //inicio
       Vec4 directionFromCenter = Vec4( dRandReal()*2.0-1.0, dRandReal()*2.0-1.0, dRandReal()*2.0-1.0 );
         if ( directionFromCenter.module() == 0.0 ) {
           directionFromCenter = Vec4(0.0,1.0,0.0);
         } else {
           directionFromCenter.normalize();
         }
       Vec4 inicio = posPelvisModel+(directionFromCenter*(1.0));
       if ( inicio.y() <= 0.1 ) {
           directionFromCenter.x2 = 0.0;
           directionFromCenter.normalize();
           inicio = posPelvisModel+(directionFromCenter*(1.0));
         }

     //velInicial
       Vec4 deltaCenter = Vec4(); //(0.0,0.0,0.0);

         deltaCenter = Vec4( dRandReal()*0.3-0.15, dRandReal()*1.6-0.75, dRandReal()*0.3-0.15 );

       Vec4 directionVelInicial = posPelvisModel+(deltaCenter)-(inicio);
         //directionVelInicial.normaliza();
       Vec4 velInicial = directionVelInicial*( 15.0 );//dRandReal()*50.0+10.0 );
       dReal massTotal;
       //if ( pelvisIsTheTarget ) {
       massTotal = 0.75;
       Object *obj = new Object(this);//position,rotation,properties,type,this);
       obj->setMaterial(MATERIAL_COPPER);
       obj->setType(TYPE_SPHERE);
       obj->setPosition(inicio);
       obj->setRotation(Quaternion());
       obj->setProperties(Vec4(0.1,0.1,0.1));
       obj->setFMass(massTotal);

       Physics::createObject(obj,space,massTotal,inicio,velInicial);
       objects_shoot.push_back(obj);


     //usar a massa total ao inves da densidade
       //assim pode ficar legal deixando a geometria do objeto aleatoria
       //definir a massa total proporcional à massa do corpo alvo
     //considerando cilindro
         //dReal density = 1000.0;//dRandReal()*6000.0+1000.0; //melhor para varias pancadas de uma vez
         //dReal density = 2000.0;//dRandReal()*6000.0+1000.0; //melhor para pancadas individuais
     //massa

//       else {
//         massTotal = fmin(2.0*massBody,5.0);
//       }

     //bodyGeom
//       dBodyID body;
//       dGeomID geom;
//       //dMatrix3 R;
//         //dRFromAxisAndAngle (R,dRandReal()*2.0-1.0,dRandReal()*2.0-1.0,dRandReal()*2.0-1.0,dRandReal()*10.0-5.0);
//       dReal sides[3];
//         for (int k=0; k<3; k++) { sides[k] = 0.07; }//dRandReal()*0.02+0.07; }
//         //dBody

//         body = dBodyCreate(world);
//           dBodySetPosition(body, this->inicio[0], this->inicio[1], this->inicio[2]);
//           //dBodySetRotation (body,R);

//         if (tipo == SPHERE) {
//           //dGeom
//           geom = dCreateSphere(space,sides[0]);
//           //dMass
//           dMass mass;
//           dMassSetSphereTotal (&mass,massTotal,sides[0]);
//             dBodySetMass(body, &mass);
//         }
//         if (tipo == BOX) {
//           //dGeom
//           geom = dCreateBox (space,2.0*sides[0],2.0*sides[1],2.0*sides[2]);
//           //dMass
//           dMass mass;
//           dMassSetBoxTotal (&mass,massTotal,2.0*sides[0],2.0*sides[1],2.0*sides[2]);
//             dBodySetMass(body, &mass);
//         }
//         if (tipo == CCYLINDER) {
//           //dGeom
//           geom = dCreateCCylinder (space,sides[0],sides[1]);
//           //dMass
//           dMass mass;
//           dMassSetCappedCylinderTotal (&mass,massTotal,3,sides[0],sides[1]);
//             dBodySetMass(body, &mass);
//         }
//         dGeomSetBody (geom,body);
//       this->bodyGeom = new BodyGeom(body,geom);
//       //guardando o ponteiro objetos nessas geometrias para impedir colisoes entre os objetos
//         dGeomSetData (this->bodyGeom->geom,objetos);

//     //bodyGeomFF
//       //30% de chance de considerar o impacto na simFF
//       //if (dRandReal() > 0.7) {
//       if (influExtSimFF) {
//         //bodyGeomFF
//           //dBody
//           body = dBodyCreate(world);
//             dBodySetPosition(body, this->inicio[0]+1.0, this->inicio[1], this->inicio[2]-1.0);
//             //dBodySetRotation (body,R);
//             dBodySetLinearVel(body, this->velInicial[0], this->velInicial[1], this->velInicial[2]);
//           if (tipo == SPHERE) {
//             //dGeom
//             geom = dCreateSphere(space,sides[0]);
//             //dMass
//             dMass mass;
//             dMassSetSphereTotal (&mass,massTotal,sides[0]);
//               dBodySetMass(body, &mass);
//           }
//           if (tipo == BOX) {
//             //dGeom
//             geom = dCreateBox (space,2.0*sides[0],2.0*sides[1],2.0*sides[2]);
//             //dMass
//             dMass mass;
//             dMassSetBoxTotal (&mass,massTotal,2.0*sides[0],2.0*sides[1],2.0*sides[2]);
//               dBodySetMass(body, &mass);
//           }
//           if (tipo == CCYLINDER) {
//             //dGeom
//             geom = dCreateCCylinder (space,sides[0],sides[1]);
//             //dMass
//             dMass mass;
//             dMassSetCappedCylinderTotal (&mass,massTotal,3,sides[0],sides[1]);
//               dBodySetMass(body, &mass);
//           }
//           dGeomSetBody (geom,body);
//         this->bodyGeom->bodyGeomFF = new BodyGeom(body,geom);
//         //guardando o ponteiro objetosFF nessas geometrias para impedir colisoes entre os objetos
//           dGeomSetData (this->bodyGeom->bodyGeomFF->geom,objetosFF);
//       }

//      //enquanto vector objetos tiver mais do que 10 objetos, apaga o primeiro
      while ( this->objects_shoot.size() > 10 ) {
        this->objects_shoot.erase(objects_shoot.begin());
      }
}

bool Scene::isGeometryFootSwing(dGeomID geom)
{
    std::vector<Object*> obj = objectsScene();
    for(unsigned int i=0;i<obj.size();i++){
        if (obj.at(i)->getGeometry()==geom) return Sensor::isSwingFoot(obj.at(i));
    }
    return false;
}



