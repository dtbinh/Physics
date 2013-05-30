#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stdio.h"

ControlPD *pd_selected;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->showMaximized();
    ui->widgetPhysics->setObjectSelected(-1);
    connect(ui->stepSim,SIGNAL(valueChanged(int)),ui->widgetPhysics,SLOT(SimStepsSimulation(int)));
    connect(ui->widgetPhysics,SIGNAL(updateObjects(std::vector<Object*>)),this,SLOT(updateListObjects(std::vector<Object*>)));
    connect(ui->widgetPhysics,SIGNAL(updateJoints(std::vector<Joint*>)),this,SLOT(updateListJoints(std::vector<Joint*>)));
    connect(ui->listWidgetObjects,SIGNAL(currentRowChanged(int)),ui->widgetPhysics,SLOT(setObjectSelected(int)));
    connect(ui->listWidgetJoints,SIGNAL(currentRowChanged(int)),ui->widgetPhysics,SLOT(setJointSelected(int)));
    connect(ui->applyForce,SIGNAL(clicked()),this,SLOT(applyForce2Object()));
    connect(ui->widgetPhysics,SIGNAL(showJoint(Joint*)),this,SLOT(infoSelectedJoint(Joint*)));
    //manipuadores controlador PD
    connect(ui->ksx,SIGNAL(valueChanged(double)),this,SLOT(updateControlPD()));
    connect(ui->ksy,SIGNAL(valueChanged(double)),this,SLOT(updateControlPD()));
    connect(ui->ksz,SIGNAL(valueChanged(double)),this,SLOT(updateControlPD()));

    connect(ui->kdx,SIGNAL(valueChanged(double)),this,SLOT(updateControlPD()));
    connect(ui->kdy,SIGNAL(valueChanged(double)),this,SLOT(updateControlPD()));
    connect(ui->kdz,SIGNAL(valueChanged(double)),this,SLOT(updateControlPD()));

    connect(ui->eulerx,SIGNAL(valueChanged(double)),this,SLOT(updateControlPD()));
    connect(ui->eulery,SIGNAL(valueChanged(double)),this,SLOT(updateControlPD()));
    connect(ui->eulerz,SIGNAL(valueChanged(double)),this,SLOT(updateControlPD()));

    connect(ui->enablepd,SIGNAL(clicked(bool)),this,SLOT(updateControlPD()));

    updateListObjects(ui->widgetPhysics->getObjectsList());
    updateListJoints(ui->widgetPhysics->getJointsList());
    ui->infoPD->setVisible(false);
    pd_selected = NULL;


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateListObjects(std::vector<Object*> objects)
{
    ui->listWidgetObjects->clear();
    for(int i=0;i<objects.size();i++){
        QString s;
        s.setNum(i);
        s.push_back(" - ");
        s.push_back(objects.at(i)->getName());
        ui->listWidgetObjects->addItem(s);
    }
}

void MainWindow::updateListJoints(std::vector<Joint*> joints)
{
    ui->listWidgetJoints->clear();
    for(int i=0;i<joints.size();i++){
        QString s;
        s.setNum(i);
        s.push_back(" - ");
        s.push_back(joints.at(i)->getName());
        ui->listWidgetJoints->addItem(s);
    }
}

void MainWindow::updateControlPD()
{
    if (pd_selected==NULL) return;
    Vec4 ks,kd,angle;
    ks.x1 = ui->ksx->value();
    ks.x2 = ui->ksy->value();
    ks.x3 = ui->ksz->value();

    kd.x1 = ui->kdx->value();
    kd.x2 = ui->kdy->value();
    kd.x3 = ui->kdz->value();


    angle.x1 = ui->eulerx->value();
    angle.x2 = ui->eulery->value();
    angle.x3 = ui->eulerz->value();

    pd_selected->setKd(kd);
    pd_selected->setKs(ks);
    pd_selected->setQuaternionWanted(Quaternion(angle));
    pd_selected->setEnabled(ui->enablepd->isChecked());
}

void MainWindow::infoSelectedJoint(Joint * joint)
{
    if(joint==NULL){
        ui->infoPD->setVisible(false);
        return;
    }
    ControlPD *pd = NULL;
    for(int i=0;i<joint->getCharacter()->getControllersPD().size();i++)
        if(joint==joint->getCharacter()->getControllersPD().at(i)->getJoint())
            pd = joint->getCharacter()->getControllersPD().at(i);

    if (pd==NULL){
        ui->infoPD->setVisible(false);
        return;
    }
    ui->infoPD->setVisible(true);
    Vec4 ks = pd->getKs();
    Vec4 kd = pd->getKd();
    Vec4 euler = pd->getQuaternionWanted().toEuler();
    ui->ksx->setValue(ks.x());
    ui->ksy->setValue(ks.y());
    ui->ksz->setValue(ks.z());
    ui->kdx->setValue(kd.x());
    ui->kdy->setValue(kd.y());
    ui->kdz->setValue(kd.z());
    ui->eulerx->setValue(euler.x());
    ui->eulery->setValue(euler.y());
    ui->eulerz->setValue(euler.z());
    pd_selected = pd;
    ui->enablepd->setChecked(pd->isEnabled());
}

void MainWindow::applyForce2Object()
{
    Vec4 force = Vec4(ui->forcex->value(),ui->forcey->value(),ui->forcez->value());
    ui->widgetPhysics->applyForce(force);
}


void MainWindow::on_actionPlay_Pause_Simulation_triggered()
{
    ui->widgetPhysics->simulationPlayPause();
}

void MainWindow::on_actionRestart_Simulation_triggered()
{
    ui->widgetPhysics->simulationRestart();
}
