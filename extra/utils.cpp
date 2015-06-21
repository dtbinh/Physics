#include "utils.h"
#include "scene/object.h"
#include "scene/scene.h"
#include "scene/character.h"
#include "mocap/mocap.h"
#include "mocap/frame.h"
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <fstream>
#include <iostream>
#include <sstream>
#include <QFileInfo>
#include <QDebug>

using namespace std;

#define DUMP_WORDS(ss, s, num_words) \
    for (int i = 0; i < num_words; ++i) {\
        ss >> s; \
    }

string read_string(std::istream& input)
{
    string str;
    string s;

    bool loop = true;
    do
    {
        input >> s;

        int begin = 0;
        int size = s.size();

        if (s[0] == '"')
        {
            begin = 1;
            size -= 1;
        }
        else
        {
            str += " ";
        }

        if (s[s.size() - 1] == '"')
        {
            loop = false;
            size -= 1;
        }

        str += s.substr(begin, size);
    }
    while (loop);

    return str;
}



Utils::Utils()
{
}

bool Utils::readModelRubens(Scene *scene, const std::string &fileName)
{
    scene->clear();
    scene->initPhysics();

    ifstream file(fileName.data(), ios::in);
    if(!file)
    {
        cerr << "File \"" << fileName << "\" not found." << endl;
        return false;
    }
    //variaveis utilizadas na leitura
    Vec4 pos = Vec4();
    Vec4 pos2 = Vec4();
    int upaxis = 1;
    float offset_x,offset_y,offset_z;
    Vec4 axis;
    Vec4 scale;
    float angle,mass;
    Vec4 anchor;
    QuaternionQ quat;
    Character *chara = new Character(scene);
    scene->addCharacter(chara);

    int release =  0;
    while (!file.eof())
      {
        string line;
        getline(file, line);

        stringstream line_ss(line);

        string s, dump, upaxis_str, body_name;
        line_ss >> s;

        //modelo da captura de movimento release = 1, caso contrário 0.

        if (s.empty())
            continue;
        else if (s[0] == '#')
            continue;
        else if (s == "parser:")
            continue;
        else if (s == "name:")
            continue;
        else if (s == "category:"){
            release = 1;
            continue;
        }
        else if (s == "upaxis:")
        {
          upaxis_str = read_string(line_ss);

          if (upaxis_str == "x-up") upaxis = 0;
          else if (upaxis_str == "y-up") upaxis = 1;
          else if (upaxis_str == "z-up") upaxis = 2;
        }
        else if (s == "offset:")
        {
            if(release){
                line_ss >> offset_z;
                line_ss >> offset_x;
                line_ss >> offset_y;
            }
            else{
                if (upaxis == 1)
                {
                    line_ss >> offset_x;
                    line_ss >> offset_y;
                    line_ss >> offset_z;
                }
                else if (upaxis == 2)
                {
                    line_ss >> offset_z;
                    line_ss >> offset_x;
                    line_ss >> offset_y;
                }

                pos.x1 += offset_x;
                pos.x2 += offset_y;
                pos.x3 += offset_z;
            }

        }

        else if (s == "box" || s == "sphere" || s =="ccylinder")
        {
          DUMP_WORDS(line_ss, dump, 1);
          printf("\nrelease: %d",release);
          body_name = read_string(line_ss);
          if (release==1){
              line_ss >> pos2.x3;
              line_ss >> pos2.x1;
              line_ss >> pos2.x2;
              line_ss >> mass;
          }else{
              if (upaxis == 1)
              {
                  line_ss >> pos2.x1;
                  line_ss >> pos2.x2;
                  line_ss >> pos2.x3;
              }
              else if (upaxis == 2)
              {
                  line_ss >> pos2.x3;
                  line_ss >> pos2.x1;
                  line_ss >> pos2.x2;
              }
              line_ss >> angle;
              if (upaxis == 1)
              {
                  line_ss >> axis.x1;
                  line_ss >> axis.x2;
                  line_ss >> axis.x3;
              }
              else if (upaxis == 2)
              {
                  line_ss >> axis.x3;
                  line_ss >> axis.x1;
                  line_ss >> axis.x2;
              }
              line_ss >> mass;
              quat.fromAxisAngle(axis,angle);
          }

          //quat.showQuaternion();

          DUMP_WORDS(line_ss, dump, 1);


          if (s == "box")
          {
              if(release){
                  line_ss >> scale.x3;
                  line_ss >> scale.x1;
                  line_ss >> scale.x2;
              }else{
                  if (upaxis == 1)
                  {
                      line_ss >> scale.x1;
                      line_ss >> scale.x2;
                      line_ss >> scale.x3;
                  }
                  else if (upaxis == 2)
                  {
                      line_ss >> scale.x3;
                      line_ss >> scale.x1;
                      line_ss >> scale.x2;
                  }
              }
            Object* obj = scene->addObject(scale,pos+pos2,quat,TYPE_CUBE,mass,chara);
            obj->setName(body_name.c_str());
          }
          else if (s == "sphere")
          {
            line_ss >> scale.x1;
            scale.x2 = scale.x1;
            scale.x3 = scale.x1;
            Object* obj = scene->addObject(scale,pos+pos2,quat,TYPE_SPHERE,mass,chara);
            obj->setName(body_name.c_str());

            //printf("%s",obj->getName().toLocal8Bit().constData());
            //return false;

          }
//          else if (s == "ccylinder")
//          {
//            line_ss >> sides[0];
//            line_ss >> sides[1];
//              sides[0] /= 2.0;
//              sides[1] /= 2.0;

//            //dBody
//            body = dBodyCreate(world);
//              dBodySetPosition(body, x+ pos[0], y+ pos[1], z+ pos[2]);
//              //iniR
//                //dRFromEulerAngles (R,grauToRad(90.0+0.0),grauToRad(0.0),grauToRad(0.0));
//                //dBodySetRotation (body,R);
//                //including the 90 degrees X rotation for ccylinders in dQuaternion q
//                  dQuaternion q1,q2;
//                  //q1
//                    dRFromEulerAngles (R,grauToRad(90.0+0.0),grauToRad(0.0),grauToRad(0.0));
//                    dRtoQ( R, q1 );
//                  //q2
//                    q2[0] = q[0]; q2[1] = q[1]; q2[2] = q[2]; q2[3] = q[3];
//                  //q
//                    dQMultiply0 (q, q2, q1); //ou o contrario
//                dBodySetQuaternion(body,q);
//            //dGeom
//            geom = dCreateCCylinder (0,sides[0],sides[1]);
//            //dMass
//            dMassSetCappedCylinderTotal (&mass,mass_value,3,sides[0],sides[1]);
//              dBodySetMass(body, &mass);
//          }

          DUMP_WORDS(line_ss, dump, 2);

          //inclui no modelo
          //dGeomSetBody (geom,body);
          //definindo a cor
          /*
          if ( body_name == "head" || body_name == "hand_right" || body_name == "hand_left" ) {
            bodyGeoms.push_back( new BodyGeom(body,geom,NULL,pele) );
          }
          else if ( body_name == "upper_arm_left" || body_name == "upper_arm_right" || body_name == "forearm_left" || body_name == "forearm_right" || body_name == "chest" || body_name == "stomach" ) {
            bodyGeoms.push_back( new BodyGeom(body,geom,NULL,camisa) );
          }
          else if ( body_name == "pelvis" || body_name == "thigh_left" || body_name == "thigh_right" || body_name == "shank_left" || body_name == "shank_right" ) {
            bodyGeoms.push_back( new BodyGeom(body,geom,NULL,calca) );
          }
          else if ( body_name == "foot_left" || body_name == "foot_right" ) {
            bodyGeoms.push_back( new BodyGeom(body,geom,NULL,sapatos) );
          }
          //*/
          //bodyGeoms.push_back( new BodyGeom(body,geom,pele,body_name) );
        }
        else if (s == "ball" || s == "fixed")
        {

          DUMP_WORDS(line_ss, dump, 1);

          QString joint_name  = QString::fromUtf8( read_string(line_ss).c_str());
          Object *father = chara->getObject(QString::fromUtf8( read_string(line_ss).c_str()));
          Object *son    = chara->getObject(QString::fromUtf8( read_string(line_ss).c_str()));


          if (s == "ball")
          {
              if(release){
                  line_ss >> anchor.x3;
                  line_ss >> anchor.x1;
                  line_ss >> anchor.x2;
              }else{
                  if (upaxis == 1)
                  {
                      line_ss >> anchor.x1;
                      line_ss >> anchor.x2;
                      line_ss >> anchor.x3;
                  }
                  else if (upaxis == 2)
                  {
                      line_ss >> anchor.x3;
                      line_ss >> anchor.x1;
                      line_ss >> anchor.x2;
                  }
              }
            anchor += pos;//Vec4(offset_x,offset_y,offset_z);
            float lim_sup_x,lim_sup_y,lim_sup_z,lim_inf_x,lim_inf_y,lim_inf_z;
            Joint* joint = scene->addJointBall(anchor,father,son,chara);
            Vec4 limTq;
            if ( !line_ss.eof() ) {
            line_ss >> lim_inf_z;
            line_ss >> lim_sup_z;
            line_ss >> lim_inf_x;
            line_ss >> lim_sup_x;
            line_ss >> lim_inf_y;
            line_ss >> lim_sup_y;
            line_ss >> limTq.x3;
            line_ss >> limTq.x1;
            line_ss >> limTq.x2;


            //printf("%s",son->getName().toUtf8().constData());

            //return false;

            joint->setLimitsAngularSuperior(Vec4(lim_sup_x,lim_sup_y,lim_sup_z));
            joint->setLimitsAngularInferior(Vec4(lim_inf_x,lim_inf_y,lim_inf_z));
            joint->setTorqueMax(limTq);
            }
            joint->setName(joint_name);
            //printf("(joint: %s, parent: %s , child: %s)\n",joint->getName().toLocal8Bit().constData(),father->getName().toLocal8Bit().constData(),son->getName().toLocal8Bit().constData());
            ControlPD *pd = new ControlPD(joint,Physics::getRotationJoint(joint),Vec4(5,5,5),Vec4(0.5,0.5,0.5));
            chara->controllers.push_back(pd);
          }
          else if (s == "fixed")
          {
              //scene->addJointFixed(father,chara);
          }
        }
        else if (s == "paw:")
        {
          DUMP_WORDS(line_ss, dump, 1);
          Object *paw = chara->getObject(QString::fromUtf8( read_string(line_ss).c_str()));
          if(paw!=NULL)
              paw->setFoot(true);

//          printf("\nIn!%s-----------------\n",paw->getName().toLocal8Bit().data());
//          paw_body = map_body_name2pos( read_string(line_ss) );

//          if (upaxis == 1)
//          {
//            line_ss >> pos_joint[0];
//            line_ss >> pos_joint[1];
//            line_ss >> pos_joint[2];
//          }
//          else if (upaxis == 2)
//          {
//            line_ss >> pos_joint[2];
//            line_ss >> pos_joint[0];
//            line_ss >> pos_joint[1];
//          }

//          line_ss >> sides[0];
//            sides[0] /= 2.0;

//          dGeomID contactGeom = dCreateGeomTransform (space);
//          dGeomTransformSetCleanup (contactGeom,1);
//          //dGeomTransformSetInfo (contactGeom,1);
//            dGeomID sphere_paw = dCreateSphere (0,sides[0]);
//            dGeomTransformSetGeom (contactGeom,sphere_paw);
//            if ( dGeomGetClass ( bodyGeoms[paw_body]->geom ) != dCCylinderClass )
//              dGeomSetPosition (sphere_paw, pos_joint[0], pos_joint[1], pos_joint[2]);
//            else
//              dGeomSetPosition (sphere_paw, pos_joint[0], -pos_joint[2], pos_joint[1]);
//          dGeomSetBody (contactGeom,bodyGeoms[paw_body]->body);

//          dGeomID contactGeom_heel = dCreateGeomTransform (space); //0); //space); //para heel colidir com o chao, atribuir space de novo
//          dGeomTransformSetCleanup (contactGeom_heel,1);
//          //dGeomTransformSetInfo (contactGeom_heel,1);
//            dGeomID sphere_paw_heel = dCreateSphere (0,sides[0]);
//            dGeomTransformSetGeom (contactGeom_heel,sphere_paw_heel);
//            if ( dGeomGetClass ( bodyGeoms[paw_body]->geom ) != dCCylinderClass )
//              dGeomSetPosition (sphere_paw_heel, pos_joint[0], -pos_joint[1], pos_joint[2]);
//            else
//              dGeomSetPosition (sphere_paw_heel, pos_joint[0], -pos_joint[2], -pos_joint[1]);
//          dGeomSetBody (contactGeom_heel,bodyGeoms[paw_body]->body);

//          paws.push_back( new Paw( contactGeom, contactGeom_heel, paw_body, bodyGeoms[paw_body] ) );
//            //*
//            if ( space != 0 ) {
//              //if ( dSpaceQuery( space, contactGeom ) )
//              //  dSpaceRemove( space, contactGeom );
//              if ( !dSpaceQuery( space, bodyGeoms[paw_body]->geom ) )
//                dSpaceAdd( space, bodyGeoms[paw_body]->geom );
//            }
            //*/

          /*
          //training wheels
          contactGeom = dCreateGeomTransform (space);
          dGeomTransformSetCleanup (contactGeom,1);
          //dGeomTransformSetInfo (contactGeom,1);
            dGeomID box_paw = dCreateBox (0,5,0.79,0.1);
            dGeomTransformSetGeom (contactGeom,box_paw);
              dGeomSetPosition (box_paw, 0,-0.3,0);
              //  dRFromEulerAngles (R,grauToRad(0.0),grauToRad(0.0),grauToRad(37.0));
              //dGeomSetRotation (box_paw,R);
          dGeomSetBody (contactGeom,bodyGeoms[0]->body);
          dGeomSetData(contactGeom,this);
          //*/
        }




      }
    //printf("\nSaiu!");
    //printf("\nQtde foot: %d",chara->getBodiesFoot().size());
    Joint* joint = chara->getParentMoreMass();
    joint->getParent()->setBodyBalance(true);
    Balance* balance = new Balance(chara);
    chara->setBalance(balance);
    //chara->updateKsKdControlPDCoros();
    return true;


}

