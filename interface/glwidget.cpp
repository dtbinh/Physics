#include "glwidget.h"
#include "scene/object.h"
#include "graphics/draw.h"
#include "math/quaternion.h"
#include "extra/utils.h"
#include <stdio.h>
#include <sys/time.h>
#include <stdio.h>
#include <GL/gl.h>                        // Header File For The OpenGL32 Library
#include <GL/freeglut.h>                        // Header File For The OpenGL32 Library
#include <GL/glu.h>                       // Header File For The GLu32 Library
#include <GL/glut.h>
#include <GL/glext.h>                     // Header File For The GLaux Library
#include "camera.h"
#include "interpolation/interpolation.h"
#include "extra/screenshot.h"
#include "extra/text.h"
#include <QPainter>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "math/vector3d.h"
//  To use functions with variables arguments
#include <stdarg.h>

//  for malloc
#include <stdlib.h>

#include <cmath>

bool enable_balance=true;
vector3d lightPosition(-1.93849,11.233,21.9049);
vector3d lightDirection(-26.4,355.2);


#define SHOTBALL 1

int mode = -1;
Object *objselshot = NULL;

int manipulate = 0;
float densidade = 100.;
int numOffBalls = 0;
//melhor configuração ciclica para caminhar: 398 582
/************** New Camera *****************/
static int scr_capture_count = 0;
static bool lbpressed = false;
//static bool mbpressed = false;
static bool rbpressed = false;

static float last_x = 0.0;
static float last_y = 0.0;
#define SET_CAM 0
#define SET_OBJECT 1
Camera* cam = new Camera();
static float savedCamera[9];
//variáveis trackball

int 	winWidth, winHeight;
//
int time_current;
int time_m;


void DesenhaTexto(void *font, char *string)
{
    // Exibe caractere a caractere
    while(*string)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15,*string++);
}

void DesenhaTextoStroke(void *font, char *string)
{
    // Exibe caractere a caractere
    while(*string)
        glutStrokeCharacter(GLUT_STROKE_ROMAN,*string++);
}
//  The number of frames
int frameCount = 0;

//  Number of frames per second
float fps = 0;

//  currentTime - previousTime is the time elapsed
//  between every call of the Idle function
int currentTime = 0, previousTime = 0;
void calculateFPS()
{
    //  Increase frame count
    frameCount++;

    //  Get the number of milliseconds since glutInit called
    //  (or first call to glutGet(GLUT ELAPSED TIME)).
    currentTime = glutGet(GLUT_ELAPSED_TIME);

    //  Calculate time passed
    int timeInterval = currentTime - previousTime;

    if(timeInterval > 1000)
    {
        //  calculate the number of frames per second
        fps = frameCount / (timeInterval / 1000.0f);

        //  Set time
        previousTime = currentTime;

        //  Reset frame count
        frameCount = 0;
    }
}

void GLWidget::drawFPS()
{

    glDisable(GL_LIGHTING);
    glColor3f(1,1,1);
    glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      glLoadIdentity();
      gluOrtho2D(0.0, winWidth, 0.0, winHeight);
      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();
      glLoadIdentity();
      glRasterPos2i(0, 690);

          QString out = QString().sprintf("\nFPS: %4.2f", FPS);

          char* n = (char*)out.toStdString().data();
          while (*n!='\0'){
              glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*n++);
          }

      glMatrixMode(GL_MODELVIEW);
      glPopMatrix();
      glMatrixMode(GL_PROJECTION);
      glPopMatrix();
      glEnable(GL_LIGHTING);


}

/************** Fim Camera *****************/

int count = 0;
QuaternionQ q;
//bool move = false;
float angle = 45;
int last_pox_x,last_pox_y;
int id_material;
int state_key;
//Vec4 eye,at;
Vec4 cam_eye = Vec4(2,2,2);
Vec4 cam_at  = Vec4(0,1,0);
int ciclo = 0;
int ciclo_arrow = 0;
int angle_quat = 0;
int angle_direction = 0;
bool shadow = false;
//variaveis de camera
//int width,height;




float 	angletrack = 0.0, axis[3], trans[3];
bool 	trackingMouse  = false;
bool 	redrawContinue = false;
bool    trackballMove  = false;



/*----------------------------------------------------------------------*/
/*
** These functions implement a simple trackball-like motion control.
*/

float lastPos[3] = {0.0, 0.0, 0.0};
int curx, cury;
int startX, startY;

void trackball_ptov(int x, int y, int width, int height, float v[3])
{
    float d, a;

    /* project x, y onto a hemi-sphere centered within width, height */
    //
    //	+-----------------------+	+
    //	|			|			|	|
    //	|			|	+		|	|
    //	|			|			|	|
    //	|-----------------------|	|	height
    //	|			|			|	|
    //	|			|			|	|
    //	|			|			|	|
    //	|			|			|	|
    //	+-----------------------+	+
    //
    //	+------width------------+
    //
    //  v[0] = (x - w/2)/(w/2)			in the range [-1, 1]
    //  v[1] = ((h -y) - h/2)/(h/2)		in the range [-1, 1]

    v[0] = (2.0*x  - width) / width;
    v[1] = (height - 2.0*y) / height;

    d = (float) sqrt(v[0]*v[0] + v[1]*v[1]);

    if (d < sqrt(2.0))
    {
        v[2] = sqrt(2.0 - d*d);
    }
    else
    {
        v[2] = 0.0;
    }

    //v[2] = (float) cos((M_PI/2.0) * ((d < 1.0) ? d : 1.0));

    a = 1.0 / (float) sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    v[0] *= a;
    v[1] *= a;
    v[2] *= a;
}


void mouseMotion(int x, int y,int winWidth,int winHeight)
{
    float curPos[3], dx, dy, dz;

    trackball_ptov(x, y, winWidth, winHeight, curPos);
    if(trackingMouse)
    {
        dx = curPos[0] - lastPos[0];
        dy = curPos[1] - lastPos[1];
        dz = curPos[2] - lastPos[2];

        if (dx || dy || dz)
        {
            angletrack = 90.0 * sqrt(dx*dx + dy*dy + dz*dz);

            axis[0] = lastPos[1]*curPos[2] - lastPos[2]*curPos[1];
            axis[1] = lastPos[2]*curPos[0] - lastPos[0]*curPos[2];
            axis[2] = lastPos[0]*curPos[1] - lastPos[1]*curPos[0];

            lastPos[0] = curPos[0];
            lastPos[1] = curPos[1];
            lastPos[2] = curPos[2];
        }
    }
}

