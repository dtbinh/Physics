#ifndef GRF_H
#define GRF_H
#define dDOUBLE
#include <ode/ode.h>
#include "math/vec4.h"
#include "scene/object.h"

#include <vector>
class GRF{
public:
  Vec4 position;
  dJointFeedback* jtFb;
  int noGroundGeom; //0:plane,plane; 1:body,plane; 2:plane,body; 3:body,body
  Object *b1;
  Object *b2;
  GRF();
  GRF(Vec4 pos,dJointFeedback* joint,int id);
  static void drawGRF(std::vector<GRF> grfs,Vec4 target); //usualmente target é o COM
  static Vec4 forcesGRF(std::vector<GRF> grfs,Object* obj);
  static Vec4 positionGRF(std::vector<GRF> grfs,Object* obj);
  static std::vector<GRF> forcesGRF2Object(std::vector<GRF> grfs,Object* obj);

};

#endif // GRF_H
