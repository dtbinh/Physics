#include "glwidget.h"
#include "scene/scene.h"
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
#include "cameraold.h"
#include "interpolation/interpolation.h"
#include "extra/screenshot.h"
#include "extra/text.h"
#include <QPainter>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "math/vector3d.h"
#include "extra/materialobj.h"
#include "graphics/ShaderPrimitives/cube.h"
#include "graphics/ShaderPrimitives/material.h"
#include "graphics/ShaderPrimitives/camera.h"
#include "graphics/shader.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


//  To use functions with variables arguments
#include <stdarg.h>

//  for malloc
#include <stdlib.h>

#include <cmath>
// Options
GLboolean shadows = true;

const GLuint SHADOW_WIDTH = 2*1024, SHADOW_HEIGHT = 2*1024;

static int stencilReflection = 1, stencilShadow = 1;
static int  renderReflection = 1;

GLfloat lightZeroPosition[] = {10.0, 14.0, 10.0, 1.0};
GLfloat lightZeroColor[] = {0.8, 1.0, 0.8, 1.0}; /* green-tinted */
GLfloat lightOnePosition[] = {-1.0, 1.0, 1.0, 0.0};
GLfloat lightOneColor[] = {0.6, 0.3, 0.2, 1.0}; /* red-tinted */
GLfloat skinColor[] = {0.1, 1.0, 0.1, 1.0}, eyeColor[] = {1.0, 0.2, 0.2, 1.0};

bool enable_balance=true;
vector3d lightPosition(-1.93849,11.233,21.9049);
vector3d lightDirection(-26.4,355.2);


Shader debugDepthQuad("../shaders/debug_quad.vert", "../shaders/debug_quad_depth.frag");
Shader simpleDepthShader("../shaders/shadow_mapping_depth.vert", "../shaders/shadow_mapping_depth.frag");
Shader shader("../shaders/shadow_mapping.vs", "../shaders/shadow_mapping.frag");




#define SHOTBALL 3

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
CameraOld* cam = new CameraOld();
static float savedCamera[9];

Vec4 vector_draw_direction;

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

MaterialPtr GLWidget::createMaterial()
{
    // Create a material and set the shaders
    MaterialPtr material( new Material );
    material->setShaders( "../shaders/phong.vert",
                          "../shaders/phong.frag" );
    return material;
}

void GLWidget::renderScene()
{
    Matrix4x4 *transform = new Matrix4x4();
    MaterialObj* mate = new MaterialObj();
    mate->setMaterial(mate,MATERIAL_RUBY);
    //transform->scale(0.5,0.5,0.5);
    transform->setIdentity();
//    scene->drawCylinder(transform,mate);
//    transform->setIdentity();
//    transform->scale(2,1.0,1.5);
//    transform->translate(-2,1,0);
    mate->setMaterial(mate,MATERIAL_COPPER);
    scene->drawCube(transform,mate);
    transform->setIdentity();
    transform->scale(1.0,1.0,1.0);
    transform->translate(0,0,2.5);
//    mate->setMaterial(mate,MATERIAL_EMERALD);
//    scene->drawSphere(transform,mate);
    transform->setIdentity();
    transform->scale(20.0,1.0,20.0);
    transform->translate(0.0,-1.0,0.0);
    mate->setMaterial(mate,MATERIAL_GOLD);
    scene->drawPlane(transform,mate);
    delete transform;
    delete mate;
}

void GLWidget::renderSceneShadow()
{
    Matrix4x4 *transform = new Matrix4x4();
    MaterialObj* mate = new MaterialObj();
    mate->setMaterial(mate,MATERIAL_RUBY);
    //transform->scale(0.5,0.5,0.5);
    transform->setIdentity();
//    scene->drawCylinder(transform,mate);
//    transform->setIdentity();
//    transform->scale(2,1.0,1.5);
//    transform->translate(-2,1,0);
    transform->setIdentity();
    transform->scale(1.0,1.0,1.0);
    transform->translate(0,1.5,0);
    mate->setMaterial(mate,MATERIAL_RUBY);
    scene->drawCubeShader(transform);


    transform->setIdentity();
    transform->scale(1.0,1.0,1.0);
    transform->translate(2,0,1);
    mate->setMaterial(mate,MATERIAL_TURQUOSIE);
    scene->drawCubeShader(transform);

    transform->setIdentity();

    //transform->scale(0.5,0.5,0.5);
    //transform->setTranslate(Vec4(-1,0,2,1.0));

    transform->scale(0.5,0.5,0.5);
    transform->setRotationX(60);
    transform->setRotationZ(60);
    transform->setTranslate(Vec4(-1,0,2,1.0));

    //transform->setRotationX(60);
    //transform->setRotationZ(60);

    mate->setMaterial(mate,MATERIAL_TURQUOSIE);
    scene->drawCubeShader(transform);


//    mate->setMaterial(mate,MATERIAL_EMERALD);
//    scene->drawSphere(transform,mate);
    transform->setIdentity();
    transform->scale(25.0,1.0,25.0);
    transform->translate(0.0,-0.5,0.0);
    mate->setMaterial(mate,MATERIAL_GOLD);
    scene->drawPlaneShader(transform);
    delete transform;
    delete mate;
}