bool Utils::saveModelRubens(Character *chara, const string &fileName)
{
    ofstream ff(fileName.data());
      ff << "parser: \"blender model exporter\"" << "\n";
      ff << "name: \"doesn't matter\"" << "\n";
      ff << "upaxis: \"y-up\"" << "\n";
      ff << "offset: 0 0 0" << "\n";
      ff << "\n";

      for (int i=0;i<chara->getNumBodies();i++) {
        //obtendo as caracteristicas da geometria//
          //BodyGeom* b;
//          dGeomID g;
//          const dReal* pos;
//          dQuaternion q;
          int type = chara->getBody(i)->getType();
            //b = bodyGeoms[i];
            //g = b->geom;
            Vec4 pos = chara->getBody(i)->getPositionCurrent();
            QuaternionQ q = chara->getBody(i)->getRotationCurrent();
            //dGeomGetQuaternion (g, q);
            //type = dGeomGetClass (g);

        //type
          if ( type == TYPE_CUBE ) ff << "box part: ";
          else if ( type == TYPE_SPHERE ) ff << "sphere part: ";
          else if ( type == TYPE_CYLINDER ) ff << "ccylinder part: ";

        //body_name
          ff << "\"" << chara->getBody(i)->getName().toStdString() << "\" ";

        //position
          ff << pos.x1 << " " << pos.x2 << " " << pos.x3 << " ";

        //orientation
          //deve-se levar em consideracao se a geometria do corpo eh do tipo ccylinder

//          if (type == dCCylinderClass) {
//            //excluding the 90 degrees X rotation for ccylinders in dQuaternion q
//              dQuaternion q1,q2;
//              //q1
//                dMatrix3 R;
//                dRFromEulerAngles (R,grauToRad(90.0+0.0),grauToRad(0.0),grauToRad(0.0));
//                dRtoQ( R, q1 );
//              //q2
//                q2[0] = q[0]; q2[1] = q[1]; q2[2] = q[2]; q2[3] = q[3];
//              //q
//                dQMultiply2 (q, q2, q1); //q = q2.(q1)^-1
//          }
          dReal angle;
          Vec4 axis;
            //Quaternion quat;
              //quat.w = q[0]; quat.xyz.x = q[1]; quat.xyz.y = q[2]; quat.xyz.z = q[3];
              q.toAxisAngle(&axis,&angle);
              ff << angle << " " << axis.x() << " " << axis.y() << " " << axis.z() << " ";

        //mass
          //dMass massBody;
          //dBodyGetMass(b->body, &massBody);
          ff << chara->getBody(i)->getMass()->mass << " ";

        //not used
          ff << "0 ";

        //geom's dimensions
          if ( type == TYPE_CUBE ) {
            Vec4 sides = chara->getBody(i)->getProperties();
            //dGeomBoxGetLengths (g,sides);
            ff << sides.x() << " " << sides.y() << " " << sides.z() << " ";
          } else if ( type == TYPE_SPHERE ) {
              Vec4 sides = chara->getBody(i)->getProperties();
            ff << 2.0*sides.x() << " ";
          } else if ( type == TYPE_CYLINDER ) {
//            dReal radius,length;
//            dGeomCCylinderGetParams (g,&radius,&length);
//            ff << 2.0*radius << " " << 2.0*length << " ";
          }

        //not used
          ff << "0 0\n";
      }

      ff << "\n";

      for (int i=0;i<chara->getNumJoints();i++) {
        //type
          ff << "ball joint: ";

        //joint_name
          ff << "\"joint_" << i << "\" ";

        //prev body
          ff << "\"" << chara->getJoint(i)->getParent()->getName().toStdString() << "\" ";

        //next body
          ff << "\"" << chara->getJoint(i)->getChild()->getName().toStdString() << "\" ";

        //position
          //calcula a posicao da junta em coordenadas globais
          Vec4 pos = chara->getJoint(i)->getPositionCurrent();
          ff << pos.x() << " " << pos.y() << " " << pos.z() << " ";

        //joint limits
          ff << chara->getJoint(i)->getLimitAngularMin().z() << " " << chara->getJoint(i)->getLimitAngularMax().z() << " " << chara->getJoint(i)->getLimitAngularMin().x() << " " << chara->getJoint(i)->getLimitAngularMax().x() << " " << chara->getJoint(i)->getLimitAngularMin().y() << " " << chara->getJoint(i)->getLimitAngularMax().y() << " " << chara->getJoint(i)->getTorqueMax().z() << " " << chara->getJoint(i)->getTorqueMax().x() << " " << chara->getJoint(i)->getTorqueMax().y() << "\n";
      }

      ff << "\n";

      for (unsigned int i=0;i<chara->getBodiesFoot().size();i++) {
        //paw
          ff << "paw: ";

        //paw_name
          ff << "\"paw_" << i << "\" ";

        //body
          ff << "\"" << chara->getBodiesFoot().at(i)->getName().toStdString() << "\" ";

          //        //position
          //          //calcula a posicao da geometria de contato em coordenadas locais, relativo ao frame do paws[i]->bodyGeom
          //          dGeomID sphere_paw = dGeomTransformGetGeom ( paws[i]->contactGeom );
          //          const dReal *local_sphere_paw_pos = dGeomGetPosition ( sphere_paw );
          //          if ( dGeomGetClass ( paws[i]->bodyGeom->geom ) != dCCylinderClass )
          //            ff << local_sphere_paw_pos[0] << " " << local_sphere_paw_pos[1] << " " << local_sphere_paw_pos[2] << " ";
          //          else
          //            ff << local_sphere_paw_pos[0] << " " << local_sphere_paw_pos[2] << " " << -local_sphere_paw_pos[1] << " ";

          //        //sphere_paw_radius
          //          ff << 2.0*dGeomSphereGetRadius( dGeomTransformGetGeom( paws[i]->contactGeom ) ) << "\n";
           ff << "\n";
      }

      ff << "\n";
      return true;

}

