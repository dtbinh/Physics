#include "balance.h"
#include "scene/scene.h"
#include "scene/character.h"
#include "scene/joint.h"
#include "scene/object.h"
#include "control.h"
#include "graphics/draw.h"
#include "sensor.h"
#include "mocap/frame.h"
#include "mocap/framequat.h"
#include "physics/physics.h"
#include <omp.h>

#define LIMIT 20.
Balance::Balance(Character* chara)
{
    this->jump = false;
    this->chara = chara;
    ksTorque = Vec4(100,100,100);
    kdTorque = Vec4(10,10,10);
    ksForce = Vec4(200,200,200);
    kdForce = Vec4(20,20,20);
    kmomang = Vec4(5,5,5);
    kmomlin = Vec4(5,5,5);
    kVel = Vec4(3900.5,3900.5,3900.5);
    kDist = Vec4(3900.5,3900.5,3900.5);
    this->enable_force = true;
    this->enable_torque = true;
    this->enable_momentum = true;
    this->compensation = 1.0;
    this->bdesired = QuaternionQ();
    this->enable_balance = true;
    this->useHierarchy = FOOTS_AIR+3;
    this->m = 20.0;
    this->angle = 45;
    //valores extraidos do modelo padrão
    this->height = 0.03;
    this->radius = 0.07;
    this->limit = 0.0;
    sim_dist = 0;
    mocap_dist = 0;
    this->steps = 1;
    velAnt = Vec4();
    this->limitsteps = 20.;
    sensor_tolerance = 0.25;
    enable_gravitycomp = true;
    this->grav_comp = 1.0;
}

void Balance::contructRelationJointsBodies()
{
    relationJointsB = MatrixF(chara->getNumJoints(),chara->getNumBodies());
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
    relationJoints = MatrixF(chara->getNumJoints(),chara->getNumJoints());
}

MatrixF Balance::getJacobianSum(Object *obj)
{
    //omp_set_num_threads(4);
    MatrixF Ad_j2w = MatrixF(6,6*this->chara->getNumJoints()); // matriz adjunta da junta para o mundo do corpo obj
    int iBody = chara->getPositionBody(obj); //id do objeto
    if (iBody<0) return Ad_j2w;
    //para cada junta que influencia o corpo b (corpo raiz eh a stance paw)
    int size = this->chara->getNumJoints();
    for (int i=0;i<size;i++) {
        MatrixF Ad_j2w_i(6,6); //adjunta da junta para corpo obj
        if (chara->hierarchy[useHierarchy][i][iBody]) {
            Ad_j2w_i = chara->getJoint(i)->getAd();
        }
        Ad_j2w.setSubmatrix(0,6*i,Ad_j2w_i);
    }


    return Ad_j2w;
}

MatrixF Balance::getInertiaFactors(Joint *joint)
{

    bool** inertiaFactorMap_local = chara->hierarchy[10];
    dMatrix3 I;
      //zera I
    int jPos = chara->getIdJoint(joint);
      for (int k=0;k<12;k++) { I[k] = 0.0; }

      //calcula a posicao da junta em coordenadas globais
      dVector3 posJunta;
      dJointGetBallAnchor(joint->getJoint(),posJunta);

      //for (BodyGeom* i=junta->next;i!=NULL;i=i->next) {
      for (int i=0;i<chara->getNumBodies();i++) {
        if (inertiaFactorMap_local[jPos][i]) {
          //massBody em coordenadas locais
            dMass massBody;
            dBodyGetMass(chara->getBody(i)->getBody(), &massBody);
          //rotacao - global = R.corpo
            //calcula R
            //nao se deve tratar a 90 degrees X rotation (essa rotacao deve ser incluida em R, pq o metodo dBodyGetMass retorna a inercia sem essa rotacao)
            const dReal* R; //dMatrix3
            R = dBodyGetRotation(chara->getBody(i)->getBody());
            //rotaciona o massBody->I
            dMassRotate( &massBody, R );
          //translacao
            //calcula a posicao do corpo em coordenadas globais
            const dReal* posBody;
            posBody = dBodyGetPosition(chara->getBody(i)->getBody());
            //calcula o deslocamento
            dVector3 desloc;
            desloc[0] = posBody[0] - posJunta[0];
            desloc[1] = posBody[1] - posJunta[1];
            desloc[2] = posBody[2] - posJunta[2];
            //translada o massBody->I
            dMassTranslate( &massBody, desloc[0], desloc[1], desloc[2] );
            //dMassTranslate( &massBody, -desloc[0], -desloc[1], -desloc[2] );
          //adiciona massBody em coordenadas globais na matriz de inercia this->I
            for (int k=0;k<12;k++) { I[k] += massBody.I[k]; }
        }
      }
      MatrixF result(3,3);
      //float fat = 1;
      //if(useHierarchy==0) fat = 0.5;
      result(0,0) = I[0];
      result(1,1) = I[5];
      result(2,2) = I[10];
      printf("Use Hierarquia: %d\n",useHierarchy);
//      printf("(%.3f,%.3f,%.3f)\n",I[0],I[5],I[10]);
      return result;
}

