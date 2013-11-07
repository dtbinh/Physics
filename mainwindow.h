#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "scene/object.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void updateListObjects(std::vector<Object*> objects);
    void updateListJoints(std::vector<Joint*> joints);
    void updateControlPD();
    void updateControlPDManipulators();
    void updateKsGeral(Vec4 ks);
    void updateKdGeral(Vec4 kd);
    void updateBalancePD(Vec4 ksT,Vec4 kdT,Vec4 ksF,Vec4 kdF,Vec4 kmom);
    void updateBalanceLocomotion();
    void updateAngleAnchor(); //atualiza o angulo do corpo escolhido na interface para o equilibrio
    void updateControlBalance();
    void infoSelectedJoint(Joint*);
    void updateSelectedObject();
    void updateControlPDPositional();
    void showSelectedObject(int i);
    void setGravity();
    void checkFoot(bool);
    void checkBodyBalance(bool);
    void checkShowEffector(bool);
    void checkShowTarget(bool);
    void checkFrameFootLeft(bool);
    void checkFrameFootRight(bool);
    void showPropertiesFootFrame(int i);
    void checkEnabledCPDP(bool);
    void checkHasCoffeeCup(bool);

    //void infoSelectedObject(Object*);
    void applyForce2Object();
    void setMaxTimeLine(int v);
    void adjustTolerance(double t);

    //manipuladores via keyboard
    void plusFrameEdition();
    void minusFrameEdition();


private slots:
    void on_actionPlay_Pause_Simulation_triggered();

    void on_actionRestart_Simulation_triggered();

    void on_actionOpen_Model_activated();

    void on_actionOpen_MoCap_triggered();

    void on_actionSave_Model_triggered();

    void on_btnStart_clicked();

    void on_btnPause_clicked();

    void on_btnRestart_clicked();

    void on_saveEditingFrame_clicked();

    void on_loadEditedFrames_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
