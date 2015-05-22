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
#include "omp.h"
#include <sys/time.h>
#include <stdio.h>
#include "mocap/framequat.h"
#include "mocap/frame.h"
#include "extra/utils.h"
#include "math/ray.h"
bool apply = true;
QList<QList <Vec4> > motion;
QList<QList <Vec4> > mocap;
QList<double> tempopass;
QList<double> sim_dist;
QList<double> mocap_dist;
bool initialize=false;
bool record = false;

Scene::Scene(GLWidget *parent)
{
    this->sim_status = true;
    this->parent = parent;
    Physics::initScene(this);
    this->sim_step = 67;
    this->frame_step = -1;
    this->status_motion = false;
    this->enableGravity = true;
    this->externalForce = Vec4();
    this->propKs = Vec4(1,1,1);
    this->propKd = Vec4(1,1,1);
    this->show_grf = true;
    this->gravity = Vec4(0,-9.8,0);
    motion.clear();
    mocap.clear();

    pose_time.start();

//    Object *ramp = addObject(Vec4(1.8,0.7,0.001),Vec4(0.2,0.1,1.5),Quaternion(Vec4(74,0,0)),TYPE_CUBE,1.2);
//    Object *cont = addObject(Vec4(1.8,0.20,0.5),Vec4(0.2,0.1,2.09),Quaternion(Vec4(0,0,0)),TYPE_CUBE,1.2);
//    Object *ramp2= addObject(Vec4(1.8,0.7,0.001),Vec4(0.2,0.1,2.68),Quaternion(Vec4(-74,0,0)),TYPE_CUBE,1.2);

//    objects.push_back(ramp);
//    objects.push_back(cont);
//    objects.push_back(ramp2);

    //objects.push_back(addObject(Vec4(0.2,0.2,0.2),Vec4(0,0.1,1.3),Quaternion(Vec4(0,0,0)),TYPE_CUBE,1.2));
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
        if (characters.at(i)->getBodiesFoot().size()>0)
            characters.at(i)->checkContactFoot(true);
    }
    if(enableGravity) Physics::setGravity(this,this->gravity);
    else Physics::setGravity(this,Vec4());
    //createLuxo();
    //createCharacter();

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

bool entrou=true;
double ti,tf,tempo; // ti = tempo inicial // tf = tempo final
timeval tempo_inicio;
void Scene::simulationStep(bool balance)
{
    if(!sim_status) return;
    Vec4 vel_des= Vec4();
    Vec4 mom_lin_des= Vec4();
    Vec4 mom_ang_des= Vec4();
    Joint* jDes = NULL; //junta que é aplicado o torque virtual
    float mass_total= 0;
    Quaternion qd = Quaternion();
    Vec4 vel_angd = Vec4();
    if(characters.size()>0){
        jDes = characters.at(0)->getJointParentBalance();
        mass_total = this->getCharacter(0)->getMassTotal();
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
                if((characters.at(0)->getMoCap()->sizeFrames()>0)&&(characters.at(0)->getMoCap()->status))
                    for(unsigned int i=0; i<characters.at(0)->getControllersPD().size();i++){
                        Quaternion des = characters.at(0)->getMoCap()->getFrameSimulation(frame_step)->quatDes.at(i);
                        Object* father = this->getCharacter(0)->getControllersPD().at(i)->getJoint()->getParent();
                        Object* child = this->getCharacter(0)->getControllersPD().at(i)->getJoint()->getChild();

                        Vec4 veldes = characters.at(0)->getMoCap()->velocityAngularBody(frame_step,characters.at(0)->getIdObject(child)) - characters.at(0)->getMoCap()->velocityAngularBody(frame_step,characters.at(0)->getIdObject(father));
                        characters.at(0)->getControllersPD().at(i)->setQuaternionWanted(des);
                        characters.at(0)->getControllersPD().at(i)->setVelocityDesired(veldes);
                        characters.at(0)->getControllersPD().at(i)->resetInertiaFactors();
                    }
                vel_des = characters.at(0)->getMoCap()->getVelCOM(frame_step);
                mom_lin_des = characters.at(0)->getMoCap()->getMomentumLinear(frame_step);
                mom_ang_des = characters.at(0)->getMoCap()->getMomentumAngular(frame_step);
                qd = characters.at(0)->getMoCap()->getFrameMotion(frame_step)->getOrientation(characters.at(0)->getIdObject(jDes->getParent()));
                vel_angd = characters.at(0)->getMoCap()->velocityAngularBody(frame_step,characters.at(0)->getIdObject(jDes->getParent()));

            }

    }
    std::vector<Object*> objs = objectsScene();
    std::vector<Joint*> jts = jointsScene();
    //#pragma omp parallel
