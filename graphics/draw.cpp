#include "draw.h"
#include "math/matrix4x4.h"
#include "extra/materialobj.h"
#include "GL/glu.h"
#include "GL/glut.h"
#include "math/quaternion.h"
#include "physics/physics.h"
#include "graphics/mesh.h"
#include "imageloader.h"
#include <cmath>

//#ifdef DEBUG_MODE
#include <iostream>
using namespace std;
//#endif

#define SEGMENTS 30
bool idraw_ground = false;
int idrawGround;
int idrawGround2;
int slices = 50;
int stacks = 50;
GLuint _textureId; //The id of the textur
#define SKYFRONT 0						// Give Front ID = 0
#define SKYBACK  1						// Give Back  ID = 1
#define SKYLEFT  2						// Give Left  ID = 2
#define SKYRIGHT 3						// Give Right ID = 3
#define SKYUP    4						// Give Up    ID = 4
#define SKYDOWN  5						// Give Down  ID = 5
GLuint SkyboxTexture[6];		// We need 6 textures for our Skybox
//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image* image) {
    GLuint textureId;
    glGenTextures(1, &textureId); //Make room for our texture
    glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
    //Map the image to the texture
    glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
                 0,                            //0 for now
                 GL_RGB,                       //Format OpenGL uses for image
                 image->width, image->height,  //Width and height
                 0,                            //The border of the image
                 GL_RGB, //GL_RGB, because pixels are stored in RGB format
                 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
                                   //as unsigned numbers
                 image->pixels);               //The actual pixel data
    return textureId; //Returns the id of the texture
}

Draw::Draw()
{
    Image* image = loadBMP("../texture/checker2.bmp");
    _textureId = loadTexture(image);


    delete image;
}

void Draw::drawCube(Matrix4x4 *transform,Vec4 p, MaterialObj *mat, float)
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
    delete transform;


}

