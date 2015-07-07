#include "character.h"
#include "control/control.h"
#include "object.h"
#include "joint.h"
#include "scene.h"
#include "graphics/draw.h"
std::vector<Joint*> result;
Character::Character(Scene *parent)
{
    this->scene = parent;
    Physics::initCharacter(this);
    this->balance = NULL;
    this->alpha = 0;
    this->wire = false;
    this->capMotion = new MoCap(this);
    this->shadow_motion = true;
    this->has_suitcase = false;

}

Character::~Character(){
    this->balance = NULL;

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
        (*it)->draw(wire);
        //if (getGRFSum(*it).module()>0) (*it)->draw((*it)->getPositionCurrent(),(*it)->getRotationCurrent(),MATERIAL_EMERALD);
    }
    for(std::vector<Joint*>::iterator it=joints.begin(); it!=joints.end(); it++){
        (*it)->draw();
    }

    bool sensor_use = false;
    if(this->getMoCap()!=NULL){
        if(this->getMoCap()->status) sensor_use = true;
    }

    std::vector<Object*> foots =  getBodiesFoot();

    if (foots.size()>0 && this->balance!=NULL)
    if(sensor_use){
        if (Sensor::getHierarchy2UseMocap(this)!=2){
            if (Sensor::getHierarchy2UseMocap(this)==0){
                for(int i=0;i<this->getNumBodies();i++)
                    if(objects.at(i)->getFoot()) objects.at(i)->draw(objects.at(i)->getPositionCurrent(),objects.at(i)->getRotationCurrent(),MATERIAL_EMERALD);
            }
            else{
                int v = Sensor::getHierarchy2UseMocap(this);
                v -= 3;
                if(objects.at(v)->getFoot()) objects.at(v)->draw(objects.at(v)->getPositionCurrent(),objects.at(v)->getRotationCurrent(),MATERIAL_EMERALD);
            }
        }
    }
    else{
        if (Sensor::getHierarchy2Use(this)!=2){
            if (Sensor::getHierarchy2Use(this)==0){
                for(int i=0;i<this->getNumBodies();i++)
                    if(objects.at(i)->getFoot()) objects.at(i)->draw(objects.at(i)->getPositionCurrent(),objects.at(i)->getRotationCurrent(),MATERIAL_EMERALD);

            }
            else{
                int v = Sensor::getHierarchy2Use(this);
                v -= 3;
                if(objects.at(v)->getFoot()) objects.at(v)->draw(objects.at(v)->getPositionCurrent(),objects.at(v)->getRotationCurrent(),MATERIAL_EMERALD);

            }
        }
    }




    if (shadow_motion) if (capMotion->sizeFrames()>0) capMotion->drawShadow(Vec4(-1.0,0,0),capMotion->frame_current);


    drawCOM();
    drawFootProjected();
    drawCOMProjected();
}

void Character::drawPreShadows()
{
    if (shadow_motion) if (capMotion->sizeFrames()>0) capMotion->drawShadow(Vec4(-1.0,0,0),capMotion->frame_current);
    for(std::vector<Joint*>::iterator it=joints.begin(); it!=joints.end(); it++){
        (*it)->drawPreShadow();
    }
    for(std::vector<Object*>::iterator it=objects.begin(); it!=objects.end(); it++){
        (*it)->drawPreShadow();
    }

}

void Character::drawShadows()
{
    if (shadow_motion) if (capMotion->sizeFrames()>0) capMotion->drawShadow(Vec4(-1.0,0,0),capMotion->frame_current);
    for(std::vector<Joint*>::iterator it=joints.begin(); it!=joints.end(); it++){
        (*it)->drawShadow();
    }
    for(std::vector<Object*>::iterator it=objects.begin(); it!=objects.end(); it++){
        (*it)->drawShadow();
    }


}

void Character::drawCOM()
{
    Draw::drawCOM(getPosCOM(),0.04,Vec4(0,1,0));
}

void Character::drawMoCap(bool b)
{
    shadow_motion = b;
}

void Character::setAlpha(float v)
{
    this->alpha = v;
}

void Character::setWireframe(bool b)
{
    this->wire = b;
}

