#include "character.h"
#include "control/control.h"
#include "object.h"
#include "joint.h"
#include "scene.h"
#include "graphics/draw.h"

Character::Character(Scene *parent)
{
    this->scene = parent;
    Physics::initCharacter(this);
    this->balance = NULL;
    this->alpha = 0;
    this->wire = false;
    this->capMotion = new MoCap(this);
    this->shadow_motion = true;

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
    }
    for(std::vector<Joint*>::iterator it=joints.begin(); it!=joints.end(); it++){
        (*it)->draw();
    }
    if (capMotion->sizeFrames()>0) capMotion->drawShadow(Vec4(-1.0,0,0),capMotion->frame_current);

    drawCOM();
    drawFootProjected();
    drawCOMProjected();
}

void Character::drawCOM()
{
    Draw::drawPoint(getPosCOM(),0.05,Vec4(0,1,0));
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
    std::vector<Object*> foots;
    for (unsigned int i=0;i<this->getNumBodies();i++)
        if (this->getBody(i)->getFoot()) foots.push_back( this->getBody(i));
    if(!(foots.size()>0)) return;
    for(unsigned int i=0;i<foots.size();i++){
        Object* foot = foots[i];
        Vec4 Cfoot_ = foot->getPositionCurrent();//centro de apoio projetado no plano xz
        Cfoot_ = Vec4(Cfoot_.x(),0,Cfoot_.z());
        Draw::drawPoint(Cfoot_,0.05,Vec4(1,1,0));
    }

}

void Character::drawCOMProjected()
{

    Vec4 COM_   = this->getPosCOM();
    COM_ = Vec4(COM_.x(),0,COM_.z());
    Draw::drawPoint(COM_,0.05,Vec4(1,1,1));
}

void Character::drawShadowMotion(int frame)
{
    Vec4 offset(-1,0,0);
    capMotion->drawShadow(offset,frame);

}

void Character::restartPhysics()
{
    Physics::initCharacter(this);
    for(unsigned int i=0;i<objects.size();i++){
        Physics::createObject(objects.at(i),this->getSpace(),objects.at(i)->getFMass(),objects.at(i)->getPosition(),objects.at(i)->getRotation());
    }
    for(unsigned int i=0;i<joints.size();i++){
        joints.at(i)->restartJoint();
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
        angMomTotal += angMom + pos ^ ((linVel-velCOM)*objects.at(i)->getFMass());
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
        angVelTotal += angVel + pos ^ (linVel-velCOM);
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
    return joints.size();
}

int Character::getNumBodies()
{
    return objects.size();
}

int Character::getIdObject(Object *obj)
{
    for (unsigned int i=0;i<objects.size();i++)
        if (objects.at(i)==obj) return i;
    return -1;
}

Joint *Character::getParentMoreMass()
{
    Joint* torso;
    float mass = 0;
    for(unsigned int i=0;i<joints.size();i++ )
        if (mass<=joints.at(i)->getParent()->getFMass()){
            mass = joints.at(i)->getParent()->getFMass();
            torso = joints.at(i);
        }
    return torso;

}

Joint* Character::getJointParentBalance()
{
    for(unsigned int i=0;i<joints.size();i++ )
        if (joints.at(i)->getParent()->getBodyBalance())
            return joints.at(i);
    return NULL;
}

Joint *Character::getJoint(int i)
{
    return this->joints.at(i);
}

Object *Character::getBody(int i)
{
    return this->objects.at(i);
}

int Character::getPositionBody(Object *obj)
{
    for(unsigned int i=0;i<getNumBodies();i++){
        if(obj==getBody(i)) return i;
    }
    return -1;
}

std::vector<Joint*> Character::getJointChilds(Joint *exclude, Object *obj)
{
    std::vector<Joint*> relation;
    for(int i=0;i<this->getNumJoints();i++)
        if (getJoint(i)->getParent()==obj && getJoint(i)!=exclude) relation.push_back(getJoint(i));
    return relation;

}

std::vector<Object*> Character::getBodiesFoot()
{
    std::vector<Object*> foots;
    for(unsigned int i=0;i<getNumBodies();i++){
        if (getBody(i)->getFoot()) foots.push_back(getBody(i));
    }
    return foots;
}

bool Character::isBodyHierarchy(Joint *joint, Object *obj)
{
    if (obj==joint->getChild()) return true;
    Object* aux = joint->getChild();
    std::vector<Joint*> relation = this->getJointChilds(joint,aux);
    int size = relation.size();
    for(int i=0;i<size;i++)
         if (isBodyHierarchy(relation.at(i),obj)) return true;
    return false;
}

bool Character::isJointHierarchy(Joint *joint, Joint *child)
{
//    Object* aux = joint->getChild();
//    //std::vector<Joint*> relation = this->getJointChilds(joint,obj);
//    std::vector<Joint*> relation = this->getJointChilds(joint,aux);
//    int size = relation.size();
//    for(int i = 0;i<size;i++)
//        if (child==relation.at(i)) return true;
//        else isBodyHierarchy(relation.at(i),joint->getChild());
    return false;
}

void Character::setBalance(Balance *balance)
{
    this->balance = balance;
}

void Character::updateKsKdControlPDCoros()
{

    for(unsigned int i=0;i<controllers.size();i++)
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
          }
      }
    return sumForces;
}

