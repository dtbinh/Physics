#ifndef GLWIDGET_H
#define GLWIDGET_H
#include <QTimer>
#include <QWidget>
#include "scene/scene.h"
#include <QGLWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QOpenGLContext>
#include <QOpenGLShaderProgram>
#include "graphics/ShaderPrimitives/abstractscene.h"
#include "graphics/ShaderPrimitives/material.h"

#include <QMatrix4x4>

class Cube;
class Camera;
class GLWidget : public QGLWidget
{
    Q_OBJECT
signals:
    void updateObjects(std::vector<Object*>);
    void updateJoints(std::vector<Joint*>);
    void updatePoseControls(std::vector<GraphicalPose*>);
    void updatePoses(std::vector<Pose*>);
    void updateObject(Object*);
    void updateKsProp(Vec4);
    void updateKdProp(Vec4);
    void updateBalancePD(Vec4,Vec4,Vec4,Vec4,Vec4,Vec4);
    void updateBalanceCone(float,float,float,float);
    void showJoint(Joint*);
    void motionCurrentFrame(int);
    void motionTotalFrame(int);
    void setSliderFoot1(int);
    void setSliderFoot2(int);
    void plusFrameEdition();
    void minusFrameEdition();
    void setForceCharacter();
    void setBeginClycleWidget(int);
    void setEndClycleWidget(int);
    void setGravity(Vec4 v);
    void setEnableGravity(bool);
    void setAngleDirectionY(int);
    void setAngleDirectionX(int);
    void setAngleDirectionZ(int);
    void setToleranceFoot(double);

public slots:

    Scene* getScene();

    //slots edição
    void setScreenShot(bool b);
    void setRenderMesh(bool b);
    void setShowInfos(bool b);

    //fricção

    void setFrictionGround(double friction);
    void setFrictionFootAir(double friction);


    //slots motion capture

    void updateMotionPosition(int pos);
    void loadMotionCapture(QString file);
    void loadFramesConfig(QString file);
    void saveFramesConfig(QString file);
    void setPlayback(bool val);
    void restartMotion();
    void showEditingFrame(bool b);
    void setEditingFrame(int frame);
    void setBeginClycle(int v);
    void setEndClycle(int v);
    void setStepsInterpolation(double val);

    //slots scene
    void saveSimulationParameters(QString file);
    void loadSimulationParameters(QString file);

    //slots parametros físicos
    void SimStepsSimulation(int steps);
    void stopSimulation();
    void startSimulation();
    void applyForce(Vec4 force);
    void setGravityParameters(Vec4 g);
    void setSimbiconForceParameters(Vec4 g);
    void setSimbiconDistanceParameters(Vec4 g);
    void setGravity(bool b);



    //slots parametros de controle

    void setProportionalKs(Vec4 ks);
    void setProportionalKd(Vec4 kd);
    void setBalanceControl(Vec4 ksT,Vec4 kdT,Vec4 ksF, Vec4 kdF,Vec4 kmomlin,Vec4 kmomang);
    void setEnableTorqueBalance(bool b);
    void setEnableForceBalance(bool b);
    void setCompensationBalance(int val);
    void setAngleBodyBalance(Vec4 v);
    void setEnableMomentumBalance(bool b);
    void setKsRelationshipKs(bool b);
    void setGravityCompensation(int value);

    //cone de ficção
    void setLimitCone(int val);
    void setMCone(double val);
    void setRadiusCone(double val);
    void setHeightCone(double val);
    void setAngleCone(double val);

    //tolerancia de equilibrio
    void setToleranceSensor(double val);

    //testes robustez
    void setKVelocityLocomotion(Vec4 k);
    void setKDistanceLocomotion(Vec4 k);
    void setVelocityDensityBalls(float den,float vel); //atualiza a densidade e velocidade das bolas
    void updateAngleGround(Vec4 ang); //atualiza o angulo do solo
    void setMassSuitcase(double val);
    void setFramesForce2Time(double val);

    //slots personagens
    std::vector<Object*> getObjectsList();
    std::vector<Joint*> getJointsList();
    std::vector<GraphicalPose*> getPoseControlList();
    std::vector<Pose*> getPoseList();
    void setObjectSelected(int row);
    Object* getObject(int row);
    void showCharacter(bool b);
    void setJointSelected(int row);
    //void setPoseControlSelected(int row);
    void loadScene(QString file);
    void saveCharacter(QString file);
    void setWireCharacter(bool wire);
    void setShowShadow(bool b);
    void setShowGRF(bool b);
    void setShowMoCap(bool b);
    void drawScene();
    void drawParameters();
    void drawPoseProgression();
    void drawForceApply();


    //slots não utilizados
    void setAlphaCharacter(int value);




public:
    Scene *scene;
    QTimer *simTimer;
    int width;
    int height;
    bool move;
    bool sim_pause;
    bool capture_pause;
    bool editing_frame;
    bool show_character;
    bool showInfo;
    int  frame_edit;
    QList<Vec4> curve_quat;
    QList<int>  curve_quat_time;

    bool load_exemple_curve;
    bool screenshot;
    bool rendermesh;
    float density,velocity;
    int frames;
    float timeBase;
    float FPS;
    QTime m_time;
    bool  has_ball_shot;
    Vec4  ball_shot_debug;


    void bindShader();
    void releaseShader();
    float mass_suitcase;

    int  frames_force;



    //Controle do personagem luxo
    bool controlLuxo;

    //Plane *plane;
    explicit GLWidget(QWidget *parent = 0);
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void updateCamera();
    void showCompensableConeFriction();
    //funções de câmera
//    void wheelEvent(QWheelEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

    //funções extras
    MoCap* pushMotionCapture();
    void drawShadows();
    void drawReflections();
    void loadCurveExample();
    void showCurveExample();
    void setScreenShot();
    void calculateFPSPaint();
    void drawFPS();
private:
    MaterialPtr createMaterial();
    void renderScene();
    void renderSceneShadow();
    void renderSceneWithShadow();
    void RenderQuad();
    void configureShaderAndMatrices();
    void prepareShaderProgram();
    void prepareVertexBuffers();


    QOpenGLShaderProgram m_program;
    QOpenGLBuffer m_vertexPositionBuffer;
    QOpenGLBuffer m_vertexColorBuffer;
    QOpenGLVertexArrayObject m_vao;

    float m_vx;
    float m_vy;
    float m_vz;
    bool m_viewCenterFixed;
    float m_panAngle;
    float m_tiltAngle;

    //shadow mapping

    GLuint depthMapFBO;
    GLuint depthMap;

    // Cube related members
    QMatrix4x4 m_modelMatrix;
    Cube* m_cube;
    Cube* m_cube2;
    Camera* m_camera;


#ifdef SHADERS_ENABLED
    QGLShaderProgram shaderProgram;
#endif

signals:

public slots:
    void simulationPlayPause();
    void simulationRestart();

private slots:
    void simStep();




};

#endif // GLWIDGET_H
