#include "physics/physics.h"
#include "scene/object.h"
#include "scene/scene.h"
#include "math/matrix4x4.h"
#include "math/vec4.h"
#include "math/quaternion.h"
#include "scene/character.h"
#include "scene/joint.h"
#include "functions.h"

//#ifdef DEBUG_MODE
#include <iostream>
using namespace std;
//#endif
static Scene* scn = NULL;


void Physics::nearCallback(void *data, dGeomID o1, dGeomID o2){
    if (dGeomIsSpace (o1) || dGeomIsSpace (o2)) {
        // Collide a space with everything else
        dSpaceCollide2 (o1, o2, data,&nearCallback);

    }else{
        Scene *scene = ((Object*)dGeomGetData(o1))->getScene();

        dBodyID b1 = dGeomGetBody(o1);
        dBodyID b2 = dGeomGetBody(o2);

        int numcont = 4;
        dContact contact[numcont];

        if (int numc = dCollide (o1,o2,numcont,&contact[0].geom,sizeof(dContact))) {
            for(int i=0;i<numc;i++){

                contact[i].surface.mode = dContactBounce; // | dContactSoftCFM;
                // friction parameter
                contact[i].surface.mu = dInfinity;
                // bounce is the amount of "bouncyness".
                contact[i].surface.bounce = 0.1;
                // bounce_vel is the minimum incoming velocity to cause a bounce
                contact[i].surface.bounce_vel = 0.0;
                // constraint force mixing parameter
                //contact.surface.soft_cfm = 0.001;

                dJointID c = dJointCreateContact (scene->getWorld(),scene->getJointGroup(),&contact[i]);
                dJointAttach (c,b1,b2);
            }
        }
    }
}


void Physics::worldStep(WorldID world, float stepSize){
    dWorldQuickStep(world, stepSize);
}

void Physics::simSingleStep (Scene *scene)
{
    // find collisions and add contact joints
    scn = scene;
    dSpaceCollide (scene->getSpace(),0,&nearCallback);
    // step the simulation
    dWorldQuickStep (scene->getWorld(),0.0005);
    // remove all contact joints
    dJointGroupEmpty (scene->getJointGroup());
}

void Physics::initCharacter(Character *chara){
    chara->setSpace(dHashSpaceCreate(chara->getScene()->getSpace()));
    chara->setJointGroup(dJointGroupCreate(0));
}

void Physics::closeCharacter(Character *chara){
    dJointGroupDestroy(chara->getJointGroup());
    dSpaceDestroy(chara->getSpace());
}

