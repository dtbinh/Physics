#ifndef GRF_H
#define GRF_H
#define dDOUBLE
#include <ode/ode.h>
#include "math/vec4.h"

class GRF{
public:
  Vec4 position;
  dJointFeedback* jtFb;
  int noGroundGeom; //0:plane,plane; 1:body,plane; 2:plane,body; 3:body,body
  GRF();
  GRF(Vec4 pos,dJointFeedback* joint,int id);
};

#endif // GRF_H