void Draw::drawCube(Matrix4x4 *transform, Vec4 p, int material)
{

    MaterialObj *mat = new MaterialObj();
    MaterialObj::setMaterial(mat,material);
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

Mesh *Draw::getMeshCube(Matrix4x4 *transform, Vec4 p, Mesh *mesh)
{
    Vec4 vertexs[8];
    Vec4 normals[6];

    vertexs[0] = (Vec4(-p.x()/2,-p.y()/2,-p.z()/2));
    vertexs[0] = (transform->transpose()).vector(vertexs[0]);
    vertexs[1] = (Vec4(-p.x()/2,-p.y()/2, p.z()/2));
    vertexs[1] = transform->transpose().vector(vertexs[1]);
    vertexs[2] = (Vec4( p.x()/2,-p.y()/2, p.z()/2));
    vertexs[2] = transform->transpose().vector(vertexs[2]);
    vertexs[3] = (Vec4( p.x()/2,-p.y()/2,-p.z()/2));
    vertexs[3] = transform->transpose().vector(vertexs[3]);
    vertexs[4] = (Vec4(-p.x()/2, p.y()/2,-p.z()/2));
    vertexs[4] = transform->transpose().vector(vertexs[4]);
    vertexs[5] = (Vec4(-p.x()/2, p.y()/2, p.z()/2));
    vertexs[5] = transform->transpose().vector(vertexs[5]);
    vertexs[6] = (Vec4( p.x()/2, p.y()/2, p.z()/2));
    vertexs[6] = transform->transpose().vector(vertexs[6]);
    vertexs[7] = (Vec4( p.x()/2, p.y()/2,-p.z()/2));
    vertexs[7] = transform->transpose().vector(vertexs[7]);

    normals[0] = Vec4::crossProduct(vertexs[3]-vertexs[0],vertexs[2]-vertexs[3]).unitary(); //vertices:0,1,2,3
    normals[1] = Vec4::crossProduct(vertexs[4]-vertexs[7],vertexs[5]-vertexs[4]).unitary(); //vertices:4,5,6,7
    normals[2] = Vec4::crossProduct(vertexs[6]-vertexs[2],vertexs[5]-vertexs[6]).unitary(); //vertices:1,2,5,6
    normals[3] = Vec4::crossProduct(vertexs[7]-vertexs[3],vertexs[6]-vertexs[7]).unitary(); //vertices:2,3,6,7
    normals[4] = Vec4::crossProduct(vertexs[4]-vertexs[5],vertexs[0]-vertexs[4]).unitary(); //vertices:0,1,4,5
    normals[5] = Vec4::crossProduct(vertexs[4]-vertexs[0],vertexs[7]-vertexs[4]).unitary(); //vertices:0,3,4,7

    Face face[6];
    face[0].vertexs.push_back(&vertexs[0]);
    face[0].vertexs.push_back(&vertexs[3]);
    face[0].vertexs.push_back(&vertexs[2]);
    face[0].vertexs.push_back(&vertexs[1]);
    face[0].normals.push_back(&normals[0]);
    mesh->faces.push_back(face[0]);


    face[1].vertexs.push_back(&vertexs[4]);
    face[1].vertexs.push_back(&vertexs[5]);
    face[1].vertexs.push_back(&vertexs[6]);
    face[1].vertexs.push_back(&vertexs[7]);
    face[1].normals.push_back(&normals[1]);
    mesh->faces.push_back(face[1]);


    face[2].vertexs.push_back(&vertexs[1]);
    face[2].vertexs.push_back(&vertexs[2]);
    face[2].vertexs.push_back(&vertexs[6]);
    face[2].vertexs.push_back(&vertexs[5]);
    face[2].normals.push_back(&normals[2]);
    mesh->faces.push_back(face[2]);


    face[3].vertexs.push_back(&vertexs[2]);
    face[3].vertexs.push_back(&vertexs[6]);
    face[3].vertexs.push_back(&vertexs[5]);
    face[3].vertexs.push_back(&vertexs[2]);
    face[3].normals.push_back(&normals[3]);
    mesh->faces.push_back(face[3]);

    glNormal3f(normals[4].x(),normals[4].y(),normals[4].z());
    glVertex3f(vertexs[4].x(),vertexs[4].y(),vertexs[4].z());
    glVertex3f(vertexs[0].x(),vertexs[0].y(),vertexs[0].z());
    glVertex3f(vertexs[1].x(),vertexs[1].y(),vertexs[1].z());
    glVertex3f(vertexs[5].x(),vertexs[5].y(),vertexs[5].z());
    face[4].vertexs.push_back(&vertexs[4]);
    face[4].vertexs.push_back(&vertexs[0]);
    face[4].vertexs.push_back(&vertexs[1]);
    face[4].vertexs.push_back(&vertexs[5]);
    face[4].normals.push_back(&normals[4]);
    mesh->faces.push_back(face[4]);


    face[5].vertexs.push_back(&vertexs[0]);
    face[5].vertexs.push_back(&vertexs[4]);
    face[5].vertexs.push_back(&vertexs[7]);
    face[5].vertexs.push_back(&vertexs[3]);
    face[5].normals.push_back(&normals[5]);
    mesh->faces.push_back(face[5]);

    return mesh;
}

void Draw::drawCylinder(Matrix4x4 *transform, MaterialObj *mat)
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

void Draw::drawCylinder(Matrix4x4 *transform, int material)
{
    MaterialObj *mat = new MaterialObj();
    MaterialObj::setMaterial(mat,material);
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

void Draw::drawCylinder(Vec4 position, Vec4 axis, double radius, double height, int material)
{
    MaterialObj *mat = new MaterialObj();
    MaterialObj::setMaterial(mat,material);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,mat->ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat->diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat->specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat->shininess*128);

    //Pega o eixo em que o cilindro é desenhado, (0,0,1), o eixo em que se deseja que ele seja desenhado, e a projeção no plano xz
    //Todos normalizados
    Vec4 normalAxis = axis;
    normalAxis.normalize();
    Vec4 drawAxis(0,0,1);
    Vec4 normalAxisProj = normalAxis.projXZ();
    normalAxisProj.normalize();

    //Pega os ângulos pelos quais precisamos rotacionar para alinhar (0,0,1) ao eixo da junta
    double rotY = acos(drawAxis*normalAxisProj)*(180/M_PI);
    double rotZ = acos(normalAxis*normalAxisProj)*(180/M_PI);

    GLUquadric *q = gluNewQuadric();
    glPushMatrix();
        glTranslatef(position.x(),position.y(),position.z());
        glRotated(rotY,0.0,1.0,0.0);
        glRotated(rotZ,0.0,0.0,1.0);
        gluCylinder(q,radius,radius,height,10,10);
    glPopMatrix();
    gluDeleteQuadric(q);

}

void Draw::drawSphere(Matrix4x4 *transform,MaterialObj *mat,float radius)
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

    delete q;

}