void GLWidget::renderSceneWithShadow()
{
    Matrix4x4 *transform = new Matrix4x4();
    MaterialObj* mate = new MaterialObj();
    mate->setMaterial(mate,MATERIAL_RUBY);
    //transform->scale(0.5,0.5,0.5);
    transform->setIdentity();
//    scene->drawCylinder(transform,mate);
//    transform->setIdentity();
//    transform->scale(2,1.0,1.5);
//    transform->translate(-2,1,0);
    transform->setIdentity();
    transform->scale(1.0,1.0,1.0);
    transform->translate(0,1.5,0);
    mate->setMaterial(mate,MATERIAL_RUBY);
    scene->drawCubeShader2(transform,mate);


    transform->setIdentity();
    transform->scale(1.0,1.0,1.0);
    transform->translate(2,0,1);
    mate->setMaterial(mate,MATERIAL_TURQUOSIE);
    scene->drawCubeShader2(transform,mate);

    transform->setIdentity();

    transform->scale(0.5,0.5,0.5);
    transform->setRotationX(60);
    transform->setRotationZ(60);
    transform->setTranslate(Vec4(-1,0,2,1.0));


    //transform->setRotationX(60.f);
    //transform->setRotationZ(60);




    mate->setMaterial(mate,MATERIAL_TURQUOSIE);
    scene->drawCubeShader2(transform,mate);


//    mate->setMaterial(mate,MATERIAL_EMERALD);
//    scene->drawSphere(transform,mate);
    transform->setIdentity();
    transform->scale(25.0,1.0,25.0);
    transform->translate(0.0,-0.5,0.0);
    mate->setMaterial(mate,MATERIAL_GOLD);
    scene->drawPlaneShader2(transform,mate);
    delete transform;
    delete mate;
}

