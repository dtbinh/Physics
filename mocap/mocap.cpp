#include "mocap.h"
#include "frame.h"
#include "framequat.h"
#include "scene/character.h"
#include "scene/joint.h"
#include "control/control.h"
#include "scene/object.h"
#include "control/sensor.h"
#include "extra/material.h"
#include "scene/scene.h"
MoCap::MoCap()
{
    frame_current = 1;
    status = false;
    file = "";
    file_load = "";
    beginClycle = 0;
    clyclic = true;
}

MoCap::MoCap(Character *chara)
{
    this->chara = chara;
    frame_current = 1;
    status = false;
    file = "";
    file_load = "";
    beginClycle = 0;
    clyclic = true;
}

void MoCap::setAddressFile(QString file)
{
    this->file = file;
}

QString MoCap::getAddressFile()
{
    return file;
}

void MoCap::setAddressFileLoad(QString file)
{
    this->file_load = file;
}

QString MoCap::getAddressFileLoad()
{
    return file_load;
}

void MoCap::updateHeightBody(Vec4 h, int id)
{
    Vec4 pos = getFrameMotion(0)->getPosition(id);
    pos.setVec4(0,pos.x2,0);
    pos = h - pos;
    float minus = 1000000;
    //extrai o menor y
    for(unsigned int i=0;i<capMot.size();i++){
        for(int j=0;j<getFrameMotion(i)->getNumFrames();j++){
            float y = getFrameMotion(i)->getPosition(j).y();
            if (y<minus) minus = y;
        }
    }
    if (minus>h.y()){
        for(unsigned int i=0;i<capMot.size();i++){
            for(int j=0;j<getFrameMotion(i)->getNumFrames();j++){
                getFrameMotion(i)->setPosition(j,getFrameMotion(i)->getPosition(j)-h.y());
            }
        }
    }else{
        for(unsigned int i=0;i<capMot.size();i++){
            for(int j=0;j<getFrameMotion(i)->getNumFrames();j++){
                getFrameMotion(i)->setPosition(j,getFrameMotion(i)->getPosition(j)+Vec4(0,h.y()-minus,0));
            }
        }
    }
}

void MoCap::copyFootsProperties()
{
    foots.clear();
    idfoots.clear();
    Vec4 aux;
    for(unsigned int i=0;i<chara->getBodiesFoot().size();i++){
        Object *foot = chara->getBodiesFoot().at(i);
        Object *newObj = new Object(Vec4(),Quaternion(),foot->getProperties(),foot->getType(),foot->getScene());
        aux.setVec4(0,foot->getProperties().y()/2.0,0);
        idfoots.push_back(chara->getIdObject(foot));
        foots.push_back(newObj);
    }
//    if(idfoots.size()>0)
//        updateHeightBody(aux,idfoots.at(0));
}

void MoCap::appendFrame(Frame* frame)
{

    capMot.push_back(frame);
}

Frame* MoCap::getFrameMotion(int i)
{
    return capMot.at(i);
}

std::vector<Frame*> MoCap::getCapMotion()
{
    return capMot;
}

FrameQuat *MoCap::getFrameSimulation(int i)
{
    return capMotFrame.at(i);
}

void MoCap::showMoCap(Vec4 offset, int frame)
{
    for(int i=0;i<chara->getNumBodies();i++){
        Vec4 position = getFrameMotion(frame)->getPosition(i);
        Quaternion orientation = getFrameMotion(frame)->getOrientation(i);
        chara->getBody(i)->draw(position+offset,orientation,MATERIAL_EMERALD);
    }
}

void MoCap::initializePosesModel(int frame)
{
    if (int(capMot.size())<frame) return;
    frame_current = frame;
    Frame* pose = this->getFrameMotion(frame);
    for(int i=0;i<pose->getNumFrames();i++){
        chara->getBody(i)->setPositionCurrent(pose->getPosition(i));
        chara->getBody(i)->setRotationCurrent(pose->getOrientation(i));
    }
}