Quaternion Physics::getRotationJoint(Joint *joint)
{
    const dReal* qAux;
      dQuaternion q,qPrev,qPrevt,qNext; //qPrevt - qPrev transposto (inverso, conjugado)
      //qPrev
        qAux = dBodyGetQuaternion(joint->getParent()->getBody());
        qPrev[0] = qAux[0]; qPrev[1] = qAux[1]; qPrev[2] = qAux[2]; qPrev[3] = qAux[3];
            dQuaternion iniQt,q2; //q1t - q1 transposto (inverso, conjugado)
            //q1
              dQuaternion inidQ;
              to_dQuaternion(joint->getParent()->getRotation(),inidQ);
              //state->iniQs[ junta->prevLoc ].to_dQuaternion(inidQ);
            //q1t
              //iniQt[0] = junta->prev->iniQ[0]; iniQt[1] = -junta->prev->iniQ[1]; iniQt[2] = -junta->prev->iniQ[2]; iniQt[3] = -junta->prev->iniQ[3];
              iniQt[0] = inidQ[0]; iniQt[1] = -inidQ[1]; iniQt[2] = -inidQ[2]; iniQt[3] = -inidQ[3];
            //q2
              q2[0] = qPrev[0]; q2[1] = qPrev[1]; q2[2] = qPrev[2]; q2[3] = qPrev[3];
            //q
              dQMultiply0 (qPrev, q2, iniQt); //ou o contrario
      //qPrevt
        qPrevt[0] = qPrev[0]; qPrevt[1] = -qPrev[1]; qPrevt[2] = -qPrev[2]; qPrevt[3] = -qPrev[3];
      //qNext
        qAux = dBodyGetQuaternion(joint->getChild()->getBody());
        qNext[0] = qAux[0]; qNext[1] = qAux[1]; qNext[2] = qAux[2]; qNext[3] = qAux[3];
            //dQuaternion iniQt,q2; //q1t - q1 transposto (inverso, conjugado)
            //q1
              //dQuaternion inidQ;
        to_dQuaternion(joint->getChild()->getRotation(),inidQ);
              //state->iniQs[ junta->nextLoc ].to_dQuaternion(inidQ);
            //q1t
              //iniQt[0] = junta->next->iniQ[0]; iniQt[1] = -junta->next->iniQ[1]; iniQt[2] = -junta->next->iniQ[2]; iniQt[3] = -junta->next->iniQ[3];
              iniQt[0] = inidQ[0]; iniQt[1] = -inidQ[1]; iniQt[2] = -inidQ[2]; iniQt[3] = -inidQ[3];
            //q2
              q2[0] = qNext[0]; q2[1] = qNext[1]; q2[2] = qNext[2]; q2[3] = qNext[3];
            //q
              dQMultiply0 (qNext, q2, iniQt); //ou o contrario
      //q
        //eh assim ao contrario dos outros casos pq esta tratando 2 corpos diferentes
        //enquanto que os outros casos envolvem: ou o mesmo corpo, ou a mesma junta
        dQMultiply0 (q, qPrevt, qNext); //ou o contrario
        //dQMultiply0 (q, qNext, qPrevt); //ou o contrario

      return Quaternion( q[0], Vec4(q[1],q[2],q[3]) );
}

void Physics::initScene(Scene *scene){
    dInitODE ();
    // create world
    scene->setWorld(dWorldCreate());

    dWorldSetGravity (scene->getWorld(),0,0,0);
    //dWorldSetGravity (scene->world,0,0,0);
    dWorldSetERP (scene->getWorld(),0.02);
    //dWorldSetCFM (scene->world,1e-3);
    dWorldSetCFM (scene->getWorld(),1e-009);
    scene->setSpace(dHashSpaceCreate(0));
    dCreatePlane (scene->getSpace(),0,1,0,0); //todo remove
    scene->setJointGroup(dJointGroupCreate(0));

    //juntas ligadas - >ERP:+ligadas
    //dWorldSetCFM (world,1e-009); //soft hard (colisao)- >CFM:+soft
    dWorldSetAutoDisableFlag (scene->getWorld(),1);
    dWorldSetContactMaxCorrectingVel (scene->getWorld(),0.3);
    dWorldSetContactSurfaceLayer (scene->getWorld(),0.00001);
}
void Physics::setEnableObject(Object *obj){
    dBodyEnable(obj->getBody());
}

void Physics::setDisableObject(Object *obj){
    dBodyDisable(obj->getBody());
}
void Physics::setEnableJoint(Joint *joint){
    dJointEnable(joint->getJoint());
}
void Physics::setDisableJoint(Joint *joint){
    dJointDisable(joint->getJoint());
}

