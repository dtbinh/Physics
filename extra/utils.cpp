#include "utils.h"
#include "scene/object.h"
#include "scene/scene.h"
#include "scene/character.h"
#include "mocap/mocap.h"
#include "mocap/frame.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <QFileInfo>
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
    Quaternion quat;
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
              scene->addJointFixed(father,chara);
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
            Quaternion q = chara->getBody(i)->getRotationCurrent();
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
        printf("\nIn load!");
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
            moCap->getFrameMotion(i)->appendOrientation(Quaternion( 1.0,0.0,0.0,0.0));
          }
        }


        //para cada corpo
        for (int j=0;j<chara->getNumBodies();j++) {
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
            Quaternion quat;
            ss >> dtmp;
            quat.setScalar(dtmp);
            ss >> dtmp;
            quat.setPosZ(dtmp);
            ss >> dtmp;
            quat.setPosX(dtmp);
            ss >> dtmp;
            quat.setPosY(dtmp);
            moCap->getFrameMotion(i)->setOrientation(j,quat);
          }
          //pula uma linha
            getline(istr, stmp);
        }
        return true;
}