void MoCap::loadFrameSimulation()
{

        //para cada frame
        Quaternion q;
        //dQuaternion q;
          //dQFromAxisAndAngle(q,0,0,1,Modelo::grauToRad(90.0));
        //inicializa o vector quatDes

        for (unsigned int i=0;i<capMot.size();i++) {
            std::vector<Quaternion> quatDes;
            for (int k=0;k<this->chara->getNumJoints();k++) {
                quatDes.push_back(Quaternion());
            }
          //calcula angDes
          for( int j=0; j<chara->getNumJoints(); j++ ) {
            //calcula quaternion relativo entre os dois corpos os quais a junta j liga
              Quaternion q,q1t,q2; //q1t - q1 transposto (inverso, conjugado)
              //q1t

              q1t = getFrameMotion(i)->getOrientation(chara->getIdObject(chara->getJoint(j)->getParent()));
              q1t = q1t.conjugate();
              //q2
              q2 = getFrameMotion(i)->getOrientation(chara->getIdObject(chara->getJoint(j)->getChild()));
              //q
              q = q1t*q2; //ou o contrario

              quatDes[j] = q;
                //acho que nao eh mais para fazer isso: quatDes[j].xyz = quatDes[j].xyz.multiplicacao(-1.0); //nao entendi pq deve-se inverter o quaternion, mas deu certo assim
          }
          //inclui o frame no vector moCap

          capMotFrame.push_back(new FrameQuat(quatDes));

        }
        frame_current = 1;
        //printf("In load frame: %d",capMotFrame.size());

}

void MoCap::stepFrame(int value)
{
    //return;
    if (!status) return;
    if (value>=int(this->capMotFrame.size())) value = 1;
    frame_current = value;
    drawShadow(Vec4(-0.5,0,0),value);
}

Vec4 MoCap::positionRelativeCOM(int frame,int foot)
{
    if(!(idfoots.size()>1)) return Vec4();
    Vec4 posMedia = Vec4(); //media da distância entre os pés

    if(foot<0) {
        for(unsigned int i=0;i<idfoots.size();i++)
            posMedia += capMot.at(frame)->getPosition(idfoots.at(i));
        posMedia /= idfoots.size();
    }
    else{
        for(unsigned int i=0;i<idfoots.size();i++)
            if(foot==idfoots.at(i)){
                posMedia += capMot.at(frame)->getPosition(idfoots.at(i));

            }

    }

    Vec4 posCOM = getPosCOM(frame);
    posCOM.x2 = 0;
    posMedia.x2 = 0;
    return posCOM - posMedia;

}

Vec4 MoCap::velocityAngularBody(int frame, int body)
{
    //calculando velocidade desejada da junta (em coordenadas locais)
    Quaternion qIdent;
    Vec4 axis;
    dReal angle;
    //delta entre qDesejado e qDesejadoMaisUm (em coordenadas do corpo prev da junta)
    Quaternion qDesejado = getFrameMotion(frame)->getOrientation(body); //ja foi calculado acima
    //indo pelo caminho mais curto

    qDesejado = qIdent.lessArc(qDesejado); //ja foi calculado acima
    int prox = frame+1;
    if(frame+1>=endClycle){
        prox = beginClycle;
    }
    if(frame+2>=int(capMotFrame.size())) prox = frame;
    Quaternion qDesejadoMaisUm = getFrameMotion(prox)->getOrientation(body);
    //indo pelo caminho mais curto
    qDesejadoMaisUm = qIdent.lessArc(qDesejadoMaisUm);
    Quaternion qDeltaVelDesejada = Quaternion::deltaQuat( qDesejadoMaisUm, qDesejado ); //ou o contrario
    //indo pelo caminho mais curto
    qDeltaVelDesejada = qIdent.lessArc(qDeltaVelDesejada);
    qDeltaVelDesejada.toAxisAngle( &axis, &angle );
    Vec4 deltaVelDesejada = axis*(angle*M_PI/180);
    //velocidade desejada da junta (em coordenadas do corpo prev da junta)
    //velDesejada = deltaVelDesejada/(intervalo*(1/120))
    //(intervalo=1) corresponde a percorrer exatamente um frame no mocap (frameRate do mocap = 120Hz)
    Vec4 velDesejada = deltaVelDesejada*((60./(chara->getScene()->getSimStep()))*60.0);
    //              dVector3 velDesejadaLocal;
    //                velDesejadaLocal[0] = velDesejada.x;
    //                velDesejadaLocal[1] = velDesejada.y;
    //                velDesejadaLocal[2] = velDesejada.z;
    //                velDesejadaLocal[3] = 1.0;

    return velDesejada;
}

