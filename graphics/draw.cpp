#include "draw.h"
#include "math/matrix4x4.h"
#include "extra/material.h"
#include "GL/glu.h"
#include "GL/glut.h"
#include "math/quaternion.h"
#include "physics/physics.h"
#include "extra/ObjMesh.h"

bool idraw_ground = false;
int idrawGround;
int idrawGround2;
int slices = 50;
int stacks = 50;


Draw::Draw()
{
}

void Draw::drawCube(Matrix4x4 *transform,Vec4 p, Material *mat, float)
{
    Vec4 vertexs[8];
    Vec4 normals[6];

    vertexs[0] = (Vec4(-p.x()/2,-p.y()/2,-p.z()/2));
    vertexs[1] = (Vec4(-p.x()/2,-p.y()/2, p.z()/2));
    vertexs[2] = (Vec4( p.x()/2,-p.y()/2, p.z()/2));
    vertexs[3] = (Vec4( p.x()/2,-p.y()/2,-p.z()/2));
    vertexs[4] = (Vec4(-p.x()/2, p.y()/2,-p.z()/2));
    vertexs[5] = (Vec4(-p.x()/2, p.y()/2, p.z()/2));
    vertexs[6] = (Vec4( p.x()/2, p.y()/2, p.z()/2));
    vertexs[7] = (Vec4( p.x()/2, p.y()/2,-p.z()/2));

    normals[0] = Vec4::crossProduct(vertexs[3]-vertexs[0],vertexs[2]-vertexs[3]).unitary(); //vertices:0,1,2,3
    normals[1] = Vec4::crossProduct(vertexs[4]-vertexs[7],vertexs[5]-vertexs[4]).unitary(); //vertices:4,5,6,7
    normals[2] = Vec4::crossProduct(vertexs[6]-vertexs[2],vertexs[5]-vertexs[6]).unitary(); //vertices:1,2,5,6
    normals[3] = Vec4::crossProduct(vertexs[7]-vertexs[3],vertexs[6]-vertexs[7]).unitary(); //vertices:2,3,6,7
    normals[4] = Vec4::crossProduct(vertexs[4]-vertexs[5],vertexs[0]-vertexs[4]).unitary(); //vertices:0,1,4,5
    normals[5] = Vec4::crossProduct(vertexs[4]-vertexs[0],vertexs[7]-vertexs[4]).unitary(); //vertices:0,3,4,7
//    normals[0] = transform->transform_normal_ray(transform,Vec4( 0,-1, 0)).unitary();
//    normals[1] = transform->transform_normal_ray(transform,Vec4( 0, 1, 0)).unitary();
//    normals[2] = transform->transform_normal_ray(transform,Vec4( 0, 0, 1)).unitary();
//    normals[3] = transform->transform_normal_ray(transform,Vec4( 1, 0, 0)).unitary();
//    normals[4] = transform->transform_normal_ray(transform,Vec4( 0, 0,-1)).unitary();
//    normals[5] = transform->transform_normal_ray(transform,Vec4(-1, 0, 0)).unitary();


    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,mat->ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat->diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat->specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat->shininess*128);
    glPushMatrix();
    glMultMatrixf(transform->getMatrix());
    glBegin(GL_QUADS);
        glNormal3f(normals[0].x(),normals[0].y(),normals[0].z());
        glVertex3f(vertexs[0].x(),vertexs[0].y(),vertexs[0].z());
        glVertex3f(vertexs[3].x(),vertexs[3].y(),vertexs[3].z());
        glVertex3f(vertexs[2].x(),vertexs[2].y(),vertexs[2].z());
        glVertex3f(vertexs[1].x(),vertexs[1].y(),vertexs[1].z());

        glNormal3f(normals[1].x(),normals[1].y(),normals[1].z());
        glVertex3f(vertexs[4].x(),vertexs[4].y(),vertexs[4].z());
        glVertex3f(vertexs[5].x(),vertexs[5].y(),vertexs[5].z());
        glVertex3f(vertexs[6].x(),vertexs[6].y(),vertexs[6].z());
        glVertex3f(vertexs[7].x(),vertexs[7].y(),vertexs[7].z());

        glNormal3f(normals[2].x(),normals[2].y(),normals[2].z());
        glVertex3f(vertexs[1].x(),vertexs[1].y(),vertexs[1].z());
        glVertex3f(vertexs[2].x(),vertexs[2].y(),vertexs[2].z());
        glVertex3f(vertexs[6].x(),vertexs[6].y(),vertexs[6].z());
        glVertex3f(vertexs[5].x(),vertexs[5].y(),vertexs[5].z());

        glNormal3f(normals[3].x(),normals[3].y(),normals[3].z());
        glVertex3f(vertexs[2].x(),vertexs[2].y(),vertexs[2].z());
        glVertex3f(vertexs[3].x(),vertexs[3].y(),vertexs[3].z());
        glVertex3f(vertexs[7].x(),vertexs[7].y(),vertexs[7].z());
        glVertex3f(vertexs[6].x(),vertexs[6].y(),vertexs[6].z());

        glNormal3f(normals[4].x(),normals[4].y(),normals[4].z());
        glVertex3f(vertexs[4].x(),vertexs[4].y(),vertexs[4].z());
        glVertex3f(vertexs[0].x(),vertexs[0].y(),vertexs[0].z());
        glVertex3f(vertexs[1].x(),vertexs[1].y(),vertexs[1].z());
        glVertex3f(vertexs[5].x(),vertexs[5].y(),vertexs[5].z());

        glNormal3f(normals[5].x(),normals[5].y(),normals[5].z());
        glVertex3f(vertexs[0].x(),vertexs[0].y(),vertexs[0].z());
        glVertex3f(vertexs[4].x(),vertexs[4].y(),vertexs[4].z());
        glVertex3f(vertexs[7].x(),vertexs[7].y(),vertexs[7].z());
        glVertex3f(vertexs[3].x(),vertexs[3].y(),vertexs[3].z());
    glEnd();
    glPopMatrix();

}

