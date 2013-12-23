#include "control.h"
#include "scene/object.h"
#include "physics/functions.h"
//#define EPSILON 0.01
//float radToGrau(float angle){
//    return (180*angle)/M_PI;
//}

float ControlPD::grauToRad2(float angle){
    return (angle*M_PI)/180;

}
dReal ControlPD::limitaValor(dReal lim, dReal valor) {
  if (valor >  lim) return  lim;
  if (valor < -lim) return -lim;
  return valor;
}

ControlPD::ControlPD(Joint *joint, Quaternion qwanted, Vec4 ks, Vec4 kd)
{
    qwanted.normalize();
    this->joint = joint;
    this->qwanted = qwanted;
    this->ks = ks;
    this->kd = kd;
    this->propKs = Vec4(1,1,1);
    this->propKd = Vec4(1,1,1);
    this->enabled = true;
    this->enable_inertia = true;
    this->velDesired = Vec4();
    inertia = Matrix(3,3);
    inertia(0,0) = 1;
    inertia(1,1) = 1;
    inertia(2,2) = 1;
}

void ControlPD::setJoint(Joint *joint)
{
    this->joint = joint;
}

Joint *ControlPD::getJoint()
{
    return joint;
}

void ControlPD::setQuaternionWanted(Quaternion qwanted)
{
    this->qwanted = qwanted;
}

Quaternion ControlPD::getQuaternionWanted()
{
    return this->qwanted;
}

void ControlPD::setKs(Vec4 ks)
{
    this->ks = ks;
}

Vec4 ControlPD::getKs()
{
    return this->ks;
}

void ControlPD::setKd(Vec4 kd)
{
    this->kd = kd;
}

Vec4 ControlPD::getKd()
{
    return this->kd;
}

void ControlPD::setEnabled(bool enabled)
{
    this->enabled = enabled;
}

bool ControlPD::isEnabled()
{
    return this->enabled;
}

void ControlPD::setEnabledInertia(bool enabled_inertia)
{
    this->enable_inertia = enabled_inertia;
}

bool ControlPD::isEnabledInertia()
{
    return this->enable_inertia;
}

