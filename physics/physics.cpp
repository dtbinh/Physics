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

        int numcont = 12;
        dContact contact[numcont];
        int i;
        if (int numc = dCollide (o1,o2,numcont,&contact[0].geom,sizeof(dContact))) {
            for(i=0; i<numcont; i++){

                contact[i].surface.mode = dContactBounce; // | dContactSoftCFM;
                // friction parameter
                if(scene->isGeometryFootSwing(o1)||scene->isGeometryFootSwing(o2))
                    contact[i].surface.mu = 0;
                else
                    contact[i].surface.mu = dInfinity;
                // bounce is the amount of "bouncyness".
                contact[i].surface.bounce = 0.1;
                // bounce_vel is the minimum incoming velocity to cause a bounce
                contact[i].surface.bounce_vel = 0.0;
                // constraint force mixing parameter
                //contact.surface.soft_cfm = 0.001;


            }
            dMatrix3 RI;
                dRSetIdentity (RI);
                //const dReal ss[3] = {0.02,0.02,0.02};
                for (i=0; i<numc; i++) {
                  dJointID c = dJointCreateContact (scene->getWorld(),scene->getJointGroup(),&contact[i]);
                    //if (show_contacts) {
                      //*
                      //FeedbackContact fbContact
                          dReal* contactPos; //dVector3
                          contactPos = contact[i].geom.pos;
                          dJointFeedback* jtFb = new dJointFeedback();
                          int noGroundGeom = 0;
                          if (dGeomGetClass(o1)==dPlaneClass) noGroundGeom = 2; //ou 0, mas nunca sera 0, pois would return antes
                          else if (dGeomGetClass(o2)==dPlaneClass) noGroundGeom = 1;
                          else noGroundGeom = 2;
                          GRF fbContact(Vec4( contactPos[0],contactPos[1],contactPos[2] ), jtFb, noGroundGeom);
                          fbContact.b1 = scene->getObject(b1);
                          fbContact.b2 = scene->getObject(b2);
                      //put fbContact in vector feedbackContacts
                        scene->addGroundForce(fbContact);
                      //set dJointFeedback
                        dJointSetFeedback(c,jtFb);
                      //*/
                    //}

                    //dJointAttach (c,b1,b2);
                  dJointAttach (c,b1,b2);
                  //if (show_contacts) dsDrawBoxD (contact[i].geom.pos,RI,ss);
                }
        }
    }
}


void Physics::worldStep(WorldID world, float stepSize){
    dWorldQuickStep(world, stepSize);
}