void Draw::drawSphere(Vec4 position, int material, float size)
{
    MaterialObj *mat = new MaterialObj();
    MaterialObj::setMaterial(mat,material);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,mat->ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat->diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat->specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat->shininess*128);

    GLUquadric *q = gluNewQuadric();

    glPushMatrix();
        gluQuadricNormals(q, GLU_SMOOTH);
        glTranslatef(position.x(),position.y(),position.z());
        gluSphere(q, size, slices, stacks);

    glPopMatrix();

    delete q;
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

    MaterialObj *mat = new MaterialObj();
    MaterialObj::setMaterial(mat,MATERIAL_RED_PLASTIC);
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

    delete q;
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
bool yes = true;
void Draw::drawCOM(Vec4 position, float size, Vec4 color)
{
    //return;
    MaterialObj *mat = new MaterialObj();
        MaterialObj::setMaterial(mat,MATERIAL_WHITE_PLASTIC);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,mat->ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat->diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat->specular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat->shininess*128);


    GLUquadric *quad =gluNewQuadric();

    //Image* image = loadBMP("../texture/checker2.bmp");
    if(yes){
    _textureId = loadTexture(loadBMP("../texture/checker2.bmp"));
    yes = false;

    //delete image;




    //Bottom
    //glPushMatrix();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_CLAMP_TO_EDGE);

    //glPopMatrix();
    }
    glPushMatrix();
    //
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId);

    gluQuadricNormals(quad, GLU_SMOOTH);
    gluQuadricTexture(quad,1);
    glTranslatef(position.x(),position.y(),position.z());
    glRotatef(90,1.0f,0.0f,0.0f);
    gluSphere(quad, size, slices, stacks);
    glPopMatrix();
    delete quad;
    delete mat;

    glDisable(GL_TEXTURE_2D);
}

void Draw::drawCOMProjected(Vec4 position, float size, Vec4 color, Vec4 rot)
{

    MaterialObj *mat = new MaterialObj();
        MaterialObj::setMaterial(mat,MATERIAL_WHITE_PLASTIC);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,mat->ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat->diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat->specular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat->shininess*128);


    GLUquadric *quad =gluNewQuadric();



    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId);

    //Bottom
    glPushMatrix();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_CLAMP_TO_EDGE);

    glPopMatrix();


    //

    gluQuadricNormals(quad, GLU_SMOOTH);
    gluQuadricTexture(quad,1);

    glTranslatef(position.x(),position.y()+0.015,position.z());
    glRotated(rot.z(),0,0,1);
    glRotated(rot.y(),0,1,0);
    glRotated(90+rot.x(),1,0,0);
    //glRotatef(90,1.0f,0.0f,0.0f);
    //gluQuadricDrawStyle(quad, GLU_FILL);
    gluClosedCylinder(quad,size,size,0.001,25,25);

    glPopMatrix();
    delete quad;
    delete mat;
    glDisable(GL_TEXTURE_2D);
}

void Draw::drawTargetProjected(Vec4 position, float size, Vec4 color, Vec4 rot)
{
    MaterialObj *mat = new MaterialObj();
        MaterialObj::setMaterial(mat,MATERIAL_YELLOW_PLASTIC);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,mat->ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat->diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat->specular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat->shininess*128);


    GLUquadric *quad =gluNewQuadric();


    //Bottom

    glPushMatrix();


    glTranslatef(position.x(),position.y()+0.01,position.z());
    //glRotatef(90,1.0f,0.0f,0.0f);
    glRotated(rot.z(),0,0,1);
    glRotated(rot.y(),0,1,0);
    glRotated(90+rot.x(),1,0,0);
    gluClosedCylinder(quad,size,size,0.001,25,25);

    glPopMatrix();

    delete quad;
    delete mat;

}

