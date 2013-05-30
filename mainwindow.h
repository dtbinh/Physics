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
    void infoSelectedJoint(Joint*);
    void applyForce2Object();


private slots:
    void on_actionPlay_Pause_Simulation_triggered();

    void on_actionRestart_Simulation_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