//    timeval tempo_fim;
//    if (initialize && entrou){
//          ti = tf = tempo = 0;
//          gettimeofday(&tempo_inicio,NULL);
//          entrou = false;
//    }
    //#pragma omp parallel
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
               if(characters.at(j)->balance!=NULL && enableGravity && balance){
                   this->getCharacter(j)->getBalance()->evaluate(jDes,mass_total,frame_step,qd,vel_angd,vel_des,mom_lin_des,mom_ang_des);
                   for(std::vector<ControlPD*>::iterator it = characters.at(j)->controllers.begin(); it!=characters.at(j)->controllers.end(); it++){
                       (*it)->evaluate();
                   }
               }else{
                   for(std::vector<ControlPD*>::iterator it = characters.at(j)->controllers.begin(); it!=characters.at(j)->controllers.end(); it++){
                       (*it)->evaluate();
                   }
               }

           }
           Physics::simSingleStep(this);
        }
//    if(initialize){
//        gettimeofday(&tempo_fim,NULL);
//        tf = (double)tempo_fim.tv_usec + ((double)tempo_fim.tv_sec * (1000000.0));
//        ti = (double)tempo_inicio.tv_usec + ((double)tempo_inicio.tv_sec * (1000000.0));
//        tempo = (tf - ti) / 1000000;
////        for(int i = 0;i<getCharacter(0)->getNumJoints();i++){
////            Vec4 orient = (getCharacter(0)->getControllersPD().at(i)->getOrientation().toEuler())*(M_PI/180.);
////            Vec4 orientmocap = getCharacter(0)->getControllersPD().at(i)->getQuaternionWanted().toEuler()*(M_PI/180.);
////            motion[i].push_back(orient);
////            mocap[i].push_back(orientmocap);
////        }
//        sim_dist.push_back(getCharacter(0)->getBalance()->sim_dist);
//        mocap_dist.push_back(getCharacter(0)->getBalance()->mocap_dist);
//        tempopass.push_back(tempo);
//    }
//    if(record){
//        QString endmotion = "/home/danilo/GitHub/ODESys/results/Walking.dist";
//        Utils::saveListDistMotion(sim_dist,mocap_dist,tempopass,endmotion.toStdString());
////        for(int i=0;i<getCharacter(0)->getNumJoints();i++){
////            QString endmotion = "/home/danilo/GitHub/ODESys/results/" + getCharacter(0)->getJoint(i)->getName()+".tab";
////            Utils::saveListMotion(motion.at(i),mocap.at(i),tempopass,endmotion.toStdString());


////        }
//        record = false;

//    }

//      printf("Tempo gasto em milissegundos %.3f\n",tempo);
        apply = true;
 objs.clear();
 jts.clear();

    int elapsedTime = pose_time.elapsed();
    //printf("%d elapsed time\n", elapsedTime);
    for (std::vector<GraphicalPose*>::iterator i = this->poseControl.begin(); i < this->poseControl.end(); i++){
       (*i)->advanceTimeEnergic(elapsedTime);
    }
    pose_time.start();
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
        objs.clear();
    }

    if (show_grf)
//    if (characters.size()>0){
//        for(int i=0;i<getCharacter(0)->getNumBodies();i++){
//            if(getCharacter(0)->getBody(i)->getFoot()){
//                GRF::drawGRFObject(groundForces,getCharacter(0)->getBody(i));
//            }
//        }
//    }
        if (show_grf) GRF::drawGRF(groundForces,Vec4());


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