void startMotion(int x, int y,int winHeight,int winWidth)
{

    trackingMouse  = true;
    redrawContinue = false;
    startX	= x; startY = y;
    curx	= x; cury	= y;
    trackball_ptov(x, y, winWidth, winHeight, lastPos);
    trackballMove  = true;
}

void stopMotion(int x, int y)
{

    trackingMouse = false;

    if (startX != x || startY != y)
    {
        redrawContinue	= true;
    }
    else
    {
        angletrack		= 0.0;
        redrawContinue	= false;
        trackballMove	= false;
    }
}

GLfloat floorShadow[4][4];
GLfloat floorPlane[4] = {0,1.,0,0};
GLfloat lightPosition7[4] = {   9.588, 9.46, 9.248, 0.0 };

//****************


GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers),parent)
{
    setFocusPolicy(Qt::StrongFocus);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_STENCIL_FUNC);
    scene = new Scene(this);
    updateObjects(scene->objectsScene());
    updateJoints(scene->jointsScene());
    simTimer = new QTimer(this);
    connect(simTimer, SIGNAL(timeout()), this, SLOT(simStep()));

    simTimer->start(0);
    simTimer->setInterval(0);

    move = false;
    sim_pause = false;
    capture_pause = true;
    editing_frame = false;
    frame_edit = 0;
    show_character = true;
    load_exemple_curve = false;
    showInfo = false;
    screenshot = false;
    frames = 0;
    time_current = 0;
    updateKsProp(scene->getProportionalKsPD());
    updateKdProp(scene->getProportionalKdPD());
    updateBalancePD(scene->getKsTorqueBalance(),scene->getKdTorqueBalance(),scene->getKsForceBalance(),scene->getKdForceBalance(),scene->getKMomLinearBalance(),scene->getKMomAngularBalance());
    density = 0.5; //massa
    velocity = 5.;
    //scene->createCharacter();

    //scene->createCharacter();
    //scene->createLuxo();
    //scene->createLuxo2();
    //controlLuxo = false;

}

void GLWidget::initializeGL()
{


    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    //glEnable(GL_MULTISAMPLE_ARB);
    //glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);

    glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
    //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);				// Black Background
    glClearDepth(1.0f);									// Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
    glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
    glShadeModel(GL_SMOOTH);
    //glEnable(GL_COLOR_MATERIAL);

    static GLfloat light1pos[4] = {  -0.892, 0.3, 0.9, 0.0 };
    static GLfloat light1diffuse[] = { 0.8f, 0.8f, 0.8, 1.0f };
    static GLfloat light1specular[] = { 0.5f, 0.5f, 0.5f, 0.0f };

    static GLfloat light2pos[4] = { 9.588, 9.46, 9.248, 0.0 };
    static GLfloat light2diffuse[] = { 0.498f, 0.5f, 0.6, 1.0f };
    static GLfloat light2specular[] = { 0.2f, 0.2f, 0.2f, 0.0f };

    static GLfloat light3pos[4] = { 0.216, -0.392, -0.216, 0.0 };
    static GLfloat light3diffuse[] = { 0.798f, 0.838f, 1.0, 1.0f };
    static GLfloat light3specular[] = { 0.06f, 0.0f, 0.0f, 0.0f };

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);

    glLightfv(GL_LIGHT0, GL_POSITION, light1pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light1diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light1specular);

    glLightfv(GL_LIGHT1, GL_POSITION, light2pos);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light2diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light2specular);

    glLightfv(GL_LIGHT2, GL_POSITION, light3pos);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light3diffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, light3specular);
//    mainShader=new shader("vertex.vs","fragment.frag");
//        quadRenderShader=new shader("quadRender.vs","quadRender.frag");
//        simpleShader=new shader("simpleShader.vs","simpleShader.frag");
//        shadowShader=new shader("shadowShader.vs","shadowShader.frag");
#ifdef SHADERS_ENABLED
//    shaderProgram.addShaderFromSourceFile(QGLShader::Vertex, "../shaders/vertex.vs");
//    shaderProgram.addShaderFromSourceFile(QGLShader::Fragment, "../shaders/fragment.frag");
//    shaderProgram.addShaderFromSourceFile(QGLShader::Vertex, "../shaders/quadRender.vs");
//    shaderProgram.addShaderFromSourceFile(QGLShader::Fragment, "../shaders/quadRender.frag");
//    shaderProgram.addShaderFromSourceFile(QGLShader::Vertex, "../shaders/simpleShader.vs");
//    shaderProgram.addShaderFromSourceFile(QGLShader::Fragment, "../shaders/simpleShader.frag");
//    shaderProgram.addShaderFromSourceFile(QGLShader::Vertex, "../shaders/shadowShader.vs");
//    shaderProgram.addShaderFromSourceFile(QGLShader::Fragment, "../shaders/shadowShader.frag");
    //printf("In\n");
//    shaderProgram.addShaderFromSourceFile(QGLShader::Vertex, "../shaders/phong.vert");
//    shaderProgram.addShaderFromSourceFile(QGLShader::Fragment, "../shaders/phong.frag");
#endif

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_FUNC);
    glEnable (GL_LINE_SMOOTH);
    glEnable (GL_POINT_SMOOTH);
glEnable(GL_TEXTURE_2D);
glEnable(GL_BLEND);
    glCullFace(GL_FRONT);

    //glEnable(GL_COLOR_MATERIAL);
//    glEnable(GL_NORMALIZE);
//    glEnable(GL_RESCALE_NORMAL);
    //glLineWidth(1.2);
    glDepthFunc(1.0);
    //    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
//    glBindTexture(GL_TEXTURE_2D, 1.0);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
    //glutInitContextFlags (GLUT_COMPATIBILITY_PROFILE );


}