bool Utils::saveSimulationConfig(Scene *scene, const string &fileName)
{

    QDomDocument doc( "SimulationPhysics" );
    QDomElement root = doc.createElement( "SimulationScene" );
    doc.appendChild( root );
    for(int i=0;i<scene->getSizeCharacter();i++){
        Vec4 vec;
        QDomElement info = doc.createElement( "Character" ); //cabeçalho
        QDomElement sim = doc.createElement("Simulation");   //simulação
        sim.setAttribute("Steps",scene->getSimStep());
        sim.setAttribute("FrictionGround", scene->getFrictionGround());
        sim.setAttribute("FrictionFootAir", scene->getFrictionFootAir());


        QDomElement mocap = doc.createElement("MoCap");   //mocap
        mocap.setAttribute("FileMocap",scene->getCharacter(i)->getMoCap()->getAddressFile());
        mocap.setAttribute("FileMocapLoad",scene->getCharacter(i)->getMoCap()->getAddressFileLoad());
        mocap.setAttribute("BeginCycle",scene->getCharacter(i)->getMoCap()->getBeginClycle());
        mocap.setAttribute("EndCycle",scene->getCharacter(i)->getMoCap()->getEndClycle());


        QDomElement cpdprop = doc.createElement("ControlPDProportional");   //Controle PD Proporcional
        vec = scene->getProportionalKsPD();
        QDomElement ksprop = doc.createElement("ksPDProporcional");   //Controle PD Proporcional
        ksprop.setAttribute("x",vec.x());
        ksprop.setAttribute("y",vec.y());
        ksprop.setAttribute("z",vec.z());
        cpdprop.appendChild(ksprop);
        vec = scene->getProportionalKdPD();
        QDomElement kdprop = doc.createElement("kdPDProporcional");
        kdprop.setAttribute("x",vec.x());
        kdprop.setAttribute("y",vec.y());
        kdprop.setAttribute("z",vec.z());
        cpdprop.appendChild(kdprop);

        QDomElement gravity = doc.createElement( "Gravity" ); //gravidade
        gravity.setAttribute("x",scene->getGravity().x());
        gravity.setAttribute("y",scene->getGravity().y());
        gravity.setAttribute("z",scene->getGravity().z());
        gravity.setAttribute("Enable",(int)scene->hasGravity());
        QDomElement offset = doc.createElement( "Offset" ); //gravidade
        offset.setAttribute("x",scene->getCharacter(i)->getOffset().x());
        offset.setAttribute("y",scene->getCharacter(i)->getOffset().y());
        offset.setAttribute("z",scene->getCharacter(i)->getOffset().z());

        QDomElement eye = doc.createElement( "Cam_Eye" ); //camera eye
        eye.setAttribute("x",scene->getCharacter(i)->getScene()->getEye().x());
        eye.setAttribute("y",scene->getCharacter(i)->getScene()->getEye().y());
        eye.setAttribute("z",scene->getCharacter(i)->getScene()->getEye().z());


        QDomElement at = doc.createElement( "Cam_At" ); //camera at
        at.setAttribute("x",scene->getCharacter(i)->getScene()->getAt().x());
        at.setAttribute("y",scene->getCharacter(i)->getScene()->getAt().y());
        at.setAttribute("z",scene->getCharacter(i)->getScene()->getAt().z());

        QDomElement up = doc.createElement( "Cam_Up" ); //camera up
        up.setAttribute("x",scene->getCharacter(i)->getScene()->getUp().x());
        up.setAttribute("y",scene->getCharacter(i)->getScene()->getUp().y());
        up.setAttribute("z",scene->getCharacter(i)->getScene()->getUp().z());


        info.appendChild(sim);
        info.appendChild(gravity);
        info.appendChild(offset);
        info.appendChild(eye);
        info.appendChild(at);
        info.appendChild(up);
        info.appendChild(mocap);
        info.appendChild(cpdprop);

        QDomElement body = doc.createElement("Bodies");
        for(int j=0;j<scene->getCharacter(i)->getNumBodies();j++){
            QDomElement bodyProperties = doc.createElement("Properties");
            bodyProperties.setAttribute("Name",scene->getCharacter(i)->getBody(j)->getName());
            Vec4 vec;
            QuaternionQ q;
            vec = scene->getCharacter(i)->getBody(j)->getPosition();
            q = scene->getCharacter(i)->getBody(j)->getRotation();
            bodyProperties.setAttribute("Mass",scene->getCharacter(i)->getBody(j)->getFMass());
            bodyProperties.setAttribute("Geometry",scene->getCharacter(i)->getBody(j)->getType());
            bodyProperties.setAttribute("Material",scene->getCharacter(i)->getBody(j)->getIntMaterial());
            bodyProperties.setAttribute("Foot",(int)scene->getCharacter(i)->getBody(j)->getFoot());
            bodyProperties.setAttribute("BodyBalance",(int)scene->getCharacter(i)->getBody(j)->getBodyBalance());
            bodyProperties.setAttribute("ObjFile",scene->getCharacter(i)->getBody(j)->getObjFile());
            QDomElement posChara = doc.createElement("Position");
            posChara.setAttribute("x",vec.x());
            posChara.setAttribute("y",vec.y());
            posChara.setAttribute("z",vec.z());
            bodyProperties.appendChild(posChara);
            QDomElement rotChara = doc.createElement("Quaternion");
            rotChara.setAttribute("w",q.getScalar());
            rotChara.setAttribute("x",q.getPosX());
            rotChara.setAttribute("y",q.getPosY());
            rotChara.setAttribute("z",q.getPosZ());
            bodyProperties.appendChild(rotChara);
            vec = scene->getCharacter(i)->getBody(j)->getProperties();
            QDomElement dimChara = doc.createElement("Dimension");
            dimChara.setAttribute("x",vec.x());
            dimChara.setAttribute("y",vec.y());
            dimChara.setAttribute("z",vec.z());
            bodyProperties.appendChild(dimChara);
            QDomElement tagPosPD = doc.createElement("ControlPDPositional");
            tagPosPD.setAttribute("Enabled",(int)scene->getCharacter(i)->getBody(j)->isEnableCPDP());
            tagPosPD.setAttribute("Effector",(int)scene->getCharacter(i)->getBody(j)->isShowEffector());
            tagPosPD.setAttribute("Target",(int)scene->getCharacter(i)->getBody(j)->isShowTarget());
            QDomElement targetPos = doc.createElement("PositionTarget");
            vec = scene->getCharacter(i)->getBody(j)->getTarget();
            targetPos.setAttribute("x",vec.x());
            targetPos.setAttribute("y",vec.y());
            targetPos.setAttribute("z",vec.z());
            tagPosPD.appendChild(targetPos);
            QDomElement ksPD = doc.createElement("KsPositionalPD");
            vec = scene->getCharacter(i)->getBody(j)->getKs();
            ksPD.setAttribute("x",vec.x());
            ksPD.setAttribute("y",vec.y());
            ksPD.setAttribute("z",vec.z());
            tagPosPD.appendChild(ksPD);
            QDomElement kdPD = doc.createElement("KdPositionalPD");
            vec = scene->getCharacter(i)->getBody(j)->getKd();
            kdPD.setAttribute("x",vec.x());
            kdPD.setAttribute("y",vec.y());
            kdPD.setAttribute("z",vec.z());
            tagPosPD.appendChild(kdPD);
            bodyProperties.appendChild(tagPosPD);
            QDomElement tagTorPD = doc.createElement("ControlPDCup");
            QDomElement ksPDcup = doc.createElement("KsCupPD");
            tagTorPD.setAttribute("Enabled",(int)scene->getCharacter(i)->getBody(j)->hasCoffeeCup());
            vec = scene->getCharacter(i)->getBody(j)->getKsCup();
            ksPDcup.setAttribute("x",vec.x());
            ksPDcup.setAttribute("y",vec.y());
            ksPDcup.setAttribute("z",vec.z());
            tagTorPD.appendChild(ksPDcup);
            QDomElement kdPDcup = doc.createElement("KdCupPD");
            vec = scene->getCharacter(i)->getBody(j)->getKdCup();
            kdPDcup.setAttribute("x",vec.x());
            kdPDcup.setAttribute("y",vec.y());
            kdPDcup.setAttribute("z",vec.z());
            tagTorPD.appendChild(kdPDcup);
            bodyProperties.appendChild(tagTorPD);
            body.appendChild(bodyProperties);
        }
        info.appendChild(body);
        QDomElement joint = doc.createElement("Joints");
        for(int j=0;j<scene->getCharacter(i)->getNumJoints();j++){
            Vec4 vec;
            QuaternionQ q;
            Joint *jo = scene->getCharacter(i)->getJoint(j);
            ControlPD *cpd = scene->getCharacter(i)->getControllersPD().at(j);
            QDomElement jointProperties = doc.createElement("Joint");
            jointProperties.setAttribute("Name",jo->getName());
            jointProperties.setAttribute("Parent",jo->getParent()->getName());
            jointProperties.setAttribute("Child",jo->getChild()->getName());
            jointProperties.setAttribute("Type",jo->getType());
            jointProperties.setAttribute("Enable",(int)cpd->isEnabled());
            vec = jo->getPositionAnchorInit();
            QDomElement anchor = doc.createElement("Anchor");
            anchor.setAttribute("x",vec.x());
            anchor.setAttribute("y",vec.y());
            anchor.setAttribute("z",vec.z());
            jointProperties.appendChild(anchor);
            q = cpd->getQuaternionWanted();
            QDomElement qpd = doc.createElement("QuaternionPD");
            qpd.setAttribute("w",q.getScalar());
            qpd.setAttribute("x",q.getPosX());
            qpd.setAttribute("y",q.getPosY());
            qpd.setAttribute("z",q.getPosZ());
            jointProperties.appendChild(qpd);
            vec = cpd->getKs();
            QDomElement kspd = doc.createElement("KsPD");
            kspd.setAttribute("x",vec.x());
            kspd.setAttribute("y",vec.y());
            kspd.setAttribute("z",vec.z());
            jointProperties.appendChild(kspd);
            vec = cpd->getKd();
            QDomElement kdpd = doc.createElement("KdPD");
            kdpd.setAttribute("x",vec.x());
            kdpd.setAttribute("y",vec.y());
            kdpd.setAttribute("z",vec.z());
            jointProperties.appendChild(kdpd);
            joint.appendChild(jointProperties);
        }
        info.appendChild(joint);
        //parametros de equilíbrio

        QDomElement balance = doc.createElement("BalanceEstrategy");
        QDomElement balanceControl = doc.createElement("BalanceControl");
        balanceControl.setAttribute("EnableForce",(int)scene->getCharacter(i)->getBalance()->getEnableForce());
        balanceControl.setAttribute("EnableTorque",(int)scene->getCharacter(i)->getBalance()->getEnableTorque());
        balanceControl.setAttribute("EnableMomentum",(int)scene->getCharacter(i)->getBalance()->getEnableMomentum());
        balanceControl.setAttribute("GravityCompensation",scene->getCharacter(i)->getBalance()->getCompensationGravity());
        QuaternionQ q = scene->getCharacter(i)->getBalance()->getDesiredQuaternion();
        QDomElement quatTq = doc.createElement("QuaternionTorque");
        quatTq.setAttribute("w",q.getScalar());
        quatTq.setAttribute("x",q.getPosX());
        quatTq.setAttribute("y",q.getPosY());
        quatTq.setAttribute("z",q.getPosZ());
        balanceControl.appendChild(quatTq);
        vec = scene->getCharacter(i)->getBalance()->getKsTorque();
        QDomElement kstqBal = doc.createElement("BalanceksTorque");
        kstqBal.setAttribute("x",vec.x());
        kstqBal.setAttribute("y",vec.y());
        kstqBal.setAttribute("z",vec.z());
        balanceControl.appendChild(kstqBal);
        vec = scene->getCharacter(i)->getBalance()->getKdTorque();
        QDomElement kdtqBal = doc.createElement("BalancekdTorque");
        kdtqBal.setAttribute("x",vec.x());
        kdtqBal.setAttribute("y",vec.y());
        kdtqBal.setAttribute("z",vec.z());
        balanceControl.appendChild(kdtqBal);
        vec = scene->getCharacter(i)->getBalance()->getKsForce();
        QDomElement ksfcBal = doc.createElement("BalanceksForce");
        ksfcBal.setAttribute("x",vec.x());
        ksfcBal.setAttribute("y",vec.y());
        ksfcBal.setAttribute("z",vec.z());
        balanceControl.appendChild(ksfcBal);
        vec = scene->getCharacter(i)->getBalance()->getKdForce();
        QDomElement kdfcBal = doc.createElement("BalancekdForce");
        kdfcBal.setAttribute("x",vec.x());
        kdfcBal.setAttribute("y",vec.y());
        kdfcBal.setAttribute("z",vec.z());
        balanceControl.appendChild(kdfcBal);

        vec = scene->getCharacter(i)->getBalance()->getKMomentumLinear();
        QDomElement klinBal = doc.createElement("BalancekMomentumLinear");
        klinBal.setAttribute("x",vec.x());
        klinBal.setAttribute("y",vec.y());
        klinBal.setAttribute("z",vec.z());
        balanceControl.appendChild(klinBal);
        vec = scene->getCharacter(i)->getBalance()->getKMomentumAngular();
        QDomElement kangBal = doc.createElement("BalancekMomentumAngular");
        kangBal.setAttribute("x",vec.x());
        kangBal.setAttribute("y",vec.y());
        kangBal.setAttribute("z",vec.z());
        balanceControl.appendChild(kangBal);


        QDomElement cone = doc.createElement("FrictionCone");
        cone.setAttribute("Module",scene->getCharacter(i)->getBalance()->getMCone());
        cone.setAttribute("Height",scene->getCharacter(i)->getBalance()->getHeightCone());
        cone.setAttribute("Radius",scene->getCharacter(i)->getBalance()->getRadiusCone());
        cone.setAttribute("Angle",scene->getCharacter(i)->getBalance()->getAngleCone());
        cone.setAttribute("Limits",scene->getCharacter(i)->getBalance()->getLimitCone());
        //sensor tolerance
        cone.setAttribute("SensorTol",scene->getCharacter(i)->getBalance()->getSensorTolerance());
        //steps
        cone.setAttribute("StepsInterpolation",scene->getCharacter(i)->getBalance()->getStepsInterpolation());
        balanceControl.appendChild(cone);

        QDomElement locomotion = doc.createElement("LocomotionParameters");
        vec = scene->getCharacter(i)->getBalance()->getKVelocityLocomotion();
        QDomElement ksloc = doc.createElement("LocomotionVelocity");
        ksloc.setAttribute("x",vec.x());
        ksloc.setAttribute("y",vec.y());
        ksloc.setAttribute("z",vec.z());
        locomotion.appendChild(ksloc);
        vec = scene->getCharacter(i)->getBalance()->getKDistanceLocomotion();
        QDomElement kdloc = doc.createElement("LocomotionDistance");
        kdloc.setAttribute("x",vec.x());
        kdloc.setAttribute("y",vec.y());
        kdloc.setAttribute("z",vec.z());
        locomotion.appendChild(kdloc);
        balanceControl.appendChild(locomotion);
        balance.appendChild(balanceControl);
        info.appendChild(balance);
        root.appendChild(info);
    }




    QString f = fileName.data();
    ofstream ff((f+".xml").toLocal8Bit().data());
    ff << doc.toString().toLocal8Bit().data();
    return true;
}

