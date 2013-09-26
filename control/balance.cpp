#include "balance.h"
#include "scene/scene.h"
#include "scene/character.h"
#include "scene/joint.h"
#include "scene/object.h"
#include "control.h"
#include "graphics/draw.h"
#include "sensor.h"
#include "mocap/frame.h"

Balance::Balance(Character* chara)
{
    this->chara = chara;
    contructRelationJointsBodies();
    //ks.setVec4(chara->getMassTotal(),chara->getMassTotal(),chara->getMassTotal());
    //kd.setVec4(2*sqrt(chara->getMassTotal()),2*sqrt(chara->getMassTotal()),2*sqrt(chara->getMassTotal()));
    ksTorque = Vec4(100,100,100);
    kdTorque = Vec4(10,10,10);
    ksForce = Vec4(200,200,200);
    kdForce = Vec4(20,20,20);
    kmom = Vec4(5,5,5);
    this->enable_force = true;
    this->enable_torque = true;
    this->enable_momentum = true;
    this->compensation = 1.0;
    this->bdesired = Quaternion();
    this->enable_balance = true;
    this->useHierarchy = FOOTS_AIR+3;
}

void Balance::contructRelationJointsBodies()
{
    relationJointsB = Matrix(chara->getNumJoints(),chara->getNumBodies());
    //inicialização
    for(int i=0;i<chara->getNumJoints();i++)
        for(int j=0;j<chara->getNumBodies();j++)
             relationJointsB(i,j) = -1;
    //preenchimento
    for(int i=0;i<chara->getNumJoints();i++)
        for(int j=0;j<chara->getNumBodies();j++)
            if(chara->isBodyHierarchy(chara->getJoint(i),chara->getBody(j))) relationJointsB(i,j) = 1;


    //relationJointsB.print(1);//imprime a matriz
}

void Balance::contructRelationJoints()
{
    relationJoints = Matrix(chara->getNumJoints(),chara->getNumJoints());
}

Matrix Balance::getJacobianSum(Object *obj)
{
    Matrix Ad_j2w = Matrix(6,6*this->chara->getNumJoints()); // matriz adjunta da junta para o mundo do corpo obj
    int iBody = chara->getPositionBody(obj); //id do objeto
    if (iBody<0) return Ad_j2w;
    //para cada junta que influencia o corpo b (corpo raiz eh a stance paw)
    for (unsigned int i=0;i<this->chara->getNumJoints();i++) {
        Matrix Ad_j2w_i(6,6); //adjunta da junta para corpo obj
        if (chara->hierarchy[useHierarchy][i][iBody]) {
            Ad_j2w_i = chara->getJoint(i)->getAd();
        }
        Ad_j2w.setSubmatrix(0,6*i,Ad_j2w_i);
    }
    return Ad_j2w;
}

Vec Balance::getTwistWrenchTotal(Vec twist)
{


    //M_com_com
    Matrix M_com2com(6,6);
    Matrix M_b2com(6,6);
    //Vec gcomTotal = Vec(6);

    //matriz jacobiana de 6 linhas e (6*num_juntas) colunas
    Matrix J = Matrix(6,6*this->chara->getNumJoints());
      for (unsigned int b=0;b<this->chara->getNumBodies();b++) {
        //J = J_com_com = Sum_b( J_com_com_b )
          //J_com_com_b = Ad_w_com^t . M_w_b . Ad_w_j
            Matrix J_b2com = Matrix(6,6*this->chara->getNumJoints());
            //Ad_w_com^t . M_w_b = Ad_b_com^t . M_b_b . Ad_b_w
              Matrix Ad_w2b = this->chara->getBody(b)->getAd();
              Matrix M_b2b = this->chara->getBody(b)->getIM();
              //matrix Ad_b_com_t = ModesGen::getAd(this->modelo->bodyGeoms[b],this->modelo).transpose();
                Matrix Ad_com2b = this->chara->getBody(b)->getAd(this->chara);
              Matrix Ad_com2b_t = Ad_com2b.transpose();
            //Ad_w_j
              Matrix Ad_j2w = getJacobianSum(this->chara->getBody(b));

            J_b2com = (Ad_com2b_t * M_b2b * Ad_w2b * Ad_j2w);
            //gcomTotal = gcomTotal + Vec(Vec4(0,this->chara->getBody(b)->getFMass()*10,0),Vec4(0,0,0));
          J = J + (J_b2com);
        //M_com_com = Sum_b( Ad_b_com^t . M_b_b . Ad_b_com )
            M_b2com = Ad_com2b_t * M_b2b * Ad_com2b;
          M_com2com = M_com2com + M_b2com;
      }

    //M_com_com^-1 * jacobian
    Matrix M_com2com_inv = M_com2com.inverse();
    Matrix M_invJ = M_com2com_inv * J;

    //jacobian transposta
    //matrix J_transp = J.transpose();
    Matrix J_transp = M_invJ.transpose();

    //wrench_com_com
    Vec wrench_com2com = twist;
      //wrench_com_com.print();

    //wrench_j_j
    Vec wrench_j2j = J_transp * wrench_com2com;

    return wrench_j2j;
}