void Draw::drawCube(Matrix4x4 *transform, Vec4 p, int material)
{
    Material *mat = new Material();
    Material::setMaterial(mat,material);
    Vec4 vertexs[8];
    Vec4 normals[6];

    vertexs[0] = (Vec4(-p.x()/2,-p.y()/2,-p.z()/2));
    vertexs[1] = (Vec4(-p.x()/2,-p.y()/2, p.z()/2));
    vertexs[2] = (Vec4( p.x()/2,-p.y()/2, p.z()/2));
    vertexs[3] = (Vec4( p.x()/2,-p.y()/2,-p.z()/2));
    vertexs[4] = (Vec4(-p.x()/2, p.y()/2,-p.z()/2));
    vertexs[5] = (Vec4(-p.x()/2, p.y()/2, p.z()/2));
    vertexs[6] = (Vec4( p.x()/2, p.y()/2, p.z()/2));
    vertexs[7] = (Vec4( p.x()/2, p.y()/2,-p.z()/2));

    normals[0] = Vec4::crossProduct(vertexs[3]-vertexs[0],vertexs[2]-vertexs[3]).unitary(); //vertices:0,1,2,3
    normals[1] = Vec4::crossProduct(vertexs[4]-vertexs[7],vertexs[5]-vertexs[4]).unitary(); //vertices:4,5,6,7
    normals[2] = Vec4::crossProduct(vertexs[6]-vertexs[2],vertexs[5]-vertexs[6]).unitary(); //vertices:1,2,5,6
    normals[3] = Vec4::crossProduct(vertexs[7]-vertexs[3],vertexs[6]-vertexs[7]).unitary(); //vertices:2,3,6,7
    normals[4] = Vec4::crossProduct(vertexs[4]-vertexs[5],vertexs[0]-vertexs[4]).unitary(); //vertices:0,1,4,5
    normals[5] = Vec4::crossProduct(vertexs[4]-vertexs[0],vertexs[7]-vertexs[4]).unitary(); //vertices:0,3,4,7
//    normals[0] = transform->transform_normal_ray(transform,Vec4( 0,-1, 0)).unitary();
//    normals[1] = transform->transform_normal_ray(transform,Vec4( 0, 1, 0)).unitary();
//    normals[2] = transform->transform_normal_ray(transform,Vec4( 0, 0, 1)).unitary();
//    normals[3] = transform->transform_normal_ray(transform,Vec4( 1, 0, 0)).unitary();
//    normals[4] = transform->transform_normal_ray(transform,Vec4( 0, 0,-1)).unitary();
//    normals[5] = transform->transform_normal_ray(transform,Vec4(-1, 0, 0)).unitary();


    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,mat->ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat->diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat->specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat->shininess*128);
    glPushMatrix();
    glMultMatrixf(transform->getMatrix());
    glBegin(GL_QUADS);
        glNormal3f(normals[0].x(),normals[0].y(),normals[0].z());
        glVertex3f(vertexs[0].x(),vertexs[0].y(),vertexs[0].z());
        glVertex3f(vertexs[3].x(),vertexs[3].y(),vertexs[3].z());
        glVertex3f(vertexs[2].x(),vertexs[2].y(),vertexs[2].z());
        glVertex3f(vertexs[1].x(),vertexs[1].y(),vertexs[1].z());

        glNormal3f(normals[1].x(),normals[1].y(),normals[1].z());
        glVertex3f(vertexs[4].x(),vertexs[4].y(),vertexs[4].z());
        glVertex3f(vertexs[5].x(),vertexs[5].y(),vertexs[5].z());
        glVertex3f(vertexs[6].x(),vertexs[6].y(),vertexs[6].z());
        glVertex3f(vertexs[7].x(),vertexs[7].y(),vertexs[7].z());

        glNormal3f(normals[2].x(),normals[2].y(),normals[2].z());
        glVertex3f(vertexs[1].x(),vertexs[1].y(),vertexs[1].z());
        glVertex3f(vertexs[2].x(),vertexs[2].y(),vertexs[2].z());
        glVertex3f(vertexs[6].x(),vertexs[6].y(),vertexs[6].z());
        glVertex3f(vertexs[5].x(),vertexs[5].y(),vertexs[5].z());

        glNormal3f(normals[3].x(),normals[3].y(),normals[3].z());
        glVertex3f(vertexs[2].x(),vertexs[2].y(),vertexs[2].z());
        glVertex3f(vertexs[3].x(),vertexs[3].y(),vertexs[3].z());
        glVertex3f(vertexs[7].x(),vertexs[7].y(),vertexs[7].z());
        glVertex3f(vertexs[6].x(),vertexs[6].y(),vertexs[6].z());

        glNormal3f(normals[4].x(),normals[4].y(),normals[4].z());
        glVertex3f(vertexs[4].x(),vertexs[4].y(),vertexs[4].z());
        glVertex3f(vertexs[0].x(),vertexs[0].y(),vertexs[0].z());
        glVertex3f(vertexs[1].x(),vertexs[1].y(),vertexs[1].z());
        glVertex3f(vertexs[5].x(),vertexs[5].y(),vertexs[5].z());

        glNormal3f(normals[5].x(),normals[5].y(),normals[5].z());
        glVertex3f(vertexs[0].x(),vertexs[0].y(),vertexs[0].z());
        glVertex3f(vertexs[4].x(),vertexs[4].y(),vertexs[4].z());
        glVertex3f(vertexs[7].x(),vertexs[7].y(),vertexs[7].z());
        glVertex3f(vertexs[3].x(),vertexs[3].y(),vertexs[3].z());
    glEnd();
    glPopMatrix();
    delete mat;
}