void Physics::createObject(Object *object, SpaceID space, float density, Vec4 position, Quaternion rotation){


    switch(object->getType()){
    case TYPE_SPHERE:{
        object->setBody(dBodyCreate (object->getScene()->getWorld()));
        object->setGeometry(dCreateSphere(space,object->getProperties().x()));
        dMassSetSphereTotal(object->getMass(),1,object->getProperties().x());
        break;
    }
    case TYPE_CUBE:{
        object->setBody(dBodyCreate (object->getScene()->getWorld()));
        object->setGeometry(dCreateBox (space,object->getProperties().x(),object->getProperties().y(),object->getProperties().z()));
        dMassSetBox ((object->getMass()),density,object->getProperties().x(),object->getProperties().y(),object->getProperties().z());
        dMassSetBoxTotal(object->getMass(),1,object->getProperties().x(),object->getProperties().y(),object->getProperties().z());
        break;
    }
//    case OBJ_CAPSULE:
//        object->body = dBodyCreate (object->scene->world);
//        object->geometry = dCreateCapsule (space,object->properties[0],object->properties[1]);
//        dMassSetCapsule (&object->mass,density,1,object->properties[0],object->properties[1]);
//        break;
    case TYPE_CYLINDER:{
        object->setBody(dBodyCreate (object->getScene()->getWorld()));
        object->setGeometry(dCreateCylinder (space,object->getProperties().x(),object->getProperties().y()));
        dMassSetCylinder(object->getMass(),1,1,object->getProperties().x(),object->getProperties().y());
        break;
    }
    default:{
        return;
        break;
    }
    }

    dGeomSetData(object->getGeometry(), (void*)(object));
    dBodySetMass (object->getBody(),object->getMass());
    dGeomSetBody (object->getGeometry(),object->getBody());
    dBodySetPosition (object->getBody(),position.x(),position.y(),position.z());

    dReal rot[] = {rotation.getScalar(),rotation.getPosX(),rotation.getPosY(),rotation.getPosZ()};
    dBodySetQuaternion (object->getBody(),rot);
}

Vec4 Physics::getPositionBody(dGeomID geom){
    const dReal *pos;
    pos = dGeomGetPosition(geom);
    return Vec4(pos[0],pos[1],pos[2]);
}

Quaternion Physics::getRotationBody(dGeomID geom){
    dQuaternion quat;
    dGeomGetQuaternion(geom,quat);
    return Quaternion(quat[0],quat[1],quat[2],quat[3]);
}


void Physics::closeScene(Scene *scene){
    // clean up
    dJointGroupDestroy (scene->getJointGroup());
    dSpaceDestroy (scene->getSpace());
    dWorldDestroy (scene->getWorld());

    //!Same with close
    //dCloseODE();
}

void Physics::getGeomTransform(GeomID geom, Matrix4x4 *transform){

    const dReal *pos;
    const dReal *rot;
    pos = dGeomGetPosition (geom);
    rot = dGeomGetRotation(geom);
    /****** debug matriz de transformação  ************/
    //    transform->showMatrix4x4();
    //    printf("\nposition: %.3f %.3f %.3f\n",pos[0],pos[1],pos[2]);
    //    for(unsigned int i=0;i<12;i++){
    //        printf("%.3f  ",rot[i]);
    //        if(i%3==0) printf("\n");
    //    }
    //printf("\n");
    transform->set( 0, rot[0] );
    transform->set( 1, rot[4] );
    transform->set( 2, rot[8] );
    transform->set( 3, 0 );    
    transform->set( 4, rot[1] );
    transform->set( 5, rot[5] );
    transform->set( 6, rot[9] );
    transform->set( 7, 0 );    
    transform->set( 8, rot[2] );
    transform->set( 9, rot[6] );
    transform->set( 10, rot[10]);
    transform->set( 11, 0 );
    transform->set( 12, pos[0] );
    transform->set( 13, pos[1] );
    transform->set( 14, pos[2] );
    transform->set( 15, 1.0 );

    //transform->translate(pos[0],pos[1],pos[2]);
}

void Physics::initJointBall(Joint* joint, Vec4 anchor)
{
    joint->setJoint(dJointCreateBall(joint->getCharacter()->getScene()->getWorld(), joint->getCharacter()->getJointGroup()));
    dJointAttach(joint->getJoint(), joint->getParent()->getBody(), joint->getChild()->getBody());
    dJointSetBallAnchor(joint->getJoint(), anchor.x(), anchor.y(), anchor.z());
}

void Physics::initJointFixed(Joint *joint){
    joint->setJoint(dJointCreateFixed(joint->getCharacter()->getScene()->getWorld(),0));
    dJointAttach(joint->getJoint(),joint->getParent()->getBody(),0);
    dJointSetFixed(joint->getJoint());
}