Vec4 MoCap::velocityLinearBody(int frame, int body)
{
    Vec4 pos_n = getFrameMotion(frame)->getPosition(body);
    int prox = frame+1;
    if(frame+1>=endClycle){
        prox = beginClycle;
    }
    if(frame+2>=int(capMotFrame.size())) prox = frame;
    Vec4 pos_n1 = getFrameMotion(prox)->getPosition(body);
    Vec4 veldesejada = (pos_n1-pos_n)*((60./(chara->getScene()->getSimStep()))*60);
    return veldesejada;
}

Vec4 MoCap::getMomentumAngular(int frame)
{
    Vec4 posCOM = getPosCOM(frame);
    Vec4 velCOM = getVelCOM(frame);
    Vec4 pos,linVel,angMom,angMomTotal;
    for (int i=0;i<chara->getNumBodies();i++) {
        pos = getFrameMotion(frame-1)->getPosition(i);
        linVel = velocityLinearBody(frame-1,i);
        pos = pos - posCOM;
        angMom = Physics::getAngularMomentumMoCap(chara->getBody(i),velocityAngularBody(frame-1,i),getFrameMotion(frame-1)->getOrientation(i));
        //Iw + r x mv
        angMomTotal += angMom + (pos ^ ((linVel-velCOM)*chara->getBody(i)->getFMass()));
    }
    return angMomTotal;
}

Vec4 MoCap::getMomentumLinear(int frame)
{
    Vec4 rm = Vec4();//somatório do produto das massas com a velocidade do objeto
    for(int i=0;i<chara->getNumBodies();i++){
        rm += velocityLinearBody(frame-1,i)*chara->getBody(i)->getFMass();
    }
    return rm;
}

Vec4 MoCap::getPosCOM(int frame)
{
    float m = 0; //somatório das massas
    Vec4 rm = Vec4();//somatório do produto das massas com a posição do objeto
    for(int i=0;i<chara->getNumBodies();i++){
        rm += getFrameMotion(frame-1)->getPosition(i)*chara->getBody(i)->getFMass();
        m  += chara->getBody(i)->getFMass();
    }
    return rm/m;
}

Vec4 MoCap::getVelCOM(int frame)
{
    float m = 0; //somatório das massas
    Vec4 rm = Vec4();//somatório do produto das massas com a velocidade do objeto
    for(int i=0;i<chara->getNumBodies();i++){
        rm += velocityLinearBody(frame-1,i)*chara->getBody(i)->getFMass();
        m  += chara->getBody(i)->getFMass();
    }
    return rm/m;
}

void MoCap::setBeginClycle(int begin)
{
    beginClycle = begin;
}

int MoCap::getBeginClycle()
{
    return beginClycle;
}

void MoCap::setEndClycle(int end)
{
    endClycle = end;
}

int MoCap::getEndClycle()
{
    return endClycle;
}

int MoCap::sizeFrames()
{
    return capMot.size();
}

void MoCap::setStatusMotion(bool b)
{
    status = b;
    if(chara->getOffset().module()==0) if (status) initializePosesModel(frame_current);
}

void MoCap::restart()
{
    frame_current = 1;
}

int MoCap::currentFrame()
{
    return frame_current;
}

void MoCap::clear()
{
    capMot.clear();
    capMotFrame.clear();
}

void MoCap::drawShadow(Vec4 offset, int frame)
{
    //if (!status) return;
    int sens = Sensor::getHierarchy2UseMocap(chara);
    for(int i=0;i<chara->getNumBodies();i++){
        Vec4 position = getFrameMotion(frame)->getPosition(i);
        Quaternion orientation = getFrameMotion(frame)->getOrientation(i);
        if(i==int(sens-3))
            for(unsigned int j=0;j<foots.size();j++){
                if(i==int(idfoots.at(j))) foots.at(j)->draw(position+offset,orientation,MATERIAL_RUBY);
            }
            //chara->getBody(i)->draw(position+offset,orientation,MATERIAL_RUBY);
        else if(sens==0 && chara->getBody(i)->getFoot())
            for(unsigned int j=0;j<foots.size();j++){
                if(i==int(idfoots.at(j))) foots.at(j)->draw(position+offset,orientation,MATERIAL_RUBY);
            }
            //chara->getBody(i)->draw(position+offset,orientation,MATERIAL_RUBY);
        else
            chara->getBody(i)->draw(position+offset,orientation,MATERIAL_WHITE_RUBBER);
    }
}
