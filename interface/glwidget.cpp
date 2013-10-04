#include "glwidget.h"
#include <GL/glu.h>
#include "scene/object.h"
#include "graphics/draw.h"
#include "math/quaternion.h"
#include "extra/utils.h"
#include <sys/time.h>
#include <stdio.h>
#include <GL/gl.h>                        // Header File For The OpenGL32 Library
#include <GL/glu.h>                       // Header File For The GLu32 Library
#include <GL/glut.h>
#include <GL/glext.h>                     // Header File For The GLaux Library
#include "camera.h"

/************** New Camera *****************/
static int slices = 16;
static int stacks = 16;

static float ax = 0.0;
static float ay = 0.0;
static float az = 0.0;

static float delta = 5.0;

static bool lbpressed = false;
//static bool mbpressed = false;
static bool rbpressed = false;

static float last_x = 0.0;
static float last_y = 0.0;

Camera* cam = new Camera();
static float savedCamera[9];



/************** Fim Camera *****************/

int count = 0;
Quaternion q;
//bool move = false;
float angle = 45;
int last_pox_x,last_pox_y;
int id_material;
int state_key;
//Vec4 eye,at;
Vec4 cam_eye = Vec4(2,2,2);
Vec4 cam_at  = Vec4(0,1,0);
int ciclo = 0;
//variaveis de camera
//int width,height;


//variáveis trackball

int 	winWidth, winHeight;

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



//****************


GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers),parent)
{
    setFocusPolicy(Qt::StrongFocus);
    glEnable(GL_MULTISAMPLE);
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
    updateKsProp(scene->getProportionalKsPD());
    updateKdProp(scene->getProportionalKdPD());
    updateBalancePD(scene->getKsTorqueBalance(),scene->getKdTorqueBalance(),scene->getKsForceBalance(),scene->getKdForceBalance(),scene->getKMomBalance());

}

void GLWidget::initializeGL()
{

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glShadeModel(GL_SMOOTH);
    //glEnable(GL_COLOR_MATERIAL);

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    static GLfloat light1pos[4] = { -0.892, 0.3, 0.9, 0.0 };
    static GLfloat light1diffuse[] = { 0.8f, 0.8f, 0.8, 1.0f };
    static GLfloat light1specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };

    static GLfloat light2pos[4] = { 0.588, 0.46, 0.248, 0.0 };
    static GLfloat light2diffuse[] = { 0.498f, 0.5f, 0.6, 1.0f };
    static GLfloat light2specular[] = { 0.2f, 0.2f, 0.2f, 1.0f };

    static GLfloat light3pos[4] = { 0.216, -0.392, -0.216, 0.0 };
    static GLfloat light3diffuse[] = { 0.798f, 0.838f, 1.0, 1.0f };
    static GLfloat light3specular[] = { 0.06f, 0.0f, 0.0f, 1.0f };

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