bool Utils::loadSimulationConfig(Scene *scene, const string &fileName)
{

    QDomDocument doc( "SimulationPhysics" );
    QFile file( fileName.data() );
    if( !file.open(QIODevice::ReadOnly) )
        return false;
    if( !doc.setContent( &file ) )
    {
        file.close();
        return false;
    }
    file.close();
    QDomElement root = doc.documentElement();
    if( root.tagName() != "SimulationScene" )
        return false;
    scene->clear();

    scene->initPhysics();
    Vec4 kss,kdd;
    QDomNode n = root.firstChild();
    while( !n.isNull() )
    {
        QDomElement e = n.toElement();
        if( !e.isNull() ){
            Character *chara = new Character(scene);
            scene->addCharacter(chara);
            Vec4 offset;
            if(e.tagName()=="Character"){
                QDomNode sim = e.firstChildElement("Simulation");
                QDomElement sime = sim.toElement();
                int steps = sime.attribute("Steps","").toInt();
                scene->setSimStep(steps);

                float fric = sime.attribute("FrictionGround","-1").toFloat();
                if (fric>=0)
                    scene->setFrictionGround(fric);
                fric = sime.attribute("FrictionFootAir","-1").toFloat();
                if (fric>=0)
                    scene->setFrictionFootAir(fric);




                sim = e.firstChildElement("Gravity");
                sime = sim.toElement();
                int b = sime.attribute("Enable","").toInt();
                if(b){
                    scene->setGravity(true);
                }else{
                    scene->setGravity(false);
                }
                float w,x,y,z;
                x = sime.attribute("x","").toFloat();
                y = sime.attribute("y","").toFloat();
                z = sime.attribute("z","").toFloat();


                Vec4 vec(x,y,z);
                scene->setGravityParameters(vec);
                sim = e.firstChildElement("Offset");
                sime = sim.toElement();
                x = sime.attribute("x","").toFloat();
                y = sime.attribute("y","").toFloat();
                z = sime.attribute("z","").toFloat();
                offset.setVec4(x,y,z);
                chara->setOffset(offset);

                // configuração de câmera
                sim = e.firstChildElement("Cam_Eye");
                if (!sim.isNull()) {
                    Vec4 eye, at,up;
                    sime = sim.toElement();
                    x = sime.attribute("x","").toFloat();
                    y = sime.attribute("y","").toFloat();
                    z = sime.attribute("z","").toFloat();
                    eye.setVec4(x,y,z);

                    sim = e.firstChildElement("Cam_At");
                    sime = sim.toElement();
                    x = sime.attribute("x","").toFloat();
                    y = sime.attribute("y","").toFloat();
                    z = sime.attribute("z","").toFloat();
                    at.setVec4(x,y,z);

                    sim = e.firstChildElement("Cam_Up");
                    sime = sim.toElement();
                    x = sime.attribute("x","").toFloat();
                    y = sime.attribute("y","").toFloat();
                    z = sime.attribute("z","").toFloat();
                    up.setVec4(x,y,z);

                    chara->getScene()->setViewer(eye,at,up);


                }




                sim = e.firstChildElement("MoCap");
                sime = sim.toElement();
                QString file = sime.attribute("FileMocap","");
                if (!file.isEmpty()){
                    chara->getMoCap()->setAddressFile(file);
                    file = sime.attribute("FileMocapLoad","");
                    if(!file.isEmpty()){
                        chara->getMoCap()->setAddressFileLoad(file);
                        int total = chara->getMoCap()->getEndClycle();
                        //QString t_s = QString().sprintf("%d",total);
                        int begin = sime.attribute("BeginCycle","0").toInt();
                        int end = sime.attribute("EndCycle","0").toInt();
                        if(!end) end = total;
                        chara->getMoCap()->setBeginClycle(begin);
                        chara->getMoCap()->setEndClycle(end);
                        //qDebug() << chara->getMoCap()->getEndClycle();
                    }
                }
                sim = e.firstChildElement("ControlPDProportional");
                sime = sim.firstChildElement("ksPDProporcional").toElement();
                x = sime.attribute("x","").toFloat();
                y = sime.attribute("y","").toFloat();
                z = sime.attribute("z","").toFloat();
                vec.setVec4(x,y,z);
                scene->setProportionalKsPD(vec);
                kss = vec;
                sime = sim.firstChildElement("kdPDProporcional").toElement();
                x = sime.attribute("x","").toFloat();
                y = sime.attribute("y","").toFloat();
                z = sime.attribute("z","").toFloat();
                vec.setVec4(x,y,z);
                scene->setProportionalKdPD(vec);
                kdd = vec;
                sim = e.firstChildElement("Bodies");
                sim = sim.firstChildElement("Properties");
                while(!sim.isNull()){ //leitura das propriedades do corpo
                    QDomNode body = sim;
                    sime = body.toElement();
                    int mat = sime.attribute("Material","").toInt();
                    QString name = sime.attribute("Name","");
                    float mass = sime.attribute("Mass","").toFloat();
//                    qDebug() << name;
//                    qDebug() << mass;
                    int type = sime.attribute("Geometry","").toInt();
                    bool foot = (bool)sime.attribute("Foot","").toInt();
                    bool bodybalance =  (bool)sime.attribute("BodyBalance","").toInt();
                    QString objfile = sime.attribute("ObjFile","");
                    Vec4 scale,pos;
                    QuaternionQ quat;
                    sime = body.firstChildElement("Position").toElement();
                    x = sime.attribute("x","").toFloat();
                    y = sime.attribute("y","").toFloat();
                    z = sime.attribute("z","").toFloat();
                    pos.setVec4(x,y,z);
                    pos = pos+offset;
                    sime = body.firstChildElement("Quaternion").toElement();
                    w = sime.attribute("w","").toFloat();
                    x = sime.attribute("x","").toFloat();
                    y = sime.attribute("y","").toFloat();
                    z = sime.attribute("z","").toFloat();
                    quat = QuaternionQ(w,x,y,z);
                    sime = body.firstChildElement("Dimension").toElement();
                    x = sime.attribute("x","").toFloat();
                    y = sime.attribute("y","").toFloat();
                    z = sime.attribute("z","").toFloat();
                    scale.setVec4(x,y,z);
                    Object* obj = scene->addObject(scale,pos,quat,type,mass,chara,mat);
                    obj->setObjFile(objfile);
                    obj->setName(name);
                    obj->setFoot(foot);
                    obj->setBodyBalance(bodybalance);
                    //propriedades de testes
                    sime = body.firstChildElement("ControlPDPositional").toElement();
                    bool target = (bool)sime.attribute("Target","").toInt();
                    bool enable = (bool)sime.attribute("Enabled","").toInt();
                    bool effector = (bool)sime.attribute("Effector","").toInt();
                    obj->setShowTarget(target);
                    obj->setEnableCPDP(enable);
                    obj->setShowEffector(effector);
                    QDomElement prop = sime.firstChildElement("PositionTarget").toElement();
                    x = prop.attribute("x","").toFloat();
                    y = prop.attribute("y","").toFloat();
                    z = prop.attribute("z","").toFloat();
                    vec.setVec4(x,y,z);
                    obj->setTarget(vec);
                    prop = sime.firstChildElement("KsPositionalPD").toElement();
                    x = prop.attribute("x","").toFloat();
                    y = prop.attribute("y","").toFloat();
                    z = prop.attribute("z","").toFloat();
                    vec.setVec4(x,y,z);
                    obj->setKs(vec);
                    prop = sime.firstChildElement("KdPositionalPD").toElement();
                    x = prop.attribute("x","").toFloat();
                    y = prop.attribute("y","").toFloat();
                    z = prop.attribute("z","").toFloat();
                    vec.setVec4(x,y,z);
                    obj->setKd(vec);
                    sime = body.firstChildElement("ControlPDCup").toElement();
                    enable = (bool)sime.attribute("Enabled","").toInt();
                    obj->setCoffeeCup(enable);
                    prop = sime.firstChildElement("KsCupPD").toElement();
                    x = prop.attribute("x","").toFloat();
                    y = prop.attribute("y","").toFloat();
                    z = prop.attribute("z","").toFloat();
                    vec.setVec4(x,y,z);
                    obj->setKsCup(vec);
                    prop = sime.firstChildElement("KdCupPD").toElement();
                    x = prop.attribute("x","").toFloat();
                    y = prop.attribute("y","").toFloat();
                    z = prop.attribute("z","").toFloat();
                    vec.setVec4(x,y,z);
                    obj->setKdCup(vec);
                    //qDebug() << obj->showInfo();

                    sim = sim.nextSibling();
                }
                sim = e.firstChildElement("Joints");
                sim = sim.firstChildElement("Joint");
                while(!sim.isNull()){
                    QDomNode joint = sim;
                    sime = joint.toElement();
                    QString name = sime.attribute("Name","");
                    QString parent = sime.attribute("Parent","");
                    QString child = sime.attribute("Child","");
                    bool enable = (bool)sime.attribute("Enable","").toInt();
                    int type = sime.attribute("Type","").toInt();
                    QDomElement prop = sime.firstChildElement("Anchor").toElement();
                    x = prop.attribute("x","").toFloat();
                    y = prop.attribute("y","").toFloat();
                    z = prop.attribute("z","").toFloat();
                    vec.setVec4(x,y,z);
                    vec+=offset;
                    if(type==JOINT_BALL){
                       Joint* joint = scene->addJointBall(vec,chara->getObject(parent),chara->getObject(child),chara);
                       joint->setName(name);
                       prop = sime.firstChildElement("QuaternionPD").toElement();
                       w = prop.attribute("w","").toFloat();
                       x = prop.attribute("x","").toFloat();
                       y = prop.attribute("y","").toFloat();
                       z = prop.attribute("z","").toFloat();
                       QuaternionQ quat(w,x,y,z);
                       prop = sime.firstChildElement("KsPD").toElement();
                       x = prop.attribute("x","").toFloat();
                       y = prop.attribute("y","").toFloat();
                       z = prop.attribute("z","").toFloat();
                       Vec4 ks(x,y,z);
                       prop = sime.firstChildElement("KdPD").toElement();
                       x = prop.attribute("x","").toFloat();
                       y = prop.attribute("y","").toFloat();
                       z = prop.attribute("z","").toFloat();
                       Vec4 kd(x,y,z);
                       ControlPD *pd = new ControlPD(joint,quat,ks,kd);
                       pd->setEnabled(enable);
                       chara->controllers.push_back(pd);
                       //qDebug() << joint->showInfo();
                    }

                    sim = sim.nextSibling();
                }


                sim = e.firstChildElement("BalanceEstrategy");
                sim = sim.firstChildElement("BalanceControl");
                sime = sim.toElement();
                Balance* balance = new Balance(chara);
                chara->setBalance(balance);
                bool enablet = (bool)sime.attribute("EnableTorque","").toInt();
                bool enablef = (bool)sime.attribute("EnableForce","").toInt();
                bool enablem = (bool)sime.attribute("EnableMomentum","").toInt();
                float grav = 1.0;

                grav = sime.attribute("GravityCompensation","1.0").toFloat();

                chara->getBalance()->setEnableForce(enablef);
                chara->getBalance()->setEnableTorque(enablet);
                chara->getBalance()->setEnableMomentum(enablem);
                chara->getBalance()->setCompensationGravity(grav);

                QDomElement prop = sime.firstChildElement("QuaternionTorque").toElement();
                w = prop.attribute("w","").toFloat();
                x = prop.attribute("x","").toFloat();
                y = prop.attribute("y","").toFloat();
                z = prop.attribute("z","").toFloat();
                QuaternionQ quat(w,x,y,z);
                chara->getBalance()->setDeriredQuaternion(quat);
                prop = sime.firstChildElement("BalanceksTorque").toElement();
                x = prop.attribute("x","").toFloat();
                y = prop.attribute("y","").toFloat();
                z = prop.attribute("z","").toFloat();
                vec.setVec4(x,y,z);
                chara->getBalance()->setKsTorque(vec);
                prop = sime.firstChildElement("BalancekdTorque").toElement();
                x = prop.attribute("x","").toFloat();
                y = prop.attribute("y","").toFloat();
                z = prop.attribute("z","").toFloat();
                vec.setVec4(x,y,z);
                chara->getBalance()->setKdTorque(vec);
                prop = sime.firstChildElement("BalanceksForce").toElement();
                x = prop.attribute("x","").toFloat();
                y = prop.attribute("y","").toFloat();
                z = prop.attribute("z","").toFloat();
                vec.setVec4(x,y,z);
                chara->getBalance()->setKsForce(vec);
                prop = sime.firstChildElement("BalancekdForce").toElement();
                x = prop.attribute("x","").toFloat();
                y = prop.attribute("y","").toFloat();
                z = prop.attribute("z","").toFloat();
                vec.setVec4(x,y,z);
                chara->getBalance()->setKdForce(vec);
                prop = sime.firstChildElement("BalancekMomentumLinear").toElement();
                x = prop.attribute("x","").toFloat();
                y = prop.attribute("y","").toFloat();
                z = prop.attribute("z","").toFloat();
                vec.setVec4(x,y,z);
                chara->getBalance()->setKMomentumLinear(vec);
                prop = sime.firstChildElement("BalancekMomentumAngular").toElement();
                x = prop.attribute("x","").toFloat();
                y = prop.attribute("y","").toFloat();
                z = prop.attribute("z","").toFloat();
                vec.setVec4(x,y,z);
                chara->getBalance()->setKMomentumAngular(vec);
                prop = sime.firstChildElement("FrictionCone").toElement();
                chara->getBalance()->setAngleCone(prop.attribute("Angle","").toFloat());
                chara->getBalance()->setHeightCone(prop.attribute("Height","").toFloat());
                chara->getBalance()->setRadiusCone(prop.attribute("Radius","").toFloat());
                chara->getBalance()->setMCone(prop.attribute("Module","").toFloat());
                chara->getBalance()->setLimitCone(prop.attribute("Limits","").toFloat());

                chara->getBalance()->setSensorTolerance(prop.attribute("SensorTol","0.3").toFloat());
                chara->getBalance()->setStepsInterpolation(prop.attribute("StepsInterpolation","100").toFloat());

                QDomElement propinfo = sime.firstChildElement("LocomotionParameters").toElement();
                prop = propinfo.firstChildElement("LocomotionVelocity").toElement();
                x = prop.attribute("x","").toFloat();
                y = prop.attribute("y","").toFloat();
                z = prop.attribute("z","").toFloat();
                vec.setVec4(x,y,z);
                chara->getBalance()->setKVelocityLocomotion(vec);
                prop = propinfo.firstChildElement("LocomotionDistance").toElement();
                x = prop.attribute("x","").toFloat();
                y = prop.attribute("y","").toFloat();
                z = prop.attribute("z","").toFloat();
                vec.setVec4(x,y,z);
                chara->getBalance()->setKDistanceLocomotion(vec);
            }
            QString file = chara->getMoCap()->getAddressFile();
            if (!file.isEmpty()){
                Utils::loadMotionCapture(chara->getMoCap(),chara,file.toStdString());
                chara->loadMotionFrames();
                if(chara->getMoCap()->endClycle<=0) chara->getMoCap()->endClycle = chara->getMoCap()->sizeFrames();
                chara->getMoCap()->copyFootsProperties();
                file = chara->getMoCap()->getAddressFileLoad();
                if(!file.isEmpty()){
                    readFramesConfig(chara,file.toStdString());
                }
            }
        }
        n = n.nextSibling();
    }
    scene->setProportionalKsPD(kss);
    scene->setProportionalKdPD(kdd);
    return true;
}