void GLWidget::resizeGL(int w, int h)
{
    const float ar = w>0 ? (float) w / (float) h : 1.0;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
    gluPerspective(30.,ar,0.001,1200000.);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
    winWidth = w;
    winHeight = h;
    scene->setProjection(Vec4(30.,ar,0.001,1200000));
    scene->setWindow(w,h);
    //printf("\nW %d H %d\n",w,h);

}
void GLWidget::drawScene(){
    //determinação da camera

    const float ar = winWidth>0 ? (float) winWidth / (float) winHeight : 1.0;
    glViewport(0, 0, winWidth, winHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
    gluPerspective(30.,ar,0.001,1200000.);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();

        glTranslated(-0.20,-0.135,-0.6);
        gluLookAt(cam->eye.x(),cam->eye.y(),cam->eye.z(), cam->at.x(),cam->at.y(),cam->at.z(), cam->up.x(),cam->up.y(),cam->up.z());
        scene->setViewer(cam->eye,cam->at,cam->up);
        glTranslated(cam->eye.x(),cam->eye.y(),cam->eye.z());
        Draw::drawAxisCameraView(0.02);
    glPopMatrix();
    glPushMatrix();


    glColor3f(1,1,0);
    QString text = QString("Lock Axis: ");
    if(cam->axis_x)
        text += "x ";
    if(cam->axis_y)
        text += "y ";
    if(cam->axis_z)
        text += "z ";
    if(cam->axis_x || cam->axis_y || cam->axis_z)

        renderText(0,0,0,text,QFont("../fonts/Quicksand_Book.otf"));
 //   glPushMatrix();

    glPopMatrix();

    gluLookAt(cam->eye.x1,cam->eye.x2,cam->eye.x3, cam->at.x1,cam->at.x2,cam->at.x3, cam->up.x1,cam->up.x2,cam->up.x3);

    //fim de determinação da camera
    glPushMatrix();

    if (shadow) drawShadows();
    if (!sim_pause) if (load_exemple_curve) showCurveExample();
    if (show_character){

        scene->draw();

        if(!capture_pause)
            if(scene->getSizeCharacter()!=0)
                if(scene->getCharacter(0)->getMoCap()->sizeFrames()>0)
                    motionCurrentFrame(scene->getCharacter(0)->getMoCap()->currentFrame());
        if (scene->getExternalForce().module()!=0)
            ciclo++;
        if (ciclo>30){
            scene->setExternalForce(Vec4(0,0,0));
            ciclo = 0;
        }
        if (ciclo_arrow>0){
            Draw::drawArrow2D(angle_quat,scene->getCharacter(0)->getPosCOM());
            ciclo_arrow++;
            if(ciclo_arrow>30) ciclo_arrow = 0;
        }

        showCompensableConeFriction();
    }



    if (editing_frame)
        if(scene->getSizeCharacter()!=0)
            if(scene->getCharacter(0)->getMoCap()->sizeFrames()>0)
                scene->getCharacter(0)->getMoCap()->showMoCap(Vec4(0,0,-2.0),frame_edit);
    glPopMatrix();

    glPushMatrix();
    //glLoadIdentity();
    //Draw::drawSkybox(Vec4(0,0,0),Vec4(20,20,20));

    //Draw::drawCoffeeCup(Vec4(0,0.5,0),MATERIAL_COPPER);
    Draw::drawGround(10);
    //if (!showInfo){ Draw::drawGroundTexture(10,0);}
    //else{
        //glClearColor(1,0,0,1);
    //}
    glPopMatrix();

    //glRasterPos2d(0.5,0.5);
    //char* p = (char*) "Pode ser assim?";
    //while (*p!='\0'){
    //    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*p++);
    //}
    //glPushMatrix();

    //glPopMatrix();
    //glEnable(GL_LIGHTING);
    //glPopMatrix();


}

void GLWidget::drawParameters()
{
    if (! scene->getSizeCharacter()>0) return;
    Character *chara = scene->getCharacter(0);
    QString dados;
    Vec4 out;
    glDisable(GL_LIGHTING);
    glColor3f(0,0,0);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, winWidth, 0.0, winHeight);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    int x = 680;
    int k = 0;
    if (chara->getBalance()->getEnableTorque()){
        glRasterPos2f(750, x);
        out = chara->getBalance()->getKsTorque();
        dados = QString().sprintf("\nTq Ks: %3.2f %3.2f %3.2f", out.x(), out.y(), out.z());
        //char* n = (char*)dados.toStdString().data();
        k = 0;
        while (k<dados.size()){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,dados.at(k).toLatin1());
            k++;
        }
        x -= 20;
        glRasterPos2f(750, x);
        out = chara->getBalance()->getKdTorque();
        dados = QString().sprintf("\nTq Kd: %3.2f %3.2f %3.2f", out.x(), out.y(), out.z());
        k = 0;
        while (k<dados.size()){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,dados.at(k).toLatin1());
            k++;
        }
        x -= 20;
    }
    if (chara->getBalance()->getEnableForce()){
        glRasterPos2f(750, x);
        out = chara->getBalance()->getKsForce();
        dados = QString().sprintf("\nFc Ks: %3.2f %3.2f %3.2f\n", out.x(), out.y(), out.z());
        k = 0;
        while (k<dados.size()){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,dados.at(k).toLatin1());
            k++;
        }
        x -= 20;
        glRasterPos2f(750, x);
        out = chara->getBalance()->getKdForce();
        dados = QString().sprintf("\nFc Kd: %3.2f %3.2f %3.2f\n", out.x(), out.y(), out.z());
        k = 0;
        while (k<dados.size()){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,dados.at(k).toLatin1());
            k++;
        }
        x -= 20;
    }
    if (chara->getBalance()->getEnableMomentum()){
        glRasterPos2f(750, x);
        out = chara->getBalance()->getKMomentumAngular();
        dados = QString().sprintf("\nMA Ks: %3.2f %3.2f %3.2f\n", out.x(), out.y(), out.z());
        k = 0;
        while (k<dados.size()){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,dados.at(k).toLatin1());
            k++;
        }
        x -= 20;
        glRasterPos2f(750, x);
        out = chara->getBalance()->getKMomentumLinear();
        dados = QString().sprintf("\nML Kd: %3.2f %3.2f %3.2f\n", out.x(), out.y(), out.z());
        k = 0;
        while (k<dados.size()){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,dados.at(k).toLatin1());
            k++;
        }
        x -= 20;
    }
    glRasterPos2f(750, x);
    out = scene->getProportionalKsPD();
    dados = QString().sprintf("\nPp Ks: %3.2f %3.2f %3.2f\n", out.x(), out.y(), out.z());
    k = 0;
    while (k<dados.size()){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,dados.at(k).toLatin1());
        k++;
    }
    x -= 20;
    glRasterPos2f(750, x);
    out = scene->getProportionalKdPD();
    dados = QString().sprintf("\nPp Kd: %3.2f %3.2f %3.2f\n", out.x(), out.y(), out.z());
    k = 0;
    while (k<dados.size()){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,dados.at(k).toLatin1());
        k++;
    }
    x -= 20;
    glRasterPos2f(750, x);
    dados = QString().sprintf("\nCone : %3.2f %3.2f %3.2f %3.2f\n", chara->getBalance()->getMCone(),chara->getBalance()->getHeightCone(),chara->getBalance()->getRadiusCone(),chara->getBalance()->getAngleCone());
    k = 0;
    while (k<dados.size()){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,dados.at(k).toLatin1());
        k++;
    }
    x -= 20;
    glRasterPos2f(750, x);
    dados = QString().sprintf("\nCLim : %3.2f\n", chara->getBalance()->getLimitCone());
    k = 0;
    while (k<dados.size()){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,dados.at(k).toLatin1());
        k++;
    }
    x -= 20;
    glRasterPos2f(750, x);
    dados = QString().sprintf("\nDensity Balls : %3.2f\n", density);
    k = 0;
    while (k<dados.size()){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,dados.at(k).toLatin1());
        k++;
    }
    x -= 20;
    glRasterPos2f(750, x);
    dados = QString().sprintf("\nVelocity Balls : %3.2f\n", velocity);
    k = 0;
    while (k<dados.size()){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,dados.at(k).toLatin1());
        k++;
    }

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glEnable(GL_LIGHTING);


}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    drawScene();
    if(showInfo){
        if (!screenshot) drawFPS();
        drawParameters();
    }
    if (screenshot) setScreenShot();

    //calculateFPS();


}