void Character::drawFootProjected()
{
//    std::vector<Object*> foots;
//    for (int i=0;i<this->getNumBodies();i++)
//        if (this->getBody(i)->getFoot()) foots.push_back( this->getBody(i));
//    if(!(foots.size()>0)) return;
//    for(unsigned int i=0;i<foots.size();i++){
//        Object* foot = foots[i];
//        Vec4 Cfoot_ = foot->getPositionCurrent();//centro de apoio projetado no plano xz
//        Cfoot_ = Vec4(Cfoot_.x(),0,Cfoot_.z());
//        Draw::drawCOMProjected(Cfoot_,0.05,Vec4(1,1,0));
//    }


    bool capture = false;
    if(this->getMoCap()!=NULL)
        if(this->capMotion->frame_current>0 && this->getMoCap()->status) capture = true;
    std::vector<Object*> foots;
    for (int i=0;i<this->getNumBodies();i++)
        if (this->getBody(i)->getFoot()) foots.push_back( this->getBody(i));
    Vec4 Cfoot_;
    int count = 0;
    //Vec4 Cfoot_;
    //int count = 0;
    bool draw = false;
    int stance = Sensor::getStanceFoot(this);

    if (stance<0 && (foots.size()==2)){
        Cfoot_ = foots.at(0)->getPositionCurrent()+foots.at(1)->getPositionCurrent();
        //Cfoot_.x2 = 0;
        count = 2;
        draw = true;
    }else if(foots.size()>0 && stance>0){
        Cfoot_ = this->getBody(stance)->getPositionCurrent();
        //Cfoot_.x2 = 0;
        count = 1;
        draw = true;
    }else if(stance<0) return;
    if(draw){
        Cfoot_ /= count;
        Draw::drawTargetProjected(Vec4::projectionPlane(Cfoot_,scene->getRotationPlaneVector(),Vec4()),0.05,Vec4(),scene->getRotationPlane());
    }

}

void Character::drawCOMProjected()
{

//    Vec4 COM_   = this->getPosCOM();
//    COM_ = Vec4(COM_.x(),0,COM_.z());
    Draw::drawCOMProjected(Vec4::projectionPlane(getPosCOM(),scene->getRotationPlaneVector(),Vec4()),0.05,Vec4(1,1,1),scene->getRotationPlane());
}

void Character::drawShadowMotion(int frame)
{
    Vec4 offset(-2.0,0,0);
    capMotion->drawShadow(offset,frame);

}

void Character::showHierarchyBodies(Object *begin, Object *end)
{
    std::vector<Joint*> jointsb = getHierarchyJoint(begin, end);
    printf("Hierarquia: <");
    for(unsigned int i=0;i<jointsb.size();i++){
        printf(" %s",jointsb.at(i)->getName().toLocal8Bit().constData());
    }
    printf(">");
}

void Character::restartPhysics()
{
    Physics::initCharacter(this);
    for(unsigned int i=0;i<objects.size();i++){
        Physics::createObject(objects.at(i),this->getSpace(),objects.at(i)->getFMass(),objects.at(i)->getPosition(),objects.at(i)->getRotation());
        Physics::setDisableObject(objects.at(i));
    }
    for(unsigned int i=0;i<joints.size();i++){
        joints.at(i)->restartJoint();
        Physics::setDisableJoint(joints.at(i));
    }
}

Object* Character::getObject(QString name)
{
    for(unsigned int i=0;i<objects.size();i++)
        if (objects.at(i)->getName() == name) return objects.at(i);
    return NULL;
}

Vec4 Character::getPosCOM()
{
    float m = 0; //somatório das massas
    Vec4 rm = Vec4();//somatório do produto das massas com a posição do objeto
    for(unsigned int i=0;i<objects.size();i++){
        rm += Physics::getRelPositionBody(objects.at(i))*objects.at(i)->getFMass();
        m  += objects.at(i)->getFMass();
    }
    return rm/m;

}

Vec4 Character::getVelCOM()
{
    float m = 0; //somatório das massas
    Vec4 rm = Vec4();//somatório do produto das massas com a velocidade do objeto
    for(unsigned int i=0;i<objects.size();i++){
        rm += Physics::getRelVelocityBody(objects.at(i))*objects.at(i)->getFMass();
        m  += objects.at(i)->getFMass();
    }
    return rm/m;
}