bool Utils::readFramesConfig(Character *chara, const string &fileName)
{
    ifstream file(fileName.data(), ios::in);
    if(!file)
    {
        cerr << "File \"" << fileName << "\" not found." << endl;
        return false;
    }
    string line;
    getline(file, line);
    stringstream line_ss(line);
    int size;
    line_ss >> size;
    if (chara->getMoCap()->sizeFrames()!=size) return false;

    int i = 0;
    while (!file.eof() && i<size)
      {
        string line;
        getline(file, line);
        stringstream line_ss(line);
        char c;
        line_ss >> c;
        if (c=='t')
            chara->getMoCap()->getFrameMotion(i)->setFootLeftGround(true);
        else
            chara->getMoCap()->getFrameMotion(i)->setFootLeftGround(false);
        line_ss >> c;
        if (c=='t')
            chara->getMoCap()->getFrameMotion(i)->setFootRightGround(true);
        else
            chara->getMoCap()->getFrameMotion(i)->setFootRightGround(false);
        i++;
    }
    return true;
}

bool Utils::saveFramesConfig(Character *chara, const string &fileName)
{
    ofstream ff(fileName.data());
    ff <<  chara->getMoCap()->sizeFrames();
    ff << "\n";
    for (int i=0;i<chara->getMoCap()->sizeFrames();i++) {
        if (chara->getMoCap()->getFrameMotion(i)->getFootLeftGround())
            ff << "t ";
        else
            ff << "f ";
        if (chara->getMoCap()->getFrameMotion(i)->getFootRightGround())
            ff << "t ";
        else
            ff << "f ";
        ff << "\n";
    }
    ff.close();
    return true;
}