Vec4 Physics::getJointBallAnchor( Joint* joint )
{
    dVector3 res;
    dJointGetBallAnchor( joint->joint, res );
    return Vec4((float)res[0],(float)res[1],(float)res[2]);
}

void Physics::closeJoint(Joint* joint){
    dJointDestroy(joint->getJoint());
}

void Physics::bodyAddTorque(dBodyID body, float x, float y, float z){
    dBodyAddTorque(body,x,y,z);
}

void Physics::bodySetTorque(dBodyID body, float x, float y, float z){
    dBodySetTorque(body,x,y,z);
}

void Physics::bodyAddForce(dBodyID body, float x, float y, float z){
    dBodyAddForce(body,x,y,z);

}

void Physics::bodySetForce(dBodyID body, float x, float y, float z){
    dBodySetForce(body,x,y,z);

}

Vec4 Physics::getAngularVelBody(Object *obj)
{
    const dReal *vel = dBodyGetAngularVel(obj->getBody());
    return Vec4(vel[0],vel[1],vel[2]);
}


//#define EPSILON 0.01
//void Physics::ControlPDBallDanilo(dJointID joint,dQuaternion tarQ,double ks,double kd)
//{

//    dBodyID parent = dJointGetBody(joint,0);
//    dBodyID child = dJointGetBody(joint,1);

//    const dReal* childQuaternion = dBodyGetQuaternion(child);
//    const dReal* parentQuaternion = dBodyGetQuaternion(parent);
//    const dReal* childAngularVelocity = dBodyGetAngularVel(child);
//    const dReal* parentAngularVelocity = dBodyGetAngularVel(parent);


//    dQuaternion tmp;  // Intermediate variable
//    dQuaternion difQ; // Change to be applied to body 2 in global coords
//    dQuaternion difGlobalQ;
//    dVector3 relativeAngularVelocity;    // Relative angular velocity
//    dVector3 distA;
//    dVector3 torque = {0};

//    dQuaternion ident = {1,0,0,0};

//    dQMultiply1(tmp,childQuaternion,parentQuaternion);
//    //    dQMultiply1(tmp,parentQuaternion,childQuaternion);

//    float prodEsc = ident[0]*tmp[0] + ident[1]*tmp[1] + ident[2]*tmp[2] + ident[3]*tmp[3];
//    if(prodEsc < 0){
//        tmp[0]*=-1;
//        tmp[1]*=-1;
//        tmp[2]*=-1;
//        tmp[3]*=-1;
//    }

//    dQMultiply0(difGlobalQ,tarQ,tmp);

//    //dQMultiply0(difGlobalQ, parentQuaternion, difQ);

//    dReal theta = 2*acos(difGlobalQ[0]);
//    dReal s = 1-difGlobalQ[0]*difGlobalQ[0];
//    s = 1/sqrt(s);

//    // Ensure we're not going the long way around
//    //if (theta>0) theta-=2*M_PI;
//    distA[0] = difGlobalQ[1]*s;
//    distA[1] = difGlobalQ[2]*s;
//    distA[2] = difGlobalQ[3]*s;

//    // Find the proportional component
//    if (fabs(theta)>EPSILON) {
//        dNormalize3(distA);

//        dOPC(torque,*,distA,ks);
//    }

//    dVector3 vel;

//    vel[0] = parentAngularVelocity[0] - childAngularVelocity[0];
//    vel[1] = parentAngularVelocity[1] - childAngularVelocity[1];
//    vel[2] = parentAngularVelocity[2] - childAngularVelocity[2];
//    // Achando a velocidade entre as juntas
//    dOP(relativeAngularVelocity,-,childAngularVelocity,parentAngularVelocity);
//    // Aplicando o fator da constante de velocidade
//    dOPEC(relativeAngularVelocity,*=,kd);
//    // Adicoionando ao torque
//    dOPE(torque,-=,relativeAngularVelocity);


