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
    void showJoint(Joint*);
public slots:
    std::vector<Object*> getObjectsList();
    std::vector<Joint*> getJointsList();
    void setObjectSelected(int row);
    void setJointSelected(int row);
    void SimStepsSimulation(int steps);
    void applyForce(Vec4 force);
public:
    Scene *scene;
    QTimer simTimer;
    int width;
    int height;
    bool move;
    bool sim_pause;
    //Plane *plane;
    explicit GLWidget(QWidget *parent = 0);
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void updateCamera();
    //funções de câmera
    void wheelEvent(QWheelEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);



signals:

public slots:
    void simulationPlayPause();
    void simulationRestart();

private slots:
    void simStep();



};

#endif // GLWIDGET_H