void Character::checkContactFoot(bool b)
{
    if (!this->getScene()->getGroundForces().size()) balance->setEnableBalance(false);
    else balance->setEnableBalance(true);
    //if (b) balance->setEnableBalance(true);
}

MoCap *Character::getMoCap()
{
    return capMotion;
}

void Character::contructHierarchyBodies()
{

    for (int i=-3;i<(int)this->getNumBodies()+1;i++) {
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
    for (unsigned int i=0;i<this->getNumJoints();i++) {
        this->hierarchy[pos+3][i] = new bool[getNumBodies()];
        for (unsigned int j=0;j<this->getNumBodies();j++) {
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

    //em todos os casos estamos considerando que a pelves é a raiz do modelo originalmente
    //primeiro caso, onde todas os pés descritos no personagem estão em contato com o solo
    //ALL_FOOTS_GROUND
    if ( pos == ALL_FOOTS_GROUND ) {
        //teste de outra matriz para os dois pés no chão
        std::vector<int> bodies;
        for (unsigned int i=0;i<this->getNumJoints();i++) {
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

                for (unsigned int j=0;j<this->getNumJoints();j++) {
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
                for (unsigned int j=0;j<this->getNumBodies();j++) {
                    //if((!(j>=5 && j<=7 && i>=0 && i<=2))&&(!(j>=0 && j<=2 && i>=5 && i<=7)))
                        this->hierarchy[pos+3][i][j] = !this->hierarchy[pos+3][i][j];

                }
            }
        }
        //fim_for
    }

    //segundo caso, onde todas os pés descritos no personagem estão no ar com ordem inversa
    //FOOTS_AIR_INV

    if ( pos == FOOTS_AIR_INV ) {
        vector<int> bodies;
        for (unsigned int i=0;i<this->getNumJoints();i++) {
            //considering the nextBody as part of the composite inertia matrix
            //this->inertiaFactorMaps[posGroundPaw+3][i][ this->juntas[i]->nextLoc ] = true;
            //puting the first body in vector bodies
            int prevBodyLoc = getPositionBody(this->getJoint(i)->getParent());
            bodies.push_back( prevBodyLoc );
            this->hierarchy[pos+3][i][prevBodyLoc] = true;
            //for each prevBody, look for the other prev prevBodies
            while ( !bodies.empty() ) {
                int body = bodies.front();
                for (unsigned int j=0;j<this->getNumJoints();j++) {
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
        for (unsigned int i=0;i<this->getNumJoints();i++) {
            //considering the prevBody as part of the composite inertia matrix
            //this->inertiaFactorMaps[posGroundPaw+3][i][ this->juntas[i]->prevLoc ] = true;
            //puting the first body in vector bodies
            int nextBodyLoc = getPositionBody(this->getJoint(i)->getChild());
            bodies.push_back(nextBodyLoc);
            this->hierarchy[pos+3][i][nextBodyLoc] = true;
            //for each nextBody, look for the other next nextBodies
            while ( !bodies.empty() ) {
                int body = bodies.front();
                for (unsigned int j=0;j<this->getNumJoints();j++) {
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
        for (unsigned int i=0;i<this->getNumJoints();i++) {
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
                for (unsigned int j=0;j<this->getNumJoints();j++) {
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
                for (unsigned int j=0;j<this->getNumBodies();j++) {
                    this->hierarchy[pos+3][i][j] = !this->hierarchy[pos+3][i][j];
                }
            }
        }
        //fim_for
    }
    if(pos==getNumBodies()){
        printf("\nIN!");
        std::vector<Object*> foots = getBodiesFoot();
        if (foots.size()>1){
            for (unsigned int i=0;i<this->getNumJoints();i++) {
                for (unsigned int j=0;j<this->getNumBodies();j++) {
                    this->hierarchy[pos+3][i][j] = this->hierarchy[getIdObject(foots.at(0))+3][i][j] && this->hierarchy[getIdObject(foots.at(1))+3][i][j];
                }

            }
        }
    }
}

void Character::showHierarchies()
{
    if(hierarchy.size()==0) return;
    for(int i=-3;i<this->getNumBodies()+1;i++){
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

void Character::loadMotionFrames()
{
    capMotion->loadFrameSimulation();
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