Vec4 Balance::getKsTorque()
{
    return ksTorque;
}

void Balance::setKsTorque(Vec4 kst)
{
    this->ksTorque = kst;
}

Vec4 Balance::getKdTorque()
{
    return kdTorque;
}

void Balance::setKdTorque(Vec4 kdt)
{
    this->kdTorque = kdt;
}

Vec4 Balance::getKsForce()
{
    return ksForce;
}

void Balance::setKsForce(Vec4 ksf)
{
    this->ksForce = ksf;
}

Vec4 Balance::getKdForce()
{
    return kdForce;
}

void Balance::setKdForce(Vec4 kdf)
{
    this->kdForce = kdf;
}

Vec4 Balance::getKMomentum()
{
    return this->kmom;
}

Vec4 Balance::getDesiredQuaternion()
{
    return bdesired.toEuler();
}

void Balance::setDeriredQuaternion(Vec4 euler)
{
    bdesired.fromEuler(euler);
}

void Balance::setKMomentum(Vec4 k)
{
    this->kmom = k;
}

float Balance::getCompensation()
{
    return this->compensation;
}

void Balance::setCompensation(float f)
{
    this->compensation = f;
}

void Balance::setEnableTorque(bool b)
{
    this->enable_torque = b;
}

void Balance::setEnableForce(bool b)
{
    this->enable_force = b;
}

void Balance::setEnableMomentum(bool b)
{
    this->enable_momentum = b;
}

Vec4 Balance::limitingTorque(Vec4 lim_inf, Vec4 lim_sup, Vec4 torque)
{
    if(torque.x()>lim_sup.x() || torque.x()<lim_inf.x())
        if (torque.x()>lim_sup.x()) torque.x1 = lim_sup.x();
        else torque.x1 = lim_inf.x();
    else if (torque.y()>lim_sup.y()|| torque.y()<lim_inf.y())
            if (torque.y()>lim_sup.y()) torque.x2 = lim_sup.y();
            else torque.x2 = lim_inf.y();
    else
        if (torque.z()>lim_sup.z() || torque.z()<lim_inf.z())
            if (torque.z()>lim_sup.z()) torque.x3 = lim_sup.z();
            else torque.x3 = lim_inf.z();
    return torque;

}