Vec4 Character::getLinearMomentum()
{
    Vec4 rm = Vec4();//somatório do produto das massas com a velocidade do objeto
    for(unsigned int i=0;i<objects.size();i++){
        rm += Physics::getRelVelocityBody(objects.at(i))*objects.at(i)->getFMass();
    }
    return rm;
}

Vec4 Character::getAngularMomentum()
{
    Vec4 posCOM = getPosCOM();
    Vec4 velCOM = getVelCOM();
    Vec4 pos,linVel,angMom,angMomTotal;
    for (unsigned int i=0;i<objects.size();i++) {
        pos = objects.at(i)->getPositionCurrent();
        linVel = objects.at(i)->getRelVelLinear();
        pos = pos - posCOM;
        angMom = Physics::getAngularMomentumBody(objects.at(i));
        //Iw + r x mv
        angMomTotal += angMom + (pos ^ ((linVel-velCOM)*objects.at(i)->getFMass()));
    }
    return angMomTotal;
}

Vec4 Character::getVelAngular()
{
    Vec4 posCOM = getPosCOM();
    Vec4 velCOM = getVelCOM();
    Vec4 pos,linVel,angVel,angVelTotal;
    for (unsigned int i=0;i<objects.size();i++) {
        pos = objects.at(i)->getPositionCurrent() - posCOM;
        linVel = objects.at(i)->getRelVelLinear();
        //w
        angVel = objects.at(i)->getRelVelAngular();
        //w + r x v
        angVelTotal += angVel + (pos ^ (linVel-velCOM));
    }
    return angVelTotal;
}

float Character::getMassTotal()
{
    float m = 0; //somatório das massas
    for(unsigned int i=0;i<objects.size();i++){
        m  += objects.at(i)->getFMass();
    }
    return m;

}

int Character::getNumJoints()
{
    if (has_suitcase)
        return joints.size()-1;
    return joints.size();
}

int Character::getNumBodies()
{
    if(has_suitcase)
        return objects.size()-1;
    else
        return objects.size();
}

int Character::getIdObject(Object *obj)
{
    for (unsigned int i=0;i<objects.size();i++)
        if (objects.at(i)==obj) return i;
    return -1;
}

int Character::getIdJoint(Joint *joint)
{
    for (unsigned int i=0;i<joints.size();i++)
        if (joints.at(i)==joint) return i;
    return -1;
}

Joint *Character::getParentMoreMass()
{
    Joint* torso = NULL;
    float mass = 0;
    for(unsigned int i=0;i<this->getNumJoints();i++ )
        if (mass<=joints.at(i)->getParent()->getFMass()){
            mass = joints.at(i)->getParent()->getFMass();
            torso = joints.at(i);
        }
    return torso;

}

Joint* Character::getJointParentBalance()
{
    for(unsigned int i=0;i<this->getNumJoints();i++ )
        if (joints.at(i)->getParent()->getBodyBalance())
            return joints.at(i);
    return NULL;
}

Joint *Character::getJoint(int i)
{
    return this->joints.at(i);
}

Joint *Character::getJoint(Object *parent, Object *child)
{
    for(int i=0;i<getNumJoints();i++ )
        if (getJoint(i)->getParent()==parent && getJoint(i)->getChild()==child)
            return getJoint(i);
    return NULL;
}

Joint *Character::getJoint2ObjectParent(Object *obj)
{
    for(int i=0;i<getNumJoints();i++){
        //qDebug() << this->getJoint(i)->getName();
        if (getJoint(i)->getParent()==obj) return getJoint(i);
    }
    return NULL;
}

Object *Character::getBody(int i)
{
    return this->objects.at(i);
}

int Character::getPositionBody(Object *obj)
{
    for(int i=0;i<getNumBodies();i++){
        if(obj==getBody(i)) return i;
    }
    return -1;
}

std::vector<Joint*> Character::getJointChilds(Joint *exclude, Object *obj)
{
    std::vector<Joint*> relation;
    for(int i=0;i<this->getNumJoints();i++){
        //qDebug() << this->getJoint(i)->getName();
        if (getJoint(i)->getParent()==obj && getJoint(i)!=exclude) relation.push_back(getJoint(i));
    }
    return relation;

}