//#ifdef SHADERS_ENABLED
//    shaderProgram.addShaderFromSourceFile(QGLShader::Vertex, ":/phong.vert");
//    shaderProgram.addShaderFromSourceFile(QGLShader::Fragment, ":/phong.frag");
//    //shaderProgram.addShaderFromSourceFile(QGLShader::Vertex, ":/toon.vert");
//    //shaderProgram.addShaderFromSourceFile(QGLShader::Fragment, ":/toon.frag");
//#endif
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    //glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    //glEnable(GL_DEPTH_FUNC);
    glEnable(GL_POINT_SMOOTH);
    //glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_RESCALE_NORMAL);
    //glLineWidth(1.2);
    //glDepthFunc(1.0);

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
//    glViewport(0,0,w,h);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluPerspective(angle,(float)w/h,0.01,12000000.0);
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    gluLookAt(cam_eye.x(),cam_eye.y(),cam_eye.z(),cam_at.x(),cam_at.y(),cam_at.z(),0,1,0);
//    height = h;
//    width  = w;



}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    //determinação da camera

    glColor3f(1,1,0);
    QString text = QString("Lock Axis: ");
    if(cam->axis_x)
        text += "x ";
    if(cam->axis_y)
        text += "y ";
    if(cam->axis_z)
        text += "z ";
    if(cam->axis_x || cam->axis_y || cam->axis_z)
        renderText(0,0,0,text,QFont("../fonts/Quicksand_Book.otf"),2000);
    gluLookAt(cam->eye.x1,cam->eye.x2,cam->eye.x3, cam->at.x1,cam->at.x2,cam->at.x3, cam->up.x1,cam->up.x2,cam->up.x3);

    //fim de determinação da camera
    glPushMatrix();

    scene->draw();
    Draw::drawGround(10);
    if (scene->getExternalForce().module()!=0)
        ciclo++;
    if (ciclo>5){
        scene->setExternalForce(Vec4(0,0,0));
        ciclo = 0;
    }
    if(!capture_pause)
        if(scene->getSizeCharacter()!=0)
            if(scene->getCharacter(0)->getMoCap()->sizeFrames()>0)
                motionCurrentFrame(scene->getCharacter(0)->getMoCap()->currentFrame());

    glPopMatrix();

//    gettimeofday(&tempo_fim,NULL);
//      tf = (double)tempo_fim.tv_usec + ((double)tempo_fim.tv_sec * (1000000.0));
//      ti = (double)tempo_inicio.tv_usec + ((double)tempo_inicio.tv_sec * (1000000.0));
//      tempo = (tf - ti) / 1000;
//      printf("Tempo gasto em milissegundos %.3f\n",tempo);




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

void GLWidget::simStep(){
//    double ti,tf,tempo; // ti = tempo inicial // tf = tempo final
//      ti = tf = tempo = 0;
//      timeval tempo_inicio,tempo_fim;
//      gettimeofday(&tempo_inicio,NULL);
    if(!sim_pause)
        scene->simulationStep();
    update();
//    gettimeofday(&tempo_fim,NULL);
//      tf = (double)tempo_fim.tv_usec + ((double)tempo_fim.tv_sec * (1000000.0));
//      ti = (double)tempo_inicio.tv_usec + ((double)tempo_inicio.tv_sec * (1000000.0));
//      tempo = (tf - ti) / 1000;
//      printf("Tempo gasto em milissegundos %.3f\n",tempo);

}

void GLWidget::wheelEvent(QWheelEvent *event)
{
//    int numDegrees = event->delta() / 8;
//    int numSteps = numDegrees / 15;
//    if (numSteps>0 && angle<180.0){
//        angle += 1;
//    }
//    else{
//        if (!(angle<=5)){
//            angle -= 1;
//        }
//    }
    //updateCamera();


}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{

    int y = event->pos().y();
    int x = event->pos().x();
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

    last_x = x;
    last_y = y;

//    if(event->buttons() & Qt::LeftButton){
//        int y = height-event->pos().y();
//        mouseMotion(event->pos().x(),y,height,width);
//        if (trackingMouse) {
//            Quaternion qnew;
//            qnew.setQuaternion(cos(0.25*angletrack*M_PI/(360.0)),Vec4(axis[0],axis[1],axis[2]).unitary()*sin(0.25*angletrack*M_PI/(360.0)));
//            qnew.normalize();
//            q = qnew*0.5 + q*0.5;
//            cam_eye = q.getMatrix().vector(cam_eye);
//            updateCamera();
//        }

//    }

}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
//    move = false;
//    trackingMouse = false;
//    int y = height-event->pos().y();
//    stopMotion(event->pos().x(),y);
//    q.setQuaternion(1,0,0,0);
    // if the left button is pressed
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
//    if(event->buttons() & Qt::LeftButton)
//    {
//        trackingMouse = true;
//        int y = height-event->pos().y();
//        startMotion(event->pos().x(),y,height,width);
//    }else{
//        trackingMouse = false;
//        int y = height-event->pos().y();
//        stopMotion(event->pos().x(),y);

