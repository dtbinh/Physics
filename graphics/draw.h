#ifndef DRAW_H
#define DRAW_H
//#define dDOUBLE

#include <ode/common.h>
#include <math/quaternion.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "extra/ObjMesh.h"
class Matrix4x4;
#include "extra/material.h"
#include "math/vec4.h"
class Mesh;

class Draw
{
public:
    Draw();
    static void drawCube(Matrix4x4 *transform,Vec4 properties, Material *mat,float alpha=0);
    static void drawCube(Matrix4x4 *transform,Vec4 properties, int material);
    static Mesh*getMeshCube(Matrix4x4 *transform,Vec4 properties, Mesh *mesh=NULL);
    static void drawCylinder(Matrix4x4 *transform, Material *mat, Vec4 properties);
    static void drawCylinder(Matrix4x4 *transform, int material);
    static void drawCylinder(Vec4 position, Vec4 axis, double radius, double height, int material);
    static void drawCylinderClosed(Vec4 position, Vec4 axis, double radius, double height, int material);
    static void drawSphere(Matrix4x4 *transform,Material *mat, float radius=0);
    static void drawSphere(Vec4 position,int material=0,float size=0.04);
    static void drawLine(Vec4 p1,Vec4 p2,Vec4 color,float width);
    static void drawSphereSelected(Vec4 position);
    static void drawPoint(Vec4 p,float size=0.5,Vec4 color=Vec4(1,1,1));
    static void drawCOM(Vec4 p,float size=0.5,Vec4 color=Vec4(1,1,1));
    static void drawCOMProjected(Vec4 p,float size=0.5,Vec4 color=Vec4(1,1,1),Vec4 rot=Vec4());
    static void drawTargetProjected(Vec4 p,float size=0.5,Vec4 color=Vec4(1,1,1),Vec4 rot=Vec4());
    static void drawGround(int size,Vec4 rot=Vec4(0,0,0),float reflect=false);
    static void drawGroundTexture(int size,int texture);
    static void drawCircle2D(Vec4 center,float radius, Vec4 color,float size,Vec4 rot=Vec4()); //desenha um circulo projetado no plano
    static void drawSkybox(Vec4 min= Vec4(0,0,0), Vec4 max = Vec4(10,10,10) ,int texture=0);
    static void drawCoffeeCup(Vec4 position, int material,QuaternionQ q=QuaternionQ());
    static void drawObj(Vec4 position, int material, QuaternionQ q, QString file, ObjMesh *n);
    static void drawObj(Matrix4x4 *transform,int material,ObjMesh *n);
    static void drawSelection(Matrix4x4 *transform,Vec4 properties,Vec4 color=Vec4(1,1,1));                        //desenha um cubo selecionado o objeto
    static void drawWireframe(Matrix4x4 *transform,Vec4 properties,Vec4 color=Vec4(1,0,0));
    static void drawSelection(Vec4 properties,float radius);                        //desenha um cubo selecionado o objeto
    static void drawArrow(Vec4 origin, Vec4 direction, float size,int material=MATERIAL_RUBY);
    static void drawArrow2D(float angle,Vec4 anchor);
    static void drawAxisCameraView(float size);
    static void gluClosedCylinder(GLUquadric*, GLdouble, GLdouble, GLdouble, GLint, GLint);
    static void shadowMatrix( GLfloat shadowMat[4][4], GLfloat groundplane[4], GLfloat lightpos[4] );
    static void setTransformODE(const dReal* pos, const dReal* R);
    static void drawText(QString text,int x, int y);
    //static void gluCapsule(GLUquadric*, GLdouble, GLdouble, GLdouble, GLint, GLint);
    static void drawArrow3D(Vec4 origin, Vec4 velocity, Vec4 direction, float size, int material, Vec4 ground_inclination = Vec4());
};

#endif // DRAW_H