void Draw::drawCylinder(Matrix4x4 *transform,Material *mat)
{
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,mat->ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat->diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat->specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat->shininess*128);

    GLUquadric *q = gluNewQuadric();
    //transform->showMatrix4x4();
    Vec4 position = Vec4(transform->get(12),transform->get(13),transform->get(14));
    Vec4 scale    = transform->getScaleSeted();
    glPushMatrix();
        //gluQuadricNormals(q, GLU_SMOOTH);
        glRotatef(90,0,1,0);
        glTranslatef(position.x(),position.y(),position.z());
        gluCylinder(q, scale.x(),scale.x(),scale.y(), slices, stacks);
    glPopMatrix();
    gluDeleteQuadric(q);
}

void Draw::drawSphere(Matrix4x4 *transform,Material *mat,float radius)
{
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,mat->ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat->diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat->specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat->shininess*128);

    GLUquadric *q = gluNewQuadric();
    //transform->showMatrix4x4();

    Vec4 position = Vec4(transform->get(12),transform->get(13),transform->get(14));
    Vec4 scale    = Vec4(radius,radius,radius);
    glPushMatrix();
        gluQuadricNormals(q, GLU_SMOOTH);
        glTranslatef(position.x(),position.y(),position.z());
        gluSphere(q, scale.x(), slices, stacks);

    glPopMatrix();

    gluDeleteQuadric(q);
}

void Draw::drawSphere(Vec4 position, int material, float size)
{
    Material *mat = new Material();
    Material::setMaterial(mat,material);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,mat->ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat->diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat->specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat->shininess*128);

    GLUquadric *q = gluNewQuadric();

    //Vec4 scale    = Vec4(0.5,0.5,0.5);
    glPushMatrix();
        gluQuadricNormals(q, GLU_SMOOTH);
        glTranslatef(position.x(),position.y(),position.z());
        gluSphere(q, size, slices, stacks);

    glPopMatrix();

    gluDeleteQuadric(q);
    delete mat;
}

void Draw::drawLine(Vec4 p1, Vec4 p2, Vec4 color, float width)
{
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glColor3f(color.x(),color.y(),color.z());
    glLineWidth(width);
    glBegin(GL_LINES);
        glVertex3f(p1.x(),p1.y(),p1.z());
        glVertex3f(p2.x(),p2.y(),p2.z());
    glEnd();
    glLineWidth(1.0);
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

void Draw::drawSphereSelected(Vec4 position)
{

    Material *mat = new Material();
    Material::setMaterial(mat,MATERIAL_RED_PLASTIC);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,mat->ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat->diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat->specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat->shininess*128);

    GLUquadric *q = gluNewQuadric();

    //Vec4 scale    = Vec4(0.5,0.5,0.5);
    glPushMatrix();
        gluQuadricNormals(q, GLU_SMOOTH);
        glTranslatef(position.x(),position.y(),position.z());
        gluSphere(q, 0.04, slices, stacks);

    glPopMatrix();

    gluDeleteQuadric(q);
    delete mat;
}