void Draw::drawCylinderClosed(Vec4 position, Vec4 axis, double radius, double height, int material)
{
    MaterialObj *mat = new MaterialObj();

        MaterialObj::setMaterial(mat,material);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,mat->ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat->diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat->specular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat->shininess*128);


        //Pega o eixo em que o cilindro é desenhado, (0,0,1), o eixo em que se deseja que ele seja desenhado, e a projeção no plano xz
        //Todos normalizados
        Vec4 normalAxis = axis;
        normalAxis.normalize();
        Vec4 drawAxis(0,0,1);
        Vec4 normalAxisProj = normalAxis.projXZ();
        normalAxisProj.normalize();

        //Pega os ângulos pelos quais precisamos rotacionar para alinhar (0,0,1) ao eixo da junta
        double rotY = acos(drawAxis*normalAxisProj)*(180/M_PI);
        double rotZ = acos(normalAxis*normalAxisProj)*(180/M_PI);

        GLUquadric *q = gluNewQuadric();
        glPushMatrix();
            glTranslatef(position.x(),position.y(),position.z());
            glRotated(rotY,0.0,1.0,0.0);
            glRotated(rotZ,0.0,0.0,1.0);
    gluClosedCylinder(q,radius,radius,height,25,25);
    glPopMatrix();
    gluDeleteQuadric(q);
    delete mat;

}




void Draw::drawGround(int size, Vec4 rot, float reflect)
{

    /*** Anterior
    Material *mat = new Material();


    glPushMatrix();

    glBegin(GL_QUADS);
    bool color = true;
    for(int i=-size;i<=size;i+=2){
        if (abs(i)%4==0) color = true;
        else color = false;
        for(int j=-size;j<=size;j+=2){
            if (color){

                mat->setMaterial(mat,MATERIAL_SILVER_POLIERT);
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,mat->ambient);
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat->diffuse);
                glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat->specular);
                glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat->shininess*64);
            }
            else{

                mat->setMaterial(mat,MATERIAL_WHITE_PLASTIC);
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,mat->ambient);
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat->diffuse);
                glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat->specular);
                glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat->shininess*128);
            }
            glNormal3f(0,1,0);
            glVertex3f(i,0,j);
            glVertex3f(i,0,j+2);
            glVertex3f(i+2,0,j+2);
            glVertex3f(i+2,0,j);
            color = !color;
        }
    }
    glEnd();
    glPopMatrix();
    delete mat;
    //    glEndList();
//    if (!idraw_ground) idraw_ground = true;
***/
    //Novo
    glPushMatrix();
    glRotated(rot.z(),0,0,1);
    glRotated(rot.y(),0,1,0);
    glRotated(rot.x(),1,0,0);

    if(reflect){
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0,1.0,1.0, 0.5);
    }

    glDisable(GL_LIGHTING);
    glLineWidth(3.0);
    glColor3f(0,0,0);
    glBegin(GL_LINES);
    for(int i=-size;i<=size;i+=2){
        for(int j=-size;j<=size;j+=2){
            glVertex3f(i,0.000,j);
            glVertex3f(i,0.000,j+2);

            glVertex3f(i,0.000,j+2);
            glVertex3f(i+2,0.000,j+2);

            glVertex3f(i+2,0.000,j+2);
            glVertex3f(i+2,0.000,j);

            glVertex3f(i+2,0.000,j);
            glVertex3f(i,0.000,j);


        }
    }

    glEnd();
    //glEnable(GL_LIGHTING);
    //glDisable(GL_LIGHTING);
    glLineWidth(1.4);
    glBegin(GL_LINES);
    for(int i=-size;i<=size;i+=2){
        for(int j=-size;j<=size;j+=2){
            for (float k=i;k<i+2;k+=0.5)
                for(float l=j;l<j+2;l+=0.5){
                    glVertex3f(k,0.0001,l);
                    glVertex3f(k,0.0001,l+.5);

                    glVertex3f(k,0.0001,l+.5);
                    glVertex3f(k+.5,0.0001,l+.5);

                    glVertex3f(k+.5,0.0001,l+.5);
                    glVertex3f(k+.5,0.0001,l);

                    glVertex3f(k+.5,0.0001,l);
                    glVertex3f(k,0.0001,l);

                }

        }
    }

    glEnd();
    glEnable(GL_LIGHTING);
    MaterialObj *mat = new MaterialObj();
    if(reflect){
        mat->setMaterial(mat,MATERIAL_ICE);
    }else{
        mat->setMaterial(mat,MATERIAL_BRASS);
    }
    //const GLfloat whiteTranspMaterial[]={1.0,1.0,1.0,0.5};

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,mat->ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat->diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat->specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat->shininess*128);
    //glEnable(GL_TEXTURE_2D);