void GLWidget::updateCamera()
{
//    glViewport(0,0,width,height);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluPerspective(angle,(float)width/(float)height,0.01,12000000.0);
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
    //    gluLookAt(cam_eye.x(),cam_eye.y(),cam_eye.z(),cam_at.x(),cam_at.y(),cam_at.z(),0,1,0);

}

void GLWidget::showCompensableConeFriction()
{
    if(!(scene->getSizeCharacter()>0)) return;
    std::vector<Object*> foots = scene->getCharacter(0)->getBodiesFoot();
    if(foots.size()==2){
        float perc = foots.at(0)->getCompensableFactor();
        int slide = (int)100*perc;
        setSliderFoot1(slide);
        perc = foots.at(1)->getCompensableFactor();
        slide = (int)100*perc;
        setSliderFoot2(slide);

    }else if (foots.size()==1){
        float perc = foots.at(0)->getCompensableFactor();
        int slide = (int)100*perc;
        setSliderFoot1(slide);
    }
}
int teste = 0;
void GLWidget::simStep(){
    double ti,tf,tempo; // ti = tempo inicial // tf = tempo final
    ti = tf = tempo = 0;
    timeval tempo_inicio,tempo_fim;
    gettimeofday(&tempo_inicio,NULL);
    if(!sim_pause){
        scene->simulationStep(enable_balance);

    }

    //calculateFPSPaint();
    update();


    //calculateFPS();


    gettimeofday(&tempo_fim,NULL);
    tf = (double)tempo_fim.tv_usec + ((double)tempo_fim.tv_sec * (1000000.0));
    ti = (double)tempo_inicio.tv_usec + ((double)tempo_inicio.tv_sec * (1000000.0));
    tempo = (tf - ti) / 1000;
    //printf("Tempo gasto em milissegundos para desenhar %.3f\n",tempo);

    double tempoFalta = (33.33 - tempo);
    if (tempoFalta>0)
      simTimer->setInterval(tempoFalta);

//    gettimeofday(&tempo_fim,NULL);
//    tf = (double)tempo_fim.tv_usec + ((double)tempo_fim.tv_sec * (1000000.0));
//    //ti = (double)tempo_inicio.tv_usec + ((double)tempo_inicio.tv_sec * (1000000.0));
//    tempo = (tf - ti) / 1000;
////    gettimeofday(&tempo_fim,NULL);
////    tf = (double)tempo_fim.tv_usec + ((double)tempo_fim.tv_sec * (1000000.0));
////    ti = (double)tempo_inicio.tv_usec + ((double)tempo_inicio.tv_sec * (1000000.0));
////    tempo = (tf - ti) / 1000;
//    printf("Tempo gasto em milissegundos para desenhar %.3f\n",tempo);

    //teste++;
    //if (teste == 10) exit(EXIT_SUCCESS);

}



void GLWidget::mouseMoveEvent(QMouseEvent *event)
{

    int y = event->pos().y();
    int x = event->pos().x();

    /*if (controlLuxo && lbpressed && !rbpressed){
        ControlPD* controller = scene->getCharacter(0)->getControllersPD().at(1);
        controller->setQuaternionWanted(Quaternion(Vec4(0,0,x)));
    } else if (controlLuxo && rbpressed && !lbpressed) {
        ControlPD* controller = scene->getCharacter(0)->getControllersPD().at(2);
        controller->setQuaternionWanted(Quaternion(Vec4(0,0,y)));
    } else if (controlLuxo && lbpressed && rbpressed){
        ControlPD* controller = scene->getCharacter(0)->getControllersPD().at(0);
        controller->setQuaternionWanted(Quaternion(Vec4(0,0,x)));
    }*/
    if (controlLuxo){
        if (lbpressed && !rbpressed){
            ControlPD* controllerUpperLower = scene->getCharacter(0)->getControllersPD().at(1);
            controllerUpperLower->setQuaternionWanted(QuaternionQ(Vec4(0,0,x)));
            ControlPD* controllerLowerFeet = scene->getCharacter(0)->getControllersPD().at(2);
            controllerLowerFeet->setQuaternionWanted(QuaternionQ(Vec4(0,0,y)));
        } if (!lbpressed && rbpressed){
            ControlPD* controllerLampUpper = scene->getCharacter(0)->getControllersPD().at(0);
            controllerLampUpper->setQuaternionWanted(QuaternionQ(Vec4(0,0,x)));
        }
    } else {
        if (lbpressed && !rbpressed) {
            cam->rotatex(y,last_y);
            cam->rotatey(x,last_x);
        }
        if (!lbpressed && rbpressed) {
            cam->translatex(x,last_x);
            cam->translatey(y,last_y);
        }
        if (lbpressed && rbpressed) {
            cam->zoom(y,last_y);
        }
    }

    last_x = x;
    last_y = y;


}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{

    int y = event->pos().y();
    int x = event->pos().x();
    if (event->button() & Qt::LeftButton) {
        // when the button is pressed
        lbpressed = false;
    }
    // if the left button is pressed
    if (event->button() & Qt::RightButton) {
        // when the button is pressed
        rbpressed = false;
    }
    last_x = x;
    last_y = y;

}