void Draw::drawPoint(Vec4 p, float size, Vec4 color)
{
     glDisable(GL_LIGHTING);

    glColor3f(color.x(),color.y(),color.z());
    GLUquadric *q = gluNewQuadric();
    //transform->showMatrix4x4();
    Vec4 position = p;
    //Vec4 scale    = transform->getScaleSeted();
    glPushMatrix();
        gluQuadricNormals(q, GLU_SMOOTH);
        glTranslatef(position.x(),position.y(),position.z());
        gluSphere(q, size, slices, stacks);
    glPopMatrix();
    gluDeleteQuadric(q);
    glEnable(GL_LIGHTING);

}

void Draw::drawGround(int size)
{

    if(idraw_ground) {
        glDisable(GL_LIGHTING);
        glPushMatrix();
        glColor3f(0.5,0.5,0.5);
        glCallList(idrawGround);
        glEnable(GL_LIGHTING);
        Material *mat = new Material();
        mat->setMaterial(mat,MATERIAL_SILVER_POLIERT);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,mat->ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat->diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat->specular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat->shininess*128);
        glCallList(idrawGround+1);
        glPopMatrix();
        delete mat;
        return;
    }


    idrawGround = glGenLists(2);
    //idrawGround2 = glGenLists(2);
    //desenha grid na base
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glNewList(idrawGround,GL_COMPILE);

    glBegin(GL_LINES);
    for(int i=-size;i<=size;i+=1){
        for(int j=size;j>=-size;j-=1){
            glVertex3f(i,0,j);
            glVertex3f(i,0,-j);
            glVertex3f(i,0,j);
            glVertex3f(-i,0,j);
        }
    }
    glEnd();
    glEndList();
    glPopMatrix();
    glEnable(GL_LIGHTING);

    //desenha solo



    glPushMatrix();
    glNewList(idrawGround+1,GL_COMPILE);
    glBegin(GL_QUADS);
    glNormal3f(0,1,0);
    glVertex3f(size,0,size);
    glVertex3f(size,0,-size);
    glVertex3f(-size,0,-size);
    glVertex3f(-size,0,size);
    glEnd();
    glEndList();
    glPopMatrix();
    glEndList();
    if (!idraw_ground) idraw_ground = true;


}

void Draw::drawCoffeeCup(Vec4 position, int material,Quaternion q)
{
    Material *mat = new Material();
    mat->setMaterial(mat,material);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,mat->ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat->diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat->specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat->shininess*128);
    static ObjMesh obj("../objs/Kop.obj");
    Matrix4x4 m = q.getMatrix();
    glPushMatrix();
    glTranslatef(position.x(),position.y(),position.z());
    glMultMatrixf(m.matrix);
    obj.draw();
    glPopMatrix();
    delete mat;
}