std::vector<Object*> Character::getBodiesFoot(Object* nofoot)
{
    std::vector<Object*> foots;
    for(int i=0;i<getNumBodies();i++){
        if (getBody(i)->getFoot() && getBody(i)!=nofoot) foots.push_back(getBody(i));
    }
    return foots;
}

std::vector<Object*> Character::getChildrens(Object *obj)
{
    std::vector<Object*> bodies;
    for(int i = 0; i<this->getNumJoints();i++){
        //qDebug() << this->getJoint(i)->getName();
        if(this->getJoint(i)->getParent()==obj) bodies.push_back(this->getJoint(i)->getChild());
    }
    return bodies;

}

bool Character::isBodyHierarchy(Joint *joint, Object *obj)
{
    if (obj==joint->getChild()) return true;
    Object* aux = joint->getChild();
    std::vector<Joint*> relation = this->getJointChilds(joint,aux);
    int size = relation.size();
    for(int i=0;i<size;i++)
         if (isBodyHierarchy(relation.at(i),obj)) return true;
    relation.clear();
    return false;
}

//bool Character::isJointHierarchy(Joint /**joint*/, Joint /**child*/)
//{
////    Object* aux = joint->getChild();
////    //std::vector<Joint*> relation = this->getJointChilds(joint,obj);
////    std::vector<Joint*> relation = this->getJointChilds(joint,aux);
////    int size = relation.size();
////    for(int i = 0;i<size;i++)
////        if (child==relation.at(i)) return true;
////        else isBodyHierarchy(relation.at(i),joint->getChild());
//    return false;
//}

void Character::setBalance(Balance *balance)
{
    this->balance = balance;
}

void Character::updateKsKdControlPDCoros()
{

    for(unsigned int i=0;i<this->getNumJoints();i++)
        controllers.at(i)->updateKsKdCoros(this->getMassTotal());
}

Balance *Character::getBalance()
{
    return this->balance;
}

Vec4 Character::getSumTorqueGRF2COM()
{

    Vec4 sumTorque = Vec4();
    Vec4 posCOM = this->getPosCOM();
    for (unsigned int fc=0;fc<this->getScene()->getGroundForces().size();fc++) {
        //position of contact (red sphere)
          //feedbackContacts[fc].pos;
        //ground force
          //dReal* groundForce3D;
            //cout<<"feedbackContacts[fc].noGroundGeom: "<<feedbackContacts[fc].noGroundGeom<<"\n";
          if ( this->getScene()->getGroundForces()[fc].noGroundGeom != 3 ) {
              dReal* groundForce = NULL; //dVector3
              if ( this->getScene()->getGroundForces()[fc].noGroundGeom == 1 ) groundForce = this->getScene()->getGroundForces()[fc].jtFb->f1;
              if ( this->getScene()->getGroundForces()[fc].noGroundGeom == 2 ) groundForce = this->getScene()->getGroundForces()[fc].jtFb->f2;
              Vec4 Force3D(groundForce[0],groundForce[1],groundForce[2]);
              Vec4 d = this->getScene()->getGroundForces()[fc].position;
              d = posCOM - d;
              sumTorque += Force3D ^ d;
              delete groundForce;
          }
      }
    return sumTorque;
}

Vec4 Character::getSumForceGRF2COM()
{
    Vec4 sumForces = Vec4();
    for (unsigned int fc=0;fc<this->getScene()->getGroundForces().size();fc++) {
        //position of contact (red sphere)
          //feedbackContacts[fc].pos;
        //ground force
          //dReal* groundForce3D;
            //cout<<"feedbackContacts[fc].noGroundGeom: "<<feedbackContacts[fc].noGroundGeom<<"\n";
          if ( this->getScene()->getGroundForces()[fc].noGroundGeom != 3 ) {
              dReal* groundForce = NULL; //dVector3
              if ( this->getScene()->getGroundForces()[fc].noGroundGeom == 1 ) groundForce = this->getScene()->getGroundForces()[fc].jtFb->f1;
              if ( this->getScene()->getGroundForces()[fc].noGroundGeom == 2 ) groundForce = this->getScene()->getGroundForces()[fc].jtFb->f2;
              Vec4 Force3D(groundForce[0],groundForce[1],groundForce[2]);
              sumForces += Force3D;
              delete groundForce;
          }
      }
    return sumForces;
}