void ControlPD::evaluate()
{
    if (!this->enabled) return;
    Quaternion qIdent = Quaternion();
    //qIdent.normaliza();
            Vec4 axis;
            dReal angle;
                Quaternion qAtual = Physics::getRotationJoint(this->joint);
                //printf("\n(%.3f,%.3f,%.3f,%.3f)",qAtual.w,qAtual.xyz.x(),qAtual.xyz.y(),qAtual.xyz.z());
                  //indo pelo caminho mais curto
                  qAtual = qIdent.lessArc(qAtual);
                  Quaternion qDesejado = this->qwanted;
                  //indo pelo caminho mais curto
                  qDesejado = qIdent.lessArc(qDesejado);
              Quaternion qDelta = Quaternion::deltaQuat( qDesejado, qAtual );
                //indo pelo caminho mais curto
                qDelta = qIdent.lessArc(qDelta);
              qDelta.toAxisAngle( &axis, &angle );
              //axis.showVec4();
                //limT ( na verdade fazendo papel de limA(limAngle) )
                dReal angleRadLimitado = limitaValor( /*this->modelo->limT*/0.1*10, grauToRad2(angle) );
              Vec4 delta = axis*( angleRadLimitado );
              //Vec4 delta = axis.multiplicacao( Modelo::grauToRad(angle) );

            //calculando delta em coordenadas globais (delta esta em coordenadas do corpo prev da junta (considerando sua orientacao inicial) (na simulacao e nao no mocap))
            dVector3 deltaLocal,deltaGlobal;
            deltaLocal[0] = delta.x();
            deltaLocal[1] = delta.y();
            deltaLocal[2] = delta.z();
              deltaLocal[3] = 1.0;
              //matriz de rotacao do corpo prev da junta - considerando sua orientacao inicial
              const dReal* RCorpoPrev; //dMatrix3
                  RCorpoPrev = dBodyGetRotation(this->joint->getParent()->getBody());
                //deve-se levar em consideracao a orientacao inicial dos corpos (iniQ already includes the 90 degrees X rotation for ccylinders)
                dMatrix3 iniR;

                  dQuaternion inidQ;
                  to_dQuaternion(this->joint->getParent()->getRotation(),inidQ);
                  ////state->iniQs[ this->modelo->juntas[i]->prevLoc ].to_dQuaternion(inidQ);
                  dQtoR( inidQ, iniR );
                dMatrix3 RiniRCorpoPrev;
                  dMULTIPLY2_333(RiniRCorpoPrev,RCorpoPrev,iniR); //RiniRCorpoPrev = RCorpoPrev.(iniR)^-1
                dMULTIPLY0_331(deltaGlobal,RiniRCorpoPrev,deltaLocal); //body to global (RiniRCorpoPrev.deltaLocal)

          //calculando o deltaVel
            //calculando velocidade atual da junta (em coordenadas globais)
              const dReal *velPrevBody = dBodyGetAngularVel (this->joint->getParent()->getBody());
              const dReal *velNextBody = dBodyGetAngularVel (this->joint->getChild()->getBody());
              dVector3 velGlobal;
              for (int j=0;j<3;j++) {
                velGlobal[j] = velNextBody[j] - velPrevBody[j];
              }

            dVector3 deltaVelGlobal;
            //for (int j=0;j<3;j++) {
              deltaVelGlobal[0] = velDesired.x()-velGlobal[0]; //desiredVelGlobal[j] - velGlobal[j] = 0.0 - velGlobal[j]
              deltaVelGlobal[1] = velDesired.y()-velGlobal[1];
              deltaVelGlobal[2] = velDesired.z()-velGlobal[2];
            //}


            //limT ( na verdade fazendo papel de limAV(limAngularVelocity) )
              for (int j=0;j<3;j++) {
                deltaVelGlobal[j] = limitaValor( /*this->modelo->limT*/0.1*1000, deltaVelGlobal[j] );
              }

          //calculando o ks
            //calculando ksLocal (em coordenadas do corpo prev da junta - considerando sua orientacao inicial)
              dMatrix3 ksLocal;
                //zera ksLocal
                for (int j=0;j<12;j++) { ksLocal[j] = 0.0; }
              //dVector3 ksFixedi;
                //state->getJointKsFixediSaved( i, state->compInertia, ksFixedi );
                ksLocal[0]  = this->ks.x()*propKs.x()*inertia(0,0);
                ksLocal[5]  = this->ks.y()*propKs.y()*inertia(1,1);
                ksLocal[10] = this->ks.z()*propKs.z()*inertia(2,2);
            //calculando ks em coordenadas globais
              dMatrix3 ksGlobal;
                //se deltaGlobal = R.deltaLocal, entao ksGlobal = R.ksLocal.R^t
                //no caso, R=RiniRCorpoPrev
                  dMatrix3 ksTemp;
                  // rotate ks matrix
                  dMULTIPLY2_333 (ksTemp,ksLocal,RiniRCorpoPrev);
                  dMULTIPLY0_333 (ksGlobal,RiniRCorpoPrev,ksTemp);

          //calculando o kd
            //calculando kdLocal (em coordenadas do corpo prev da junta - considerando sua orientacao inicial)
              dMatrix3 kdLocal;
                //zera kdLocal
                for (int j=0;j<12;j++) { kdLocal[j] = 0.0; }
                kdLocal[0]  = this->kd.x()*propKd.x()*inertia(0,0);
                kdLocal[5]  = this->kd.y()*propKd.y()*inertia(1,1);
                kdLocal[10] = this->kd.z()*propKd.z()*inertia(2,2);
            //calculando kd em coordenadas globais
              dMatrix3 kdGlobal;
                //se deltaGlobal = R.deltaLocal, entao kdGlobal = R.kdLocal.R^t
                //no caso, R=RiniRCorpoPrev
                  dMatrix3 kdTemp;
                  // rotate kd matrix
                  dMULTIPLY2_333 (kdTemp,kdLocal,RiniRCorpoPrev);
                  dMULTIPLY0_333 (kdGlobal,RiniRCorpoPrev,kdTemp);

          //formula do PD controller: torque = ks.delta + kd.deltaVel
            dVector3 torque;
              dVector3 torqueKs;
                dMULTIPLY0_331 (torqueKs,ksGlobal,deltaGlobal);
              dVector3 torqueKd;
                dMULTIPLY0_331 (torqueKd,kdGlobal,deltaVelGlobal);
            for (int j=0;j<3;j++) {
              torque[j] = torqueKs[j] + torqueKd[j];
            }
//            if(joint->parent->getFoot())
//                this->joint->getParent()->addTorque(-torque[0]*0.5,-torque[1]*0.5,-torque[2]*0.5);
//            else
            Vec4 tq = Vec4(torque[0],torque[1],torque[2]);

            //Vec4 tq = limitingTorquePD(Vec4(torque[0],torque[1],torque[2]));
            //Vec4 tq_father = limitingTorquePD(Vec4(-torque[0],-torque[1],-torque[2]));

            //if (this->joint->getParent()->getFoot()) printf("\nControle PD: tq(%.3f,%.3f,%.3f)",-tq.x(),-tq.y(),-tq.z());
               this->joint->getParent()->addTorque(-tq.x(),-tq.y(),-tq.z());
            //if(this->joint->getChild()->getFoot()) printf("\nControle PD: tq(%.3f,%.3f,%.3f)",tq.x(),tq.y(),tq.z());
               this->joint->getChild()->addTorque(tq.x(),tq.y(),tq.z());

}