void Draw::drawSelection(Matrix4x4 *transform,Vec4 p,Vec4 color)
{
    Vec4 vertexs[8];
    vertexs[0] = (Vec4(-p.x()/2,-p.y()/2,-p.z()/2));
    vertexs[1] = (Vec4(-p.x()/2,-p.y()/2, p.z()/2));
    vertexs[2] = (Vec4( p.x()/2,-p.y()/2, p.z()/2));
    vertexs[3] = (Vec4( p.x()/2,-p.y()/2,-p.z()/2));
    vertexs[4] = (Vec4(-p.x()/2, p.y()/2,-p.z()/2));
    vertexs[5] = (Vec4(-p.x()/2, p.y()/2, p.z()/2));
    vertexs[6] = (Vec4( p.x()/2, p.y()/2, p.z()/2));
    vertexs[7] = (Vec4( p.x()/2, p.y()/2,-p.z()/2));

    float less = fmin(fmin(p.x(),p.y()),p.z())*0.3;
    glColor3f(color.x(),color.y(),color.z());

    glDisable(GL_LIGHTING);
    glPushMatrix();
    glMultMatrixf(transform->getMatrix());
    glBegin(GL_LINES);
        //canto maximo
        glVertex3f(vertexs[6].x(),vertexs[6].y(),vertexs[6].z());
        glVertex3f(vertexs[6].x(),vertexs[6].y(),vertexs[6].z()-less);

        glVertex3f(vertexs[6].x(),vertexs[6].y(),vertexs[6].z());
        glVertex3f(vertexs[6].x()-less,vertexs[6].y(),vertexs[6].z());

        glVertex3f(vertexs[6].x(),vertexs[6].y(),vertexs[6].z());
        glVertex3f(vertexs[6].x(),vertexs[6].y()-less,vertexs[6].z());
        //canto minimo
        glVertex3f(vertexs[0].x(),vertexs[0].y(),vertexs[0].z());
        glVertex3f(vertexs[0].x(),vertexs[0].y(),vertexs[0].z()+less);

        glVertex3f(vertexs[0].x(),vertexs[0].y(),vertexs[0].z());
        glVertex3f(vertexs[0].x(),vertexs[0].y()+less,vertexs[0].z());

        glVertex3f(vertexs[0].x(),vertexs[0].y(),vertexs[0].z());
        glVertex3f(vertexs[0].x()+less,vertexs[0].y(),vertexs[0].z());

        //canto baixo
        glVertex3f(vertexs[6].x(),vertexs[0].y(),vertexs[0].z());
        glVertex3f(vertexs[6].x(),vertexs[0].y()+less,vertexs[0].z());

        glVertex3f(vertexs[6].x(),vertexs[0].y(),vertexs[0].z());
        glVertex3f(vertexs[6].x(),vertexs[0].y(),vertexs[0].z()+less);

        glVertex3f(vertexs[6].x(),vertexs[0].y(),vertexs[0].z());
        glVertex3f(vertexs[6].x()-less,vertexs[0].y(),vertexs[0].z());

        //canto baixo
        glVertex3f(vertexs[6].x(),vertexs[0].y(),vertexs[6].z());
        glVertex3f(vertexs[6].x(),vertexs[0].y()+less,vertexs[6].z());

        glVertex3f(vertexs[6].x(),vertexs[0].y(),vertexs[6].z());
        glVertex3f(vertexs[6].x(),vertexs[0].y(),vertexs[6].z()-less);

        glVertex3f(vertexs[6].x(),vertexs[0].y(),vertexs[6].z());
        glVertex3f(vertexs[6].x()-less,vertexs[0].y(),vertexs[6].z());

        //canto baixo
        glVertex3f(vertexs[0].x(),vertexs[0].y(),vertexs[6].z());
        glVertex3f(vertexs[0].x(),vertexs[0].y(),vertexs[6].z()-less);

        glVertex3f(vertexs[0].x(),vertexs[0].y(),vertexs[6].z());
        glVertex3f(vertexs[0].x(),vertexs[0].y()+less,vertexs[6].z());

        glVertex3f(vertexs[0].x(),vertexs[0].y(),vertexs[6].z());
        glVertex3f(vertexs[0].x()+less,vertexs[0].y(),vertexs[6].z());

        //canto cima
        glVertex3f(vertexs[6].x(),vertexs[6].y(),vertexs[0].z());
        glVertex3f(vertexs[6].x(),vertexs[6].y()-less,vertexs[0].z());

        glVertex3f(vertexs[6].x(),vertexs[6].y(),vertexs[0].z());
        glVertex3f(vertexs[6].x()-less,vertexs[6].y(),vertexs[0].z());

        glVertex3f(vertexs[6].x(),vertexs[6].y(),vertexs[0].z());
        glVertex3f(vertexs[6].x(),vertexs[6].y(),vertexs[0].z()+less);

        //canto cima
        glVertex3f(vertexs[0].x(),vertexs[6].y(),vertexs[6].z());
        glVertex3f(vertexs[0].x(),vertexs[6].y(),vertexs[6].z()-less);

        glVertex3f(vertexs[0].x(),vertexs[6].y(),vertexs[6].z());
        glVertex3f(vertexs[0].x(),vertexs[6].y()-less,vertexs[6].z());

        glVertex3f(vertexs[0].x(),vertexs[6].y(),vertexs[6].z());
        glVertex3f(vertexs[0].x()+less,vertexs[6].y(),vertexs[6].z());

        //canto cima
        glVertex3f(vertexs[0].x(),vertexs[6].y(),vertexs[0].z());
        glVertex3f(vertexs[0].x()+less,vertexs[6].y(),vertexs[0].z());

        glVertex3f(vertexs[0].x(),vertexs[6].y(),vertexs[0].z());
        glVertex3f(vertexs[0].x(),vertexs[6].y(),vertexs[0].z()+less);

        glVertex3f(vertexs[0].x(),vertexs[6].y(),vertexs[0].z());
        glVertex3f(vertexs[0].x(),vertexs[6].y()-less,vertexs[0].z());

    glEnd();
    glPopMatrix();
    glEnable(GL_LIGHTING);
    return;

}

