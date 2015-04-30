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
    void updateControlPDManipulators2();
    void updateKsGeral(Vec4 ks);
    void updateKdGeral(Vec4 kd);
    void updateBalancePD(Vec4 ksT,Vec4 kdT,Vec4 ksF,Vec4 kdF,Vec4 kmomlin, Vec4 kmomang);
    void updateBalanceConeFriction(float m,float angle,float radius,float height);
    void updateBalanceLocomotion();
    void updateAngleAnchor(); //atualiza o angulo do corpo escolhido na interface para o equilibrio
    void updateControlBalance();
    void infoSelectedJoint(Joint*);
    void updateSelectedObject();
    void updateControlPDPositional();
    void showSelectedObject(int i);
    void setGravity();
    void setGravity(Vec4);
    void setSimbiconDistance();
    void setSimbiconForce();
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

    //balls shot
    void setBallsConfiguration();


private slots:
    void on_actionPlay_Pause_Simulation_triggered();

    void on_actionRestart_Simulation_triggered();

    void on_actionOpen_MoCap_triggered();

    void on_actionSave_Model_triggered();

    void on_btnStart_clicked();

    void on_btnPause_clicked();

    void on_btnRestart_clicked();

    void on_saveEditingFrame_clicked();

    void on_loadEditedFrames_clicked();

    void on_actionSave_Simulation_triggered();

    void on_actionOpen_Simulation_triggered();

    void on_checkBox_clicked(bool checked);

    void on_xksTqBal_valueChanged(double arg1);

    void on_xkdTqBal_valueChanged(double arg1);

    void on_xksForBal_valueChanged(double arg1);

    void on_xkdForBal_valueChanged(double arg1);

    void on_xkmomlinBal_valueChanged(double arg1);

    void on_xkmomangBal_valueChanged(double arg1);

    void on_xksPdProp_valueChanged(double arg1);

    void on_xkdPdProp_valueChanged(double arg1);

    void on_posx_ks_valueChanged(double arg1);

    void on_posx_kd_valueChanged(double arg1);

    void on_ksx_valueChanged(double arg1);

    void on_kdx_valueChanged(double arg1);

    void on_ksy_valueChanged(double arg1);

    void on_ksz_valueChanged(double arg1);

    void on_angleBalBodyy_valueChanged(int arg1);

    void on_angleBalBodyx_valueChanged(int arg1);

    void on_angleBalBodyz_valueChanged(int arg1);

    void on_actionOpen_Model_triggered();



private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
