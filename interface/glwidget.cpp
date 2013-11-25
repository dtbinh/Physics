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
#include "interpolation/interpolation.h"

/************** New Camera *****************/
//static int slices = 16;
//static int stacks = 16;

//static float ax = 0.0;
//static float ay = 0.0;
//static float az = 0.0;

//static float delta = 5.0;

static bool lbpressed = false;
//static bool mbpressed = false;
static bool rbpressed = false;

static float last_x = 0.0;
static float last_y = 0.0;

Camera* cam = new Camera();
static float savedCamera[9];

//
int time_current;
int time_m;

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
int ciclo_arrow = 0;
int angle_quat = 0;
int angle_direction = 0;
bool shadow = false;
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

GLfloat floorShadow[4][4];
GLfloat floorPlane[4] = {0,1.,0,0};
GLfloat lightPosition[4] = {   9.588, 9.46, 9.248, 0.0 };

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
    editing_frame = false;
    frame_edit = 0;
    show_character = true;
    load_exemple_curve = false;
    time_current = 0;
    updateKsProp(scene->getProportionalKsPD());
    updateKdProp(scene->getProportionalKdPD());
    updateBalancePD(scene->getKsTorqueBalance(),scene->getKdTorqueBalance(),scene->getKsForceBalance(),scene->getKdForceBalance(),scene->getKMomLinearBalance(),scene->getKMomAngularBalance());

}

void GLWidget::initializeGL()
{

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    //glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    //glShadeModel(GL_SMOOTH);
    //glEnable(GL_COLOR_MATERIAL);

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
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_FUNC);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);

    //glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_RESCALE_NORMAL);
    //glLineWidth(1.2);
    glDepthFunc(1.0);

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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glLoadIdentity();
    //determinação da camera
    glPushMatrix();
        glTranslated(-0.20,-0.135,-0.6);
        gluLookAt(cam->eye.x(),cam->eye.y(),cam->eye.z(), cam->at.x(),cam->at.y(),cam->at.z(), cam->up.x(),cam->up.y(),cam->up.z());
        glTranslated(cam->eye.x(),cam->eye.y(),cam->eye.z());
        Draw::drawAxisCameraView(0.02);
    glPopMatrix();

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
        if (ciclo>5){
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
    Draw::drawGround(10);
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
    if(event->key() == Qt::Key_B ){
        if((scene->getSizeCharacter()<1)) return;
        scene->shotBallsCharacterRandom(scene->getCharacter(0),7);
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
    if(event->key() == Qt::Key_Left){
        angle_direction=angle_direction-1;
        setAngleDirection(angle_direction);
    }
    if(event->key() == Qt::Key_Right){
        angle_direction=angle_direction+1;
        setAngleDirection(angle_direction);

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

MoCap *GLWidget::pushMotionCapture()
{
    if (scene->getSizeCharacter()<1) return NULL;
    return scene->getCharacter(0)->getMoCap();
}

void GLWidget::drawShadows()
{
    Draw::shadowMatrix(floorShadow, floorPlane, lightPosition);

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
    Quaternion q;
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

void GLWidget::setAlphaCharacter(int value)
{
    scene->setAlphaCharacter((float)value/100.0);
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



}