void Character::checkContactFoot(bool/* b*/)
{
    //if (!this->getScene()->getGroundForces().size()) balance->setEnableBalance(false);
    //else
    balance->setEnableBalance(true);
    //if (b) balance->setEnableBalance(true);
}

MoCap *Character::getMoCap()
{
    return capMotion;
}

void Character::contructHierarchyBodies()
{

    for (int i=-3;i<(int)this->getNumBodies()+2;i++) {
        bool** aux;
        this->hierarchy.push_back(aux);
        this->setHierarchyMap(i);
    }
}

void Character::setHierarchyMap(int pos)
{
    //contrução/inicialização da matriz de relacionamento de influência da junta no corpo
    //onde as linhas representam as juntas e as colunas os corpos
    this->hierarchy[pos+3] = new bool*[getNumJoints()];
    for (int i=0;i<this->getNumJoints();i++) {
        this->hierarchy[pos+3][i] = new bool[getNumBodies()];
        for (int j=0;j<this->getNumBodies();j++) {
            this->hierarchy[pos+3][i][j] = false;
        }
    }

    /*
          A construção segue uma lógica, primeiramente serão construidas 3 matrizes, nos seguintes casos:
      ALL_FOOTS_GROUND = onde todos os pés estão em contato com o solo
      FOOTS_AIR_INV = onde serão considerados a ordem inversa da forma onde nenhum pé esta em contato com o solo
      FOOTS_AIR     = onde os pés estão em contato com o solo
      nos demais casos será considerado a matriz relativa a cada junta do persongem.
     ****/

    //em todos os casos estamos considerando que a pelvis é a raiz do modelo originalmente
    //primeiro caso, onde todas os pés descritos no personagem estão em contato com o solo
    //ALL_FOOTS_GROUND
    if ( pos == ALL_FOOTS_GROUND ) {
        //teste de outra matriz para os dois pés no chão
        std::vector<int> bodies;
        for (int i=0;i<this->getNumJoints();i++) {
            bool in_posGroundPaw_chain = false;
            //puting the first body in vector bodies
            int nextBodyLoc = getPositionBody(this->getJoint(i)->getChild());
            bodies.push_back(nextBodyLoc);
            this->hierarchy[pos+3][i][nextBodyLoc] = true;
            //for each nextBody, look for the other next nextBodies
            while (!bodies.empty()) {
                int body = bodies.front();
                //verificando se a ground paw eh filha da junta i na hierarquia dos corpos
                if (getBody(body)->getFoot()) in_posGroundPaw_chain = true;

                for (int j=0;j<this->getNumJoints();j++) {
                    if (getPositionBody(this->getJoint(j)->getParent()) == body) {
                        nextBodyLoc = getPositionBody(this->getJoint(j)->getChild());
                        bodies.push_back(nextBodyLoc);
                        this->hierarchy[pos+3][i][nextBodyLoc] = true;
                    }
                }
                bodies.erase(bodies.begin());
            }
            bodies.clear();
            //invertendo o mapeamento realizado para essa junta
            if (in_posGroundPaw_chain) {
                for (int j=0;j<this->getNumBodies();j++) {
                    if((j==9 || j==11 || j==13) && (i==0 || i==1 || i==2)){
                        this->hierarchy[pos+3][i][j] = true;
                    }if((j==8 || j==10 || j==12) && (i==3 || i==4 || i==5)){
                        this->hierarchy[pos+3][i][j] = true;
                    }
                    //if(!(((j==9 || j==11 || j==13) && (i==0 || i==1 || i==2))||(((j==8 || j==10 || j==12) && (i==3 || i==4 || i==5)))))
                        this->hierarchy[pos+3][i][j] = !this->hierarchy[pos+3][i][j];

                }
            }
        }
        //fim_for
    }
    //this->hierarchy[0].print();

    //segundo caso, onde todas os pés descritos no personagem estão no ar com ordem inversa
    //FOOTS_AIR_INV

    if ( pos == FOOTS_AIR_INV ) {
        vector<int> bodies;
        for (int i=0;i<this->getNumJoints();i++) {
            //considering the nextBody as part of the composite inertia matrix
            //this->inertiaFactorMaps[posGroundPaw+3][i][ this->juntas[i]->nextLoc ] = true;
            //puting the first body in vector bodies
            int prevBodyLoc = getPositionBody(this->getJoint(i)->getParent());
            bodies.push_back( prevBodyLoc );
            this->hierarchy[pos+3][i][prevBodyLoc] = true;
            //for each prevBody, look for the other prev prevBodies
            while ( !bodies.empty() ) {
                int body = bodies.front();
                for (int j=0;j<this->getNumJoints();j++) {
                    if (getPositionBody(this->getJoint(j)->getChild()) == body) {
                        prevBodyLoc = getPositionBody(this->getJoint(j)->getParent());
                        bodies.push_back(prevBodyLoc);
                        this->hierarchy[pos+3][i][prevBodyLoc] = true;
                    }
                }
                bodies.erase(bodies.begin());
            }
            bodies.clear();
        }
    }
    //terceiro caso, onde todas os pés descritos no personagem estão no ar
    //FOOTS_AIR

    if ( pos == FOOTS_AIR ) {
        vector<int> bodies;
        for (int i=0;i<this->getNumJoints();i++) {
            //considering the prevBody as part of the composite inertia matrix
            //this->inertiaFactorMaps[posGroundPaw+3][i][ this->juntas[i]->prevLoc ] = true;
            //puting the first body in vector bodies
            int nextBodyLoc = getPositionBody(this->getJoint(i)->getChild());
            bodies.push_back(nextBodyLoc);
            this->hierarchy[pos+3][i][nextBodyLoc] = true;
            //for each nextBody, look for the other next nextBodies
            while ( !bodies.empty() ) {
                int body = bodies.front();
                for (int j=0;j<this->getNumJoints();j++) {
                    if (getPositionBody(this->getJoint(j)->getParent()) == body) {
                        nextBodyLoc = getPositionBody(this->getJoint(j)->getChild());
                        bodies.push_back( nextBodyLoc );
                        this->hierarchy[pos+3][i][ nextBodyLoc ] = true;
                    }
                }
                bodies.erase(bodies.begin());
            }
            bodies.clear();
        }
    }

    //caso geral, para cada corpo
    if ( pos >= 0 && pos<getNumBodies()) {
        int bg_loc = pos;
        vector<int> bodies;
        for (int i=0;i<this->getNumJoints();i++) {
            bool in_posGroundPaw_chain = false;
            //puting the first body in vector bodies
            int nextBodyLoc = getPositionBody(this->getJoint(i)->getChild());
            bodies.push_back( nextBodyLoc );
            this->hierarchy[pos+3][i][nextBodyLoc] = true;
            //for each nextBody, look for the other next nextBodies
            while ( !bodies.empty() ) {
                int body = bodies.front();
                //verificando se a ground paw eh filha da junta i na hierarquia dos corpos
                if (body==bg_loc) in_posGroundPaw_chain = true;
                for (int j=0;j<this->getNumJoints();j++) {
                    if (getPositionBody(this->getJoint(j)->getParent()) == body) {
                        nextBodyLoc = getPositionBody(this->getJoint(j)->getChild());
                        bodies.push_back( nextBodyLoc );
                        this->hierarchy[pos+3][i][nextBodyLoc] = true;
                    }
                }
                bodies.erase(bodies.begin());
            }
            bodies.clear();

            //invertendo o mapeamento realizado para essa junta
            if (in_posGroundPaw_chain) {
                for (int j=0;j<this->getNumBodies();j++) {
                    this->hierarchy[pos+3][i][j] = !this->hierarchy[pos+3][i][j];
                }
            }
        }
        //fim_for
    }

    if(pos==getNumBodies()){
        int bg_loc = pos;
        vector<int> bodies;
        for (int i=0;i<this->getNumJoints();i++) {
            bool in_posGroundPaw_chain = false;
            //puting the first body in vector bodies
            int nextBodyLoc = getPositionBody(this->getJoint(i)->getChild());
            bodies.push_back( nextBodyLoc );
            this->hierarchy[pos+3][i][nextBodyLoc] = true;
            //for each nextBody, look for the other next nextBodies
            while ( !bodies.empty() ) {
                int body = bodies.front();
                //verificando se a ground paw eh filha da junta i na hierarquia dos corpos
                if (body==bg_loc) in_posGroundPaw_chain = true;
                for (int j=0;j<this->getNumJoints();j++) {
                    if (getPositionBody(this->getJoint(j)->getParent()) == body) {
                        nextBodyLoc = getPositionBody(this->getJoint(j)->getChild());
                        bodies.push_back( nextBodyLoc );
                        this->hierarchy[pos+3][i][nextBodyLoc] = true;
                    }
                }
                bodies.erase(bodies.begin());
            }
            bodies.clear();

            //invertendo o mapeamento realizado para essa junta
            if (in_posGroundPaw_chain) {
                for (int j=0;j<this->getNumBodies();j++) {
                    if(i>2)
                       this->hierarchy[pos+3][i][j] = 0;
                    else
                       this->hierarchy[pos+3][i][j] = !this->hierarchy[pos+3][i][j];
                }
            }else{
                for (int j=0;j<this->getNumBodies();j++)
                    if(i>2)
                       this->hierarchy[pos+3][i][j] = 0;
            }
        }
    }
    if(pos==getNumBodies()+1){
        int bg_loc = pos;
        vector<int> bodies;
        for (int i=0;i<this->getNumJoints();i++) {
            bool in_posGroundPaw_chain = false;
            //puting the first body in vector bodies
            int nextBodyLoc = getPositionBody(this->getJoint(i)->getChild());
            bodies.push_back( nextBodyLoc );
            this->hierarchy[pos+3][i][nextBodyLoc] = true;
            //for each nextBody, look for the other next nextBodies
            while ( !bodies.empty() ) {
                int body = bodies.front();
                //verificando se a ground paw eh filha da junta i na hierarquia dos corpos
                if (body==bg_loc) in_posGroundPaw_chain = true;
                for (int j=0;j<this->getNumJoints();j++) {
                    if (getPositionBody(this->getJoint(j)->getParent()) == body) {
                        nextBodyLoc = getPositionBody(this->getJoint(j)->getChild());
                        bodies.push_back( nextBodyLoc );
                        this->hierarchy[pos+3][i][nextBodyLoc] = true;
                    }
                }
                bodies.erase(bodies.begin());
            }
            bodies.clear();

            //invertendo o mapeamento realizado para essa junta
            if (in_posGroundPaw_chain) {
                for (int j=0;j<this->getNumBodies();j++) {
                    if(i>5 && i<3)
                       this->hierarchy[pos+3][i][j] = 0;
                    else
                       this->hierarchy[pos+3][i][j] = !this->hierarchy[pos+3][i][j];
                }
            }else{
                for (int j=0;j<this->getNumBodies();j++)
                    if(i>5 || i<3)
                       this->hierarchy[pos+3][i][j] = 0;
            }
        }
    }
    //showHierarchies();

}