void ControlPD::updateKsKdCoros(float MassTotal)
{

    ks = Vec4(MassTotal,MassTotal,MassTotal);
    kd = Vec4(1,1,1)*2*sqrt(MassTotal);
}

void ControlPD::setProportionalKs(Vec4 pks)
{
    propKs = pks;
}

void ControlPD::setProportionalKd(Vec4 pkd)
{
    propKd = pkd;
}

void ControlPD::setVelocityDesired(Vec4 vel)
{
    this->velDesired = vel;
}

Quaternion ControlPD::getOrientation()
{
    Quaternion qIdent;
    Quaternion qAtual = Physics::getRotationJoint(this->joint);
    qAtual = qIdent.lessArc(qAtual);
    return qAtual;
}

Vec4 ControlPD::getProportionalKs()
{
    return propKs;
}

Vec4 ControlPD::getProportionalKd()
{
    return propKd;
}

Vec4 ControlPD::limitingTorquePD(Vec4 tq)
{
    Vec4 sup = joint->getTorqueMax();
    Vec4 result;
    if(tq.x()>sup.x()) result.x1 = sup.x();
    else result.x1 = tq.x();
    if(tq.y()>sup.y()) result.x2 = sup.y();
    else result.x2 = tq.y();
    if(tq.z()>sup.z()) result.x3 = sup.z();
    else result.x3 = tq.z();

    return result;
}

Vec4 ControlPD::limitingTorquePD(Vec4 tq, Joint *j)
{
    Vec4 sup = j->getTorqueMax();
    Vec4 result;
    if(tq.x()>sup.x()) result.x1 = sup.x();
    else result.x1 = tq.x();
    if(tq.y()>sup.y()) result.x2 = sup.y();
    else result.x2 = tq.y();
    if(tq.z()>sup.z()) result.x3 = sup.z();
    else result.x3 = tq.z();

    return result;
}

