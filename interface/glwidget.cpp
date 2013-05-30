#include "glwidget.h"
#include <GL/glu.h>
#include "scene/object.h"
#include "graphics/draw.h"
#include "math/quaternion.h"

int count = 0;
Quaternion q;
//bool move = false;
float angle = 45;
int last_pox_x,last_pox_y;
int id_material;
int state_key;
//Vec4 eye,at;
Vec4 cam_eye = Vec4(10,20,0);
Vec4 cam_at  = Vec4(0,0,0);

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
    connect(&simTimer, SIGNAL(timeout()), this, SLOT(simStep()));
    simTimer.start(1);
    move = false;
    sim_pause = false;


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
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(angle,(float)w/h,0.01,12000000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cam_eye.x(),cam_eye.y(),cam_eye.z(),cam_at.x(),cam_at.y(),cam_at.z(),0,1,0);
    height = h;
    width  = w;



}

void GLWidget::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

    scene->draw();
    Draw::drawGround(100);

    glPopMatrix();




}

void GLWidget::updateCamera()
{

    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(angle,(float)width/(float)height,0.01,12000000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cam_eye.x(),cam_eye.y(),cam_eye.z(),cam_at.x(),cam_at.y(),cam_at.z(),0,1,0);

}

void GLWidget::simStep(){
    if(!sim_pause)
        scene->simulationStep();
    update();

}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;
    if (numSteps>0 && angle<180.0){
        angle += 1;
    }
    else{
        if (!(angle<=5)){
            angle -= 1;
        }
    }
    updateCamera();


}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{


    if(event->buttons() & Qt::LeftButton){
        int y = height-event->pos().y();
        mouseMotion(event->pos().x(),y,height,width);
        if (trackingMouse) {
            Quaternion qnew;
            qnew.setQuaternion(cos(0.25*angletrack*M_PI/(360.0)),Vec4(axis[0],axis[1],axis[2]).unitary()*sin(0.25*angletrack*M_PI/(360.0)));
            qnew.normalize();
            q = qnew*0.5 + q*0.5;
            cam_eye = q.getMatrix().vector(cam_eye);
            updateCamera();
        }

    }

}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    move = false;
    trackingMouse = false;
    int y = height-event->pos().y();
    stopMotion(event->pos().x(),y);
    q.setQuaternion(1,0,0,0);

}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        trackingMouse = true;
        int y = height-event->pos().y();
        startMotion(event->pos().x(),y,height,width);
    }else{
        trackingMouse = false;
        int y = height-event->pos().y();
        stopMotion(event->pos().x(),y);

    }


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
    if(event->key() == Qt::Key_C ){
        scene->addObject(Vec4(1,1,1),Vec4(0,30,0),Quaternion(),TYPE_CUBE);
        //count++;
    }
    if(event->key() == Qt::Key_S ){
        scene->addObject(Vec4(0.5,0.5,0.5),Vec4(0,30,0),Quaternion(),TYPE_SPHERE);
        //count++;
    }
    if(event->key() == Qt::Key_Y ){
        scene->addObject(Vec4(0.5,1.0,0.5),Vec4(0,30,0),Quaternion(),TYPE_CYLINDER);
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
    updateObjects(scene->objectsScene());
    updateGL();
}

void GLWidget::SimStepsSimulation(int steps)
{
    scene->setSimStep(steps);
}

void GLWidget::applyForce(Vec4 force)
{
    scene->applyForce(force);
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