Vec Balance::getTwistWrenchTotal(Vec twist, Vec4 com)
{


    omp_set_num_threads(4);
    //M_com_com
    MatrixF M_com2com(6,6);

    //Vec gcomTotal = Vec(6);

    //matriz jacobiana de 6 linhas e (6*num_juntas) colunas
    MatrixF J(6,6*this->chara->getNumJoints());

    MatrixF Js[omp_get_max_threads()];
    MatrixF M_com2coms[omp_get_max_threads()];

    for (int i=0;i<omp_get_max_threads();i++){
        Js[i] = MatrixF(6,6*this->chara->getNumJoints());
        M_com2coms[i] = MatrixF(6,6);
    }


    //if (useHierarchy==0) com = Vec4(com.x()*0.5,com.y()*0.5,com.z()*0.5);
    int size = this->chara->getNumBodies();
    #pragma omp parallel for schedule(dynamic)
    for (int b=0;b<size;b++) {
        //J = J_com_com = Sum_b( J_com_com_b )
        //J_com_com_b = Ad_w_com^t . M_w_b . Ad_w_j

        MatrixF M_b2com(6,6);
        MatrixF J_b2com(6,6*this->chara->getNumJoints());
        //Ad_w_com^t . M_w_b = Ad_b_com^t . M_b_b . Ad_b_w


        MatrixF Ad_w2b = this->chara->getBody(b)->getAd();
        MatrixF M_b2b = this->chara->getBody(b)->getIM();
        //matrix Ad_b_com_t = ModesGen::getAd(this->modelo->bodyGeoms[b],this->modelo).transpose();
        MatrixF Ad_com2b = this->chara->getBody(b)->getAd(com);


        MatrixF Ad_com2b_t = Ad_com2b.transpose();
        //Ad_com2b_t.print();
        //Ad_w_j
        MatrixF Ad_j2w = MatrixF(6,6*this->chara->getNumJoints());//getJacobianSum(this->chara->getBody(b));

        int size = this->chara->getNumJoints();
        for (int i=0;i<size;i++) {
            MatrixF Ad_j2w_i(6,6); //adjunta da junta para corpo obj
            if (chara->hierarchy[useHierarchy][i][b]) {
                Ad_j2w_i = chara->getJoint(i)->getAd();
            }
            Ad_j2w.setSubmatrix(0,6*i,Ad_j2w_i);
        }





        J_b2com = (Ad_com2b_t * M_b2b * Ad_w2b * Ad_j2w);
        //J_b2com.print();
        //gcomTotal = gcomTotal + Vec(Vec4(0,this->chara->getBody(b)->getFMass()*10,0),Vec4(0,0,0));
        Js[omp_get_thread_num()] = Js[omp_get_thread_num()] + (J_b2com);
        //J = J + J_b2com;
        //M_com_com = Sum_b( Ad_b_com^t . M_b_b . Ad_b_com )
        M_b2com = Ad_com2b_t * M_b2b * Ad_com2b;
        //M_com2com =  M_com2com + M_b2com;
        //printf("\nMatrix: (%d,%d)\n", M_b2com.getNumRows(),M_b2com.getNumColumns());
        //M_b2com.print();
        M_com2coms[omp_get_thread_num()] = M_com2coms[omp_get_thread_num()] + M_b2com;
    }

    //qDebug() << "pass!";
      for (int i=0;i<omp_get_max_threads();i++){
          J = J + Js[i];
          M_com2com = M_com2com + M_com2coms[i];
      }

      //M_com2com.print();


    //M_com_com^-1 * jacobian
    MatrixF M_com2com_inv = M_com2com.inverse();
    MatrixF M_invJ = M_com2com_inv * J;

    //M_invJ.print();


    //jacobian transposta
    MatrixF J_transp = M_invJ.transpose();

    //wrench_com_com
    Vec wrench_com2com = twist;

    //wrench_j_j
    Vec wrench_j2j = J_transp * wrench_com2com;

    return wrench_j2j;
}