//     glVertex3f(-dim,0.0,+dim);
//             glVertex3f(+dim,0.0,+dim);
//             glVertex3f(+dim,0.0,-dim);
//            ; glVertex3f(-dim,0.0,-dim);
    //glPushMatrix();
    glBegin(GL_QUADS);
    for(int i=-size;i<=size;i+=2){
        for(int j=-size;j<=size;j+=2){
            glNormal3f(0,1,0);
            glTexCoord2d(0.0,0.0); glVertex3f(i,0,j);
            glTexCoord2d(10.0,0.0); glVertex3f(i,0,j+2);
            glTexCoord2d(10.0,10.0); glVertex3f(i+2,0,j+2);
            glTexCoord2d(0.0,10.0); glVertex3f(i+2,0,j);
        }
    }
    glEnd();
    //glPopMatrix();

    glPopMatrix();

    //glDisable(GL_TEXTURE_2D);


    delete mat;

}

bool loadedtext = true;
GLuint texName;
Image *texture;
void Draw::drawGroundTexture(int size, int text)
{
     glPushMatrix();
    if (loadedtext){

    QString end = "../texture/wood2.bmp";

    //const char *name = end.toLatin1().constData();
    //Image *texture = new Image(&name,100,100);
    texture = loadBMP(end.toStdString().data());

    glGenTextures(1, &texName);
    glBindTexture(GL_TEXTURE_2D, texName);

    //glBindTexture( GL_TEXTURE_2D, textName );

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glActiveTexture( GL_TEXTURE0 );
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->width,
                texture->height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                texture->pixels);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexImage2D(GL_TEXTURE_2D, 0, texture_c, texture_w, texture_h, 0, GL_RGB, GL_UNSIGNED_BYTE,
    //              texture);

    //glGenerateMipmap(texName);
    loadedtext = false;
    }
    //texture = loadBMP("../texture/wood.bmp");
//    if(loadedtext)
//        texName = loadTexture(loadBMP("../texture/wood.bmp"));
//    loadedtext = false;
    //this->loadedtext = true;

//    glDisable(GL_LIGHTING);
//    glLineWidth(3.0);
//    glColor3f(0,0,0);
//    glBegin(GL_LINES);
//    for(int i=-size;i<=size;i+=2){
//        for(int j=-size;j<=size;j+=2){
//            glVertex3f(i,0.000,j);
//            glVertex3f(i,0.000,j+2);

//            glVertex3f(i,0.000,j+2);
//            glVertex3f(i+2,0.000,j+2);

//            glVertex3f(i+2,0.000,j+2);
//            glVertex3f(i+2,0.000,j);

//            glVertex3f(i+2,0.000,j);
//            glVertex3f(i,0.000,j);


//        }
//    }

//    glEnd();
//    glEnable(GL_LIGHTING);
//    glDisable(GL_LIGHTING);
//    glLineWidth(1.0);
//    glBegin(GL_LINES);
//    for(int i=-size;i<=size;i+=2){
//        for(int j=-size;j<=size;j+=2){
//            for (float k=i;k<i+2;k+=0.5)
//                for(float l=j;l<j+2;l+=0.5){
//                    glVertex3f(k,0.0001,l);
//                    glVertex3f(k,0.0001,l+.5);

//                    glVertex3f(k,0.0001,l+.5);
//                    glVertex3f(k+.5,0.0001,l+.5);

//                    glVertex3f(k+.5,0.0001,l+.5);
//                    glVertex3f(k+.5,0.0001,l);

//                    glVertex3f(k+.5,0.0001,l);
//                    glVertex3f(k,0.0001,l);

//                }

//        }
//    }

//    glEnd();
    glEnable(GL_LIGHTING);
    MaterialObj *mat = new MaterialObj();
    mat->setMaterial(mat,MATERIAL_WHITE_PLASTIC);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,mat->ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat->diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat->specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat->shininess*128);

    glPushMatrix();


    glEnable(GL_TEXTURE_2D);
    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D, texName);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glBegin(GL_QUADS);
    for(int i=-size;i<=size;i+=2){
        for(int j=-size;j<=size;j+=2){
            glNormal3f(0,1,0);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(i,0,j);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(i,0,j+2);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(i+2,0,j+2);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(i+2,0,j);
        }
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    glPopMatrix();


    delete mat;
    //delete texture;
    //delete texture;

}

