#include "grf.h"
#include "graphics/draw.h"
GRF::GRF()
{
}

GRF::GRF(Vec4 pos,dJointFeedback *joint,int id)
{
    this->position = pos;
    this->jtFb = joint;
    this->noGroundGeom = id;
}

void GRF::drawGRF(std::vector<GRF> grfs, Vec4 target)
{
    //fs::vector GRFwrench_COM = fs::vector(6);
    float size = 0;
for (unsigned int fc=0;fc<grfs.size();fc++) {
    Vec4 groundForce;
    if ( grfs[fc].noGroundGeom != 3 ) {
      //groundForce
        dReal* groundforce = NULL; //dVector3
          if ( grfs[fc].noGroundGeom == 1 ) groundforce = grfs[fc].jtFb->f1;
          if ( grfs[fc].noGroundGeom == 2 ) groundforce = grfs[fc].jtFb->f2;
          groundForce = Vec4(groundforce[0],groundforce[1],groundforce[2]);
    }
    if (groundForce.module()>size) size = groundForce.module();
}

      //para todos os contatos
      for (unsigned int fc=0;fc<grfs.size();fc++) {
        //position of contact (red sphere)
          //feedbackContacts[fc].pos;
        //ground force e ground torque
          Vec4 groundForce;
          Vec4 groundTorque;
          //Vetor3D groundForce3D;
          //Vetor3D groundTorque3D;
            //cout<<"feedbackContacts[fc].noGroundGeom: "<<feedbackContacts[fc].noGroundGeom<<"\n";
          if ( grfs[fc].noGroundGeom != 3 ) {
            //groundForce
              dReal* groundforce = NULL; //dVector3
                if ( grfs[fc].noGroundGeom == 1 ) groundforce = grfs[fc].jtFb->f1;
                if ( grfs[fc].noGroundGeom == 2 ) groundforce = grfs[fc].jtFb->f2;
              groundForce = Vec4(groundforce[0],groundforce[1],groundforce[2]);
            //groundTorque
              dReal* groundtorque = NULL; //dVector3
                if ( grfs[fc].noGroundGeom == 1 ) groundtorque = grfs[fc].jtFb->t1;
                if ( grfs[fc].noGroundGeom == 2 ) groundtorque = grfs[fc].jtFb->t2;
              groundTorque = Vec4(groundtorque[0],groundtorque[1],groundtorque[2]);
            //print
              /*
              groundForce = feedbackContacts[fc].jtFb->f1;
                cout << "groundForce1: " << groundForce[0] << ", " << groundForce[1] << ", " << groundForce[2] << "\n";
              groundForce = feedbackContacts[fc].jtFb->f2;
                cout << "groundForce2: " << groundForce[0] << ", " << groundForce[1] << ", " << groundForce[2] << "\n";
              groundTorque = feedbackContacts[fc].jtFb->t1;
                cout << "groundTorque1: " << groundTorque[0] << ", " << groundTorque[1] << ", " << groundTorque[2] << "\n";
              groundTorque = feedbackContacts[fc].jtFb->t2;
                cout << "groundTorque2: " << groundTorque[0] << ", " << groundTorque[1] << ", " << groundTorque[2] << "\n";
              //*/
          }
        //relativo ao COM
          //COMpos
          //  Vetor3D COMpos = this->modelo->getCOG();
          //fc2COM
            Vec4 fc2Target = target - ( grfs[fc].position );
            Vec4 pos = grfs[fc].position;
              //cout<<"feedbackContacts[fc].pos[0]: "<<feedbackContacts[fc].pos.x<<", feedbackContacts[fc].pos[1]: "<<feedbackContacts[fc].pos.y<<", feedbackContacts[fc].pos[2]: "<<feedbackContacts[fc].pos.z<<"\n";
          //torque_fc
            Vec4 torque_fc = groundForce^(fc2Target);
            //torque_fc.add( groundTorque3D );
          //force_fc
            Vec4 force_fc = groundForce;
            Draw::drawArrow(pos,force_fc/size,(force_fc.module()/size)/8.,MATERIAL_BRASS);
        //adiciona ao vector GRFwrench_COM
//          //torque_fc
//            GRFwrench_COM[0] += torque_fc.x;
//            GRFwrench_COM[1] += torque_fc.y;
//            GRFwrench_COM[2] += torque_fc.z;
//          //force_fc
//            GRFwrench_COM[3] += force_fc.x;
//            GRFwrench_COM[4] += force_fc.y;
//            GRFwrench_COM[5] += force_fc.z;
      }

      //print
        //GRFwrench_COM.print();

      return;
}