GLuint quadVAO = 0;
GLuint quadVBO;
void GLWidget::RenderQuad()
{
    if (quadVAO == 0)
    {
        GLfloat quadVertices[] = {
            // Positions        // Texture Coords
            -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
             1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
             1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
        };
        // Setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void GLWidget::configureShaderAndMatrices()
{

}

void GLWidget::prepareShaderProgram()
{
    // Load and compile the vertex shader
    if ( !m_program.addShaderFromSourceFile( QOpenGLShader::Vertex, "../shaders/basic.vert" ) )
        qCritical() << "Could not compile vertex shader. Log:" << m_program.log();

    // Load and compile the fragment shader
    if ( !m_program.addShaderFromSourceFile( QOpenGLShader::Fragment, "../shaders/basic.frag" ) )
        qCritical() << "Could not compile fragment shader. Log:" << m_program.log();

    // Link the shaders together into a complete shader program (pipeline)
    if ( !m_program.link() )
        qCritical() << "Could not link shader program. Log:" << m_program.log();
}

void GLWidget::prepareVertexBuffers()
{

    // The data for our triangle
    float positionData[] = {
        -0.8f, -0.8f, 1.0f,
         0.8f, -0.8f, 1.0f,
         0.0f,  0.8f, 1.0f
    };
    float colorData[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };

    //
    // Populate the buffer objects
    //

    // The vertex positions
    m_vertexPositionBuffer.create();
    m_vertexPositionBuffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
    m_vertexPositionBuffer.bind();
    m_vertexPositionBuffer.allocate( positionData, 3 * 3 * sizeof( float ) );

    // The vertex colors
    m_vertexColorBuffer.create();
    m_vertexColorBuffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
    m_vertexColorBuffer.bind();
    m_vertexColorBuffer.allocate( colorData, 3 * 3 * sizeof( float ) );

    //
    // Associate our VBO's with input attribute variables in the vertex shader
    //

    // Make sure our shader is the currently active one
    m_program.bind();

    // Bind the position buffer, switch it "on", tell OpenGL what the data format is
    m_vertexPositionBuffer.bind();
    m_program.enableAttributeArray( "vertexPosition" );
    m_program.setAttributeBuffer( "vertexPosition", GL_FLOAT, 0, 3 );

    // Bind the color buffer, switch it "on", tell OpenGL what the data format is
    m_vertexColorBuffer.bind();
    m_program.enableAttributeArray( "vertexColor" );
    m_program.setAttributeBuffer( "vertexColor", GL_FLOAT, 0, 3 );
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
Vec4 cam_at  = Vec4(0,0,0);
int ciclo = 0;
int ciclo_arrow = 0;
int angle_quat = 0;
int angle_directiony = 0;
int angle_directionx = 0;
int angle_directionz = 0;
bool shadow = false;
//variaveis de camera
//int width,height;



GLfloat floorShadow[4][4];
GLfloat floorPlane[4] = {0,1.,0,0};
GLfloat lightPosition7[4] = {   9.588, 9.46, 9.248, 1.0 };


GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers),parent),
          m_camera( new Camera( this ) )
{

    QGLFormat glFormat;
    glFormat.setVersion( 3, 3 );
    glFormat.setProfile( QGLFormat::CoreProfile ); // Requires >=Qt-4.8.0
    glFormat.setSampleBuffers( true );
    glFormat.setSamples(16);

    this->setFormat(glFormat);

    // Initialize the camera position and orientation
    m_camera->setPosition( QVector3D( 5.0f, 5.0f, 5.0f ) );
    m_camera->setViewCenter( QVector3D( 0.0f, 0.0f, 0.0f ) );
    m_camera->setUpVector( QVector3D( 0.0f, 1.0f, 0.0f ) );


    setFocusPolicy(Qt::StrongFocus);
   glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
////    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_MULTISAMPLE);
////    glEnable(GL_STENCIL_FUNC);
    scene = new Scene(this);
    scene->setCamera(m_camera);
    //scene->initializeShaders();


//    updateObjects(scene->objectsScene());
//    updateJoints(scene->jointsScene());
//    updatePoseControls(scene->poseControlsScene());
    simTimer = new QTimer(this);
    connect(simTimer, SIGNAL(timeout()), this, SLOT(simStep()));


//    //simTimer->start(0);
//    //simTimer->setInterval(0);

//    move = false;
//    sim_pause = false;
//    capture_pause = true;
//    editing_frame = false;
//    frame_edit = 0;
//    show_character = true;
//    load_exemple_curve = false;
//    showInfo = false;
//    screenshot = false;
//    frames = 0;
//    time_current = 0;
//    updateKsProp(scene->getProportionalKsPD());
//    updateKdProp(scene->getProportionalKdPD());
//    updateBalancePD(scene->getKsTorqueBalance(),scene->getKdTorqueBalance(),scene->getKsForceBalance(),scene->getKdForceBalance(),scene->getKMomLinearBalance(),scene->getKMomAngularBalance());
//    density = 0.5; //massa
//    velocity = 5.;

//    mass_suitcase = 1.0;
//    has_ball_shot = false;
//    ball_shot_debug = Vec4();
//    frames_force = 6;





    //exit(1);


    //scene->createCharacter();

    //scene->createCharacter();
    //scene->createLuxo();
    //scene->createLuxo2();
    //controlLuxo = false;

}

void GLWidget::initializeGL()
{
    // Create a material that performs multi-texturing
//    MaterialPtr material = createMaterial();

//    // Create a cube and set the material on it
//    m_cube = new Cube( this );
//    m_cube->setMaterial( material );
//    m_cube->create();


    // Enable depth testing
    glEnable( GL_DEPTH_TEST );

    // Set the clear color to white
    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
    m_modelMatrix.setToIdentity();
    scene->initializeShaders();


    glGenFramebuffers(1, &depthMapFBO);

    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);



//        // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
//         GLuint FramebufferName = 0;
//         glGenFramebuffers(1, &FramebufferName);
//         glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

//         // Depth texture. Slower than a depth buffer, but you can sample it later in your shader
//         GLuint depthTexture;
//         glGenTextures(1, &depthTexture);
//         glBindTexture(GL_TEXTURE_2D, depthTexture);
//         glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, 1024, 1024, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

//         glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

//         glDrawBuffer(GL_NONE); // No color buffer is drawn to.

//         // Always check that our framebuffer is ok
//         if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE);


//    // Create a vertex array object (VAO) - more on this later!
//    m_vao.create();
//    m_vao.bind();

//    // Load, compile and link the shader program
//    prepareShaderProgram();

//    // Prepare our geometry and associate it with shader program inputs
//    prepareVertexBuffers();


//    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
//    //glEnable(GL_MULTISAMPLE_ARB);
//    //glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
//    glEnable(GL_DEPTH_TEST);
//    //glEnable(GL_CULL_FACE);

//    glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
//    //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);				// Black Background
//    glClearDepth(1.0f);									// Depth Buffer Setup
//    glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
//    glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
//    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
//    glShadeModel(GL_SMOOTH);
//    //glEnable(GL_COLOR_MATERIAL);

//    static GLfloat light1pos[4] = {  -0.892, 0.3, 0.9, 0.0 };
//    static GLfloat light1diffuse[] = { 0.8f, 0.8f, 0.8, 1.0f };
//    static GLfloat light1specular[] = { 0.5f, 0.5f, 0.5f, 0.0f };

//    static GLfloat light2pos[4] = { 9.588, 9.46, 9.248, 0.0 };
//    static GLfloat light2diffuse[] = { 0.498f, 0.5f, 0.6, 1.0f };
//    static GLfloat light2specular[] = { 0.2f, 0.2f, 0.2f, 0.0f };

//    static GLfloat light3pos[4] = { 0.216, -0.392, -0.216, 0.0 };
//    static GLfloat light3diffuse[] = { 0.798f, 0.838f, 1.0, 1.0f };
//    static GLfloat light3specular[] = { 0.06f, 0.0f, 0.0f, 0.0f };

//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);
//    glEnable(GL_LIGHT1);
//    glEnable(GL_LIGHT2);

//    glLightfv(GL_LIGHT0, GL_POSITION, light1pos);
//    glLightfv(GL_LIGHT0, GL_DIFFUSE, light1diffuse);
//    glLightfv(GL_LIGHT0, GL_SPECULAR, light1specular);

//    glLightfv(GL_LIGHT1, GL_POSITION, light2pos);
//    glLightfv(GL_LIGHT1, GL_DIFFUSE, light2diffuse);
//    glLightfv(GL_LIGHT1, GL_SPECULAR, light2specular);

//    glLightfv(GL_LIGHT2, GL_POSITION, light3pos);
//    glLightfv(GL_LIGHT2, GL_DIFFUSE, light3diffuse);
//    glLightfv(GL_LIGHT2, GL_SPECULAR, light3specular);
////    mainShader=new shader("vertex.vs","fragment.frag");
////        quadRenderShader=new shader("quadRender.vs","quadRender.frag");
////        simpleShader=new shader("simpleShader.vs","simpleShader.frag");
////        shadowShader=new shader("shadowShader.vs","shadowShader.frag");
//#ifdef SHADERS_ENABLED
////    shaderProgram.addShaderFromSourceFile(QGLShader::Vertex, "../shaders/vertex.vs");
////    shaderProgram.addShaderFromSourceFile(QGLShader::Fragment, "../shaders/fragment.frag");
////    shaderProgram.addShaderFromSourceFile(QGLShader::Vertex, "../shaders/quadRender.vs");
////    shaderProgram.addShaderFromSourceFile(QGLShader::Fragment, "../shaders/quadRender.frag");
////    shaderProgram.addShaderFromSourceFile(QGLShader::Vertex, "../shaders/simpleShader.vs");
////    shaderProgram.addShaderFromSourceFile(QGLShader::Fragment, "../shaders/simpleShader.frag");
////    shaderProgram.addShaderFromSourceFile(QGLShader::Vertex, "../shaders/shadowShader.vs");
////    shaderProgram.addShaderFromSourceFile(QGLShader::Fragment, "../shaders/shadowShader.frag");
//    //printf("In\n");
////    shaderProgram.addShaderFromSourceFile(QGLShader::Vertex, "../shaders/phong.vert");
////    shaderProgram.addShaderFromSourceFile(QGLShader::Fragment, "../shaders/phong.frag");
//#endif

//    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
//    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
//    glEnable(GL_DEPTH_TEST);
//    glShadeModel(GL_SMOOTH);
//    glEnable(GL_DEPTH_FUNC);
//    glEnable (GL_LINE_SMOOTH);
//    glEnable (GL_POINT_SMOOTH);
//glEnable(GL_TEXTURE_2D);
//glEnable(GL_BLEND);
//    glCullFace(GL_FRONT);

//    //glEnable(GL_COLOR_MATERIAL);
////    glEnable(GL_NORMALIZE);
////    glEnable(GL_RESCALE_NORMAL);
//    //glLineWidth(1.2);
//    glDepthFunc(1.0);
//    //    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
////    glBindTexture(GL_TEXTURE_2D, 1.0);
////    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
//    //glutInitContextFlags (GLUT_COMPATIBILITY_PROFILE );


}