bool Utils::loadMotionCapture(MoCap *moCap,Character *chara, const string &fileName)
{
        moCap->clear();
        ifstream file(fileName.data(), ios::in);
        stringstream ss;
        std::string stmp;
        //int itmp;
        int num_frames = 0;
        double dtmp = 0.0;
        std::istream& istr = * (istream*) &file;
        //ifstream istr(fileName.data(), ios::in);
        getline(istr, stmp);
        ss << stmp;
        ss >> stmp;
        ss >> num_frames;
        ss.clear();


        //inicializa moCapMot
        for (int i=0;i<num_frames;i++) {
            moCap->appendFrame(new Frame());
          //inicializa os vectors pos e quat
          for (int j=0;j<chara->getNumBodies();j++) {
            moCap->getFrameMotion(i)->appendPosition( Vec4(0.0,0.0,0.0) );
            moCap->getFrameMotion(i)->appendOrientation(QuaternionQ( 1.0,0.0,0.0,0.0));
          }
        }



        //para cada corpo
        for (int j=0;j<chara->getNumBodies();j++) {
            if(j==15){
                for( int i=0; i<num_frames; i++ ) {
                    moCap->getFrameMotion(i)->setOrientation(j,moCap->getFrameMotion(i)->getOrientation(3));
                }
            }
            if(j==16){
                for( int i=0; i<num_frames; i++ ) {
                  moCap->getFrameMotion(i)->setOrientation(j,moCap->getFrameMotion(i)->getOrientation(4));
                }
            }
            else{

          //pula uma linha
            getline(istr, stmp);
          //le as n posicoes
          for( int i=0; i<num_frames; i++ ) {
            getline(istr, stmp);
            ss.clear();
            Vec4 pos;
            ss << stmp;
            ss >> dtmp;
            pos.x3 = dtmp;
            ss >> dtmp;
            pos.x1 = dtmp;
            ss >> dtmp;
            pos.x2 = dtmp;
//            Caso do chute rodado
//            if(j==13) //caso o pé esteja invertido
//                moCap->getFrameMotion(i)->setPosition(j,pos-Vec4(0,0.031,0));
//            else
//                moCap->getFrameMotion(i)->setPosition(j,pos);
            moCap->getFrameMotion(i)->setPosition(j,pos);
          }
          //pula duas linhas
            getline(istr, stmp);
            getline(istr, stmp);
          //le os n quaternions
          for( int i=0; i<num_frames; i++ ) {
            getline(istr, stmp);
            ss.clear();
            ss << stmp;
            QuaternionQ quat;
            ss >> dtmp;
            quat.setScalar(dtmp);
            ss >> dtmp;
            quat.setPosZ(dtmp);
            ss >> dtmp;
            quat.setPosX(dtmp);
            ss >> dtmp;
            quat.setPosY(dtmp);
//            Caso swing one foot
//            if (j==3)
//                moCap->getFrameMotion(i)->setOrientation(j,quat*Quaternion(0,180,0));
//            else
//            Caso chute rodado            
//            if(j==13) //caso o pé esteja invertido
//                moCap->getFrameMotion(i)->setOrientation(j,quat*QuaternionQ(-8,10,160));
//            else
                moCap->getFrameMotion(i)->setOrientation(j,quat);
          }
          //pula uma linha
            getline(istr, stmp);
            }
        }
        //moCap->setEndClycle(moCap->sizeFrames());
        return true;
}