//    dBodyAddTorque(child,torque[0],torque[1],torque[2]);
//    dBodyAddTorque(parent,-torque[0],-torque[1],-torque[2]);
//}


//void Physics::ControlPDBallRubens(Joint *joint,dQuaternion qTarget,double ks,double kd){
//    dQuaternion qIdent = {1,0,0,0};

//    dQuaternion qAux;
//    const dReal *readQ;

//    //prev
//    readQ = dBodyGetQuaternion(joint->parent->body);
//    dQuaternion qPrev = {readQ[0],readQ[1],readQ[2],readQ[3]};

//    Quaternion4f *parentIniQ = joint->parent->initialRotation;
//    dQuaternion prevIniQ = {parentIniQ->w, -parentIniQ->x, -parentIniQ->y, -parentIniQ->z};
//    dQMultiply0 (qAux, qPrev, prevIniQ);

//    qPrev[0] = qAux[0];
//    qPrev[1] = -qAux[1];
//    qPrev[2] = -qAux[2];
//    qPrev[3] = -qAux[3];

//    //next
//    readQ = dBodyGetQuaternion(joint->child->body);
//    dQuaternion qNext = {readQ[0],readQ[1],readQ[2],readQ[3]};

//    Quaternion4f *childIniQ = joint->child->initialRotation;
//    dQuaternion nextIniQ = {childIniQ->w, -childIniQ->x, -childIniQ->y, -childIniQ->z};
//    dQMultiply0 (qAux, qNext, nextIniQ);

//    qNext[0] = qAux[0];
//    qNext[1] = qAux[1];
//    qNext[2] = qAux[2];
//    qNext[3] = qAux[3];

//    //current quaternion
//    dQuaternion qCurrent;
//    dQMultiply0 (qCurrent, qPrev, qNext);

//    dReal dotProdQ;

//    //minor arc
//    dotProdQ = ( qCurrent[0]*qIdent[0] + qCurrent[1]*qIdent[1] + qCurrent[2]*qIdent[2] + qCurrent[3]*qIdent[3] );
//    if( dotProdQ < -dotProdQ ){
//        qCurrent[0]*=-1;
//        qCurrent[1]*=-1;
//        qCurrent[2]*=-1;
//        qCurrent[3]*=-1;
//    }

//    //dQuaternion qTarget = {1,0,0,0};

//    dQuaternion qDelta;
//    dQuaternion qMinusCurrent = {qCurrent[0],-qCurrent[1],-qCurrent[2],-qCurrent[3]};

//    //delta quat
//    dQMultiply0(qDelta, qTarget, qMinusCurrent);
//    //minor arc
//    dotProdQ = ( qDelta[0]*qIdent[0] + qDelta[1]*qIdent[1] + qDelta[2]*qIdent[2] + qDelta[3]*qIdent[3] );
//    if( dotProdQ < -dotProdQ ){
//        qDelta[0]*=-1;
//        qDelta[1]*=-1;
//        qDelta[2]*=-1;
//        qDelta[3]*=-1;
//    }

//    //to axis angle
//    dVector3 axis;
//    dReal angle;

//    angle = 2*acos(qDelta[0]);
//    dReal s = qDelta[0]*qDelta[0];
//    if(s > 0.0){
//        s = dRecipSqrt(s);     //s = 1/sqrt(s)
//        axis[0] = qDelta[1]*s; //x = qx*s
//        axis[1] = qDelta[2]*s; //y = qy*s
//        axis[2] = qDelta[3]*s; //z = qz*s
//    }else{
//        //arbitrary axis
//        s = 0;
//        axis[0] = 1;
//        axis[1] = 1;
//        axis[2] = 1;
//    }

//    //limited rad angle
//    dReal limitedRadAngle = angle;
//    dReal limT = 0.1*10;
//    if(limitedRadAngle < -limT){
//        limitedRadAngle = -limT;
//    }else if(limitedRadAngle > limT){
//        limitedRadAngle = limT;
//    }