void Character::showHierarchies()
{

    if(hierarchy.size()==0) return;
    for(int i=-3;i<-2;i++){
        printf("Corpo (%d):\n",i);
        for(int j=0;j<getNumJoints();j++){
            for(int k=0;k<getNumBodies();k++){
                if(hierarchy[i+3][j][k]) printf(" 1 ");
                else printf(" 0 ");
            }
            printf("\n");
        }
    }
}

void Character::restartCollideWithGround()
{
    for(int i=0;i<this->getNumBodies();i++){
        getBody(i)->setCollideWithGround(false);
    }
}

void Character::loadMotionFrames()
{
    capMotion->loadFrameSimulation();
}

void Character::clearVectorsGlobais()
{
    result.clear();
}

std::vector<Joint*> Character::getHierarchyJoint(Object *begin, Object *end)
{
    if(begin==end) return result;
    std::vector<Object*> childs;
    childs = getChildrens(begin);
    for (unsigned int i=0;i<childs.size();i++){
        Joint *j = getJoint(begin,childs.at(i));
        if (checkHierarchy(j,end)){
            result.push_back(getJoint(begin,childs.at(i)));
            getHierarchyJoint(childs.at(i),end);
        }
    }
    childs.clear();
    return result;
}

bool Character::checkHierarchy(Joint *joint, Object *at)
{
    if (joint==NULL) return false;
    if (joint->getChild()==at) return true;
    return checkHierarchy(getJoint2ObjectParent(joint->getChild()),at);
    return false;

}