void Draw::drawWireframe(Matrix4x4 *transform, Vec4 p,Vec4 color)
{
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    Vec4 vertexs[8];
    Vec4 normals[6];

    vertexs[0] = (Vec4(-p.x()/2,-p.y()/2,-p.z()/2));
    vertexs[1] = (Vec4(-p.x()/2,-p.y()/2, p.z()/2));
    vertexs[2] = (Vec4( p.x()/2,-p.y()/2, p.z()/2));
    vertexs[3] = (Vec4( p.x()/2,-p.y()/2,-p.z()/2));
    vertexs[4] = (Vec4(-p.x()/2, p.y()/2,-p.z()/2));
    vertexs[5] = (Vec4(-p.x()/2, p.y()/2, p.z()/2));
    vertexs[6] = (Vec4( p.x()/2, p.y()/2, p.z()/2));
    vertexs[7] = (Vec4( p.x()/2, p.y()/2,-p.z()/2));

    normals[0] = Vec4::crossProduct(vertexs[3]-vertexs[0],vertexs[2]-vertexs[3]).unitary(); //vertices:0,1,2,3
    normals[1] = Vec4::crossProduct(vertexs[4]-vertexs[7],vertexs[5]-vertexs[4]).unitary(); //vertices:4,5,6,7
    normals[2] = Vec4::crossProduct(vertexs[6]-vertexs[2],vertexs[5]-vertexs[6]).unitary(); //vertices:1,2,5,6
    normals[3] = Vec4::crossProduct(vertexs[7]-vertexs[3],vertexs[6]-vertexs[7]).unitary(); //vertices:2,3,6,7
    normals[4] = Vec4::crossProduct(vertexs[4]-vertexs[5],vertexs[0]-vertexs[4]).unitary(); //vertices:0,1,4,5
    normals[5] = Vec4::crossProduct(vertexs[4]-vertexs[0],vertexs[7]-vertexs[4]).unitary(); //vertices:0,3,4,7
//    normals[0] = transform->transform_normal_ray(transform,Vec4( 0,-1, 0)).unitary();
//    normals[1] = transform->transform_normal_ray(transform,Vec4( 0, 1, 0)).unitary();
//    normals[2] = transform->transform_normal_ray(transform,Vec4( 0, 0, 1)).unitary();
//    normals[3] = transform->transform_normal_ray(transform,Vec4( 1, 0, 0)).unitary();
//    normals[4] = transform->transform_normal_ray(transform,Vec4( 0, 0,-1)).unitary();
//    normals[5] = transform->transform_normal_ray(transform,Vec4(-1, 0, 0)).unitary();

    glDisable(GL_LIGHTING);
    glColor3f(color.x(),color.y(),color.z());
    glPushMatrix();
    glMultMatrixf(transform->getMatrix());
    glBegin(GL_QUADS);

        glVertex3f(vertexs[0].x(),vertexs[0].y(),vertexs[0].z());
        glVertex3f(vertexs[3].x(),vertexs[3].y(),vertexs[3].z());
        glVertex3f(vertexs[2].x(),vertexs[2].y(),vertexs[2].z());
        glVertex3f(vertexs[1].x(),vertexs[1].y(),vertexs[1].z());


        glVertex3f(vertexs[4].x(),vertexs[4].y(),vertexs[4].z());
        glVertex3f(vertexs[5].x(),vertexs[5].y(),vertexs[5].z());
        glVertex3f(vertexs[6].x(),vertexs[6].y(),vertexs[6].z());
        glVertex3f(vertexs[7].x(),vertexs[7].y(),vertexs[7].z());


        glVertex3f(vertexs[1].x(),vertexs[1].y(),vertexs[1].z());
        glVertex3f(vertexs[2].x(),vertexs[2].y(),vertexs[2].z());
        glVertex3f(vertexs[6].x(),vertexs[6].y(),vertexs[6].z());
        glVertex3f(vertexs[5].x(),vertexs[5].y(),vertexs[5].z());


        glVertex3f(vertexs[2].x(),vertexs[2].y(),vertexs[2].z());
        glVertex3f(vertexs[3].x(),vertexs[3].y(),vertexs[3].z());
        glVertex3f(vertexs[7].x(),vertexs[7].y(),vertexs[7].z());
        glVertex3f(vertexs[6].x(),vertexs[6].y(),vertexs[6].z());


        glVertex3f(vertexs[4].x(),vertexs[4].y(),vertexs[4].z());
        glVertex3f(vertexs[0].x(),vertexs[0].y(),vertexs[0].z());
        glVertex3f(vertexs[1].x(),vertexs[1].y(),vertexs[1].z());
        glVertex3f(vertexs[5].x(),vertexs[5].y(),vertexs[5].z());


        glVertex3f(vertexs[0].x(),vertexs[0].y(),vertexs[0].z());
        glVertex3f(vertexs[4].x(),vertexs[4].y(),vertexs[4].z());
        glVertex3f(vertexs[7].x(),vertexs[7].y(),vertexs[7].z());
        glVertex3f(vertexs[3].x(),vertexs[3].y(),vertexs[3].z());
    glEnd();
    glPopMatrix();
    glEnable(GL_LIGHTING);
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

}