void GLWidget::mousePressEvent(QMouseEvent *event)
{

    if(mode==SHOTBALL){
        Object *obj;
        obj = scene->objectClicked(event->pos().x(),(scene->height)-event->pos().y());
        objselshot = obj;
        return;
    }
    int y = event->pos().y();
    int x = event->pos().x();
    // if the left button is pressed
        if (event->button() & Qt::LeftButton) {
            // when the button is pressed
                lbpressed = true;
        }
        // if the left button is pressed
        if (event->button() & Qt::RightButton) {
            // when the button is pressed
                rbpressed = true;
        }

        last_x = x;
        last_y = y;


}



/***********************Signal & Slots*********************************/
void GLWidget::simulationPlayPause()
{
    sim_pause = !sim_pause;
    //qDebug() << "Pause/Play";
}

void GLWidget::simulationRestart()
{
    scene->restartPhysics();
    numOffBalls = 0;
    densidade = 100;
    sim_pause = true;
}


void GLWidget::keyPressEvent(QKeyEvent *event)
{
    //printf("%s\n",event->text().toUtf8().constData());
    if(event->key() == Qt::Key_Space ){
        simulationPlayPause();
    }

    if(event->key() == Qt::Key_C ){
        manipulate = SET_CAM;
    }
    if(event->key() == Qt::Key_L ){
        enable_balance = !enable_balance;
    }
    if(event->key() == Qt::Key_O ){
        manipulate = SET_OBJECT;
        mode = -1;
    }
    if(event->key() == Qt::Key_J ){
        scene->habiliteJump();
    }

    if(event->key() == Qt::Key_Z ){
        //scene->addObject(Vec4(1,1,1),Vec4(0,30,0),Quaternion(),TYPE_CUBE);
        //count++;
        cam->lockAxisZ(!cam->axis_z);
    }
    if(event->key() == Qt::Key_X ){
        cam->lockAxisX(!cam->axis_x);
        //scene->addObject(Vec4(0.5,0.5,0.5),Vec4(0,30,0),Quaternion(),TYPE_SPHERE);
        //count++;
    }
    if(event->key() == Qt::Key_Y ){
        cam->lockAxisY(!cam->axis_y);
        //scene->addObject(Vec4(0.5,1.0,0.5),Vec4(0,30,0),Quaternion(),TYPE_CYLINDER);
        //count++;
    }
    if(event->key() == Qt::Key_S ){
        mode = SHOTBALL;
        //scene->addObject(Vec4(0.5,1.0,0.5),Vec4(0,30,0),Quaternion(),TYPE_CYLINDER);
        //count++;
    }
    if(event->key() == Qt::Key_B ){
        if((scene->getSizeCharacter()==0)) return;
        if(mode == SHOTBALL){
            if(objselshot!=NULL){
                scene->shotBallsCharacterBody(objselshot,velocity,density);
                return;
            }
        }

        srand(time(NULL));
         int x = ( rand() % 8 );
         scene->shotBallsCharacterBody(scene->getCharacter(0)->getBody(x),velocity,density);
        //scene->shotBallsCharacterRandom(scene->getCharacter(0),x,density);
        //cam->lockAxisY(!cam->axis_y);
        //scene->addObject(Vec4(0.5,1.0,0.5),Vec4(0,30,0),Quaternion(),TYPE_CYLINDER);
        //count++;
    }
    if(event->key() == Qt::Key_D ){
        if((scene->getSizeCharacter()<1)) return;
        load_exemple_curve = true;
        loadCurveExample();
        //cam->lockAxisY(!cam->axis_y);
        //scene->addObject(Vec4(0.5,1.0,0.5),Vec4(0,30,0),Quaternion(),TYPE_CYLINDER);
        //count++;
    }
    if(event->key() == Qt::Key_U ){
        scene->loadSceneObjects();
        //cam->lockAxisY(!cam->axis_y);
        //scene->addObject(Vec4(0.5,1.0,0.5),Vec4(0,30,0),Quaternion(),TYPE_CYLINDER);
        //count++;
    }
    if(event->key() == Qt::Key_Plus ){
        plusFrameEdition();
        //Physics::bodySetTorque(scene->select->getBody(),100.0,10.0,10.0);
        //Physics::bodyAddForce(scene->select->getBody(),1,0,1);
        //Vec4 *n = new Vec4(2,2,2);
        //scene->select->appTorque(n);
        //Physics::bodyAddForce(scene->select->getBody(),100.0,10.0,10.0);
        //scene->addObject(Vec4(0.5,1.0,0.5),Vec4(0,30,0),Quaternion(),TYPE_CYLINDER);
        //count++;
    }
    if(event->key() == Qt::Key_Minus ){
        minusFrameEdition();
    }
    if(event->key() == Qt::Key_F ){
        setForceCharacter();
    }
    if(event->key() == Qt::Key_Up){
        if(manipulate==SET_OBJECT){
            Object *obj = scene->getCharacter(0)->getObjectSelected();
            if (obj==NULL) return;
            Vec4 pos = obj->getTarget();
            pos += Vec4(0.05,0,0);
            obj->setTarget(pos);
        }
    }
    if(event->key() == Qt::Key_Down){
        if(manipulate==SET_OBJECT){
            Object *obj = scene->getCharacter(0)->getObjectSelected();
            if (obj==NULL) return;
            Vec4 pos = obj->getTarget();
            pos += Vec4(-0.05,0,0);
            obj->setTarget(pos);
        }

    }


    if(event->key() == Qt::Key_Left){
        if(manipulate==SET_OBJECT){
            Object *obj = scene->getCharacter(0)->getObjectSelected();
            if (obj==NULL) return;
            Vec4 pos = obj->getTarget();
            pos += Vec4(0,0,0.05);
            obj->setTarget(pos);
        }else{
            angle_direction=angle_direction-1;
            setAngleDirection(angle_direction);
        }
    }
    if(event->key() == Qt::Key_Right){
        if(manipulate==SET_OBJECT){
            Object *obj = scene->getCharacter(0)->getObjectSelected();
            if (obj==NULL) return;
            Vec4 pos = obj->getTarget();
            pos += Vec4(0,0,-0.05);
            obj->setTarget(pos);
        }else{
            angle_direction=angle_direction+1;
            setAngleDirection(angle_direction);
        }

    }
    if(event->key() == Qt::Key_C){
        static int posCam = 0;
        posCam++;
        if (cam->type == CAMERA_FAR) {
            delete cam;
            if (posCam%5==0) cam = new Camera(); //CameraDistante(0,1,5, 0,1,0, 0,1,0);
            if (posCam%5==1) cam = new Camera(5,1,0, 0,1,0, 0,1,0);
            if (posCam%5==2) cam = new Camera(0,1,-5, 0,1,0, 0,1,0);
            if (posCam%5==3) cam = new Camera(-5,1,0, 0,1,0, 0,1,0);
            if (posCam%5==4) cam = new Camera(savedCamera[0],savedCamera[1],savedCamera[2],savedCamera[3],savedCamera[4],savedCamera[5],savedCamera[6],savedCamera[7],savedCamera[8]);
        } else if (cam->type == CAMERA_GAME) {
            delete cam;
            cam = new Camera();
        }
    }
    if(event->key() == Qt::Key_S){
        //save current camera location
        savedCamera[0] = cam->eye.x();
        savedCamera[1] = cam->eye.y();
        savedCamera[2] = cam->eye.z();
        savedCamera[3] = cam->at.x();
        savedCamera[4] = cam->at.y();
        savedCamera[5] = cam->at.z();
        savedCamera[6] = cam->up.x();
        savedCamera[7] = cam->up.y();
        savedCamera[8] = cam->up.z();
    }

    if (event->key() == Qt::Key_M){
        controlLuxo = !controlLuxo;
    }

    updateObjects(scene->objectsScene());
    updateGL();
}