void Balance::evaluate()
{
    //int se = Sensor::getStateFoots(this->chara);
    //printf("\nEstado do sensor: %d",se);

    if(!(enable_balance)) return;
    int frame = chara->getMoCap()->frame_current;
    //chara->showHierarchies();
    if(chara->getMoCap()->sizeFrames()>0)
        if(chara->getMoCap()->status){
            useHierarchy = Sensor::getHierarchy2UseMocap(chara);
            //if(useHierarchy!=0) printf("\nSensor: pé %d",useHierarchy);
        }else{
            useHierarchy = Sensor::getHierarchy2Use(chara);
        }
    else
        useHierarchy = Sensor::getHierarchy2Use(chara);
    if(useHierarchy==FOOTS_AIR+3 || useHierarchy==FOOTS_AIR_INV+3) return;
    //_ significa projetado no chão, ou seja y = 0;
    //Fcom = ks *(Péapoio_ - COM_) - kd * velCOM_  //calculo da força
    //Tcom = ks *(qd - qa^-1) - kd*(wa)
    Vec4 Cfoot_ = Vec4(); //centro de apoio projetado no plano xz
    int count = 0;
    //if(useHierarchy==0)
    for (unsigned int i=0;i<chara->getNumBodies();i++)
        if (chara->getBody(i)->getFoot()){
            Cfoot_ += chara->getBody(i)->getPositionCurrent();
            count++;
        }

    if (!count) return; //caso o personagem não tenha nenhum pé
    Cfoot_ = Cfoot_/count;
    Cfoot_ = Vec4(Cfoot_.x(),0,Cfoot_.z());
    Vec4 COM_   = chara->getPosCOM();
    Vec4 velCOM_ = chara->getVelCOM();
    COM_ = Vec4(COM_.x(),0,COM_.z());
    velCOM_ = Vec4(velCOM_.x(),0,velCOM_.z());
    Vec4 velCOM_moCap;
    if(chara->getMoCap()->sizeFrames()>0)
        if(chara->getMoCap()->status){
            velCOM_moCap = chara->getMoCap()->getVelCOM(frame);
        }
    Vec momentum;

    Vec4 mom_ang_des;
    Vec4 mom_lin_des;
    if((chara->getMoCap()->sizeFrames()>0)&&(chara->getMoCap()->status))
            mom_ang_des = chara->getMoCap()->getMomentumAngular(frame);
            mom_lin_des = chara->getMoCap()->getMomentumLinear(frame);
    if (enable_momentum)
        momentum = Vec((mom_ang_des-chara->getAngularMomentum()).mult(kmom),(mom_lin_des-chara->getLinearMomentum()).mult(kmom));
    else
        momentum = Vec(Vec4(),Vec4());

    //computando a força no centro de massa
    Vec4 Fcom = ksForce.mult(Cfoot_ - COM_) + kdForce.mult(velCOM_moCap - velCOM_);

    //computando o torque no centro de massa
    Joint* jDes = chara->getJointParentBalance();
    Vec4 Tcom = Vec4(0,0,0);
    if (jDes!=NULL)
        if((chara->getMoCap()->sizeFrames()>0)&&(chara->getMoCap()->status)){
           Tcom = ControlPD::getTorquePDCOM(jDes,ksTorque,kdTorque,chara->getMoCap()->getFrameMotion(frame)->getOrientation(chara->getIdObject(jDes->getParent())),jDes->getParent()->getRelVelAngular()-chara->getMoCap()->velocityAngularBody(frame,chara->getIdObject(jDes->getParent())));
        }else
        Tcom = ControlPD::getTorquePDCOM(jDes,ksTorque,kdTorque,bdesired,jDes->getParent()->getRelVelAngular());
    else
        printf("\nSem junta desejada!");

    Vec wrench;
    if (enable_force && enable_torque)
        wrench = Vec(Tcom,Fcom-chara->getScene()->getGravity()*chara->getMassTotal()) + momentum;
    else if (enable_force)
        wrench = Vec(Vec4(),Fcom-chara->getScene()->getGravity()*chara->getMassTotal()) + momentum;
    else if (enable_torque)
        wrench = Vec(Tcom,chara->getScene()->getGravity()*chara->getMassTotal()) + momentum;
    else
        wrench = momentum;

    Vec wrenchTotal = getTwistWrenchTotal(wrench);
    //printf("Matrix in use: %d",useHierarchy);
    float factor = 1.0;
    if(useHierarchy==0) factor = 0.5;
    else factor = 1;
    for (unsigned int i=0;i<chara->getNumJoints();i++){
        Joint *joint = chara->getJoint(i);
        Vec4 torque(wrenchTotal[i+i*5],wrenchTotal[i+1+i*5],wrenchTotal[i+2+i*5]);
        //torque = limitingTorque(joint->getTorqueMax()*(-1),joint->getTorqueMax(),torque);
        if(chara->hierarchy[useHierarchy][i][chara->getPositionBody(joint->getChild())]){
            if (joint->getChild()->getFoot())
                joint->getChild()->addTorque((torque)*(1-compensation)*factor);
            else
                joint->getChild()->addTorque((torque)*factor);
            if (joint->getParent()->getFoot())
                joint->getParent()->addTorque((torque)*(-1)*(1-compensation)*factor);
            else
                joint->getParent()->addTorque((torque)*(-1)*factor);
        }else{
            if (joint->getParent()->getFoot())
                joint->getParent()->addTorque((torque)*(1-compensation)*factor);
            else
                joint->getParent()->addTorque((torque)*factor);
            if (joint->getChild()->getFoot())
                joint->getChild()->addTorque((torque)*(-1)*(1-compensation)*factor);    
            else
                joint->getChild()->addTorque((torque)*(-1)*factor);
        }

    }


}

void Balance::setEnableBalance(bool b)
{
    this->enable_balance = b;
}