void Draw::drawSelection(Vec4 p,float )
{
    Vec4 vertexs[8];
    vertexs[0] = (Vec4(-p.x()/2,-p.y()/2,-p.z()/2));
    vertexs[1] = (Vec4(-p.x()/2,-p.y()/2, p.z()/2));
    vertexs[2] = (Vec4( p.x()/2,-p.y()/2, p.z()/2));
    vertexs[3] = (Vec4( p.x()/2,-p.y()/2,-p.z()/2));
    vertexs[4] = (Vec4(-p.x()/2, p.y()/2,-p.z()/2));
    vertexs[5] = (Vec4(-p.x()/2, p.y()/2, p.z()/2));
    vertexs[6] = (Vec4( p.x()/2, p.y()/2, p.z()/2));
    vertexs[7] = (Vec4( p.x()/2, p.y()/2,-p.z()/2));

    float less = fmin(fmin(p.x(),p.y()),p.z())*0.3;
    //glColor3f(1,1,1);

    glDisable(GL_LIGHTING);
    glPushMatrix();
    glColor3f(1,0,0);
    glBegin(GL_LINES);
        //canto maximo
        glVertex3f(vertexs[6].x(),vertexs[6].y(),vertexs[6].z());
        glVertex3f(vertexs[6].x(),vertexs[6].y(),vertexs[6].z()-less);

        glVertex3f(vertexs[6].x(),vertexs[6].y(),vertexs[6].z());
        glVertex3f(vertexs[6].x()-less,vertexs[6].y(),vertexs[6].z());

        glVertex3f(vertexs[6].x(),vertexs[6].y(),vertexs[6].z());
        glVertex3f(vertexs[6].x(),vertexs[6].y()-less,vertexs[6].z());
        //canto minimo
        glVertex3f(vertexs[0].x(),vertexs[0].y(),vertexs[0].z());
        glVertex3f(vertexs[0].x(),vertexs[0].y(),vertexs[0].z()+less);

        glVertex3f(vertexs[0].x(),vertexs[0].y(),vertexs[0].z());
        glVertex3f(vertexs[0].x(),vertexs[0].y()+less,vertexs[0].z());

        glVertex3f(vertexs[0].x(),vertexs[0].y(),vertexs[0].z());
        glVertex3f(vertexs[0].x()+less,vertexs[0].y(),vertexs[0].z());

        //canto baixo
        glVertex3f(vertexs[6].x(),vertexs[0].y(),vertexs[0].z());
        glVertex3f(vertexs[6].x(),vertexs[0].y()+less,vertexs[0].z());

        glVertex3f(vertexs[6].x(),vertexs[0].y(),vertexs[0].z());
        glVertex3f(vertexs[6].x(),vertexs[0].y(),vertexs[0].z()+less);

        glVertex3f(vertexs[6].x(),vertexs[0].y(),vertexs[0].z());
        glVertex3f(vertexs[6].x()-less,vertexs[0].y(),vertexs[0].z());

        //canto baixo
        glVertex3f(vertexs[6].x(),vertexs[0].y(),vertexs[6].z());
        glVertex3f(vertexs[6].x(),vertexs[0].y()+less,vertexs[6].z());

        glVertex3f(vertexs[6].x(),vertexs[0].y(),vertexs[6].z());
        glVertex3f(vertexs[6].x(),vertexs[0].y(),vertexs[6].z()-less);

        glVertex3f(vertexs[6].x(),vertexs[0].y(),vertexs[6].z());
        glVertex3f(vertexs[6].x()-less,vertexs[0].y(),vertexs[6].z());

        //canto baixo
        glVertex3f(vertexs[0].x(),vertexs[0].y(),vertexs[6].z());
        glVertex3f(vertexs[0].x(),vertexs[0].y(),vertexs[6].z()-less);

        glVertex3f(vertexs[0].x(),vertexs[0].y(),vertexs[6].z());
        glVertex3f(vertexs[0].x(),vertexs[0].y()+less,vertexs[6].z());

        glVertex3f(vertexs[0].x(),vertexs[0].y(),vertexs[6].z());
        glVertex3f(vertexs[0].x()+less,vertexs[0].y(),vertexs[6].z());

        //canto cima
        glVertex3f(vertexs[6].x(),vertexs[6].y(),vertexs[0].z());
        glVertex3f(vertexs[6].x(),vertexs[6].y()-less,vertexs[0].z());

        glVertex3f(vertexs[6].x(),vertexs[6].y(),vertexs[0].z());
        glVertex3f(vertexs[6].x()-less,vertexs[6].y(),vertexs[0].z());

        glVertex3f(vertexs[6].x(),vertexs[6].y(),vertexs[0].z());
        glVertex3f(vertexs[6].x(),vertexs[6].y(),vertexs[0].z()+less);

        //canto cima
        glVertex3f(vertexs[0].x(),vertexs[6].y(),vertexs[6].z());
        glVertex3f(vertexs[0].x(),vertexs[6].y(),vertexs[6].z()-less);

        glVertex3f(vertexs[0].x(),vertexs[6].y(),vertexs[6].z());
        glVertex3f(vertexs[0].x(),vertexs[6].y()-less,vertexs[6].z());

        glVertex3f(vertexs[0].x(),vertexs[6].y(),vertexs[6].z());
        glVertex3f(vertexs[0].x()+less,vertexs[6].y(),vertexs[6].z());

        //canto cima
        glVertex3f(vertexs[0].x(),vertexs[6].y(),vertexs[0].z());
        glVertex3f(vertexs[0].x()+less,vertexs[6].y(),vertexs[0].z());

        glVertex3f(vertexs[0].x(),vertexs[6].y(),vertexs[0].z());
        glVertex3f(vertexs[0].x(),vertexs[6].y(),vertexs[0].z()+less);

        glVertex3f(vertexs[0].x(),vertexs[6].y(),vertexs[0].z());
        glVertex3f(vertexs[0].x(),vertexs[6].y()-less,vertexs[0].z());

    glEnd();
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

void Draw::drawArrow(Vec4 origin, Vec4 direction, float size,int material)
{
    GLUquadricObj *quad = gluNewQuadric();
    float larg = size;
    Vec4 from = origin;
    Vec4 to = direction*size+origin;
    Vec4 from2to = to-from;
    float tam = from2to.module();
    from2to.normalize();
    Material *mat = new Material();
    mat->setMaterial(mat,material);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,mat->ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat->diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat->specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat->shininess*128);
    glPushMatrix();
    //transformacao
    Vec4 k = Vec4( 0.0,0.0,1.0 );
    Vec4 axis = k^from2to;
    float angle = acos(k*from2to);
    Quaternion quat;
    quat.fromAxisAngle(axis,angle*180/M_PI);
    if (from2to.z()==-1.0) quat.fromAxisAngle( Vec4(1.0,0.0,0.0), 180.0 );
    dQuaternion dQ;
    to_dQuaternion(quat,dQ);
    dMatrix3 R;
    dQtoR(dQ,R);
    dVector3 pos;
    pos[0] = from.x(); pos[1] = from.y(); pos[2] = from.z();
    setTransformODE(pos,R);
    //desenha seta
    if (larg == 0.0) {
        gluClosedCylinder(quad, 0.03*tam, 0.03*tam, tam, 10, 10);
        glTranslated(0,0,tam);
        gluClosedCylinder(quad, 0.1*tam, 0.0, 0.2*tam, 10, 10);
    } else {
        gluClosedCylinder(quad, 0.03*2.0*larg, 0.03*2.0*larg, tam, 10, 10);
        glTranslated(0,0,tam);
        gluClosedCylinder(quad, 0.1*larg, 0.0, 0.2*larg, 10, 10);
    }
    glPopMatrix();

    gluDeleteQuadric( quad );
    delete mat;
}

