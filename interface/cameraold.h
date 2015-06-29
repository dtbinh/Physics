#ifndef CAMERAOLD_H
#define CAMERAOLD_H
#include "math/vec4.h"

#define CAMERA_FAR 0
#define CAMERA_GAME 1

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class CameraOld
{
public:

   Vec4 eye; //e
   Vec4 at;  //c
   Vec4 up;  //u
   bool axis_x,axis_y,axis_z;

   int type;

   CameraOld();
   CameraOld(Vec4 e, Vec4 c, Vec4 u);
   CameraOld(float ex, float ey, float ez, float cx, float cy, float cz, float ux, float uy, float uz);
   ~CameraOld();

   //vetor Vec e u sempre perpendiculares e u sempre unitario
   void zoom(float, float);
   void translatex(float, float);
   void translatey(float, float);
   void rotatex(float, float);
   void rotatey(float, float);
   void rotatez(float, float);
   Vec4 getPickedPoint(float, float);
   void adapteVetorAtDist2Game();
   void adapteVetorAtGame2Dist(float r);
   void lockAxisX(bool b);
   void lockAxisY(bool b);
   void lockAxisZ(bool b);

   glm::mat4 GetViewMatrix();

};

#endif // CAMERAOLD_H