MoCap *GLWidget::pushMotionCapture()
{
    if (scene->getSizeCharacter()<1) return NULL;
    return scene->getCharacter(0)->getMoCap();
}

void GLWidget::drawShadows()
{
    Draw::shadowMatrix(floorShadow, floorPlane, lightPosition7);

      glPushMatrix();


            /* Draw the floor with stencil value 3.  This helps us only
               draw the shadow once per floor pixel (and only on the
               floor pixels). */
            glEnable(GL_STENCIL_TEST);
            glStencilFunc(GL_ALWAYS, 3, 0xffffffff);
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);


        /* Draw "top" of floor.  Use blending to blend in reflection (jah foi habilitado o blend). */
        Draw::drawGround(10);

        if (true) {

          /* Render the projected shadow. */

          /* Now, only render where stencil is set above 2 (ie, 3 where
               the top floor is).  Update stencil with 2 where the shadow
               gets drawn so we don't redraw (and accidently reblend) the
               shadow). */
          glStencilFunc(GL_LESS, 2, 0xffffffff);  /* draw if ==1 */
          glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

          /* To eliminate depth buffer artifacts, we use polygon offset
             to raise the depth of the projected shadow slightly so
             that it does not depth buffer alias with the floor. */
          //glEnable(GL_POLYGON_OFFSET_EXT);
          //glStencilFunc( GL_EQUAL, 0x1, 0xffffffff );
          //glStencilOp( GL_KEEP, GL_KEEP, GL_ZERO );
          glEnable(GL_POLYGON_OFFSET_FILL);
          glPolygonOffset(-0.9, -0.9f);


          /* Render 50% black shadow color on top of whatever the
             floor appareance is. */
//            glDisable( GL_DEPTH_TEST );
          glEnable( GL_BLEND );
          glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
          glDisable(GL_LIGHTING);  /* Force the 20% black. */
          glColor4f(0.0, 0.0, 0.0, 0.5);

          glPushMatrix();
            /* Project the shadow. */
            glMultMatrixf((GLfloat *) floorShadow);
            if(show_character) scene->drawShadows();
            if (editing_frame)
                if(scene->getSizeCharacter()!=0)
                    if(scene->getCharacter(0)->getMoCap()->sizeFrames()>0)
                        scene->getCharacter(0)->getMoCap()->showMoCap(Vec4(0,0,-2.0),frame_edit);
          glPopMatrix();


          glDisable(GL_BLEND);
          glDisable(GL_POLYGON_OFFSET_FILL);
          glDisable(GL_STENCIL_TEST);
          glEnable( GL_DEPTH_TEST );
          glEnable(GL_LIGHTING);
        }

        glPopMatrix();
}

void GLWidget::loadCurveExample()
{
    curve_quat.clear();
    curve_quat_time.clear();
    curve_quat.append(Vec4(0.0, 0.0, 0.0)); curve_quat_time.append(0);
    curve_quat.append(Vec4(0.0, 30.0, 0.0));curve_quat_time.append(150);
    curve_quat.append(Vec4(0.0, 75.0, 0.0));curve_quat_time.append(150);
    curve_quat.append(Vec4(0.0, 20.0, 0.0));curve_quat_time.append(150);
    curve_quat.append(Vec4(0.0, 130.0, 0.0));curve_quat_time.append(400);
    curve_quat.append(Vec4(0.0, 250.0, 0.0));curve_quat_time.append(600);
    curve_quat.append(Vec4(0.0, 180.0, 0.0));curve_quat_time.append(350);
    curve_quat.append(Vec4(0.0, 220.0, 0.0));curve_quat_time.append(150);
    curve_quat.append(Vec4(0.0, 320.0, 0.0));curve_quat_time.append(450);
    curve_quat.append(Vec4(0.0, 0.0, 0.0)); curve_quat_time.append(150);
}

void GLWidget::showCurveExample()
{
    int total_time = 0;
    int frame_current,frame_time;
    QList<int> time_acumulate;
    for(int i=0;i<curve_quat.size();i++){
        total_time += curve_quat_time[i];
        time_acumulate.append(total_time);
    }

    for(int i=0;i<curve_quat.size();i++){
        if(time_acumulate[i]>time_current){
            frame_current = i;
            frame_time    = time_current-(total_time-time_acumulate[i-1]);
            break;
        }
    }

    time_m = time_acumulate[time_acumulate.size()-1]-1+curve_quat_time[curve_quat_time.size()-1];
    QuaternionQ q;
    if(time_current<total_time){
            //chama a interpolação de Quaternion Esferica (SQUAD)
            q = Interpolation::KeyFramestoSquad(curve_quat,frame_current,(time_current - time_acumulate[frame_current-1]) /(double)curve_quat_time[frame_current]);
    }
    time_current++;


    if(time_current>total_time){ time_current = 0;
    }

    setAngleDirection(q.toEuler().y());
    //time_m = total_time;
}