Vec4 Character::getGRFSum(Object *obj)
{
    //std::vector<GRF*> grfs = scene->groundForces;
    return GRF::forcesGRF(scene->groundForces,obj);

}

std::vector<GRF> Character::getGRFsObject(Object *obj)
{
    return GRF::forcesGRF2Object(scene->groundForces,obj);
}

void Character::setKsRetaionshipKd()
{
    for(unsigned int i=0;i<controllers.size();i++){
        controllers.at(i)->setKd(controllers.at(i)->getKs()*0.1);
    }
}

void Character::setOffset(Vec4 offset)
{
    this->offset = offset;
}

Object *Character::getObjectSelected()
{
    for(int i=0;i<this->getNumBodies();i++){
        if (this->getBody(i)->isSelected()){
            return this->getBody(i);
        }
    }
    return NULL;
}

Vec4 Character::getOffset()
{
    return offset;
}

int Character::findJointIndex(Joint *searchJoint)
{
    std::vector<Joint*> charJoints = this->joints;
    if (charJoints.size() != 0) {
        int index = 0;
        for (std::vector<Joint*>::iterator i = charJoints.begin(); i != charJoints.end(); ++i){
            if ((*i) == searchJoint){
                return index;
            }
            index++;
        }
        return -1;
    } else {
        return -1;
    }
}