void Draw::gluClosedCylinder(GLUquadric* quad, GLdouble base, GLdouble top, GLdouble height, GLint slices, GLint stacks)
{
    gluQuadricNormals(quad, GLU_SMOOTH);			// Create Smooth Normals
    gluQuadricTexture(quad, GL_TRUE);		  		// Create Texture Coords

    glPushMatrix();
    gluCylinder(quad, base, top, height, slices, stacks);
    glTranslated(0,0,height);
    gluDisk(quad, 0, top, slices, stacks);
    glRotated(180,0,1,0);
    glTranslated(0,0,height);
    gluDisk(quad, 0, base, slices, stacks);
    glPopMatrix();
}

void Draw::setTransformODE(const dReal *pos, const dReal *R)
{
    GLfloat matrix[16];
      matrix[0]=R[0];
      matrix[1]=R[4];
      matrix[2]=R[8];
      matrix[3]=0;
      matrix[4]=R[1];
      matrix[5]=R[5];
      matrix[6]=R[9];
      matrix[7]=0;
      matrix[8]=R[2];
      matrix[9]=R[6];
      matrix[10]=R[10];
      matrix[11]=0;
      matrix[12]=pos[0];
      matrix[13]=pos[1];
      matrix[14]=pos[2];
      matrix[15]=1;

    glMultMatrixf (matrix);
}