void GLWidget::setScreenShot()
{
    // screen capture
      if (screenshot) {
        //screen capture count
        scr_capture_count++;
        //capturing images
        if ( scr_capture_count % 1 == 0 )
        {
          std::string temp_str = "";
          char num[20];

          temp_str += "../screenshot/frames/image";
          sprintf(num, "%04d", scr_capture_count/1);
          temp_str += num;
          temp_str += ".ppm";

          SCREENSHOT_Take(0, 0, winWidth, winHeight, temp_str.c_str(), SCREENSHOT_PPM);
        }
      }
}

void GLWidget::calculateFPSPaint()
{
    if (frames == 0) {
        m_time.start();
    }
    frames++;
    if (m_time.elapsed()>1000.){
        FPS = float(frames);
        frames = 0;
    }
}

void GLWidget::SimStepsSimulation(int steps)
{
    scene->setSimStep(steps);
}

void GLWidget::applyForce(Vec4 force)
{
    //scene->applyForce(force);
    scene->setExternalForce(force);
}

void GLWidget::loadScene(QString file)
{
    Utils::readModelRubens(scene,file.toStdString());
    updateObjects(scene->objectsScene());
    updateJoints(scene->jointsScene());
    updateKsProp(scene->getProportionalKsPD());
    updateKdProp(scene->getProportionalKdPD());
    updateBalancePD(scene->getKsTorqueBalance(),scene->getKdTorqueBalance(),scene->getKsForceBalance(),scene->getKdForceBalance(),scene->getKMomLinearBalance(),scene->getKMomAngularBalance());
    if(scene->getSizeCharacter()>0){
        scene->getCharacter(0)->contructHierarchyBodies();
    }

}

void GLWidget::saveCharacter(QString file)
{
    if (scene->getSizeCharacter()==0) return;
    Utils::saveModelRubens(scene->getCharacter(0),file.toStdString());

}

void GLWidget::loadMotionCapture(QString file)
{
    if (scene->getSizeCharacter()==0) return;
    Utils::loadMotionCapture(scene->getCharacter(0)->getMoCap(),scene->getCharacter(0),file.toStdString());
    scene->getCharacter(0)->loadMotionFrames();
    motionTotalFrame(scene->getCharacter(0)->getMoCap()->sizeFrames());
    scene->getCharacter(0)->getMoCap()->copyFootsProperties();
    scene->getCharacter(0)->getMoCap()->setAddressFile(file);


}

void GLWidget::loadFramesConfig(QString file)
{
    if (scene->getSizeCharacter()==0) return;
    if (scene->getCharacter(0)->getMoCap()==NULL) return;
    Utils::readFramesConfig(scene->getCharacter(0),file.toStdString());
    scene->getCharacter(0)->getMoCap()->setAddressFileLoad(file);

}

void GLWidget::saveFramesConfig(QString file)
{
    if (scene->getSizeCharacter()==0) return;
    if (scene->getCharacter(0)->getMoCap()==NULL) return;
    Utils::saveFramesConfig(scene->getCharacter(0),file.toStdString());
}

void GLWidget::setPlayback(bool val)
{

    capture_pause = val;
    if (scene->getSizeCharacter()==0) return;
    if(scene->getCharacter(0)->getMoCap()->sizeFrames()>0){
        scene->getCharacter(0)->getMoCap()->setStatusMotion(!capture_pause);
        scene->statusMotionCapture(!val);
    }

}

void GLWidget::restartMotion()
{
    motionCurrentFrame(1);
    if (scene->getSizeCharacter()==0) return;
    if(scene->getCharacter(0)->getMoCap()->sizeFrames()>0){
        scene->restartMotionCapture();
        if(scene->getCharacter(0)->offset.module()==0) scene->getCharacter(0)->getMoCap()->initializePosesModel(1);
    }
    //Utils::loadMotionCapture(scene->getCharacter(0)->getMoCap(),scene->getCharacter(0),file.toStdString());


}

void GLWidget::showEditingFrame(bool b)
{
    if(scene->getSizeCharacter()!=0)
        if(scene->getCharacter(0)->getMoCap()->sizeFrames()>0)
            editing_frame = b;
}

void GLWidget::setEditingFrame(int frame)
{
    if(scene->getSizeCharacter()!=0)
        if(scene->getCharacter(0)->getMoCap()->sizeFrames()>0)
            frame_edit = frame;
}

void GLWidget::setBeginClycle(int v)
{
    if (scene->getSizeCharacter()==0) return;
    if(scene->getCharacter(0)->getMoCap()->sizeFrames()>0){
        scene->getCharacter(0)->getMoCap()->setBeginClycle(v);
    }
}

void GLWidget::setEndClycle(int v)
{
    if (scene->getSizeCharacter()==0) return;
    if(scene->getCharacter(0)->getMoCap()->sizeFrames()>0){
        scene->getCharacter(0)->getMoCap()->setEndClycle(v);
    }
}

void GLWidget::setStepsInterpolation(double val)
{
    if(!(scene->getSizeCharacter()>0)) return;
    scene->getCharacter(0)->getBalance()->setStepsInterpolation(val);
}

void GLWidget::saveSimulationParameters(QString file)
{
   // if (scene->getSizeCharacter()==0) return;
    Utils::saveSimulationConfig(scene,file.toStdString());
}

void GLWidget::stopSimulation()
{
    //disconnect(simTimer, SIGNAL(timeout()), this, SLOT(simStep()));
    simTimer->stop();
    sim_pause = true;
}

void GLWidget::startSimulation()
{
    //connect(simTimer, SIGNAL(timeout()), this, SLOT(simStep()));
    sim_pause = false;
    simTimer->start(0);
}

void GLWidget::setGravityParameters(Vec4 g)
{
    scene->setGravityParameters(g);

}

void GLWidget::setSimbiconForceParameters(Vec4 g)
{
    scene->setKVelocityLocomotion(g);
}

void GLWidget::setSimbiconDistanceParameters(Vec4 g)
{
    scene->setKDistanceLocomotion(g);
}

void GLWidget::setGravity(bool b)
{
    scene->setGravity(b);
}

void GLWidget::setProportionalKs(Vec4 ks)
{
    scene->setProportionalKsPD(ks);
}

void GLWidget::setProportionalKd(Vec4 kd)
{
    scene->setProportionalKdPD(kd);
}

void GLWidget::setBalanceControl(Vec4 ksT, Vec4 kdT, Vec4 ksF, Vec4 kdF, Vec4 kmomlin,Vec4 kmomang)
{
    scene->setKdForceBalance(kdF);
    scene->setKsForceBalance(ksF);
    scene->setKdTorqueBalance(kdT);
    scene->setKsTorqueBalance(ksT);
    scene->setKMomLinearBalance(kmomlin);
    scene->setKMomAngularBalance(kmomang);
}