Vec Balance::getJacobianLocomotion(std::vector<Joint*> joints, Object *effector,Vec twist)
{
//    Matrix M_com2com(6,6);
//    Matrix M_b2com(6,6);
//    //Vec gcomTotal = Vec(6);

//    //matriz jacobiana de 6 linhas e (6*num_juntas) colunas
//    Matrix J = Matrix(6,6*joints.size());
//    Matrix Ad_j = Matrix(6,6*joints.size());
//      for (unsigned int b=0;b<joints.size();b++) {
//        //J = J_com_com = Sum_b( J_com_com_b )
//          //J_com_com_b = Ad_w_com^t . M_w_b . Ad_w_j
//          Object* body = joints.at(b)->getChild();
//            Matrix J_b2com = Matrix(6,6*joints.size());
//            //Ad_w_com^t . M_w_b = Ad_b_com^t . M_b_b . Ad_b_w
//              Matrix Ad_w2b = body->getAd();
//              Matrix M_b2b = body->getIM();
//              //matrix Ad_b_com_t = ModesGen::getAd(this->modelo->bodyGeoms[b],this->modelo).transpose();
//                Matrix Ad_com2b = body->getAd();
//              Matrix Ad_com2b_t = Ad_com2b.transpose();
//            //Ad_w_j
//              Matrix Ad_j2w = joints.at(b)->getAd(effector->getPositionCurrent());
//            //J_b2com = Ad_j2w;//(Ad_com2b_t * M_b2b * Ad_w2b * Ad_j2w);
//          Ad_j.setSubmatrix(0,6*b,Ad_j2w);


//            //gcomTotal = gcomTotal + Vec(Vec4(0,this->chara->getBody(b)->getFMass()*10,0),Vec4(0,0,0));
//          //J = J + (J_b2com);
//        //M_com_com = Sum_b( Ad_b_com^t . M_b_b . Ad_b_com )
//            //M_b2com = Ad_com2b_t * M_b2b * Ad_com2b;
//          //M_com2com = M_com2com + M_b2com;
//      }

//    //M_com_com^-1 * jacobian
//    //Matrix M_com2com_inv = M_com2com.inverse();
//    //Matrix M_invJ = M_com2com_inv * J;

//    //jacobian transposta
//    //matrix J_transp = J.transpose();
//    Matrix J_transp = Ad_j.transpose();

//    //wrench_com_com
//    Vec wrench_com2com = twist;
//      //wrench_com_com.print();

//    //wrench_j_j
//    Vec wrench_j2j = J_transp * wrench_com2com;

//    return wrench_j2j;
    omp_set_num_threads(4);
    //M_com_com
    MatrixF M_com2com(6,6);

    //Vec gcomTotal = Vec(6);

    //matriz jacobiana de 6 linhas e (6*num_juntas) colunas
    MatrixF J(6,6*this->chara->getNumJoints());

    MatrixF Js[omp_get_max_threads()];
    MatrixF M_com2coms[omp_get_max_threads()];

    for (int i=0;i<omp_get_max_threads();i++){
        Js[i] = MatrixF(6,6*this->chara->getNumJoints());
        M_com2coms[i] = MatrixF(6,6);
    }

    int size = this->chara->getNumBodies();
    #pragma omp parallel for schedule(dynamic)
    for (int b=0;b<size;b++) {
        //J = J_com_com = Sum_b( J_com_com_b )
        //J_com_com_b = Ad_w_com^t . M_w_b . Ad_w_j
        MatrixF M_b2com(6,6);
        MatrixF J_b2com(6,6*this->chara->getNumJoints());
        //Ad_w_com^t . M_w_b = Ad_b_com^t . M_b_b . Ad_b_w
        MatrixF Ad_w2b = this->chara->getBody(b)->getAd();
        MatrixF M_b2b = this->chara->getBody(b)->getIM();
        //matrix Ad_b_com_t = ModesGen::getAd(this->modelo->bodyGeoms[b],this->modelo).transpose();
        MatrixF Ad_com2b = this->chara->getBody(b)->getAd(effector->getPositionCurrent());
        MatrixF Ad_com2b_t = Ad_com2b.transpose();
        //Ad_w_j
        MatrixF Ad_j2w =getJacobianSum(this->chara->getBody(b));

        J_b2com = (Ad_com2b_t * M_b2b * Ad_w2b * Ad_j2w);
        //gcomTotal = gcomTotal + Vec(Vec4(0,this->chara->getBody(b)->getFMass()*10,0),Vec4(0,0,0));
        Js[omp_get_thread_num()] = Js[omp_get_thread_num()] + (J_b2com);
        //M_com_com = Sum_b( Ad_b_com^t . M_b_b . Ad_b_com )
        M_b2com = Ad_com2b_t * M_b2b * Ad_com2b;
        M_com2coms[omp_get_thread_num()] = M_com2coms[omp_get_thread_num()] + M_b2com;
    }

      for (int i=0;i<omp_get_max_threads();i++){
          J = J + Js[i];
          M_com2com = M_com2com + M_com2coms[i];
      }

    //M_com_com^-1 * jacobian
    MatrixF M_com2com_inv = M_com2com.inverse();
    MatrixF M_invJ = M_com2com_inv * J;


    //jacobian transposta
    MatrixF J_transp = M_invJ.transpose();

    //wrench_com_com
    Vec wrench_com2com = twist;

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

void Balance::habiliteJump(bool b)
{
    this->jump = b;
    this->init_jump = this->chara->getPosCOM().y();
    this->max_jump = this->chara->getPosCOM().y()+0.2;
    this->min_jump = this->chara->getPosCOM().y()-0.2;
    steps = 1;
}

Vec4 Balance::getKVelocityLocomotion()
{
    return this->kVel;
}

void Balance::setKVelocityLocomotion(Vec4 k)
{
    this->kVel = k;
}

Vec4 Balance::getKDistanceLocomotion()
{
    return this->kDist;
}

void Balance::setKDistanceLocomotion(Vec4 k)
{
    this->kDist = k;
}

Vec4 Balance::getKMomentumLinear()
{
    return this->kmomlin;
}

Vec4 Balance::getKMomentumAngular()
{
    return this->kmomang;
}



QuaternionQ Balance::getDesiredQuaternion()
{
    return bdesired;
}

void Balance::setDeriredQuaternion(Vec4 euler)
{
    bdesired.fromEuler(euler);
}

void Balance::setDeriredQuaternion(QuaternionQ quat)
{
    this->bdesired = quat;
}

void Balance::setKMomentumLinear(Vec4 k)
{
    this->kmomlin = k;
}

void Balance::setKMomentumAngular(Vec4 k)
{
    this->kmomang = k;
}

float Balance::getCompensation()
{
    return this->compensation;
}

void Balance::setCompensation(float f)
{
    this->compensation = f;
}

void Balance::setStepsInterpolation(float limit)
{
    this->limitsteps = limit;
    steps = 0;
}

float Balance::getStepsInterpolation()
{
    return limitsteps;
}

void Balance::setCompensationGravity(double val)
{
    this->grav_comp = val;
}

float Balance::getCompensationGravity()
{
    return this->grav_comp;
}

void Balance::setLimitCone(float v)
{
    this->limit = v;
}

float Balance::getLimitCone()
{
    return this->limit;
}

float Balance::getMCone()
{
    return m;
}

void Balance::setMCone(float val)
{
    m = val;
}

float Balance::getRadiusCone()
{
    return radius;
}

void Balance::setRadiusCone(float val)
{
    this->radius = val;
}

float Balance::getHeightCone()
{
    return height;
}

void Balance::setHeightCone(float val)
{
    this->height = val;
}

float Balance::getAngleCone()
{
    return angle;
}

void Balance::setAngleCone(float val)
{
    this->angle = val;
}

void Balance::setLimitSteps(int value)
{
    limitsteps = value;
}

int Balance::getLimitSteps()
{
    return limitsteps;
}

void Balance::setSensorTolerance(float value)
{
    sensor_tolerance = value;
    Sensor::setTolerance(sensor_tolerance);
}

float Balance::getSensorTolerance()
{
    return sensor_tolerance;
}

float Balance::getTorqueMaxCompensable(Object *foot, Vec4 torque)
{
    float ang = this->angle*M_PI/180.;
    Vec4 Tmax = Vec4(m*cos(ang),m*sin(ang),0)^Vec4(-radius,height,0);
    //if(torque.projXZ().module()>Tmax.module()) return 0;
    //if(torque.projXZ().module()<0) return 1;
    float ratio = Tmax.module()/torque.projXZ().module();
    if (ratio>=1) ratio = 1.;
    foot->setCompensableFactor(ratio);
    if(limit/100.>ratio){
        enable_balance = false;
    }
    return ratio;
}

void Balance::setEnableTorque(bool b)
{
    this->enable_torque = b;
}

bool Balance::getEnableTorque()
{
    return enable_torque;
}

void Balance::setEnableForce(bool b)
{
    this->enable_force = b;
}

bool Balance::getEnableForce()
{
    return enable_force;
}

void Balance::setEnableGravityCompensation(bool b)
{
    this->enable_gravitycomp = b;
}

bool Balance::getEnableGravityCompensation()
{
    return this->enable_gravitycomp;
}

void Balance::setEnableMomentum(bool b)
{
    this->enable_momentum = b;
}

bool Balance::getEnableMomentum()
{
    return this->enable_momentum;
}

Vec4 Balance::limitingTorque(Vec4 lim_inf, Vec4 lim_sup, Vec4 torque)
{
    if(torque.x()>lim_sup.x() || torque.x()<lim_inf.x()){
        if (torque.x()>lim_sup.x()) torque.x1 = lim_sup.x();
        else torque.x1 = lim_inf.x();
    }
    else if (torque.y()>lim_sup.y()|| torque.y()<lim_inf.y()){
            if (torque.y()>lim_sup.y()) torque.x2 = lim_sup.y();
            else torque.x2 = lim_inf.y();
    }
    else
        if (torque.z()>lim_sup.z() || torque.z()<lim_inf.z()){
            if (torque.z()>lim_sup.z()) torque.x3 = lim_sup.z();
            else torque.x3 = lim_inf.z();
        }
    return torque;

}

Vec4 Balance::limitingTorque(float x, Vec4 torque)
{
    float mod = torque.module();
    if (mod>x) torque = torque*(x/mod);
    return torque;
}


void Balance::evaluate(Joint* jDes,float mass_total,int frame,QuaternionQ qdesired,Vec4 vel_ang_des, Vec4 velCOM_moCap,Vec4 mom_lin_des,Vec4 mom_ang_des)
{

    bool nomocap = false; //quando não existe movimento capturado
    QuaternionQ quat(Vec4(0,0,0));
    if(!(enable_balance)) return; //desabilita o controle de equilíbrio
    int h = useHierarchy;
    if((frame>=0)&&(chara->getMoCap()->status)){ // se existe uma captura de movimento
        useHierarchy = Sensor::getHierarchy2UseMocap(chara);
        if (useHierarchy!=h) steps = 1;
    }
    else{ // quando não existe uma captura de movimento
        useHierarchy = Sensor::getHierarchy2Use(chara);
        nomocap = true;
        if (useHierarchy!=h) steps = 1; // reinicializa os passos para suavização da posição desejada para o COM limite 2000
                                        // quando há a troca de contato (dois pés para um e vice-versa)
    }

    if(useHierarchy==FOOTS_AIR+3 || useHierarchy==FOOTS_AIR_INV+3){ //caso os pés do modelo estejam fora do contado com o solo
        jump = false;
        return;
    }


    //definição de SISTEMA LOCAL do personagem
    bool capture = false;
    if(frame>0 && chara->getMoCap()->status) capture = true;
    std::vector<Object*> foots;

    for (int i=0;i<chara->getNumBodies();i++)
        if (this->chara->getBody(i)->getFoot()) foots.push_back(chara->getBody(i));

    Vec4 Cfoot_;
    float media;
    int count = 0;

    if (useHierarchy == 0){
        media = (foots.at(0)->getPositionCurrent().projXZ()+foots.at(1)->getPositionCurrent().projXZ()).module()/2.0;
        Cfoot_ = foots.at(0)->getPositionCurrent()+foots.at(1)->getPositionCurrent();
        Cfoot_.x2 = 0;
        count = 2;
    }else{
        Cfoot_ = chara->getBody(useHierarchy-3)->getPositionCurrent();
        Cfoot_.x2 = 0;
        count = 1;
    }

    Cfoot_ /= count;

    Vec4 COM   = chara->getPosCOM();
    Vec4 COM_   = Vec4(COM.x(),0,COM.z()); // projeção do COM no plano xz

    Vec4 velCOM = chara->getVelCOM();
    Vec4 velCOM_ = chara->getVelCOM();
    velCOM_ = Vec4(velCOM_.x(),0,velCOM_.z()); // projeção da velocidade do COM no plano xz

    if((chara->getMoCap()->sizeFrames()>0)&&(chara->getMoCap()->status)){
            velCOM_moCap = quat.getVecRotation(bdesired,velCOM_moCap);
            velCOM_moCap.x2 = 0;

        }else{
        velCOM_moCap = Vec4();
    }
    Vec momentum;

    if(!nomocap){ // com mocap
        mom_ang_des = quat.getVecRotation(bdesired,mom_ang_des);
        mom_lin_des = quat.getVecRotation(bdesired,mom_lin_des);
    }else{ // sem mocap
        mom_ang_des = Vec4();
        mom_lin_des = Vec4();
    }
    if (enable_momentum)
        momentum = Vec((mom_ang_des-chara->getAngularMomentum()).mult(kmomang),(mom_lin_des-chara->getLinearMomentum()).mult(kmomlin));
    else
        momentum = Vec(Vec4(),Vec4());

    //computando a força de controle no centro de massa
    Vec4 Fcom;
    int val=-1; // se o personagem estiver com os dois pés no chão
    //if ((useHierarchy!=FOOTS_AIR+3 || useHierarchy!=FOOTS_AIR_INV+3))
    if((frame>=0)&&(chara->getMoCap()->status)){
        if (useHierarchy==0) val = -1;
        else val = useHierarchy-3; // guarda o status do único pé que esta como pé de apoio
        if (limitsteps==0)
            Fcom = ksForce.mult(chara->getMoCap()->positionRelativeCOM(frame,val) - (COM_ - Cfoot_)) + kdForce.mult((velCOM_moCap) - velCOM_);
        else
            Fcom = ksForce.mult(chara->getMoCap()->positionRelativeCOM(frame,val) - (COM_ - Cfoot_))*(1 - (1-steps/limitsteps)) + kdForce.mult((velCOM_moCap) - velCOM_)*(1 - (1-steps/limitsteps));
    }else{ //caso o personagem não siga uma captura de movimento
        if(limitsteps==0){
            Fcom = ksForce.mult(chara->getMoCap()->positionRelativeCOM(frame,val) - (COM_ - Cfoot_)) + kdForce.mult((velCOM_moCap) - velCOM_);
        }else{
            Fcom = ksForce.mult(Cfoot_ - COM_)*(1 - (1-steps/limitsteps)) - kdForce.mult(velCOM_)*(1 - (1-steps/limitsteps));
        }
        if (useHierarchy!=0 && (chara->hasEffectorEnabled())){ // se só houver um pé em contado com o solo
            Object* foot_ground = chara->getBody(useHierarchy-3); // pega a posição do pé que esta em contato com o solo
            Object* foot_air = chara->getBodiesFoot(foot_ground)[0]; // pega a posição do pé que esta no ar
            Vec4 vp = foot_air->getPositionCurrent() - foot_ground->getPositionCurrent();
            vp = vp.projXZ(); // pega a posição projetada de FootAir - FootGround
            if (vp*Fcom < 0){ //Fcom = Fc (texto do email)
                Fcom = Fcom - vp.unitary()*(Fcom*(vp.unitary())); //estratégia anterior verificar!!!
            }
        }
    }


    //computando o torque no centro de massa
    Vec4 Tcom = Vec4(0,0,0);
    if (jDes!=NULL)
        if((frame>0)&&(chara->getMoCap()->status)){
            QuaternionQ q = bdesired*qdesired;
            Tcom = ControlPD::getTorquePDCOM(jDes,ksTorque,kdTorque,q,jDes->getParent()->getRelVelAngular()-(quat.getVecRotation(bdesired,vel_ang_des)));
        }else{

        Tcom = ControlPD::getTorquePDCOM(jDes,ksTorque,kdTorque,bdesired,jDes->getParent()->getRelVelAngular());
        }
    else
        printf("\nSem junta desejada!");

    Vec4 gravity;

    if(enable_gravitycomp) gravity = chara->getScene()->getGravity()*mass_total*(grav_comp);//*0.8;

    Vec wrench;

    if (jump){
        //printf("\n Jump Steps: %d",steps);
        float descida = (2/3.)*limitsteps;
        float subida = limitsteps - descida;
        //printf("\n Descida: %d",descida);
        float y_current;
        if (steps<(int)descida){
            y_current = init_jump - (init_jump - min_jump)*(steps/(descida-1));
            Vec4 com = init_jump;
            com.x2 = y_current;
            Vec4 Fvert = ksForce.mult(com - COM) - kdForce.mult(velCOM);
            Fcom += Vec4(0,Fvert.y(),0) + (gravity);
            printf("\nDescida Steps: %d, y_des: %.3f",steps,y_current);
        }else{

            if (steps < (subida/2. + descida)){
                y_current = min_jump - (min_jump - init_jump)*(steps/(subida/2.+descida-1));
                //y_current  = init_jump - min_jump;// - (init_jump - min_jump)*((descida)-steps)/((subida/2 + descida));
            }else{
                y_current = max_jump - (max_jump - init_jump)*(steps/limitsteps);
                //y_current = min_jump - (min_jump - init_jump)*(steps/(subida/2+descida-1));
            }
            Vec4 com = init_jump;
            com.x2 = y_current;
            Vec4 Fvert = ksForce.mult(com - COM) - kdForce.mult(velCOM);
            Fcom += Vec4(0,Fvert.y(),0) + (gravity);
            printf("\nSubida Steps: %d, y_des: %.3f",steps,y_current);
        }

    }

    if (!(enable_force || enable_torque || enable_momentum)) return;
    if (enable_force && enable_torque && enable_momentum)
        wrench = Vec(Tcom,Fcom-(gravity)) + momentum;
    else if(enable_force && enable_torque)
        wrench = Vec(Tcom,Fcom-(gravity));
    else if (enable_force)
        wrench = Vec(Vec4(),Fcom-(gravity)) + momentum;
    else if (enable_torque)
        wrench = Vec(Tcom,(gravity)*(-1)) + momentum;
    else
        wrench = Vec(Vec4(),(gravity)*(-1))+ momentum;

    Vec wrenchTotal = getTwistWrenchTotal(wrench,COM);

    //qDebug() << "begin!";

    float factor = 1.0;



    for (int i=0;i<chara->getNumJoints();i++){

        Joint *joint = chara->getJoint(i);
        Vec4 torque(wrenchTotal[i+i*5],wrenchTotal[i+1+i*5],wrenchTotal[i+2+i*5]);
        int id_parent,id_child;
        id_parent = chara->getIdObject(joint->getParent());
        id_child = chara->getIdObject(joint->getChild());

        if(chara->hierarchy[useHierarchy][i][chara->getPositionBody(joint->getChild())]){

            if (joint->getChild()->getFoot()&&(useHierarchy==0 || useHierarchy==id_child+3|| useHierarchy==id_child+3)){

                joint->getChild()->addTorque((torque)*(1-getTorqueMaxCompensable(joint->getChild(),torque))*factor);
                torque = torque*(1-getTorqueMaxCompensable(joint->getChild(),torque));

            }
            else
                joint->getChild()->addTorque((torque)*factor);
            if (joint->getParent()->getFoot()&&(useHierarchy==0 || useHierarchy==id_child+3||useHierarchy==id_parent+3)){

                joint->getParent()->addTorque((torque)*(-1)*(1-getTorqueMaxCompensable(joint->getParent(),torque))*factor);
                torque = torque*(1-getTorqueMaxCompensable(joint->getChild(),torque));

            }
            else
                joint->getParent()->addTorque((torque)*(-1)*factor);
        }else{

            if (joint->getParent()->getFoot()&&(useHierarchy==0 ||useHierarchy==id_parent+3|| useHierarchy==id_parent+3)){

                joint->getParent()->addTorque((torque)*(1-getTorqueMaxCompensable(joint->getParent(),torque))*factor);
                torque = torque*(1-getTorqueMaxCompensable(joint->getChild(),torque));

            }
            else
                joint->getParent()->addTorque((torque)*factor);
            if (joint->getChild()->getFoot()&&(useHierarchy==0 || useHierarchy==id_parent+3 || useHierarchy==id_child+3))
            {
                joint->getChild()->addTorque((torque)*(-1)*(1-getTorqueMaxCompensable(joint->getChild(),torque))*factor);
                torque = torque*(1-getTorqueMaxCompensable(joint->getChild(),torque));

            }
            else
                joint->getChild()->addTorque((torque)*(-1)*factor);
        }
    }

    if(frame>0){
    mocap_dist = (chara->getMoCap()->positionRelativeCOM(frame,val)).module();
    Cfoot_ = Sensor::getSupportProjected(chara,capture);
    Cfoot_.x2 = 0;
    COM_   = chara->getPosCOM();
    COM_.x2 = 0;
    sim_dist = (COM_ - Cfoot_).module();
    }
    steps++;
    if (steps >= limitsteps){

        steps = limitsteps;
        jump = false;
    }

    //qDebug() << "end!";

    //evaluateSIMBICON();

}

void Balance::evaluateSIMBICON()
{
    //    /****************** Teste de SwingFoot *********************/

    Vec wrenchSwing;
    int foot_swing = Sensor::getSwingFoot(this->chara);
    std::vector<Joint*> hier;
    if (chara->getNumJoints()>0){
        for(unsigned int i=0; i<6;i++){
            hier.push_back(chara->getJoint(i));
        }
    }
    else{
        return;
    }
    //int aux;
    int sta = Sensor::getStanceFoot(this->chara);

    if (!(foot_swing<0) && sta>0){ //caso tenha um pé no ar
        //Vec4 acom_;
        //Vec4 bcom_;
//        if((chara->getMoCap()->sizeFrames()>0)||(chara->getMoCap()->status)){
//            acom_ = chara->getMoCap()->getPosCOM(frame);
//            bcom_ = chara->getMoCap()->getFrameMotion(frame)->getPosition(Sensor::getStanceFoot(this->chara));
//        }
        Vec4 a = chara->getPosCOM();
        a.x2 = 0;
        //Vec4 acom_ = chara->getMoCap()->getPosCOM(frame);
        //acom_.x2 = 0;
        //Vec4 bcom_ = chara->getMoCap()->getFrameMotion(frame)->getPosition(Sensor::getStanceFoot(this->chara));
        //bcom_.x2 = 0;
        //Vec4 distcom_ = (acom_-bcom_);
        //distcom_.x2 = 0;
        Vec4 b = chara->getBody(sta)->getPositionCurrent();
        b.x2 = 0;
        Vec4 dist_ = (a-b);
        dist_.x2 = 0;
        Vec4 vel_  = chara->getVelCOM();
        vel_.x2 = 0;

        //velCOM_moCap.x2 = 0;
        //Object* pelvis = chara->getBody(7);
        //Quaternion newq(Vec4(0,90,0));
        Vec4 Fswing = Vec4(20,20,20).mult(dist_) + Vec4(5,5,5).mult(vel_);
        printf("dist_ (%.3f,%.3f,%.3f)\n",dist_.x(),dist_.y(),dist_.z());
        printf("vel_ (%.3f,%.3f,%.3f)\n",vel_.x(),vel_.y(),vel_.z());
        printf("Fswing (%.3f,%.3f,%.3f)\n",Fswing.x(),Fswing.y(),Fswing.z());
        Vec wrench = Vec(Vec4(),Fswing);
        int l1,l2;
//        if(foot_swing==2){
//            l1 = 0;
//            l2 = 2;
//        }else{
//            l1 = 3;
//            l2 = 5;
//        }
        l1=0;l2=5;
        //chara->clearVectorsGlobais();
        for (unsigned int i=l1;i<l2+1;i++){
            hier.push_back(chara->getJoint(i));
        }
        //hier = chara->getHierarchyJoint(pelvis,chara->getBody(foot_swing));
        wrenchSwing = getJacobianLocomotion(hier,chara->getBody(foot_swing),wrench);
        //printf("Size vector: %d",wrenchSwing.size());


        float factor = 1.0;

        int j = 0;
        for (int i=l1;i<l2+1;i++){
            Joint *joint = chara->getJoint(i);
            Vec4 torque(wrenchSwing[j+j*5],wrenchSwing[j+1+j*5],wrenchSwing[j+2+j*5]);


            //printf("Torque Joint %d - (%.3f,%.3f,%.3f)\n",j,torque.x(),torque.y(),torque.z());
            //joint->getChild()->addTorque((torque));//*(1-getTorqueMaxCompensable(joint->getChild(),torque))*factor);
            //joint->getParent()->addTorque((torque)*(-1));//*factor);
            int id_parent = chara->getIdObject(joint->getParent());
            int id_child = chara->getIdObject(joint->getChild());
            //if (joint->getChild()->getFoot()&&(useHierarchy==0 || useHierarchy==id_child+3))
            //joint->getChild()->addTorque((torque));//*(1-getTorqueMaxCompensable(joint->getChild(),torque))*factor);
            //else
            //joint->getParent()->addTorque((torque)*(-1));//*factor);

            if(chara->hierarchy[useHierarchy][i][chara->getPositionBody(joint->getChild())]){
                if (joint->getChild()->getFoot()&&(useHierarchy==0 || useHierarchy==id_child+3))
                    joint->getChild()->addTorque((torque)*(1-getTorqueMaxCompensable(joint->getChild(),torque))*factor);
                else
                    joint->getChild()->addTorque((torque)*factor);
                if (joint->getParent()->getFoot()&&(useHierarchy==0 || useHierarchy==id_parent+3))
                    joint->getParent()->addTorque((torque)*(-1)*(1-getTorqueMaxCompensable(joint->getParent(),torque))*factor);
                else
                    joint->getParent()->addTorque((torque)*(-1)*factor);
            }else{
                if (joint->getParent()->getFoot()&&(useHierarchy==0 || useHierarchy==id_parent+3))
                    joint->getParent()->addTorque((torque)*(1-getTorqueMaxCompensable(joint->getParent(),torque))*factor);
                else
                    joint->getParent()->addTorque((torque)*factor);
                if (joint->getChild()->getFoot()&&(useHierarchy==0 || useHierarchy==id_child+3))
                    joint->getChild()->addTorque((torque)*(-1)*(1-getTorqueMaxCompensable(joint->getChild(),torque))*factor);
                else
                    joint->getChild()->addTorque((torque)*(-1)*factor);
            }
            j++;
        }
    }
    velAnt = chara->getVelCOM();
        /**************************************/

}

void Balance::setEnableBalance(bool b)
{
    this->enable_balance = b;
}