//    //rad to deg
//    angle = (angle*180)/M_PI;

//    //truncate 10 decimal digits
//    angle = trunc( angle * pow(10,10) ) / pow(10,10) ;
//    axis[0] = trunc( axis[0] * pow(10,10) ) / pow(10,10) ;
//    axis[1] = trunc( axis[1] * pow(10,10) ) / pow(10,10) ;
//    axis[2] = trunc( axis[2] * pow(10,10) ) / pow(10,10) ;

//    dVector3 localDelta = {axis[0]*limitedRadAngle, axis[1]*limitedRadAngle, axis[2]*limitedRadAngle, 1.0};

//    const dReal *rotationPrevBody = dBodyGetRotation( dJointGetBody (joint->joint, 0) );

//    dMatrix3 iniRot;
//    dQuaternion iniQuat;

//    //"set iniQuat";

//    dRfromQ(iniRot, iniQuat);

//    dMatrix3 iniRotPrevBody;
//    dMultiply2_333(iniRotPrevBody,rotationPrevBody,iniRot);

//    dVector3 globalDelta;
//    dMultiply0_331(globalDelta,iniRotPrevBody,localDelta);

//    const dReal *prevBodyVel = dBodyGetAngularVel( dJointGetBody (joint->joint, 0) );
//    const dReal *nextBodyVel = dBodyGetAngularVel( dJointGetBody (joint->joint, 1) );
//    dVector3 globalVel;
//    for(int i=0;i<3;i++){
//        globalVel[i] = nextBodyVel[i] - prevBodyVel[i];
//    }
//    dVector3 deltaGlobalVel;
//    limT = 0.1*1000;
//    for(int i=0;i<3;i++){
//        deltaGlobalVel[i] = -globalVel[i];

//        if(deltaGlobalVel[i] < -limT){
//            deltaGlobalVel[i] = -limT;
//        }else if(deltaGlobalVel[i] > limT){
//            deltaGlobalVel[i] = limT;
//        }
//    }

//    //ks
//    dMatrix3 localKS;
//    for(int i=0;i<12;i++){ localKS[i] = 0; }

//    dVector3 fixedKSI = {ks,ks,ks};// = getJointKsFixediSaved( i, state->compInertia, ksFixedi );

//    localKS[0]  = fixedKSI[0];
//    localKS[5]  = fixedKSI[1];
//    localKS[10] = fixedKSI[2];

//    dMatrix3 tempKS;
//    dMultiply2_333 (tempKS,localKS,iniRotPrevBody);
//    dMatrix3 globalKS;
//    dMultiply0_333 (globalKS,iniRotPrevBody,tempKS);

//    //kd

//    //dReal ratioKSKD = 0.2;

//    dMatrix3 localKD;
//    for (int i=0;i<12;i++) { localKD[i] = 0.0; }
//    localKD[0] = kd;
//    localKD[5] = kd;
//    localKD[10] = kd;
//    //localKD[0]  = localKS[0] * ratioKSKD;
//    //localKD[5]  = localKS[5] * ratioKSKD;
//    //localKD[10] = localKS[10] * ratioKSKD;


//    dMatrix3 tempKD;
//    dMultiply2_333 (tempKD,localKD,iniRotPrevBody);
//    dMatrix3 globalKD;
//    dMultiply0_333 (globalKD,iniRotPrevBody,tempKD);

//    dVector3 torque;
//    dVector3 torqueKS;
//    dMultiply0_331 (torqueKS,globalKS,globalDelta);
//    dVector3 torqueKD;
//    dMultiply0_331 (torqueKD,globalKD,deltaGlobalVel);

//    for(int i=0;i<3;i++){
//        torque[i] = torqueKS[i] + torqueKD[i];
//    }

//    dBodyAddTorque(joint->parent->body, -torque[0],-torque[1],-torque[2]);
//    dBodyAddTorque(joint->child->body,  torque[0], torque[1], torque[2]);
//}

