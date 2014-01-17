#include "draw.h"
#include "math/matrix4x4.h"
#include "extra/material.h"
#include "GL/glu.h"
#include "GL/glut.h"
#include "math/quaternion.h"
#include "physics/physics.h"

#include "imageloader.h"

bool idraw_ground = false;
int idrawGround;
int idrawGround2;
int slices = 50;
int stacks = 50;
GLuint _textureId; //The id of the textur

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
    delete transform;


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

    delete q;

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
    Material *mat = new Material();
        Material::setMaterial(mat,MATERIAL_WHITE_PLASTIC);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,mat->ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat->diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat->specular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat->shininess*128);


    GLUquadric *quad =gluNewQuadric();

    //Image* image = loadBMP("../texture/checker2.bmp");
    if(yes)
    _textureId = loadTexture(loadBMP("../texture/checker2.bmp"));
    yes = false;

    //delete image;


    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId);

    //Bottom
    glPushMatrix();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_CLAMP_TO_EDGE);

    glPopMatrix();
    glPushMatrix();
    //

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

void Draw::drawCOMProjected(Vec4 position, float size, Vec4 color)
{

    Material *mat = new Material();
        Material::setMaterial(mat,MATERIAL_WHITE_PLASTIC);
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
    glPushMatrix();
    //

    gluQuadricNormals(quad, GLU_SMOOTH);
    gluQuadricTexture(quad,1);
    glTranslatef(position.x(),0.005,position.z());
    glRotatef(90,1.0f,0.0f,0.0f);
    //gluQuadricDrawStyle(quad, GLU_FILL);
    gluClosedCylinder(quad,size,size,0.001,25,25);
    glPopMatrix();
    delete quad;
    delete mat;
    glDisable(GL_TEXTURE_2D);
}

void Draw::drawTargetProjected(Vec4 position, float size, Vec4 color)
{
    Material *mat = new Material();
        Material::setMaterial(mat,MATERIAL_YELLOW_PLASTIC);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,mat->ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat->diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat->specular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat->shininess*128);


    GLUquadric *quad =gluNewQuadric();


    //Bottom

    glPushMatrix();

    glTranslatef(position.x(),0.01,position.z());
    glRotatef(90,1.0f,0.0f,0.0f);
    gluClosedCylinder(quad,size,size,0.001,25,25);
    glPopMatrix();
    delete quad;
    delete mat;

}

void Draw::drawGround(int size)
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
    glEnable(GL_LIGHTING);
    glDisable(GL_LIGHTING);
    glLineWidth(1.0);
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
    Material *mat = new Material();
    mat->setMaterial(mat,MATERIAL_BRONZE);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,mat->ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat->diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat->specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat->shininess*128);

    glPushMatrix();
    glBegin(GL_QUADS);
    for(int i=-size;i<=size;i+=2){
        for(int j=-size;j<=size;j+=2){
            glNormal3f(0,1,0);
            glVertex3f(i,0,j);
            glVertex3f(i,0,j+2);
            glVertex3f(i+2,0,j+2);
            glVertex3f(i+2,0,j);
        }
    }
    glEnd();
    glPopMatrix();


   delete mat;

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

void Draw::drawObj(Vec4 position, int material,Quaternion q, QString file,ObjMesh *n)
{
    Material *mat = new Material();
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
    Material *mat = new Material();
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