void GLWidget::resizeGL(int w, int h)
{
//    glViewport( 0, 0, w, qMax( h, 1 ) );
//    const float ar = w>0 ? (float) w / (float) h : 1.0;

    glViewport(0, 0, w, h);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    //glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
//    gluPerspective(30.,ar,0.001,1200000.);

//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity() ;
    winWidth = w;
    winHeight = h;
//    scene->setProjection(Vec4(30.,ar,0.001,1200000));
//    scene->setWindow(w,h);
//    //printf("\nW %d H %d\n",w,h);
    float aspect = static_cast<float>( w ) / static_cast<float>( h );
    m_camera->setPerspectiveProjection( 30.0f, aspect, 0.1, 10000.0f );
    scene->setWindow(w,h);


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

    if (shadow){
        if (scene->isGroundIce()) drawReflections();
        else drawShadows();
    }
    if (!sim_pause) if (load_exemple_curve) showCurveExample();
    if (show_character){

        if(has_ball_shot){
            Draw::drawSphere(ball_shot_debug,MATERIAL_COPPER);
        }
        scene->draw();

        if(!capture_pause)
            if(scene->getSizeCharacter()!=0)
                if(scene->getCharacter(0)->getMoCap()->sizeFrames()>0)
                    motionCurrentFrame(scene->getCharacter(0)->getMoCap()->currentFrame());
        if (scene->getExternalForce().module()!=0){
            drawForceApply();
            ciclo++;
        }
        if (ciclo>=frames_force){
            scene->setExternalForce(Vec4(0,0,0));
            ciclo = 0;
        }
        if (ciclo_arrow>0){
            //Draw::drawArrow2D(angle_quat,scene->getCharacter(0)->getPosCOM());
            if(scene->getCharacter(0)->getMoCap()->status){
                Draw::drawArrow3D(scene->getCharacter(0)->getPosCOM(),scene->getCharacter(0)->getVelCOM(),Vec4(angle_directionx,angle_directiony,angle_directionz),0.3,MATERIAL_EMERALD,Vec4());
            }else{
                Draw::drawArrow3D(scene->getCharacter(0)->getPosCOM(),Vec4(),Vec4(angle_directionx,angle_directiony,angle_directionz),0.3,MATERIAL_EMERALD,Vec4());
            }
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
    Draw::drawGround(10,scene->rotate_plane,scene->isGroundIce());
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
    if ((chara->getBalance() != NULL) && (chara->getBalance()->getEnableTorque())){
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
    if ((chara->getBalance() != NULL) && (chara->getBalance()->getEnableForce())){
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
    if ((chara->getBalance() != NULL) && (chara->getBalance()->getEnableMomentum())){
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
    if (chara->getBalance() != NULL){
        x -= 20;
        glRasterPos2f(750, x);
        dados = QString().sprintf("\nCone : %3.2f %3.2f %3.2f %3.2f\n", chara->getBalance()->getMCone(),chara->getBalance()->getHeightCone(),chara->getBalance()->getRadiusCone(),chara->getBalance()->getAngleCone());
        k = 0;
        while (k<dados.size()){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,dados.at(k).toLatin1());
            k++;
        }
    }
    if (chara->getBalance() != NULL) {
        x -= 20;
        glRasterPos2f(750, x);
        dados = QString().sprintf("\nCLim : %3.2f\n", chara->getBalance()->getLimitCone());
        k = 0;
        while (k<dados.size()){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,dados.at(k).toLatin1());
            k++;
        }
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

void GLWidget::drawPoseProgression()
{
    if (! scene->getSizeCharacter()>0) {
        return;
    }
    QString dados;
    glDisable(GL_LIGHTING);
    glColor3f(0,0,0);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, winWidth, 0.0, winHeight);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    std::vector<GraphicalPose*> poseControl = this->scene->poseControlsScene();
    GraphicalPose* currPoseCont;
    int x = 20;
    int y = 680;
    if (poseControl.size() > 0){
        currPoseCont = poseControl.at(0);
        std::vector<Pose*> poses = currPoseCont->getPoses();

        for (int i = 0; i < poses.size(); i++){

            glPushAttrib(GL_CURRENT_COLOR);

            if (i == currPoseCont->getCurrentPos()) {
                //std::cout << "\ncurrent: " <<  poses.at(i)->getName().toStdString();
                glColor3f(255,0,0);
            } else if (i == currPoseCont->getNextPos()) {
                glColor3f(0,255,0);
            } else {
                glColor3f(0,0,0);
            }

            glRasterPos2f(x,y);
            dados = QString("\n%1\n").arg(poses.at(i)->getName());
            //std::cout << dados.toStdString() << "\n";

            for (int k=0; k<dados.size(); k++){
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, dados.at(k).toLatin1());
            }

            glPopAttrib();
            y -= 20;
        }

    }

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glEnable(GL_LIGHTING);

}
void GLWidget::drawForceApply()
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
    if(scene->getExternalForce().module()==0) return;
    glRasterPos2f(800, x);
    out = chara->getBalance()->getKsTorque();
    dados = QString().sprintf("%3.2f N for %3.1f sec",scene->getExternalForce().module(),frames_force/30. );
    //char* n = (char*)dados.toStdString().data();
    k = 0;
    while (k<dados.size()){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,dados.at(k).toLatin1());
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
    // Clear the buffer with the current clearing color
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


//    // 1. Render depth of scene to texture (from light's perspective)
//    // - Get light projection/view matrix.
//    glm::mat4 lightProjection, lightView;
//    glm::mat4 lightSpaceMatrix;
//    GLfloat near_plane = 1.0f, far_plane = 7.5f;
//    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
//    lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(1.0));
//    lightSpaceMatrix = lightProjection * lightView;
//    // - render scene from light's point of view
    //simpleDepthShader.Use();
//    glUniformMatrix4fv(glGetUniformLocation(simpleDepthShader.Program, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
GLfloat near_plane = 1.0f, far_plane = 7.5f;
glCullFace(GL_FRONT);

//    renderScene();
//    renderSceneShadow();
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        renderSceneShadow();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // 2. Render scene as normal
    glViewport(0, 0, winWidth, winHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //shader.Use();

//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, woodTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    renderSceneWithShadow();



    glCullFace(GL_BACK);



//    debugDepthQuad.Use();
//    glUniform1f(glGetUniformLocation(debugDepthQuad.Program, "near_plane"), near_plane);
//    glUniform1f(glGetUniformLocation(debugDepthQuad.Program, "far_plane"), far_plane);
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, depthMap);
    //RenderQuad();






//    // 1. first render to depth map
//    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
//    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
//        glClear(GL_DEPTH_BUFFER_BIT);
//        configureShaderAndMatrices();
        //renderScene();
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//    // 2. then render scene as normal with shadow mapping (using depth map)
//    glViewport(0, 0, winWidth, winHeight);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    ConfigureShaderAndMatrices();
//    glBindTexture(GL_TEXTURE_2D, depthMap);
//    RenderScene();





//    m_cube->material()->bind();
//    QOpenGLShaderProgramPtr shader = m_cube->material()->shader();

//    m_modelMatrix.setToIdentity();
//    m_modelMatrix.translate(0,0,0.5);
//    QMatrix4x4 modelViewMatrix = m_camera->viewMatrix() * m_modelMatrix;
//    QMatrix3x3 normalMatrix = modelViewMatrix.normalMatrix();
//    QMatrix4x4 mvp = m_camera->viewProjectionMatrix() * m_modelMatrix;

//    shader->setUniformValue( "modelViewMatrix", modelViewMatrix );
//    shader->setUniformValue( "normalMatrix", normalMatrix );
//    shader->setUniformValue( "projectionMatrix", m_camera->projectionMatrix() );
//    shader->setUniformValue( "mvp", mvp );

//    // Set the lighting parameters
//    shader->setUniformValue( "light.position", QVector4D( 5.0f, 5.0f, 5.0f, 1.0f ) );
//    shader->setUniformValue( "light.intensity", QVector3D( 1.0f, 1.0f, 1.0f ) );

//    // Set the material properties
//    shader->setUniformValue( "material.ka", QVector3D( 0.1f, 0.1f, 0.3f ) );
//    shader->setUniformValue( "material.kd", QVector3D( 0.0f, 0.2f, 0.9f ) );
//    shader->setUniformValue( "material.ks", QVector3D( 0.4f, 0.4f, 0.4f ) );
//    shader->setUniformValue( "material.shininess", 20.0f );

//    // Let the mesh setup the remainder of its state and draw itself
//    m_cube->render();

//    m_cube->material()->bind();


//    m_modelMatrix.setToIdentity();
//    m_modelMatrix.translate(0.5,1.5,-0.5);
//    modelViewMatrix = m_camera->viewMatrix() * m_modelMatrix;
//    normalMatrix = modelViewMatrix.normalMatrix();
//    mvp = m_camera->viewProjectionMatrix() * m_modelMatrix;

//    shader->setUniformValue( "modelViewMatrix", modelViewMatrix );
//    shader->setUniformValue( "normalMatrix", normalMatrix );
//    shader->setUniformValue( "projectionMatrix", m_camera->projectionMatrix() );
//    shader->setUniformValue( "mvp", mvp );

//    // Set the lighting parameters
//    shader->setUniformValue( "light.position", QVector4D( 5.0f, 5.0f, 5.0f, 1.0f ) );
//    shader->setUniformValue( "light.intensity", QVector3D( 1.0f, 1.0f, 1.0f ) );

//    // Set the material properties
//    shader->setUniformValue( "material.ka", QVector3D( 0.1f, 0.9f, 0.3f ) );
//    shader->setUniformValue( "material.kd", QVector3D( 0.0f, 0.2f, 0.0f ) );
//    shader->setUniformValue( "material.ks", QVector3D( 0.4f, 0.4f, 0.4f ) );
//    shader->setUniformValue( "material.shininess", 20.0f );

//    m_cube->render();



//    // Clear the color buffer
//    glClear( GL_COLOR_BUFFER_BIT );

//    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//    // Make our shader program current


//    m_program.bind();


//    glm::mat4 view;
//    view = cam->GetViewMatrix();
//    // Projection
//    glm::mat4 projection;
//    projection = glm::perspective(45.f, (GLfloat)winWidth/(GLfloat)winHeight, 0.1f, 1000.0f);
//    // Get the uniform locations
//    GLint modelLoc = glGetUniformLocation(m_program.programId(), "model");
//    GLint viewLoc = glGetUniformLocation(m_program.programId(), "view");
//    GLint projLoc = glGetUniformLocation(m_program.programId(), "projection");

//    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
//    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));



//    // Draw the triangle! Woot!
//    glDrawArrays( GL_TRIANGLES, 0, 3 );


//    m_program.release();

    //qDebug() << "update";

//    //GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
//    glLoadIdentity();

//    drawScene();
//    if(showInfo){
//        if (!screenshot) drawFPS();
//        drawParameters();
//        drawPoseProgression();
//    }

    //glutSwapBuffers();
    //glutSwapBuffers();

    //calculateFPS();
    setScreenShot();

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
//    if(!sim_pause){
//        scene->simulationStep(enable_balance);
//        update();
//        if (screenshot) setScreenShot();
//    }else{
        update();
//    }

    //calculateFPSPaint();
    //update();


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

            m_camera->rotatex(y,last_y);
            m_camera->rotatey(x,last_x);
        }
        if (!lbpressed && rbpressed) {
            cam->translatex(x,last_x);
            cam->translatey(y,last_y);

            m_camera->translatex(x,last_x);
            m_camera->translatey(y,last_y);
        }
        if (lbpressed && rbpressed) {
            cam->zoom(y,last_y);
            m_camera->zoom(y,last_y);
        }
    }

    last_x = x;
    last_y = y;
    update();


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
        update();


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
    update();
}


void GLWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_R){
        mode = SHOTBALL;
        manipulate = SHOTBALL;
        this->simulationPlayPause();
        if(has_ball_shot){
            if(mode == SHOTBALL){
                if(objselshot!=NULL){
                    scene->shotBallsCharacterBody(objselshot,velocity,ball_shot_debug,density);
                }
            }
            ball_shot_debug = Vec4();
        }
        has_ball_shot = !has_ball_shot;
        update();
        return;
    }

    if(event->key() == Qt::Key_P ){
        if(scene->getCharacter(0)->has_suitcase) scene->getCharacter(0)->deleteSuitcase();
        else  scene->getCharacter(0)->setSuitcase(3,mass_suitcase);
        updateJoints(scene->jointsScene());
    }

    if(event->key() == Qt::Key_S ){
        setScreenShot(!screenshot);
        return;
    }

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
        return;
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
         int x = ( rand() % scene->getCharacter(0)->getNumBodies() );
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
//    if(event->key() == Qt::Key_Up){
//        if(manipulate==SET_OBJECT){
//            Object *obj = scene->getCharacter(0)->getObjectSelected();
//            if (obj==NULL) return;
//            Vec4 pos = obj->getTarget();
//            pos += Vec4(0.05,0,0);
//            obj->setTarget(pos);
//            updateObject(obj);
//        }
//    }
//    if(event->key() == Qt::Key_Down){
//        if(manipulate==SET_OBJECT){
//            Object *obj = scene->getCharacter(0)->getObjectSelected();
//            if (obj==NULL) return;
//            Vec4 pos = obj->getTarget();
//            pos += Vec4(-0.05,0,0);
//            obj->setTarget(pos);
//            updateObject(obj);
//        }

