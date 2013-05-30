#ifndef DRAW_H
#define DRAW_H

class Matrix4x4;
class Material;
#include "math/vec4.h"

class Draw
{
public:
    Draw();
    static void drawCube(Matrix4x4 *transform,Vec4 properties, Material *mat);
    static void drawCylinder(Matrix4x4 *transform,Material *mat);
    static void drawSphere(Matrix4x4 *transform,Material *mat);
    static void drawSphere(Vec4 position);
    static void drawPoint(Vec4 p,float size=0.5);
    static void drawGround(int size);
    static void drawSelection(Matrix4x4 *transform,Vec4 properties);                        //desenha um cubo selecionado o objeto
};

#endif // DRAW_H
