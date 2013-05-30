#include "control.h"
#include "scene/object.h"
#include "physics/functions.h"
#define EPSILON 0.01
//float radToGrau(float angle){
//    return (180*angle)/M_PI;
//}

float grauToRad2(float angle){
    return (angle*M_PI)/180;

}
dReal limitaValor(dReal lim, dReal valor) {
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
    this->enabled = true;
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
              dVector3 ksFixedi;
                //state->getJointKsFixediSaved( i, state->compInertia, ksFixedi );
                ksLocal[0]  = this->ks.x();
                ksLocal[5]  = this->ks.y();
                ksLocal[10] = this->ks.z();
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
                kdLocal[0]  = this->kd.x();
                kdLocal[5]  = this->kd.y();
                kdLocal[10] = this->kd.z();
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
            /*
            for (int j=0;j<3;j++) {
              torque[j] = this->modelo->ks*10.0*deltaGlobal[j] + this->modelo->kd*deltaVelGlobal[j];
                //torque[j] = this->modelo->ks*deltaGlobal[j] + (this->modelo->ks/10.0)*deltaVelGlobal[j];
              //para nao precisar mudar os sliders
              torque[j] *= 100.0;
            }
            //*/
            //printf("\nok!");
            //printf("\n(%.3f,%.3f,%.3f)",torque[0],torque[1],torque[2]);

        //aplica torques
        //--this->modelo->juntas[i]->passT = Vec4(torque[0],torque[1],torque[2]);
            this->joint->getParent()->appTorque(-torque[0],-torque[1],-torque[2]);
            this->joint->getChild()->appTorque(torque[0],torque[1],torque[2]);
          //dBodyAddTorque(this->joint->getParent()->getBody(), -torque[0],-torque[1],-torque[2]);
          //dBodyAddTorque(this->joint->getChild()->getBody(),  torque[0], torque[1], torque[2]);







//    Quaternion qIdent = Quaternion();
//          Vec4 axis;
//          float angle;
//          Quaternion qAtual = Physics::getRotationJoint(this->joint);
//                //indo pelo caminho mais curto
//                //qAtual = qIdent.menorArco(qAtual);
//              Quaternion qDesejado = qIdent;
//                //indo pelo caminho mais curto
//                //qDesejado = qIdent.menorArco(qDesejado);
//            Quaternion qDelta = qDesejado.inverse()*qAtual;//Quaternion::deltaQuat( qDesejado, qAtual );
//              //indo pelo caminho mais curto
//              //qDelta = qIdent.menorArco(qDelta);
//            axis = qDelta.toAxis();
//            angle = qDelta.toAngle();
//              //limT ( na verdade fazendo papel de limA(limAngle) )
//              //dReal angleRadLimitado = limitaValor( /*this->modelo->limT*/0.1*10, (angle*M_PI/180) );
//            //Vec4 delta = axis*angleRadLimitado;
//            Vec4 delta = axis*(angle*M_PI/180);

//          //calculando delta em coordenadas globais (delta esta em coordenadas do corpo prev da junta (considerando sua orientacao inicial) (na simulacao e nao no mocap))
//          dVector3 deltaLocal,deltaGlobal;
//            deltaLocal[0] = delta.x();
//            deltaLocal[1] = delta.y();
//            deltaLocal[2] = delta.z();
//            deltaLocal[3] = 1.0;
//            //matriz de rotacao do corpo prev da junta - considerando sua orientacao inicial
//            const dReal* RCorpoPrev; //dMatrix3
//                RCorpoPrev = dBodyGetRotation(joint->getParent()->getBody());
//              //deve-se levar em consideracao a orientacao inicial dos corpos (iniQ already includes the 90 degrees X rotation for ccylinders)
//              dMatrix3 iniR;
//                //dQtoR( this->modelo->juntas[i]->prev->iniQ, iniR );
//                dQuaternion inidQ;
//                //state->iniQs[ this->modelo->juntas[i]->prevLoc ].to_dQuaternion(inidQ);
//                dQtoR( inidQ, iniR );
//              dMatrix3 RiniRCorpoPrev;
//                dMULTIPLY2_333(RiniRCorpoPrev,RCorpoPrev,iniR); //RiniRCorpoPrev = RCorpoPrev.(iniR)^-1
//              dMULTIPLY0_331(deltaGlobal,RiniRCorpoPrev,deltaLocal); //body to global (RiniRCorpoPrev.deltaLocal)

//        //calculando o deltaVel
//          //calculando velocidade atual da junta (em coordenadas globais)
//            const dReal *velPrevBody = dBodyGetAngularVel (joint->getParent()->getBody());
//            const dReal *velNextBody = dBodyGetAngularVel (joint->getChild()->getBody());
//            dVector3 velGlobal;
//            for (int j=0;j<3;j++) {
//              velGlobal[j] = velNextBody[j] - velPrevBody[j];
//            }

//          dVector3 deltaVelGlobal;
//          for (int j=0;j<3;j++) {
//            deltaVelGlobal[j] = -velGlobal[j]; //desiredVelGlobal[j] - velGlobal[j] = 0.0 - velGlobal[j]
//          }

//          //limT ( na verdade fazendo papel de limAV(limAngularVelocity) )
//            for (int j=0;j<3;j++) {
//              //deltaVelGlobal[j] = limitaValor( /*this->modelo->limT*/0.1*1000, deltaVelGlobal[j] );
//            }

//        //calculando o ks
//          //calculando ksLocal (em coordenadas do corpo prev da junta - considerando sua orientacao inicial)
//            dMatrix3 ksLocal;
//              //zera ksLocal
//              for (int j=0;j<12;j++) { ksLocal[j] = 0.0; }
//            dVector3 ksFixedi;
//              //state->getJointKsFixediSaved( i, state->compInertia, ksFixedi );
//              ksLocal[0]  = ksFixedi[0];
//              ksLocal[5]  = ksFixedi[1];
//              ksLocal[10] = ksFixedi[2];
//          //calculando ks em coordenadas globais
//            dMatrix3 ksGlobal;
//              //se deltaGlobal = R.deltaLocal, entao ksGlobal = R.ksLocal.R^t
//              //no caso, R=RiniRCorpoPrev
//                dMatrix3 ksTemp;
//                // rotate ks matrix
//                dMULTIPLY2_333 (ksTemp,ksLocal,RiniRCorpoPrev);
//                dMULTIPLY0_333 (ksGlobal,RiniRCorpoPrev,ksTemp);

//        //calculando o kd
//          //calculando kdLocal (em coordenadas do corpo prev da junta - considerando sua orientacao inicial)
//            dMatrix3 kdLocal;
//              //zera kdLocal
//              for (int j=0;j<12;j++) { kdLocal[j] = 0.0; }
//              //kdLocal[0]  = ksLocal[0] *state->ratiokdFixed;
//              //kdLocal[5]  = ksLocal[5] *state->ratiokdFixed;
//              //kdLocal[10] = ksLocal[10]*state->ratiokdFixed;
//          //calculando kd em coordenadas globais
//            dMatrix3 kdGlobal;
//              //se deltaGlobal = R.deltaLocal, entao kdGlobal = R.kdLocal.R^t
//              //no caso, R=RiniRCorpoPrev
//                dMatrix3 kdTemp;
//                // rotate kd matrix
//                dMULTIPLY2_333 (kdTemp,kdLocal,RiniRCorpoPrev);
//                dMULTIPLY0_333 (kdGlobal,RiniRCorpoPrev,kdTemp);

//        //formula do PD controller: torque = ks.delta + kd.deltaVel
//          dVector3 torque;
//            dVector3 torqueKs;
//              dMULTIPLY0_331 (torqueKs,ksGlobal,deltaGlobal);
//            dVector3 torqueKd;
//              dMULTIPLY0_331 (torqueKd,kdGlobal,deltaVelGlobal);
//          for (int j=0;j<3;j++) {
//            torque[j] = torqueKs[j] + torqueKd[j];
//          }
//          /*
//          for (int j=0;j<3;j++) {
//            torque[j] = this->modelo->ks*10.0*deltaGlobal[j] + this->modelo->kd*deltaVelGlobal[j];
//              //torque[j] = this->modelo->ks*deltaGlobal[j] + (this->modelo->ks/10.0)*deltaVelGlobal[j];
//            //para nao precisar mudar os sliders
//            torque[j] *= 100.0;
//          }
//          //*/

//      //aplica torques
//      //this->modelo->juntas[i]->passT = Vec4(torque[0],torque[1],torque[2]);
//        dBodyAddTorque(joint->getParent()->getBody(), -torque[0],-torque[1],-torque[2]);
            //        dBodyAddTorque(joint->getParent()->getBody(),  torque[0], torque[1], torque[2]);

}