void Scene::setRenderMesh(bool b)
{
    std::vector<Object*> objs = objectsScene();
    for(unsigned int i=0;i<objs.size();i++)
        objs.at(i)->setRenderMesh(b);
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

void Scene::setViewer(Vec4 eye, Vec4 at, Vec4 up)
{
    viewer[0] = eye;
    viewer[1] = at;
    viewer[2] = up;
}

void Scene::setProjection(Vec4 p)
{
    projection = p;
}

void Scene::setWindow(int width, int height)
{
    this->width = width;
    this->height = height;
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

Joint* Scene::addJointHinge(Vec4 anchor, Vec4 axis, Object *parent, Object *child, Character *chara)
{
    Joint *joint = NULL;
    if (chara!=NULL){
        joint = new Joint(chara, JOINT_HINGE);
        joint->setParent(parent);
        joint->setChild(child);
        joint->initJoint(anchor, axis);
    }
    return joint;
}

Joint *Scene::addJointFixed(Object *parent,Object *child,Character *chara)
{

    Joint *joint = NULL;
    if(chara != NULL){
        joint = new Joint(chara,JOINT_FIXED);
        joint->setParent(parent);
        joint->setChild(child);
        joint->initJoint();
    }else{
        joint = new Joint(JOINT_FIXED);
        joint->setScene(this);
        joint->setParent(parent);
        joint->setChild(child);
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

std::vector<Object *> Scene::objectsSceneChara()
{
    std::vector<Object*> allobjetcs;
    for(unsigned int i=0;i<characters.size();i++) for(unsigned int j=0;j<characters.at(i)->objects.size();j++) allobjetcs.push_back(characters.at(i)->objects.at(j));
    //for(unsigned int i=0;i<objects.size();i++) allobjetcs.push_back(objects.at(i));
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
    sim_step = 0;
    frame_step = 0;
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
    std::vector<Object*> objs = objectsSceneChara();
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

Vec4 Scene::getKVelocityLocomotion()
{
    if (characters.size()==0) return Vec4();
    if (characters.at(0)->getBalance()==NULL) return Vec4();
    return characters.at(0)->getBalance()->getKVelocityLocomotion();
}

void Scene::setKVelocityLocomotion(Vec4 k)
{
    if (characters.size()==0) return;
    if (characters.at(0)->getBalance()==NULL) return;
    characters.at(0)->getBalance()->setKVelocityLocomotion(k);
}

Vec4 Scene::getKDistanceLocomotion()
{
    if (characters.size()==0) return Vec4();
    if (characters.at(0)->getBalance()==NULL) return Vec4();
    return characters.at(0)->getBalance()->getKDistanceLocomotion();
}

void Scene::setKDistanceLocomotion(Vec4 k)
{
    if (characters.size()==0) return;
    if (characters.at(0)->getBalance()==NULL) return;
    characters.at(0)->getBalance()->setKDistanceLocomotion(k);
}

void Scene::setLimitSteps(int value)
{
    if (characters.size()==0) return;
    if (characters.at(0)->getBalance()==NULL) return;
    characters.at(0)->getBalance()->setLimitSteps(value);
}

int Scene::getLimitSteps()
{
    if (characters.size()==0) return 0;
    if (characters.at(0)->getBalance()==NULL) return 0;
    return characters.at(0)->getBalance()->getLimitSteps();
}

void Scene::setCompensacao(int value)
{
    if (characters.size()==0) return;
    if (characters.at(0)->getBalance()==NULL) return;
    characters.at(0)->getBalance()->setCompensation(value/100.);
}

Object *Scene::objectClicked(int width, int height)
{
    Matrix4x4 changetoviewer;
    changetoviewer.setIdentity();
    Vec4 kv,iv,jv,kvl,ivl,jvl;
    kv = (viewer[0] - viewer[1])/((viewer[0] - viewer[1]).module());
    iv = (viewer[2] ^ kv)/(viewer[2] ^ kv).module();
    jv = (kv ^ iv)/(kv ^ iv).module();
    ivl.setVec4(iv.x1,jv.x1,kv.x1);
    jvl.setVec4(iv.x2,jv.x2,kv.x2);
    kvl.setVec4(iv.x3,jv.x3,kv.x3);
    Vec4 translate(-(iv*viewer[0]),-(jv*viewer[0]),-(kv*viewer[0]));

    changetoviewer.setAxisX(iv);
    changetoviewer.setAxisY(jv);
    changetoviewer.setAxisZ(kv);
    changetoviewer.setTranslate(translate);

    changetoviewer = changetoviewer.transpose();
    changetoviewer.setTranslate(viewer[0]);


    float h = 2.0*projection.x3*(tan(M_PI*projection.x1/360.0));
    float w = h*projection.x2;
    float deltax = w/this->width;
    float deltay = h/this->height;

    int j = height;
    int i = width;


    float alfa,beta;

    alfa  = -w/2.0 + deltax/2.0  + i*deltax;
    beta  = -h/2.0 + deltay/2.0 + j*deltay;


    Vec4 dir(alfa,beta,-projection.x3);
    Ray ray(changetoviewer.transpose().vector(Vec4(0,0,0)),changetoviewer.transpose().vector(dir));
    ray.setDirection((ray.direction - ray.origin).unitary());
    //ray.origin.showVec4(); //mostrar a origem
    //ray.positionRay(20).showVec4(); //mostra a direção
    return getObject(ray);
}

Object *Scene::getObject(Ray ray)
{
    Object *obj = NULL;
    Object *objsel = NULL;
    std::vector<Object*> objectschara;
    objectschara = objectsSceneChara();
    float t = 120000;  //parametro t
    for (int i=0;i<objectschara.size();i++){
        obj = objectschara[i];
        float t_ = obj->intersectionRay(ray,t);
        if (t_<t && t_>0){
            objsel = obj;
            t = t_;
        }
    }

    return objsel;
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
    startRecorder(b);
}

void Scene::shotBallsCharacterRandom(Character *chara,int posPelvis,float den)
{
    //if ( inicio.modulo() == 0.0 ) {
    //srand(time(NULL));
    //int r = rand() % chara->getNumBodies();
    Vec4 posPelvisModel = chara->getBody(posPelvis)->getPositionCurrent();
    //float mass = chara->getBody(posPelvis)->getFMass();
    //this->objetos.push_back( new Objeto( Simulation::world, Simulation::space, posPelvisModelo, mass.mass, influExtSimFF, true, SPHERE ) ); //para a geometria do objeto ser aleatoria, basta nao definir o ultimo parametro
    //inicio
       Vec4 directionFromCenter = Vec4( dRandReal()*3.0-1.0, dRandReal()*1.2, dRandReal()*3.0-1.0 );
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

       Vec4 directionVelInicial = posPelvisModel-(inicio);

         //directionVelInicial.normaliza();
       Vec4 velInicial = directionVelInicial*(3);// dRandReal()*30.0+10.0 );
       //Vec4 vell = velInicial;
       //vell.x2=0;
       //printf("\nVelocidade: %.3f",vell.module());
       dReal massTotal;
       //if ( pelvisIsTheTarget ) {
       massTotal = (dReal)den;
       Object *obj = new Object(this);//position,rotation,properties,type,this);
       obj->setMaterial(MATERIAL_EMERALD);
       obj->setType(TYPE_SPHERE);
       obj->setPosition(inicio);
       obj->setRotation(Quaternion());
       obj->setProperties(Vec4(massTotal/1000.,massTotal/1000.,massTotal/1000.));
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
          this->objects_shoot.at(0)->clearPhysics();
        this->objects_shoot.erase(objects_shoot.begin());
      }
}

void Scene::shotBallsCharacterBody(Object *body, float velocity, float den)
{
    //if ( inicio.modulo() == 0.0 ) {
    //srand(time(NULL));
    //int r = rand() % chara->getNumBodies();
    Vec4 posPelvisModel = body->getPositionCurrent();
    //float mass = chara->getBody(posPelvis)->getFMass();
    //this->objetos.push_back( new Objeto( Simulation::world, Simulation::space, posPelvisModelo, mass.mass, influExtSimFF, true, SPHERE ) ); //para a geometria do objeto ser aleatoria, basta nao definir o ultimo parametro
    //inicio
       Vec4 directionFromCenter = Vec4( dRandReal()*3.0-1.0, dRandReal()*1.2, dRandReal()*3.0-1.0 );
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
       Vec4 deltaCenter = velocity; //(0.0,0.0,0.0);

         deltaCenter = Vec4( dRandReal()*0.3-0.15, dRandReal()*1.6-0.75, dRandReal()*0.3-0.15 );

       Vec4 directionVelInicial = posPelvisModel-(inicio);

       directionVelInicial.normalize();
       Vec4 velInicial = directionVelInicial*(velocity);// dRandReal()*30.0+10.0 );
       //Vec4 vell = velInicial;
       //vell.x2=0;
       //printf("\nVelocidade: %.3f",vell.module());
       dReal massTotal;
       //if ( pelvisIsTheTarget ) {
       massTotal = (dReal)den;
       Object *obj = new Object(this);//position,rotation,properties,type,this);
       obj->setMaterial(MATERIAL_EMERALD);
       obj->setType(TYPE_SPHERE);
       obj->setPosition(inicio);
       obj->setRotation(Quaternion());
       obj->setProperties(Vec4(100/1000.,100/1000.,100/1000.));
       obj->setFMass(massTotal);

       Physics::createObject(obj,space,massTotal,inicio,velInicial);
       objects_shoot.push_back(obj);

       while ( this->objects_shoot.size() > 10 ) {
           this->objects_shoot.at(0)->clearPhysics();
         this->objects_shoot.erase(objects_shoot.begin());
       }
}

void Scene::habiliteJump()
{
    if(this->characters.size()>0){
        this->getCharacter(0)->getBalance()->habiliteJump(true);
    }
}

bool Scene::isGeometryFootSwing(dGeomID geom)
{
    std::vector<Object*> obj = objectsScene();
    for(unsigned int i=0;i<obj.size();i++){
        if (obj.at(i)->getGeometry()==geom) return Sensor::isSwingFoot(obj.at(i),obj.at(i)->getCharacter());
    }
    obj.clear();
    return false;
}

void Scene::createRamp()
{
    return;


  //  Object *ramp = addObject(Vec4(1.8,0.7,0.001),Vec4(-0.1,0.1,1.5),Quaternion(Vec4(75,0,0)),TYPE_CUBE,1.2,0,MATERIAL_BRASS);
    Object *ramp = addObject(Vec4(1.8,0.7,0.001),Vec4(-0.1,0.02,1.5),Quaternion(Vec4(80,0,0)),TYPE_CUBE,1.2,0,MATERIAL_BRASS);
    Object *cont = addObject(Vec4(1.8,0.12,0.5),Vec4(-0.1,0.02,2.09),Quaternion(Vec4(0,0,0)),TYPE_CUBE,20,0,MATERIAL_BRASS);
    Object *ramp2= addObject(Vec4(1.8,0.7,0.001),Vec4(-0.1,0.02,2.68),Quaternion(Vec4(-80,0,0)),TYPE_CUBE,1.2,0,MATERIAL_BRASS);
    objects.push_back(ramp);
    objects.push_back(cont);
    objects.push_back(ramp2);
    joints.push_back(addJointFixed(ramp,ramp2,NULL));
    //qDebug() << "Criou";
    //joints.push_back(addJointFixed(ramp2,));
//    joints.push_back(addJointFixed(cont,NULL));

    // objects.push_back(addObject(Vec4(0.8,0.1,0.4),Vec4(0.2,0.05,1.3),Quaternion(Vec4(0,0,0)),TYPE_CUBE,1.2));
}

void Scene::createCharacter()
{
    Character *chara = new Character(this);
    this->addCharacter(chara);
    Object *A = addObject(Vec4(0.2,0.5,0.2),Vec4(0,0.90,0),Quaternion(1,0,0,0),TYPE_CUBE,3.0,chara,MATERIAL_CHROME);
    Object *B = addObject(Vec4(0.2,0.5,0.2),Vec4(0,0.35,0),Quaternion(1,0,0,0),TYPE_CUBE,2.5,chara,MATERIAL_EMERALD);
    A->setFoot(false);
    B->setFoot(false);
    Joint* joint = this->addJointHinge(Vec4(0,0.625,0),Vec4(0.0,0.0,1.0),A,B,chara);
    //Joint* joint = this->addJointBall(Vec4(0,0.7,0),A,B,chara);
    ControlPD *pd = new ControlPD(joint,Quaternion(1,0,0,0),Vec4(),Vec4());
    pd->setEnabled(false);
    chara->controllers.push_back(pd);
    chara->contructHierarchyBodies();
}

void Scene::createLuxo()
{
    int materialLuxo = MATERIAL_SILVER;
    Character *luxo = new Character(this);
    this->addCharacter(luxo);

    Object *lamp = addObject(Vec4(0.05,0.1,0.12),Vec4(0,0.8,0),Quaternion(1,0,0,0),TYPE_CUBE,0.5,luxo,materialLuxo);
    Object *upperBody = addObject(Vec4(0.05,0.2,0.12), Vec4(0,0.6,0),Quaternion(1,0,0,0),TYPE_CUBE,1,luxo,materialLuxo);
    Object *lowerBody = addObject(Vec4(0.05,0.3,0.12), Vec4(0,0.3,0), Quaternion(1,0,0,0),TYPE_CUBE,3,luxo,materialLuxo);
    Object *feet = addObject(Vec4(1.0,0.01,0.5), Vec4(0,0.05,0), Quaternion(1,0,0,0),TYPE_CUBE,4,luxo,materialLuxo);

    lamp->setFoot(false);
    upperBody->setFoot(false);
    lowerBody->setFoot(false);
    feet->setFoot(false);

    Joint *upperLamp = this->addJointHinge(Vec4(0,0.725,0), Vec4(0,0,1), upperBody, lamp, luxo);
    upperLamp->setName("upperLamp");
    upperLamp->setRadiusHinge(0.025);
    Joint *lowerUpper = this->addJointHinge(Vec4(0,0.475,0), Vec4(0,0,1), lowerBody, upperBody, luxo);
    lowerUpper->setName("lowerUpper");
    lowerUpper->setRadiusHinge(0.025);
    Joint *feetLower = this->addJointHinge(Vec4(0,0.1025,0), Vec4(0,0,1), feet, lowerBody, luxo); //ancora correta dadas as configurações do luxo
    feetLower->setName("feetLower");
    feetLower->setRadiusHinge(0.0475);
    //Joint *feetLower = addJointHinge(Vec4(0,0.0775,0), Vec4(0,0,1), feet, lowerBody, luxo);
    //feetLower->setName("feetLower");

    ControlPD *upperLampControl = new ControlPD(upperLamp,Quaternion(1,0,0,0),Vec4(),Vec4());
    ControlPD *lowerUpperControl = new ControlPD(lowerUpper,Quaternion(1,0,0,0),Vec4(),Vec4());
    ControlPD *feetLowerControl = new ControlPD(feetLower,Quaternion(1,0,0,0),Vec4(),Vec4());

    upperLampControl->setEnabled(true);
    lowerUpperControl->setEnabled(true);
    feetLowerControl->setEnabled(true);


    //o luxo ficava "dançando" pq as constantes kd estavam muito altas, reduzi todas para 2.0 (antes estava 5.0)
    upperLampControl->setKs(Vec4(0.0,0.0,50.0));
    upperLampControl->setKd(Vec4(0.0,0.0,2.0));
    lowerUpperControl->setKs(Vec4(0.0,0.0,50.0));
    lowerUpperControl->setKd(Vec4(0.0,0.0,2.0));
    feetLowerControl->setKs(Vec4(0.0,0.0,50.0));
    feetLowerControl->setKd(Vec4(0.0,0.0,2.0));

    luxo->controllers.push_back(upperLampControl);
    luxo->controllers.push_back(lowerUpperControl);
    luxo->controllers.push_back(feetLowerControl);

    luxo->contructHierarchyBodies();


    //construindo as poses do Luxo
    std::vector<Vec4> pose1a;
    pose1a.push_back(Vec4(0,0,90));
    pose1a.push_back(Vec4(0,0,25));
    pose1a.push_back(Vec4(0,0,20));
    Pose* pose1 = addPose(luxo,pose1a);
    pose1->setName("pose 1");

    std::vector<Vec4> pose2a;
    /*pose2a.push_back(Vec4(0,0,100));
    pose2a.push_back(Vec4(0,0,30));
    pose2a.push_back(Vec4(0,0,40));*/
    pose2a.push_back(Vec4(0,0,70));
    pose2a.push_back(Vec4(0,0,0));
    pose2a.push_back(Vec4(0,0,80));
    Pose* pose2 = addPose(luxo,pose2a);
    pose2->setName("pose 2");

    std::vector<Vec4> pose3a;
    /*pose3a.push_back(Vec4(0,0,70));
    pose3a.push_back(Vec4(0,0,5));
    pose3a.push_back(Vec4(0,0,30));*/
    pose3a.push_back(Vec4(0,0,-60));
    pose3a.push_back(Vec4(0,0,0));
    pose3a.push_back(Vec4(0,0,-70));
    Pose* pose3 = addPose(luxo,pose3a);
    pose3->setName("pose3");

    /*std::vector<Vec4> pose4a;
    pose4a.push_back(Vec4(0,0,40));
    pose4a.push_back(Vec4(0,0,5));
    pose4a.push_back(Vec4(0,0,0));
    Pose* pose4 = addPose(luxo,pose4a);
    pose4->setName("pose 4");*/

    GraphicalPose* luxoPose = addGraphicalPose(luxo);
    std::cout << luxoPose->getCumulativeTimeIntervals().size() << " cumulative time intervals size\n";
    luxoPose->pushBackPose(pose1, 240);
    luxoPose->pushBackPose(pose2, 240);
    luxoPose->pushBackPose(pose3, 240);
    //luxoPose->pushBackPose(pose4, 240);

    //Necessário para ele não sair voando com o pcg
    this->setGravity(true);
}
void Scene::createLuxo2()
{
    int materialLuxo = MATERIAL_SILVER;
    Character *luxo = new Character(this);
    this->addCharacter(luxo);

    Object *lamp = addObject(Vec4(0.05,0.1,0.05),Vec4(0,0.8,0),Quaternion(1,0,0,0),TYPE_CUBE,0.5,luxo,materialLuxo);
    Object *upperBody = addObject(Vec4(0.05,0.2,0.05), Vec4(0,0.6,0),Quaternion(1,0,0,0),TYPE_CUBE,1,luxo,materialLuxo);
    Object *lowerBody = addObject(Vec4(0.05,0.3,0.05), Vec4(0,0.3,0), Quaternion(1,0,0,0),TYPE_CUBE,3,luxo,materialLuxo);
    Object *feet = addObject(Vec4(1.2,0.01,0.7), Vec4(0,0.05,0), Quaternion(1,0,0,0),TYPE_CUBE,10,luxo,materialLuxo);

    lamp->setFoot(false);
    upperBody->setFoot(false);
    lowerBody->setFoot(false);
    feet->setFoot(true);

    Joint *upperLamp = addJointHinge(Vec4(0,0.725,0), Vec4(0,0,1), upperBody, lamp, luxo);
    upperLamp->setName("upperLamp");
    Joint *lowerUpper = addJointHinge(Vec4(0,0.475,0), Vec4(0,0,1), lowerBody, upperBody, luxo);
    lowerUpper->setName("lowerUpper");
    Joint *feetLower = addJointHinge(Vec4(0,0.125,0), Vec4(0,0,1), feet, lowerBody, luxo);
    feetLower->setName("feetLower");


    ControlPD *upperLampControl = new ControlPD(upperLamp,Quaternion(1,0,0,0),Vec4(),Vec4());
    ControlPD *lowerUpperControl = new ControlPD(lowerUpper,Quaternion(1,0,0,0),Vec4(),Vec4());
    ControlPD *feetLowerControl = new ControlPD(feetLower,Quaternion(1,0,0,0),Vec4(),Vec4());

    upperLampControl->setEnabled(true);
    lowerUpperControl->setEnabled(true);
    feetLowerControl->setEnabled(true);

    upperLampControl->setKs(Vec4(0.0,0.0,50.0));
    upperLampControl->setKd(Vec4(0.0,0.0,5.0));
    lowerUpperControl->setKs(Vec4(0.0,0.0,50.0));
    lowerUpperControl->setKd(Vec4(0.0,0.0,5.0));
    feetLowerControl->setKs(Vec4(0.0,0.0,50.0));
    feetLowerControl->setKd(Vec4(0.0,0.0,5.0));

    luxo->controllers.push_back(upperLampControl);
    luxo->controllers.push_back(lowerUpperControl);
    luxo->controllers.push_back(feetLowerControl);

    luxo->contructHierarchyBodies();
}


void Scene::startRecorder(bool b)
{
    if (b){
        initialize = b;
        for (int i=0;i<getCharacter(0)->getNumJoints();i++){
            QList<Vec4> n;
            QList<Vec4> n1;
            motion.append(n);
            mocap.append(n1);
        }
    }
    if ((initialize) && (!b)){
        initialize = false;
        record = true;
    }
}

Pose *Scene::addPose(Character *character, std::vector<Vec4> angles)
{
    Pose* newPose = new Pose(character, angles);
    this->poses.push_back(newPose);
    return newPose;
}

GraphicalPose *Scene::addGraphicalPose(Character *character)
{
    GraphicalPose* newGraphicalPose = new GraphicalPose(character);
    this->poseControl.push_back(newGraphicalPose);
    return newGraphicalPose;
}