bool Utils::saveListMotion(QList<Vec4> motion, QList<Vec4> mocap, QList<double> y, const string &fileName)
{
    ofstream ff(fileName.data());
    ff << motion.size();
    ff << "\n\n";
    for (int i=0;i<mocap.size();i++) {
        ff << y.at(i);
        ff << " , ";
        ff << mocap.at(i).x1;
        ff << " , ";
        ff << mocap.at(i).x2;
        ff << " , ";
        ff << mocap.at(i).x3;
        ff << " , ";
        ff << motion.at(i).x1;
        ff << " , ";
        ff << motion.at(i).x2;
        ff << " , ";
        ff << motion.at(i).x3;
        ff << "\n";
    }
//    ff << "\n\n" ;
//    for (int i=0;i<motion.size();i++) {

//        ff << "\n";
//    }
//    ff << "\n\n";
//    for (int i=0;i<mocap.size();i++) {
//        ff << y.at(i);
//        ff << " , ";
//        ff << mocap.at(i).x2;
//        ff << "\n";
//    }
//    ff << "\n\n" ;
//    for (int i=0;i<motion.size();i++) {
//        ff << y.at(i);
//        ff << " , ";
//        ff << motion.at(i).x2;
//        ff << "\n";
//    }
//    ff << "\n\n";
//    for (int i=0;i<mocap.size();i++) {
//        ff << y.at(i);
//        ff << " , ";
//        ff << mocap.at(i).x3;
//        ff << "\n";
//    }
//    ff << "\n\n" ;
//    for (int i=0;i<motion.size();i++) {
//        ff << y.at(i);
//        ff << " , ";
//        ff << motion.at(i).x3;
//        ff << "\n";
//    }
    ff.close();
    return true;
}

bool Utils::saveListDistMotion(QList<double> sim, QList<double> mocap, QList<double> tim, const string &fileName)
{
    ofstream ff(fileName.data());
    ff << tim.size();
    ff << "\n\n";
    for (int i=0;i<tim.size();i++) {
        ff << tim.at(i);
        ff << " , ";
        ff << mocap.at(i);
        ff << " , ";
        ff << sim.at(i);
        ff << "\n";
    }
    ff.close();
    return true;
}