void GLWidget::setEnableTorqueBalance(bool b)
{
    scene->setEnableTorqueBalance(b);
}

void GLWidget::setEnableForceBalance(bool b)
{
    scene->setEnableForceBalance(b);
}

void GLWidget::setEnableMomentumBalance(bool b)
{
    scene->setEnableMomentumBalance(b);
}

void GLWidget::setKsRelationshipKs(bool b)
{
    if(b){
        if(scene->getSizeCharacter()<1) return;
        scene->getCharacter(0)->setKsRetaionshipKd();
    }
}

void GLWidget::setLimitCone(int val)
{
    if(scene->getSizeCharacter()<1) return;
    scene->getCharacter(0)->getBalance()->setLimitCone(val);
}

void GLWidget::setKVelocityLocomotion(Vec4 k)
{
    if(scene->getSizeCharacter()<1) return;
    scene->getCharacter(0)->getBalance()->setKVelocityLocomotion(k);
}

void GLWidget::setKDistanceLocomotion(Vec4 k)
{
    if(scene->getSizeCharacter()<1) return;
    scene->getCharacter(0)->getBalance()->setKDistanceLocomotion(k);
}

void GLWidget::setVelocityDensityBalls(float den, float vel)
{
    density = den;
    velocity = vel;
}

void GLWidget::setAlphaCharacter(int value)
{
    scene->setAlphaCharacter((float)value/100.0);
}

void GLWidget::bindShader()
{
#ifdef SHADERS_ENABLED
    shaderProgram.bind();
#endif
}

void GLWidget::releaseShader()
{
#ifdef SHADERS_ENABLED
    shaderProgram.release();
#endif
}

void GLWidget::setWireCharacter(bool wire)
{
    scene->setWireCharacter(wire);
}

void GLWidget::setShowShadow(bool b)
{
    shadow = b;
}

void GLWidget::setShowGRF(bool b)
{
    scene->drawGRF(b);
}

void GLWidget::setShowMoCap(bool b)
{
    if(scene->getSizeCharacter()<1) return;
    scene->getCharacter(0)->drawMoCap(b);
}

void GLWidget::setCompensationBalance(int val)
{
    scene->setCompensacao(val);
}

void GLWidget::setMCone(double val)
{
    if(!(scene->getSizeCharacter()>0)) return;
    scene->getCharacter(0)->getBalance()->setMCone(val);
}

void GLWidget::setRadiusCone(double val)
{
    if(!(scene->getSizeCharacter()>0)) return;
    scene->getCharacter(0)->getBalance()->setRadiusCone(val);

}

void GLWidget::setHeightCone(double val)
{
    if(!(scene->getSizeCharacter()>0)) return;
    scene->getCharacter(0)->getBalance()->setHeightCone(val);

}

void GLWidget::setAngleCone(double val)
{
    if(!(scene->getSizeCharacter()>0)) return;
    scene->getCharacter(0)->getBalance()->setAngleCone(val);

}

void GLWidget::setToleranceSensor(double val)
{
    if(!(scene->getSizeCharacter()>0)) return;
    scene->getCharacter(0)->getBalance()->setSensorTolerance(val);
}

void GLWidget::setAngleBodyBalance(Vec4 v)
{
   if(!(scene->getSizeCharacter()>0)) return;
   scene->setAngleBodyBalance(v);
   angle_quat = v.y();
   ciclo_arrow = 1;
   angle_direction = v.y();
}



std::vector<Object*> GLWidget::getObjectsList()
{
    return this->scene->objectsScene();
}

std::vector<Joint*> GLWidget::getJointsList()
{
    return this->scene->jointsScene();
}

void GLWidget::setObjectSelected(int row)
{

    std::vector<Object*> objs = scene->objectsScene();
    if (row<0) return;
    for (unsigned int i=0;i<objs.size();i++) objs.at(i)->setSelected(false);
    objs.at(row)->setSelected(true);
    objs.clear();

}

Object *GLWidget::getObject(int row)
{
    std::vector<Object*> objs = scene->objectsScene();
    if (row<0) return NULL;
    return objs.at(row);
}

void GLWidget::showCharacter(bool b)
{
    show_character = b;
}

void GLWidget::setJointSelected(int row)
{
    std::vector<Joint*> jts = scene->jointsScene();
    for (unsigned int i=0;i<jts.size();i++) jts.at(i)->setSelected(false);
    if (row<0){
        showJoint(NULL);
        return;
    }
    jts.at(row)->setSelected(true);
    showJoint(jts.at(row));
    jts.clear();
}

void GLWidget::loadSimulationParameters(QString file)
{
    Utils::loadSimulationConfig(scene,file.toStdString());
    updateKsProp(scene->getProportionalKsPD());
    updateKdProp(scene->getProportionalKdPD());
    updateObjects(scene->objectsScene());
    updateJoints(scene->jointsScene());
    updateBalancePD(scene->getKsTorqueBalance(),scene->getKdTorqueBalance(),scene->getKsForceBalance(),scene->getKdForceBalance(),scene->getKMomLinearBalance(),scene->getKMomAngularBalance());
    setGravity(scene->getGravity());
    scene->getGravity().showVec4();
    setEnableGravity(scene->hasGravity());
    for(int i=0;i<scene->getSizeCharacter();i++){
        scene->getCharacter(i)->contructHierarchyBodies();
        if(scene->getCharacter(i)->getMoCap()->sizeFrames()>0){
            motionTotalFrame(scene->getCharacter(i)->getMoCap()->sizeFrames());
        }
        updateBalanceCone(scene->getCharacter(i)->getBalance()->getMCone(),scene->getCharacter(i)->getBalance()->getAngleCone(),scene->getCharacter(i)->getBalance()->getRadiusCone(),scene->getCharacter(i)->getBalance()->getHeightCone());

    }
    setToleranceFoot(scene->getCharacter(0)->getBalance()->getSensorTolerance());
    //scene->getCharacter(0)->showHierarchies();
    //printf("\nMassa total: %.3f",scene->getCharacter(0)->getMassTotal());
    //scene->createRamp();





}

void GLWidget::setScreenShot(bool b)
{
    this->screenshot = b;
    scr_capture_count = 0;
}

void GLWidget::setRenderMesh(bool b)
{
    this->rendermesh = b;
    scene->setRenderMesh(this->rendermesh);
}

void GLWidget::setShowInfos(bool b)
{
    showInfo = b;
}

