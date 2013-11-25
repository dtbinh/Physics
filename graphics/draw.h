#ifndef DRAW_H
#define DRAW_H
#define dDOUBLE
#include <ode/common.h>
#include <math/quaternion.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
class Matrix4x4;
#include "extra/material.h"
#include "math/vec4.h"

class Draw
{
public:
    Draw();
    static void drawCube(Matrix4x4 *transform,Vec4 properties, Material *mat,float alpha=0);
    static void drawCube(Matrix4x4 *transform,Vec4 properties, int material);
    static void drawCylinder(Matrix4x4 *transform,Material *mat);
    static void drawSphere(Matrix4x4 *transform,Material *mat, float radius=0);
    static void drawSphere(Vec4 position,int material=0,float size=0.04);
    static void drawLine(Vec4 p1,Vec4 p2,Vec4 color,float width);
    static void drawSphereSelected(Vec4 position);
    static void drawPoint(Vec4 p,float size=0.5,Vec4 color=Vec4(1,1,1));
    static void drawGround(int size);
    static void drawCoffeeCup(Vec4 position, int material,Quaternion q=Quaternion());
    static void drawSelection(Matrix4x4 *transform,Vec4 properties,Vec4 color=Vec4(1,1,1));                        //desenha um cubo selecionado o objeto
    static void drawWireframe(Matrix4x4 *transform,Vec4 properties,Vec4 color=Vec4(1,0,0));
    static void drawSelection(Vec4 properties,float radius);                        //desenha um cubo selecionado o objeto
    static void drawArrow(Vec4 origin, Vec4 direction, float size,int material=MATERIAL_RUBY);
    static void drawArrow2D(float angle,Vec4 anchor);
    static void drawAxisCameraView(float size);
    static void gluClosedCylinder(GLUquadric*, GLdouble, GLdouble, GLdouble, GLint, GLint);
    static void shadowMatrix( GLfloat shadowMat[4][4], GLfloat groundplane[4], GLfloat lightpos[4] );
    static void setTransformODE(const dReal* pos, const dReal* R);
    //static void gluCapsule(GLUquadric*, GLdouble, GLdouble, GLdouble, GLint, GLint);
};

#endif // DRAW_H