Vec4 ControlPD::getTorquePD(Joint *joint, Vec4 ks, Vec4 kd, Quaternion qDesired)
{
    Quaternion qIdent = Quaternion();
    //qIdent.normaliza();
            Vec4 axis;
            dReal angle;
                Quaternion qAtual = Physics::getRotationJoint(joint);
                //printf("\n(%.3f,%.3f,%.3f,%.3f)",qAtual.w,qAtual.xyz.x(),qAtual.xyz.y(),qAtual.xyz.z());
                  //indo pelo caminho mais curto
                  qAtual = qIdent.lessArc(qAtual);
                  Quaternion qDesejado = qDesired;
                  //indo pelo caminho mais curto
                  qDesejado = qIdent.lessArc(qDesejado);
              Quaternion qDelta = Quaternion::deltaQuat( qDesejado, qAtual );
                //indo pelo caminho mais curto
                qDelta = qIdent.lessArc(qDelta);
              qDelta.toAxisAngle( &axis, &angle );
              //axis.showVec4();
                //limT ( na verdade fazendo papel de limA(limAngle) )
                dReal angleRadLimitado = limitaValor( /*this->modelo->limT*/0.1*10, grauToRad2(angle) );
              Vec4 delta = axis*( angleRadLimitado );
              //Vec4 delta = axis.multiplicacao( Modelo::grauToRad(angle) );

            //calculando delta em coordenadas globais (delta esta em coordenadas do corpo prev da junta (considerando sua orientacao inicial) (na simulacao e nao no mocap))
            dVector3 deltaLocal,deltaGlobal;
            deltaLocal[0] = delta.x();
            deltaLocal[1] = delta.y();
            deltaLocal[2] = delta.z();
              deltaLocal[3] = 1.0;
              //matriz de rotacao do corpo prev da junta - considerando sua orientacao inicial
              const dReal* RCorpoPrev; //dMatrix3
                  RCorpoPrev = dBodyGetRotation(joint->getParent()->getBody());
                //deve-se levar em consideracao a orientacao inicial dos corpos (iniQ already includes the 90 degrees X rotation for ccylinders)
                dMatrix3 iniR;

                  dQuaternion inidQ;
                  to_dQuaternion(joint->getParent()->getRotation(),inidQ);
                  ////state->iniQs[ this->modelo->juntas[i]->prevLoc ].to_dQuaternion(inidQ);
                  dQtoR( inidQ, iniR );
                dMatrix3 RiniRCorpoPrev;
                  dMULTIPLY2_333(RiniRCorpoPrev,RCorpoPrev,iniR); //RiniRCorpoPrev = RCorpoPrev.(iniR)^-1
                dMULTIPLY0_331(deltaGlobal,RiniRCorpoPrev,deltaLocal); //body to global (RiniRCorpoPrev.deltaLocal)

          //calculando o deltaVel
            //calculando velocidade atual da junta (em coordenadas globais)
              const dReal *velPrevBody = dBodyGetAngularVel (joint->getParent()->getBody());
              const dReal *velNextBody = dBodyGetAngularVel (joint->getChild()->getBody());
              dVector3 velGlobal;
              for (int j=0;j<3;j++) {
                velGlobal[j] = velNextBody[j] - velPrevBody[j];
              }

            dVector3 deltaVelGlobal;
            for (int j=0;j<3;j++) {
              deltaVelGlobal[j] = -velGlobal[j]; //desiredVelGlobal[j] - velGlobal[j] = 0.0 - velGlobal[j]
            }

            //limT ( na verdade fazendo papel de limAV(limAngularVelocity) )
              for (int j=0;j<3;j++) {
                deltaVelGlobal[j] = limitaValor( /*this->modelo->limT*/0.1*1000, deltaVelGlobal[j] );
              }

          //calculando o ks
            //calculando ksLocal (em coordenadas do corpo prev da junta - considerando sua orientacao inicial)
              dMatrix3 ksLocal;
                //zera ksLocal
                for (int j=0;j<12;j++) { ksLocal[j] = 0.0; }
              //dVector3 ksFixedi;
                //state->getJointKsFixediSaved( i, state->compInertia, ksFixedi );
                ksLocal[0]  = ks.x();
                ksLocal[5]  = ks.y();
                ksLocal[10] = ks.z();
            //calculando ks em coordenadas globais
              dMatrix3 ksGlobal;
                //se deltaGlobal = R.deltaLocal, entao ksGlobal = R.ksLocal.R^t
                //no caso, R=RiniRCorpoPrev
                  dMatrix3 ksTemp;
                  // rotate ks matrix
                  dMULTIPLY2_333 (ksTemp,ksLocal,RiniRCorpoPrev);
                  dMULTIPLY0_333 (ksGlobal,RiniRCorpoPrev,ksTemp);

          //calculando o kd
            //calculando kdLocal (em coordenadas do corpo prev da junta - considerando sua orientacao inicial)
              dMatrix3 kdLocal;
                //zera kdLocal
                for (int j=0;j<12;j++) { kdLocal[j] = 0.0; }
                kdLocal[0]  = ksLocal[0]*0.1;
                kdLocal[5]  = ksLocal[5]*0.1;
                kdLocal[10] = ksLocal[10]*0.1;
            //calculando kd em coordenadas globais
              dMatrix3 kdGlobal;
                //se deltaGlobal = R.deltaLocal, entao kdGlobal = R.kdLocal.R^t
                //no caso, R=RiniRCorpoPrev
                  dMatrix3 kdTemp;
                  // rotate kd matrix
                  dMULTIPLY2_333 (kdTemp,kdLocal,RiniRCorpoPrev);
                  dMULTIPLY0_333 (kdGlobal,RiniRCorpoPrev,kdTemp);

          //formula do PD controller: torque = ks.delta + kd.deltaVel
            dVector3 torque;
              dVector3 torqueKs;
                dMULTIPLY0_331 (torqueKs,ksGlobal,deltaGlobal);
              dVector3 torqueKd;
                dMULTIPLY0_331 (torqueKd,kdGlobal,deltaVelGlobal);
            for (int j=0;j<3;j++) {
              torque[j] = torqueKs[j] + torqueKd[j];
            }
            return Vec4(torque[0],torque[1],torque[2]);

}

