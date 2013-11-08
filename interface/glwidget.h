#ifndef GLWIDGET_H
#define GLWIDGET_H
#include "scene/scene.h"
#include <QGLWidget>
#include <QTimer>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>

class Plane;
class GLWidget : public QGLWidget
{
    Q_OBJECT
signals:
    void updateObjects(std::vector<Object*>);
    void updateJoints(std::vector<Joint*>);
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

public slots:

    //slots motion capture

    void loadMotionCapture(QString file);
    void loadFramesConfig(QString file);
    void saveFramesConfig(QString file);
    void setPlayback(bool val);
    void restartMotion();
    void showEditingFrame(bool b);
    void setEditingFrame(int frame);

    //slots scene
    void saveSimulationParameters(QString file);
    void loadSimulationParameters(QString file);

    //slots parametros físicos
    void SimStepsSimulation(int steps);
    void stopSimulation();
    void startSimulation();
    void applyForce(Vec4 force);
    void setGravityParameters(Vec4 g);
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

    //cone de ficção
    void setMCone(double val);
    void setRadiusCone(double val);
    void setHeightCone(double val);
    void setAngleCone(double val);


    void setKVelocityLocomotion(Vec4 k);
    void setKDistanceLocomotion(Vec4 k);

    //slots personagens
    std::vector<Object*> getObjectsList();
    std::vector<Joint*> getJointsList();
    void setObjectSelected(int row);
    Object* getObject(int row);
    void showCharacter(bool b);
    void setJointSelected(int row);
    void loadScene(QString file);
    void saveCharacter(QString file);
    void setWireCharacter(bool wire);


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
    int  frame_edit;
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



signals:

public slots:
    void simulationPlayPause();
    void simulationRestart();

private slots:
    void simStep();



};

#endif // GLWIDGET_H