//    }
    if(event->key() == Qt::Key_Left && event->modifiers()==Qt::CTRL){
        if(manipulate==SET_OBJECT){
            Object *obj = scene->getCharacter(0)->getObjectSelected();
            if (obj==NULL) return;
            Vec4 pos = obj->getTarget();
            pos += Vec4(0,0,0.05);
            obj->setTarget(pos);
            updateObject(obj);
            return;
        }else if(manipulate==SHOTBALL){
            ball_shot_debug += Vec4(0,0,0.05);
        }
        else{
            angle_directionz=angle_directionz+1;
            setAngleDirectionZ(angle_directionz);
        }
        update();
        return;
    }
    if(event->key() == Qt::Key_Right && event->modifiers()==Qt::CTRL){
        if(manipulate==SET_OBJECT){
            Object *obj = scene->getCharacter(0)->getObjectSelected();
            if (obj==NULL) return;
            Vec4 pos = obj->getTarget();
            pos += Vec4(0,0,-0.05);
            obj->setTarget(pos);
            updateObject(obj);
            return;

        }else if(manipulate==SHOTBALL){
            ball_shot_debug += Vec4(0,0,-0.05);
        }
        else{
            angle_directionz=angle_directionz-1;
            setAngleDirectionZ(angle_directionz);
        }
        update();
        return;

    }


    if(event->key() == Qt::Key_Left){
        if(manipulate==SET_OBJECT){
            Object *obj = scene->getCharacter(0)->getObjectSelected();
            if (obj==NULL) return;
            Vec4 pos = obj->getTarget();
            pos += Vec4(-0.05,0,0);
            obj->setTarget(pos);
            updateObject(obj);
            return;
        }else if(manipulate==SHOTBALL){
            ball_shot_debug += Vec4(-0.05,0,0);
        }
        else{
            angle_directiony=angle_directiony+1;
            setAngleDirectionY(angle_directiony);
        }
    }
    if(event->key() == Qt::Key_Right){
        if(manipulate==SET_OBJECT){
            Object *obj = scene->getCharacter(0)->getObjectSelected();
            if (obj==NULL) return;
            Vec4 pos = obj->getTarget();
            pos += Vec4(0.05,0,0);
            obj->setTarget(pos);
            updateObject(obj);
            return;
        }else if(manipulate==SHOTBALL){
            ball_shot_debug += Vec4(0.05,0,0);
        }
        else{
            angle_directiony=angle_directiony-1;
            setAngleDirectionY(angle_directiony);
        }
    }

    if(event->key() == Qt::Key_Up){
        if(manipulate==SET_OBJECT){
            Object *obj = scene->getCharacter(0)->getObjectSelected();
            if (obj==NULL) return;
            Vec4 pos = obj->getTarget();
            pos += Vec4(0,0.05,0);
            obj->setTarget(pos);
            updateObject(obj);
            return;
        }else if(manipulate==SHOTBALL){
            ball_shot_debug += Vec4(0,0.05,0);
        }
        else{
            angle_directionx=angle_directionx-1;
            setAngleDirectionX(angle_directionx);
        }
    }
    if(event->key() == Qt::Key_Down){
        if(manipulate==SET_OBJECT){
            Object *obj = scene->getCharacter(0)->getObjectSelected();
            if (obj==NULL) return;
            Vec4 pos = obj->getTarget();
            pos += Vec4(0,-0.05,0);
            obj->setTarget(pos);
            updateObject(obj);
            return;
        }else if(manipulate==SHOTBALL){
            ball_shot_debug += Vec4(0,-0.05,0);
        }
        else{
            angle_directionx=angle_directionx+1;
            setAngleDirectionX(angle_directionx);
        }

    }
    if(event->key() == Qt::Key_L){
        static int posCam = 0;
        posCam++;
        if (cam->type == CAMERA_FAR) {
            delete cam;
            if (posCam%5==0) cam = new CameraOld(); //CameraDistante(0,1,5, 0,1,0, 0,1,0);
            if (posCam%5==1) cam = new CameraOld(5,1,0, 0,1,0, 0,1,0);
            if (posCam%5==2) cam = new CameraOld(0,1,-5, 0,1,0, 0,1,0);
            if (posCam%5==3) cam = new CameraOld(-5,1,0, 0,1,0, 0,1,0);
            if (posCam%5==4) cam = new CameraOld(savedCamera[0],savedCamera[1],savedCamera[2],savedCamera[3],savedCamera[4],savedCamera[5],savedCamera[6],savedCamera[7],savedCamera[8]);
        } else if (cam->type == CAMERA_GAME) {
            delete cam;
            cam = new CameraOld();
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
    update();
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
        Draw::drawGround(10,scene->getRotationPlane());

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

void GLWidget::drawReflections()
{
    Draw::shadowMatrix(floorShadow, floorPlane, lightPosition7);

      glPushMatrix();

        if (renderReflection) {
          /* We can eliminate the visual "artifact" of seeing the "flipped"
             dinosaur underneath the floor by using stencil.  The idea is
               draw the floor without color or depth update but so that
               a stencil value of one is where the floor will be.  Later when
               rendering the dinosaur reflection, we will only update pixels
               with a stencil value of 1 to make sure the reflection only
               lives on the floor, not below the floor. */

          /* Don't update color or depth. */
          glDisable(GL_DEPTH_TEST);
          glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

          /* Draw 1 into the stencil buffer. */
          glEnable(GL_STENCIL_TEST);
          glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
          glStencilFunc(GL_ALWAYS, 1, 0xffffffff);

          /* Now render floor; floor pixels just get their stencil set to 1. */
          Draw::drawGround(10,scene->rotate_plane,scene->isGroundIce());

          /* Re-enable update of color and depth. */
          glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
          glEnable(GL_DEPTH_TEST);

          /* Now, only render where stencil is set to 1. */
          glStencilFunc(GL_EQUAL, 1, 0xffffffff);  /* draw if ==1 */
          glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

          glPushMatrix();

            /* The critical reflection step: Reflect dinosaur through the floor
               (the Y=0 plane) to make a relection. */
            glScalef(1.0, -1.0, 1.0);

            /* Enable front face culling. */
            glCullFace(GL_FRONT);

            /* Draw the reflected dinosaur. */
            scene->drawShadows();
              //drawReflections();

            /* Re-enable back face culling. */
            glCullFace(GL_BACK);

          glPopMatrix();

          glDisable(GL_STENCIL_TEST);
        }

        /* Back face culling will get used to only draw either the top or the
           bottom floor.  This let's us get a floor with two distinct
           appearances.  The top floor surface is reflective and kind of red.
           The bottom floor surface is not reflective and blue. */

        /* Draw "bottom" of floor in blue. */
        glFrontFace(GL_CW);  /* Switch face orientation. */
        //retira a transparencia da parte de baixo do chao (tornando-se opaca)
        //glDisable(GL_LIGHTING);
        //glColor4f(1.0, 1.0, 1.0, 1.0);
        Draw::drawGround(10,scene->rotate_plane,scene->isGroundIce());
        //glEnable(GL_LIGHTING);
        glFrontFace(GL_CCW);

        if (1) {
            /* Draw the floor with stencil value 3.  This helps us only
               draw the shadow once per floor pixel (and only on the
               floor pixels). */
            glEnable(GL_STENCIL_TEST);
            glStencilFunc(GL_ALWAYS, 3, 0xffffffff);
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        }

        /* Draw "top" of floor.  Use blending to blend in reflection (jah foi habilitado o blend). */
        Draw::drawGround(10,scene->rotate_plane,scene->isGroundIce());

        if (1) {

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
          glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(-2.0, -1.0);

          /* Render 50% black shadow color on top of whatever the
             floor appareance is. */
          glDisable(GL_LIGHTING);  /* Force the 20% black. */
          glColor4f(0.0, 0.0, 0.0, 0.5);

          glPushMatrix();
            /* Project the shadow. */
            glMultMatrixf((GLfloat *) floorShadow);
            scene->drawShadows();
          glPopMatrix();

          glEnable(GL_LIGHTING);

          //glDisable(GL_POLYGON_OFFSET_EXT);
          glDisable(GL_POLYGON_OFFSET_FILL);

          glDisable(GL_STENCIL_TEST);
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

    setAngleDirectionY(q.toEuler().y());
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
      //qDebug()<< "width: " << winWidth <<"height: "<< winHeight;

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
    update();
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

Scene *GLWidget::getScene()
{
    return this->scene;
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

void GLWidget::setGravityCompensation(int value)
{
    if(scene->getSizeCharacter()<1) return;
    scene->getCharacter(0)->getBalance()->setCompensationGravity(value/100.);
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

void GLWidget::updateAngleGround(Vec4 ang)
{
    scene->setRotationPlane(ang);
    Vec4 normal = scene->getRotationPlaneVector();
    floorPlane[0] = normal.x();
    floorPlane[1] = normal.y();
    floorPlane[2] = normal.z();
    update();
}

void GLWidget::setMassSuitcase(double val)
{
    mass_suitcase = val;
    //verifica se exite alguma maleta para reiniciar com o novo peso
    if(scene->getCharacter(0)->has_suitcase){
        scene->getCharacter(0)->deleteSuitcase();
        scene->getCharacter(0)->setSuitcase(3,mass_suitcase);
    }
    updateJoints(scene->jointsScene());
}

void GLWidget::setFramesForce2Time(double val)
{
    this->frames_force = (int)(val*30.);
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
   vector_draw_direction = v;
   angle_quat = v.y();
   ciclo_arrow = 1;
   angle_directiony = v.y();
   angle_directionx = v.x();
   angle_directionz = v.z();
}



std::vector<Object*> GLWidget::getObjectsList()
{
    return this->scene->objectsScene();
}

std::vector<Joint*> GLWidget::getJointsList()
{
    return this->scene->jointsScene();
}

std::vector<GraphicalPose *> GLWidget::getPoseControlList()
{
    return this->scene->poseControlsScene();
}

std::vector<Pose *> GLWidget::getPoseList()
{
    return this->scene->posesScene();
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

/*void GLWidget::setPoseControlSelected(int row)
{
    showPoseControl(scene->poseControlsScene().at(row));
}*/

void GLWidget::loadSimulationParameters(QString file)
{
    Utils::loadSimulationConfig(scene,file.toStdString());
    updateKsProp(scene->getProportionalKsPD());
    updateKdProp(scene->getProportionalKdPD());
    updateObjects(scene->objectsScene());
    updateJoints(scene->jointsScene());
    updateBalancePD(scene->getKsTorqueBalance(),scene->getKdTorqueBalance(),scene->getKsForceBalance(),scene->getKdForceBalance(),scene->getKMomLinearBalance(),scene->getKMomAngularBalance());
    setGravity(scene->getGravity());
    setEnableGravity(scene->hasGravity());
    for(int i=0;i<scene->getSizeCharacter();i++){
        scene->getCharacter(i)->contructHierarchyBodies();
        if(scene->getCharacter(i)->getMoCap()->sizeFrames()>0){
            motionTotalFrame(scene->getCharacter(i)->getMoCap()->sizeFrames());
        }
        updateBalanceCone(scene->getCharacter(i)->getBalance()->getMCone(),scene->getCharacter(i)->getBalance()->getAngleCone(),scene->getCharacter(i)->getBalance()->getRadiusCone(),scene->getCharacter(i)->getBalance()->getHeightCone());

    }
    setToleranceFoot(scene->getCharacter(0)->getBalance()->getSensorTolerance());


    cam->eye = scene->getEye();
    cam->at = scene->getAt();
    cam->up = scene->getUp();
    //scene->createArena();





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

void GLWidget::setFrictionGround(double friction)
{
    this->scene->setFrictionGround(friction);
    update();
}

void GLWidget::setFrictionFootAir(double friction)
{
    this->scene->setFrictionFootAir(friction);
    update();
}

void GLWidget::updateMotionPosition(int i)
{
    scene->getCharacter(0)->getMoCap()->setCurrentFrame(i);
    scene->setFrameCurrent(i);
    if(scene->getCharacter(0)->offset.module()==0) scene->getCharacter(0)->getMoCap()->initializePosesModel(i);
}