void Physics::simSingleStep (Scene *scene)
{
    scene->clearGroundForces();
    std::vector<Object*> objs = scene->objectsScene();
    for(unsigned int i=0;i<objs.size();i++)
        dBodyEnable(objs.at(i)->getBody());
    scn = scene;
    dSpaceCollide (scene->getSpace(),0,&nearCallback);
    // step the simulation
    dWorldStep (scene->getWorld(),0.0005);
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

Quaternion Physics::getRotationJointInit(Joint *joint)
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


void Physics::setGravity(Scene *scene,Vec4 g)
{
    dWorldSetGravity (scene->getWorld(),g.x(),g.y(),g.z());
}
void Physics::setSceneInUse(Scene *scene)
{
    scn = scene;
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
    scn = scene;
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

Vec4 Physics::getAnchorJoint(Joint *joint)
{
    dVector3 pos;
    dJointGetBallAnchor(joint->getJoint(),pos);
    return Vec4(pos[0],pos[1],pos[2]);
}
void Physics::createObject(Object *object, dSpaceID space, float mass, Vec4 position, Vec4 Velocity)
{
    switch(object->getType()){
    case TYPE_SPHERE:{
        object->setBody(dBodyCreate (object->getScene()->getWorld()));
        object->setGeometry(dCreateSphere(space,object->getProperties().x()));
        dMassSetSphereTotal(object->getMass(),mass,object->getProperties().x());
        break;
    }
    case TYPE_CUBE:{
        object->setBody(dBodyCreate(object->getScene()->getWorld()));
        object->setGeometry(dCreateBox(space,object->getProperties().x(),object->getProperties().y(),object->getProperties().z()));
        //dMassSetBox ((object->getMass()),density,object->getProperties().x(),object->getProperties().y(),object->getProperties().z());
        dMassSetBoxTotal(object->getMass(),mass,object->getProperties().x(),object->getProperties().y(),object->getProperties().z());
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
    dBodySetLinearVel(object->getBody(), Velocity.x(), Velocity.y(), Velocity.z());

    dReal rot[] = {1,0,0,0};
    dBodySetQuaternion (object->getBody(),rot);
}



void Physics::createObject(Object *object, SpaceID space, float mass, Vec4 position, Quaternion rotation){


    switch(object->getType()){
    case TYPE_SPHERE:{
        object->setBody(dBodyCreate (object->getScene()->getWorld()));
        object->setGeometry(dCreateSphere(space,object->getProperties().x()));
        dMassSetSphereTotal(object->getMass(),mass,object->getProperties().x());
        break;
    }
    case TYPE_CUBE:{
        object->setBody(dBodyCreate(object->getScene()->getWorld()));
        object->setGeometry(dCreateBox(space,object->getProperties().x(),object->getProperties().y(),object->getProperties().z()));
        //dMassSetBox ((object->getMass()),density,object->getProperties().x(),object->getProperties().y(),object->getProperties().z());
        dMassSetBoxTotal(object->getMass(),mass,object->getProperties().x(),object->getProperties().y(),object->getProperties().z());
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

Vec4 Physics::getPositionBody(GeomID g){
    const dReal *pos;
//    printf("\nIn");
    pos = dGeomGetPosition(g);
//    printf("\nOut");
    return Vec4(pos[0],pos[1],pos[2]);


//    Matrix4x4 *mat = new Matrix4x4();
//    getGeomTransform(geom,mat);
//    return Vec4(mat->get(12),mat->get(13),mat->get(14));
}

Quaternion Physics::getRotationBody(Object *obj){
    dQuaternion quat;
    dGeomGetQuaternion(obj->getGeometry(),quat);
    return Quaternion(quat[0],quat[1],quat[2],quat[3]);
}

void Physics::updateObject(Object *obj)
{
    dGeomSetBody (obj->getGeometry(),obj->getBody());
    obj->setGeometry(dCreateBox (obj->getScene()->getSpace(),obj->getProperties().x(),obj->getProperties().y(),obj->getProperties().z()));
    dBodySetPosition(obj->getBody(),obj->getPosition().x(),obj->getPosition().y(),obj->getPosition().z());

    dBodySetMass (obj->getBody(),obj->getMass());
    dReal rot[] = {obj->getRotation().getScalar(),obj->getRotation().getPosX(),obj->getRotation().getPosY(),obj->getRotation().getPosZ()};
    dBodySetQuaternion (obj->getBody(),rot);
}
Matrix Physics::getMatrixRotation(Object *obj){
    const dReal* Rbody; //dMatrix3
    Rbody = dBodyGetRotation(obj->getBody());

    //deve-se levar em consideracao se a geometria do corpo eh do tipo ccylinder
    //por hora não estamos trabalhando com cilindros
    /*--------- a fazer
    if (dGeomGetClass (b->geom) == dCCylinderClass) {
        dMatrix3 Rccyl;
        dRFromEulerAngles (Rccyl,grauToRad(90.0),grauToRad(0.0),grauToRad(0.0));
        dMatrix3 RccylBody;
        dMULTIPLY2_333(RccylBody,Rbody,Rccyl); //RccylBody = Rbody.(Rccyl)^-1
        R = conv_dMatrix3(RccylBody);
    } else {
        R = conv_dMatrix3(Rbody);
    }*/
    return getMatrix2dMatrix3(Rbody);
}

void Physics::closeScene(Scene *scene){
    // clean up
    dJointGroupDestroy (scene->getJointGroup());
    dSpaceDestroy (scene->getSpace());
    dWorldDestroy (scene->getWorld());

    //!Same with close
    dCloseODE();
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
    try{
        dBodyAddTorque(body,x,y,z);
    }catch(...){
        return;
    }


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

void Physics::setPositionBody(Object *body,Vec4 pos){
    dBodySetPosition(body->getBody(),pos.x(),pos.y(),pos.z());
}

void Physics::setRotationBody(Object *body,Quaternion quat){
    dQuaternion q;
    q[0] = quat.getScalar();
    q[1] = quat.getPosX();
    q[2] = quat.getPosY();
    q[3] = quat.getPosZ();
    dBodySetQuaternion(body->getBody(),q);
}


Vec4 Physics::getAngularVelBody(Object *obj)
{
    const dReal *vel = dBodyGetAngularVel(obj->getBody());
    return Vec4(vel[0],vel[1],vel[2]);
}

Vec4 Physics::getLinearVelBody(Object *obj)
{
    const dReal *vel = dBodyGetLinearVel(obj->getBody());
    return Vec4(vel[0],vel[1],vel[2]);
}

Vec4 Physics::getAngularMomentumBody(Object *obj){

    dVector3 angmom;
    const dReal *angvel;
    const dReal* R; //dMatrix3
    dMass massBody;
    dBodyGetMass(obj->getBody(), &massBody);
    R = dBodyGetRotation(obj->getBody());
    angvel = dBodyGetAngularVel(obj->getBody());
    //rotaciona o massBody->I
    dMassRotate( &massBody, R );
    //Iw
    dMULTIPLY0_331(angmom,massBody.I,angvel); //angmom=massBody.I.angvel
    return Vec4(angmom[0],angmom[1],angmom[2]);
}

Vec4 Physics::getAngularMomentumMoCap(Object* obj,Vec4 vel, Quaternion q){
    dVector3 angmom;
    const dReal *angvel;
    //const dReal * R; //dMatrix3
    dMatrix3 Rot;
    dMass massBody;
    dBodyGetMass(obj->getBody(), &massBody);
    dQuaternion quat;
    quat[0] = q.getScalar();
    quat[1] = q.getPosX();
    quat[2] = q.getPosY();
    quat[3] = q.getPosZ();
    dRfromQ(Rot,quat);
    angvel = getVec42dReal(vel);

    //rotaciona o massBody->I
    dMassRotate( &massBody, Rot );
    //Iw
    dMULTIPLY0_331(angmom,massBody.I,angvel); //angmom=massBody.I.angvel
    return Vec4(angmom[0],angmom[1],angmom[2]);
}

Vec4 Physics::getRelVelocityBody(Object *obj){
    dVector3 res;
    dMass mass;
    dBodyGetMass(obj->getBody(),&mass);
    dBodyGetRelPointVel(obj->getBody(),mass.c[0],mass.c[1],mass.c[2],res);
    return Vec4(res[0],res[1],res[2]);
}

Vec4 Physics::getRelPositionBody(Object *obj){
    dVector3 res;
    dMass mass;
    dBodyGetMass(obj->getBody(),&mass);
    dBodyGetRelPointPos(obj->getBody(),mass.c[0],mass.c[1],mass.c[2],res);
    return Vec4(res[0],res[1],res[2]);
}
