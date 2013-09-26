#include "grf.h"

GRF::GRF()
{
}

GRF::GRF(Vec4 pos,dJointFeedback *joint,int id)
{
    this->position = pos;
    this->jtFb = joint;
    this->noGroundGeom = id;
}