Vec4 ControlPD::getTorquePDCOM(Joint *joint, Vec4 ks, Vec4 kd, Quaternion qDesired, Vec4 velocity)
{
    Quaternion qIdent = Quaternion();
    //qIdent.normaliza();
            Vec4 axis;
            dReal angle;
                Quaternion qAtual = joint->getParent()->getRotationCurrent();//Physics::getRotationJointInit(joint);
                //printf("\n(%.3f,%.3f,%.3f,%.3f)",qAtual.w,qAtual.xyz.x(),qAtual.xyz.y(),qAtual.xyz.z());
                  //indo pelo caminho mais curto
                  qAtual = qIdent.lessArc(qAtual);
                  Quaternion qDesejado = qDesired;
                  //indo pelo caminho mais curto
                  qDesejado = qIdent.lessArc(qDesejado);
              Quaternion qDelta = Quaternion::deltaQuat( qDesejado, qAtual );
                //indo pelo caminho mais curto
                qDelta = qIdent.lessArc(qDelta);
              qDelta.toAxisAngle( &axis, &angle );
              //axis.showVec4();
                //limT ( na verdade fazendo papel de limA(limAngle) )
                dReal angleRadLimitado = limitaValor( /*this->modelo->limT*/0.1*10, grauToRad2(angle) );
              Vec4 delta = axis*( angleRadLimitado );
              return ks.mult(delta) - kd.mult(velocity);
//              //Vec4 delta = axis.multiplicacao( Modelo::grauToRad(angle) );

//            //calculando delta em coordenadas globais (delta esta em coordenadas do corpo prev da junta (considerando sua orientacao inicial) (na simulacao e nao no mocap))
//            dVector3 deltaLocal,deltaGlobal;
//            deltaGlobal[0] = delta.x();
//            deltaGlobal[1] = delta.y();
//            deltaGlobal[2] = delta.z();
//              deltaGlobal[3] = 1.0;
//              //matriz de rotacao do corpo prev da junta - considerando sua orientacao inicial
////              const dReal* RCorpoPrev; //dMatrix3
////                  RCorpoPrev = dBodyGetRotation(joint->getParent()->getBody());
////                //deve-se levar em consideracao a orientacao inicial dos corpos (iniQ already includes the 90 degrees X rotation for ccylinders)
////                dMatrix3 iniR;

////                  dQuaternion inidQ;
////                  to_dQuaternion(joint->getParent()->getRotation(),inidQ);
////                  ////state->iniQs[ this->modelo->juntas[i]->prevLoc ].to_dQuaternion(inidQ);
////                  dQtoR( inidQ, iniR );
//                dMatrix3 RiniRCorpoPrev;
////                  dMULTIPLY2_333(RiniRCorpoPrev,RCorpoPrev,iniR); //RiniRCorpoPrev = RCorpoPrev.(iniR)^-1
//                dMULTIPLY0_331(deltaGlobal,RiniRCorpoPrev,deltaLocal); //body to global (RiniRCorpoPrev.deltaLocal)

//          //calculando o deltaVel
//            //calculando velocidade atual da junta (em coordenadas globais)

//              //const dReal *vel = {velocity.x(),velocity.y(),velocity.z()};
//              dVector3 velGlobal;
//              velGlobal[0] = velocity.x();
//              velGlobal[1] = velocity.y();
//              velGlobal[2] = velocity.z();
////              for (int j=0;j<3;j++) {
////                velGlobal[j] = vel[j];
////              }


//            dVector3 deltaVelGlobal;
//            for (int j=0;j<3;j++) {
//              deltaVelGlobal[j] = -velGlobal[j]; //desiredVelGlobal[j] - velGlobal[j] = 0.0 - velGlobal[j]
//            }

//            //limT ( na verdade fazendo papel de limAV(limAngularVelocity) )
//              for (int j=0;j<3;j++) {
//                deltaVelGlobal[j] = limitaValor( /*this->modelo->limT*/0.1*10, deltaVelGlobal[j] );
//              }

//          //calculando o ks
//            //calculando ksLocal (em coordenadas do corpo prev da junta - considerando sua orientacao inicial)
//              dMatrix3 ksLocal;
//                //zera ksLocal
//                for (int j=0;j<12;j++) { ksLocal[j] = 0.0; }
//              //dVector3 ksFixedi;
//                //state->getJointKsFixediSaved( i, state->compInertia, ksFixedi );
//                ksLocal[0]  = ks.x();
//                ksLocal[5]  = ks.y();
//                ksLocal[10] = ks.z();
//            //calculando ks em coordenadas globais
//              dMatrix3 ksGlobal;
//                //se deltaGlobal = R.deltaLocal, entao ksGlobal = R.ksLocal.R^t
//                //no caso, R=RiniRCorpoPrev
//                  dMatrix3 ksTemp;
//                  // rotate ks matrix
//                  dMULTIPLY2_333 (ksTemp,ksLocal,RiniRCorpoPrev);
//                  dMULTIPLY0_333 (ksGlobal,RiniRCorpoPrev,ksTemp);

//          //calculando o kd
//            //calculando kdLocal (em coordenadas do corpo prev da junta - considerando sua orientacao inicial)
//              dMatrix3 kdLocal;
//                //zera kdLocal
//                for (int j=0;j<12;j++) { kdLocal[j] = 0.0; }
//                kdLocal[0]  = kd.x();
//                kdLocal[5]  = kd.y();
//                kdLocal[10] = kd.z();
//            //calculando kd em coordenadas globais
//              dMatrix3 kdGlobal;
//                //se deltaGlobal = R.deltaLocal, entao kdGlobal = R.kdLocal.R^t
//                //no caso, R=RiniRCorpoPrev
//                  dMatrix3 kdTemp;
//                  // rotate kd matrix
//                  dMULTIPLY2_333 (kdTemp,kdLocal,RiniRCorpoPrev);
//                  dMULTIPLY0_333 (kdGlobal,RiniRCorpoPrev,kdTemp);

//          //formula do PD controller: torque = ks.delta + kd.deltaVel
//            dVector3 torque;
//              dVector3 torqueKs;
//                dMULTIPLY0_331 (torqueKs,ksGlobal,deltaGlobal);
//              dVector3 torqueKd;
//                dMULTIPLY0_331 (torqueKd,kdGlobal,deltaVelGlobal);
//            for (int j=0;j<3;j++) {
//              torque[j] = torqueKs[j] + torqueKd[j];
//            }
              //            return Vec4(torque[0],torque[1],torque[2]);
}

void ControlPD::setInertiaFactors(Matrix i)
{
    if(enable_inertia) inertia = i;
    else resetInertiaFactors();
}

void ControlPD::resetInertiaFactors()
{
    inertia = Matrix(3,3);
    inertia(0,0) = 1;
    inertia(1,1) = 1;
    inertia(2,2) = 1;
}