ControlPD *Character::getController(int i)
{
    return this->controllers.at(i);
}

QString Character::getName()
{
    return this->name;
}

void Character::setName(QString name)
{
    this->name = name;
}
void Character::setSuitcase(int body, float mass)
{
    Object* obj = this->getBody(body);
    obj->setRotationCurrent(QuaternionQ(1,0,0,0));
    //qDebug() << obj->getName();
    float space = 0.1;
    Vec4 new_prop = Vec4(0.1,0.4,0.5);
    Vec4 pos = obj->getPositionCurrent();
    Vec4 ancor;
    pos -= Vec4(0,obj->getProperties().y()/2.,0);
    ancor = pos - Vec4(0,space/2.,0);
    pos -= Vec4(0,space,0);
    pos -= Vec4(0,new_prop.y()/2.0,0);
    Object* new_obj = this->scene->addObject(new_prop,pos,QuaternionQ(1,0,0,0),TYPE_CUBE,mass,this,MATERIAL_CHROME);
    Joint* new_joint = this->scene->addJointHinge(ancor,Vec4(0,0,1),obj,new_obj,this);
    new_obj->setName("suitcase");
    new_joint->setRadiusHinge(space/2.);
    new_obj->setFoot(false);
    new_joint->setName(obj->getName()+"__suitcase");
    ControlPD *suit = new ControlPD(new_joint,QuaternionQ(1,0,0,0),Vec4(),Vec4());
    this->controllers.push_back(suit);
    this->has_suitcase = true;
}

void Character::deleteSuitcase()
{
    this->has_suitcase = false;
    Physics::closeObject(objects.at(objects.size()-1));
    Physics::closeJoint(joints.at(joints.size()-1));
//    delete scene->objects.back();
//    scene->objects.pop_back();
    //delete objects.back();
    objects.pop_back();
    //delete joints.back();
    joints.pop_back();
    //delete controllers.back();
    controllers.pop_back();


}

void Character::isFall(bool b)
{
     for(int i=0;i<this->controllers.size();i++)   {
         this->controllers.at(i)->setFall(b);
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

bool Character::hasEffectorEnabled()
{
    for(int i=0;i<this->getNumBodies();i++){
        if (objects.at(i)->isEnableCPDP()) return true;
    }
    return false;
}