void Draw::drawCircle2D(Vec4 center, float radius, Vec4 color, float size, Vec4 rot)
{
    glPushMatrix();
    glRotated(rot.z(),0,0,1);
    glRotated(rot.y(),0,1,0);
    glRotated(rot.x(),1,0,0);
    Vec4 vertexs[SEGMENTS];
    float alpha = 2*M_PI / SEGMENTS;
    for (int i = 0;i<SEGMENTS;i++){
            vertexs[i].setVec4(cos(alpha*i)*radius+center.x(),center.y(),sin(alpha*i)*radius+center.z());
    }
    glLineWidth(size);
    glDisable(GL_LIGHTING);
    glColor3f(color.x(),color.y(),color.z());
    glBegin(GL_LINE_STRIP);
    for (int i = 0;i<SEGMENTS;i++){
        glVertex3f(vertexs[i].x(),vertexs[i].y()+0.01,vertexs[i].z());
    }
    glVertex3f(vertexs[0].x(),vertexs[0].y()+0.01,vertexs[0].z());
    glEnd();
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

bool sky = true;

void Draw::drawSkybox(Vec4 min, Vec4 max, int texture)
{
    // Center the Skybox around the given x,y,z position
    if(sky){
    BMP_Texture(SkyboxTexture,QString("../texture/skybox/Citadella2/front.bmp"),  SKYFRONT);
    BMP_Texture(SkyboxTexture,QString("../texture/skybox/Citadella2/back.bmp"),   SKYBACK);
    BMP_Texture(SkyboxTexture,QString("../texture/skybox/Citadella2/left.bmp"),   SKYLEFT);
    BMP_Texture(SkyboxTexture,QString("../texture/skybox/Citadella2/right.bmp"),  SKYRIGHT);
    BMP_Texture(SkyboxTexture,QString("../texture/skybox/Citadella2/up.bmp"),     SKYUP);
    BMP_Texture(SkyboxTexture,QString("../texture/skybox/Citadella2/down.bmp"),   SKYDOWN);
    }
    sky = false;


    float x = min.x(),y=min.y(),z=min.z();
    float width=max.x(), height=max.y(), length=max.z();
    x = x - width  / 2.;
    y = y - height / 2;
    z = z - length / 2;

    glEnable(GL_LIGHTING);
    MaterialObj *mat = new MaterialObj();
    mat->setMaterial(mat,MATERIAL_WHITE_PLASTIC);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,mat->ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat->diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat->specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat->shininess*128);
        // Draw Front side
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SKYFRONT]);
//        glBegin(GL_QUADS);
//            glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z+length);
//            glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height, z+length);
//            glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y+height, z+length);
//            glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y,		z+length);
//        glEnd();

        // Draw Back side
        glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SKYBACK]);
        glBegin(GL_QUADS);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y,		z);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+height, z);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z);
        glEnd();

        // Draw Left side
        glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SKYLEFT]);
        glBegin(GL_QUADS);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height,	z);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z+length);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z+length);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z);
        glEnd();

        // Draw Right side
        glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SKYRIGHT]);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y,		z);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y,		z+length);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+height,	z+length);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y+height,	z);
        glEnd();

        // Draw Up side
        glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SKYUP]);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y+height, z);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y+height, z+length);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height,	z+length);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z);
        glEnd();

        // Draw Down side
        glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SKYDOWN]);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z+length);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y,		z+length);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y,		z);
        glEnd();
        delete mat;
}

void Draw::drawCoffeeCup(Vec4 position, int material, QuaternionQ q)
{
    MaterialObj *mat = new MaterialObj();
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

void Draw::drawObj(Vec4 position, int material,QuaternionQ q, QString file,ObjMesh *n)
{
    MaterialObj *mat = new MaterialObj();
    mat->setMaterial(mat,material);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,mat->ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat->diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat->specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat->shininess*128);

    Matrix4x4 m = q.getMatrix();
    glPushMatrix();
    glTranslatef(position.x(),position.y(),position.z());
    glMultMatrixf(m.matrix);
    n->draw();
    glPopMatrix();
    delete mat;
}

void Draw::drawObj(Matrix4x4 *transform, int material, ObjMesh *n)
{
    MaterialObj *mat = new MaterialObj();
    mat->setMaterial(mat,material);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,mat->ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat->diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat->specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat->shininess*128);
    glPushMatrix();
    glMultMatrixf(transform->getMatrix());
    n->draw();
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
    delete transform;
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
    delete transform;
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
    glLineWidth(0.5);
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
    glPushMatrix();
    GLUquadricObj *quad = gluNewQuadric();
    float larg = size;
    Vec4 from = origin;
    Vec4 to = direction*size+origin;
    Vec4 from2to = to-from;
    float tam = from2to.module();
    from2to.normalize();
    MaterialObj *mat = new MaterialObj();
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
    QuaternionQ quat;
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

    glPopMatrix();
    gluDeleteQuadric( quad );
    delete mat;

}