//    }
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

}

void GLWidget::simulationRestart()
{
    scene->restartPhysics();
}


void GLWidget::keyPressEvent(QKeyEvent *event)
{
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
    if(event->key() == Qt::Key_F ){
        //Physics::bodySetTorque(scene->select->getBody(),100.0,10.0,10.0);
        //Physics::bodyAddForce(scene->select->getBody(),1,0,1);
        Vec4 *n = new Vec4(2,2,2);
        //scene->select->appTorque(n);
        //Physics::bodyAddForce(scene->select->getBody(),100.0,10.0,10.0);
        //scene->addObject(Vec4(0.5,1.0,0.5),Vec4(0,30,0),Quaternion(),TYPE_CYLINDER);
        //count++;
    }
    if(event->key() == Qt::Key_Up){
        cam_at.x2 +=0.1;
        updateCamera();
    }
    if(event->key() == Qt::Key_Down){
        cam_at.x2 -=0.1;
        updateCamera();

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



    updateObjects(scene->objectsScene());
    updateGL();
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
    updateBalancePD(scene->getKsTorqueBalance(),scene->getKdTorqueBalance(),scene->getKsForceBalance(),scene->getKdForceBalance(),scene->getKMomBalance());
    if(scene->getSizeCharacter()>0){
        scene->getCharacter(0)->contructHierarchyBodies();

        scene->getCharacter(0)->showHierarchies();
    }
}

void GLWidget::saveCharacter(QString file)
{
    Utils::saveModelRubens(scene->getCharacter(0),file.toStdString());
 //Functions::saveScene(scene,file);
}

void GLWidget::loadMotionCapture(QString file)
{
    if (scene->getSizeCharacter()==0) return;
    Utils::loadMotionCapture(scene->getCharacter(0)->getMoCap(),scene->getCharacter(0),file.toStdString());
    //int frames = scene->getCharacter(0)->getMoCap()->sizeFrames();
    //printf("\nTamanho de frames: %d",frames);
    scene->getCharacter(0)->loadMotionFrames();
    motionTotalFrame(scene->getCharacter(0)->getMoCap()->sizeFrames());
    scene->getCharacter(0)->getMoCap()->copyFootsProperties();

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
        scene->getCharacter(0)->getMoCap()->initializePosesModel(1);
    }
    //Utils::loadMotionCapture(scene->getCharacter(0)->getMoCap(),scene->getCharacter(0),file.toStdString());


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

void GLWidget::setBalanceControl(Vec4 ksT, Vec4 kdT, Vec4 ksF, Vec4 kdF, Vec4 kmom)
{
    scene->setKdForceBalance(kdF);
    scene->setKsForceBalance(ksF);
    scene->setKdTorqueBalance(kdT);
    scene->setKsTorqueBalance(ksT);
    scene->setKMomBalance(kmom);
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

void GLWidget::setAlphaCharacter(int value)
{
    scene->setAlphaCharacter((float)value/100.0);
}

void GLWidget::setWireCharacter(bool wire)
{
    scene->setWireCharacter(wire);
}

void GLWidget::setCompensationBalance(int val)
{
    scene->setCompensacao(val);
}

void GLWidget::setAngleBodyBalance(Vec4 v)
{
   scene->setAngleBodyBalance(v);
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
    for (int i=0;i<objs.size();i++) objs.at(i)->setSelected(false);
    if (row<0) return;
    objs.at(row)->setSelected(true);

}

Object *GLWidget::getObject(int row)
{
    std::vector<Object*> objs = scene->objectsScene();
    if (row<0) return NULL;
    return objs.at(row);
}

void GLWidget::setJointSelected(int row)
{
    std::vector<Joint*> jts = scene->jointsScene();
    for (int i=0;i<jts.size();i++) jts.at(i)->setSelected(false);
    if (row<0){
        showJoint(NULL);
        return;
    }
    jts.at(row)->setSelected(true);
    showJoint(jts.at(row));
}