void Draw::drawArrow3D(Vec4 origin, Vec4 velocity, Vec4 dir, float size,int material, Vec4 ground_inclination) //origen COM
{
    Vec4 direction = Vec4(0,0,1);
    Vec4 begin = Vec4(0,0,0.0);
    if(velocity.module()!=0 && velocity*direction<0){
        direction = Vec4(0,0,-1);
        begin = Vec4(0,0,-0.0);
    }

    QuaternionQ qat(dir);
    direction = qat.getMatrix().vector(direction);

    glPushMatrix();
    GLUquadricObj *quad = gluNewQuadric();
    float larg = size;
    Vec4 from = origin+begin;
    Vec4 to = direction*size+origin+begin;
    Vec4 from2to = to-from;
    float tam = from2to.module();
    from2to.normalize();
    MaterialObj *mat = new MaterialObj();
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
    QuaternionQ quat;
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

    glPopMatrix();
    gluDeleteQuadric( quad );
    delete mat;

}


void Draw::drawArrow2D(float angle, Vec4 anchor)
{
    Vec4 pos = anchor;
    float x,z;
    //c(0,1)
    float c1,c2;
    c1 = 0;
    c2 = 0.5;
    x = c1*cos(angle*M_PI/180.) + c2*sin(angle*M_PI/180.);
    z = c2*cos(angle*M_PI/180.) - c1*sin(angle*M_PI/180.);

    float x1,x2,x3,x4,x5,x6,x7;
    float z1,z2,z3,z4,z5,z6,z7;
    c1 = 0.15;
    c2 = 0;
    x1 = c1*cos(angle*M_PI/180.) + c2*sin(angle*M_PI/180.);
    z1 = c2*cos(angle*M_PI/180.) - c1*sin(angle*M_PI/180.);
    c1 = -.15;
    c2 = 0;
    x2 = c1*cos(angle*M_PI/180.) + c2*sin(angle*M_PI/180.);
    z2 = c2*cos(angle*M_PI/180.) - c1*sin(angle*M_PI/180.);
    c1 = -.15;
    c2 = .7;
    x3 = c1*cos(angle*M_PI/180.) + c2*sin(angle*M_PI/180.);
    z3 = c2*cos(angle*M_PI/180.) - c1*sin(angle*M_PI/180.);
    c1 = .15;
    c2 = .7;
    x4 = c1*cos(angle*M_PI/180.) + c2*sin(angle*M_PI/180.);
    z4 = c2*cos(angle*M_PI/180.) - c1*sin(angle*M_PI/180.);
    c1 = .0;
    c2 = 1.1;
    x5 = c1*cos(angle*M_PI/180.) + c2*sin(angle*M_PI/180.);
    z5 = c2*cos(angle*M_PI/180.) - c1*sin(angle*M_PI/180.);
    c1 = .25;
    c2 = .7;
    x6 = c1*cos(angle*M_PI/180.) + c2*sin(angle*M_PI/180.);
    z6 = c2*cos(angle*M_PI/180.) - c1*sin(angle*M_PI/180.);
    c1 = -.25;
    c2 = .7;
    x7 = c1*cos(angle*M_PI/180.) + c2*sin(angle*M_PI/180.);
    z7 = c2*cos(angle*M_PI/180.) - c1*sin(angle*M_PI/180.);

//    z´ = z cos B - x sen B
//    x´ = x cos B + z sen B

    glPushMatrix();
    glDisable(GL_LIGHTING);
    glTranslatef(0,0.05,0);
    glColor3f(0,0.3,0);
    glBegin(GL_QUADS);
        glNormal3f(0,1,0);
        glVertex3f(pos.x()+x+x1,0,pos.z()+z+z1);
        glVertex3f(pos.x()+x+x2,0,pos.z()+z+z2);
        glVertex3f(pos.x()+x+x3,0,pos.z()+z+z3);
        glVertex3f(pos.x()+x+x4,0,pos.z()+z+z4);

    glEnd();
    glBegin(GL_TRIANGLES);
    glNormal3f(0,1,0);
    glVertex3f(pos.x()+x+x5,0,pos.z()+z+z5);
    glVertex3f(pos.x()+x+x6,0,pos.z()+z+z6);
    glVertex3f(pos.x()+x+x7,0,pos.z()+z+z7);

    glEnd();
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

void Draw::drawAxisCameraView(float tam)
{
      const GLfloat redMaterial[]={1.,0.,0.,1.};
      const GLfloat greenMaterial[]={0.,1.,0.,1.};
      const GLfloat blueMaterial[]={0.,0.,1.,1.};
      const GLfloat blackMaterial[]={0.,0.,0.,1.};

      glMaterialfv(GL_FRONT, GL_AMBIENT, blackMaterial);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, blackMaterial);
      glMaterialfv(GL_FRONT, GL_SPECULAR, blackMaterial);

      GLUquadricObj *quad = gluNewQuadric();

      glLineWidth( 2 ) ;

      //x
      glMaterialfv(GL_FRONT, GL_AMBIENT, redMaterial);

      glBegin(GL_LINES);
        glVertex3f(0,0,0);
        glVertex3f(tam,0,0);
      glEnd();
      glPushMatrix();
        glRotated(90,0,1,0);
        glTranslated(0,0,tam);
        Draw::gluClosedCylinder(quad, 0.1*tam, 0, 0.2*tam, 10, 10);
      glPopMatrix();
      //y
      glMaterialfv(GL_FRONT, GL_AMBIENT, greenMaterial);

      glBegin(GL_LINES);
        glVertex3f(0,0,0);
        glVertex3f(0,tam,0);
      glEnd();
      glPushMatrix();
        glRotated(90,-1,0,0);
        glTranslated(0,0,tam);
        Draw::gluClosedCylinder(quad, 0.1*tam, 0, 0.2*tam, 10, 10);
      glPopMatrix();
      //z
      glMaterialfv(GL_FRONT, GL_AMBIENT, blueMaterial);

      glBegin(GL_LINES);
        glVertex3f(0,0,0);
        glVertex3f(0,0,tam);
      glEnd();
      glPushMatrix();
        glTranslated(0,0,tam);
        Draw::gluClosedCylinder(quad, 0.1*tam, 0, 0.2*tam, 10, 10);
      glPopMatrix();

      glLineWidth( 1 ) ;

      gluDeleteQuadric( quad );
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

void Draw::shadowMatrix( GLfloat shadowMat[4][4], GLfloat groundplane[4], GLfloat lightpos[4] )
{

      GLfloat dot;

      // Find dot product between light position vector and ground plane normal
      dot = groundplane[0] * lightpos[0] +
        groundplane[1] * lightpos[1] +
        groundplane[2] * lightpos[2] +
        groundplane[3] * lightpos[3];

      shadowMat[0][0] = dot - lightpos[0] * groundplane[0];
      shadowMat[1][0] = 0.f - lightpos[0] * groundplane[1];
      shadowMat[2][0] = 0.f - lightpos[0] * groundplane[2];
      shadowMat[3][0] = 0.f - lightpos[0] * groundplane[3];

      shadowMat[0][1] = 0.f - lightpos[1] * groundplane[0];
      shadowMat[1][1] = dot - lightpos[1] * groundplane[1];
      shadowMat[2][1] = 0.f - lightpos[1] * groundplane[2];
      shadowMat[3][1] = 0.f - lightpos[1] * groundplane[3];

      shadowMat[0][2] = 0.f - lightpos[2] * groundplane[0];
      shadowMat[1][2] = 0.f - lightpos[2] * groundplane[1];
      shadowMat[2][2] = dot - lightpos[2] * groundplane[2];
      shadowMat[3][2] = 0.f - lightpos[2] * groundplane[3];

      shadowMat[0][3] = 0.f - lightpos[3] * groundplane[0];
      shadowMat[1][3] = 0.f - lightpos[3] * groundplane[1];
      shadowMat[2][3] = 0.f - lightpos[3] * groundplane[2];
      shadowMat[3][3] = dot - lightpos[3] * groundplane[3];

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

void Draw::drawText(QString text, int x, int y)
{

    glDisable(GL_LIGHTING);
    glColor3f(1,1,1);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, 800, 0.0, 600);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glRasterPos2f(x, y);

    int k = 0;
    while (k<text.size()){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,text.at(k).toLatin1());
        k++;
    }

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glEnable(GL_LIGHTING);
}
