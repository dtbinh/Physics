#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stdio.h"
#include <QFileDialog>
#include <QString>
#include <QPalette>
#include <QColorDialog>
#include "mocap/frame.h"
ControlPD *pd_selected;
Joint*     joint_selected;
Object*    obj_selected;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->showMaximized();
    ui->groupBoxCone->setVisible(false);
    ui->widgetPhysics->setObjectSelected(-1);
    //manipuladores do movimento
    connect(ui->widgetPhysics,SIGNAL(motionCurrentFrame(int)),ui->iframe,SLOT(setNum(int)));
    connect(ui->widgetPhysics,SIGNAL(motionTotalFrame(int)),ui->nframe,SLOT(setNum(int)));
    connect(ui->widgetPhysics,SIGNAL(motionCurrentFrame(int)),ui->timeLineMotion,SLOT(setValue(int)));
    connect(ui->widgetPhysics,SIGNAL(motionTotalFrame(int)),this,SLOT(setMaxTimeLine(int)));
    connect(ui->sensorTolerance,SIGNAL(valueChanged(double)),this,SLOT(adjustTolerance(double)));
    connect(ui->widgetPhysics,SIGNAL(motionTotalFrame(int)),this,SLOT(setMaxTimeLine(int)));
    connect(ui->checkFrameFootLeft,SIGNAL(clicked(bool)),this,SLOT(checkFrameFootLeft(bool)));
    connect(ui->checkFrameFootRight,SIGNAL(clicked(bool)),this,SLOT(checkFrameFootRight(bool)));
    connect(ui->checkShowChara,SIGNAL(clicked(bool)),ui->widgetPhysics,SLOT(showCharacter(bool)));
    connect(ui->checkShowEditingFrame,SIGNAL(clicked(bool)),ui->widgetPhysics,SLOT(showEditingFrame(bool)));
    connect(ui->frameEdit,SIGNAL(valueChanged(int)),ui->widgetPhysics,SLOT(setEditingFrame(int)));
    connect(ui->frameEdit,SIGNAL(valueChanged(int)),this,SLOT(showPropertiesFootFrame(int)));
    connect(ui->widgetPhysics,SIGNAL(plusFrameEdition()),this,SLOT(plusFrameEdition()));
    connect(ui->widgetPhysics,SIGNAL(minusFrameEdition()),this,SLOT(minusFrameEdition()));

    //manipuladores de atributos da simulação
    connect(ui->stepSim,SIGNAL(valueChanged(int)),ui->widgetPhysics,SLOT(SimStepsSimulation(int)));
    connect(ui->gravx,SIGNAL(valueChanged(double)),this,SLOT(setGravity()));
    connect(ui->gravy,SIGNAL(valueChanged(double)),this,SLOT(setGravity()));
    connect(ui->gravz,SIGNAL(valueChanged(double)),this,SLOT(setGravity()));
    connect(ui->enableGravity,SIGNAL(clicked(bool)),this,SLOT(setGravity()));




    //manipuladores de junta
    connect(ui->widgetPhysics,SIGNAL(updateJoints(std::vector<Joint*>)),this,SLOT(updateListJoints(std::vector<Joint*>)));
    connect(ui->listWidgetJoints,SIGNAL(currentRowChanged(int)),ui->widgetPhysics,SLOT(setJointSelected(int)));
    connect(ui->widgetPhysics,SIGNAL(showJoint(Joint*)),this,SLOT(infoSelectedJoint(Joint*)));
    connect(ui->listWidgetObjects,SIGNAL(currentRowChanged(int)),ui->widgetPhysics,SLOT(setObjectSelected(int)));
    connect(ui->listWidgetObjects,SIGNAL(currentRowChanged(int)),this,SLOT(showSelectedObject(int)));


    //manipuladores de Objetos
    connect(ui->widgetPhysics,SIGNAL(setForceCharacter()),this,SLOT(applyForce2Object()));

    connect(ui->widgetPhysics,SIGNAL(updateObjects(std::vector<Object*>)),this,SLOT(updateListObjects(std::vector<Object*>)));
    connect(ui->scalex,SIGNAL(valueChanged(double)),this,SLOT(updateSelectedObject()));
    connect(ui->scaley,SIGNAL(valueChanged(double)),this,SLOT(updateSelectedObject()));
    connect(ui->scalez,SIGNAL(valueChanged(double)),this,SLOT(updateSelectedObject()));
    connect(ui->posx,SIGNAL(valueChanged(double)),this,SLOT(updateSelectedObject()));
    connect(ui->posy,SIGNAL(valueChanged(double)),this,SLOT(updateSelectedObject()));
    connect(ui->posz,SIGNAL(valueChanged(double)),this,SLOT(updateSelectedObject()));
    connect(ui->oeulerx,SIGNAL(valueChanged(int)),this,SLOT(updateSelectedObject()));
    connect(ui->oeulery,SIGNAL(valueChanged(int)),this,SLOT(updateSelectedObject()));
    connect(ui->oeulerz,SIGNAL(valueChanged(int)),this,SLOT(updateSelectedObject()));
    connect(ui->mass,SIGNAL(valueChanged(double)),this,SLOT(updateSelectedObject()));
    connect(ui->isFoot,SIGNAL(clicked(bool)),this,SLOT(checkFoot(bool)));
    connect(ui->isBodyBalance,SIGNAL(clicked(bool)),this,SLOT(checkBodyBalance(bool)));

    connect(ui->applyForce,SIGNAL(clicked()),this,SLOT(applyForce2Object()));
        //controle PD posicional
    connect(ui->posx_target,SIGNAL(valueChanged(double)),this,SLOT(updateControlPDPositional()));
    connect(ui->posy_target,SIGNAL(valueChanged(double)),this,SLOT(updateControlPDPositional()));
    connect(ui->posz_target,SIGNAL(valueChanged(double)),this,SLOT(updateControlPDPositional()));
    connect(ui->posx_ks,SIGNAL(valueChanged(double)),this,SLOT(updateControlPDPositional()));
    connect(ui->posy_ks,SIGNAL(valueChanged(double)),this,SLOT(updateControlPDPositional()));
    connect(ui->posz_ks,SIGNAL(valueChanged(double)),this,SLOT(updateControlPDPositional()));
    connect(ui->posx_kd,SIGNAL(valueChanged(double)),this,SLOT(updateControlPDPositional()));
    connect(ui->posy_kd,SIGNAL(valueChanged(double)),this,SLOT(updateControlPDPositional()));
    connect(ui->posz_kd,SIGNAL(valueChanged(double)),this,SLOT(updateControlPDPositional()));
    connect(ui->enable_cpdp,SIGNAL(clicked(bool)),this,SLOT(checkEnabledCPDP(bool)));
    connect(ui->show_target,SIGNAL(clicked(bool)),this,SLOT(checkShowTarget(bool)));
    connect(ui->show_effector,SIGNAL(clicked(bool)),this,SLOT(checkShowEffector(bool)));
    connect(ui->hasCup,SIGNAL(clicked(bool)),this,SLOT(checkHasCoffeeCup(bool)));

    //manipuladores do equilíbrio e controladores geral
        //cone de fricção
    connect(ui->widgetPhysics,SIGNAL(updateBalanceCone(float,float,float,float)),this,SLOT(updateBalanceConeFriction(float,float,float,float)));
    connect(ui->widgetPhysics,SIGNAL(setSliderFoot1(int)),ui->coneFoot1,SLOT(setValue(int)));
    connect(ui->widgetPhysics,SIGNAL(setSliderFoot2(int)),ui->coneFoot2,SLOT(setValue(int)));
    connect(ui->widgetPhysics,SIGNAL(setSliderFoot1(int)),ui->conePerc1,SLOT(setNum(int)));
    connect(ui->widgetPhysics,SIGNAL(setSliderFoot2(int)),ui->conePerc2,SLOT(setNum(int)));
    connect(ui->coneM,SIGNAL(valueChanged(double)),ui->widgetPhysics,SLOT(setMCone(double)));
    connect(ui->coneRadius,SIGNAL(valueChanged(double)),ui->widgetPhysics,SLOT(setRadiusCone(double)));
    connect(ui->coneHeight,SIGNAL(valueChanged(double)),ui->widgetPhysics,SLOT(setHeightCone(double)));
    connect(ui->coneAngle,SIGNAL(valueChanged(double)),ui->widgetPhysics,SLOT(setAngleCone(double)));
        //manipuladores de desenho do personagem
    connect(ui->checkWireChara,SIGNAL(clicked(bool)),ui->widgetPhysics,SLOT(setWireCharacter(bool)));

        //manipuladores de equilíbrio
    connect(ui->angleBalBodyx,SIGNAL(valueChanged(int)),this,SLOT(updateAngleAnchor()));
    connect(ui->angleBalBodyy,SIGNAL(valueChanged(int)),this,SLOT(updateAngleAnchor()));
    connect(ui->angleBalBodyz,SIGNAL(valueChanged(int)),this,SLOT(updateAngleAnchor()));
    connect(ui->checkForceBal,SIGNAL(clicked(bool)),ui->widgetPhysics,SLOT(setEnableForceBalance(bool)));
    connect(ui->checkTorqueBal,SIGNAL(clicked(bool)),ui->widgetPhysics,SLOT(setEnableTorqueBalance(bool)));
    connect(ui->checkMomentum,SIGNAL(clicked(bool)),ui->widgetPhysics,SLOT(setEnableMomentumBalance(bool)));
    connect(ui->widgetPhysics,SIGNAL(updateBalancePD(Vec4,Vec4,Vec4,Vec4,Vec4,Vec4)),this,SLOT(updateBalancePD(Vec4,Vec4,Vec4,Vec4,Vec4,Vec4)));


    connect(ui->xkmomlinBal,SIGNAL(valueChanged(double)),this,SLOT(updateControlBalance()));
    connect(ui->ykmomlinBal,SIGNAL(valueChanged(double)),this,SLOT(updateControlBalance()));
    connect(ui->zkmomlinBal,SIGNAL(valueChanged(double)),this,SLOT(updateControlBalance()));
    connect(ui->xkmomangBal,SIGNAL(valueChanged(double)),this,SLOT(updateControlBalance()));
    connect(ui->ykmomangBal,SIGNAL(valueChanged(double)),this,SLOT(updateControlBalance()));
    connect(ui->zkmomangBal,SIGNAL(valueChanged(double)),this,SLOT(updateControlBalance()));

    connect(ui->xkdForBal,SIGNAL(valueChanged(double)),this,SLOT(updateControlBalance()));
    connect(ui->ykdForBal,SIGNAL(valueChanged(double)),this,SLOT(updateControlBalance()));
    connect(ui->zkdForBal,SIGNAL(valueChanged(double)),this,SLOT(updateControlBalance()));

    connect(ui->xksForBal,SIGNAL(valueChanged(double)),this,SLOT(updateControlBalance()));
    connect(ui->yksForBal,SIGNAL(valueChanged(double)),this,SLOT(updateControlBalance()));
    connect(ui->zksForBal,SIGNAL(valueChanged(double)),this,SLOT(updateControlBalance()));

    connect(ui->xksTqBal,SIGNAL(valueChanged(double)),this,SLOT(updateControlBalance()));
    connect(ui->yksTqBal,SIGNAL(valueChanged(double)),this,SLOT(updateControlBalance()));
    connect(ui->zksTqBal,SIGNAL(valueChanged(double)),this,SLOT(updateControlBalance()));

    connect(ui->xkdTqBal,SIGNAL(valueChanged(double)),this,SLOT(updateControlBalance()));
    connect(ui->ykdTqBal,SIGNAL(valueChanged(double)),this,SLOT(updateControlBalance()));
    connect(ui->zkdTqBal,SIGNAL(valueChanged(double)),this,SLOT(updateControlBalance()));

    //connect(ui->sliderComp,SIGNAL(valueChanged(int)),ui->spinComp,SLOT(setValue(int)));
    //connect(ui->spinComp,SIGNAL(valueChanged(int)),ui->sliderComp,SLOT(setValue(int)));
    //connect(ui->spinComp,SIGNAL(valueChanged(int)),ui->widgetPhysics,SLOT(setCompensationBalance(int)));

        //manipuladores dos controladores PD propocionais do objeto

    connect(ui->xksPdProp,SIGNAL(editingFinished()),this,SLOT(updateControlPDManipulators()));
    connect(ui->yksPdProp,SIGNAL(editingFinished()),this,SLOT(updateControlPDManipulators()));
    connect(ui->zksPdProp,SIGNAL(editingFinished()),this,SLOT(updateControlPDManipulators()));
    connect(ui->xkdPdProp,SIGNAL(editingFinished()),this,SLOT(updateControlPDManipulators()));
    connect(ui->ykdPdProp,SIGNAL(editingFinished()),this,SLOT(updateControlPDManipulators()));
    connect(ui->zkdPdProp,SIGNAL(editingFinished()),this,SLOT(updateControlPDManipulators()));

    connect(ui->widgetPhysics,SIGNAL(updateKsProp(Vec4)),this,SLOT(updateKsGeral(Vec4)));
    connect(ui->widgetPhysics,SIGNAL(updateKdProp(Vec4)),this,SLOT(updateKdGeral(Vec4)));

        //manipuladores das constantes de locomoção
    connect(ui->xLocDist,SIGNAL(valueChanged(double)),this,SLOT(updateBalanceLocomotion()));
    connect(ui->yLocDist,SIGNAL(valueChanged(double)),this,SLOT(updateBalanceLocomotion()));
    connect(ui->zLocDist,SIGNAL(valueChanged(double)),this,SLOT(updateBalanceLocomotion()));
    connect(ui->xLocVel,SIGNAL(valueChanged(double)),this,SLOT(updateBalanceLocomotion()));
    connect(ui->yLocVel,SIGNAL(valueChanged(double)),this,SLOT(updateBalanceLocomotion()));
    connect(ui->zLocVel,SIGNAL(valueChanged(double)),this,SLOT(updateBalanceLocomotion()));



    //manipuladores controlador PD
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
    //ui->widgetPhysics->loadScene("/home/danilo/GitHub/ODESys/models/new/biped_3D/biped3D.model");
    pd_selected = NULL;
    joint_selected = NULL;
    obj_selected = NULL;


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateListObjects(std::vector<Object*> objects)
{
    ui->listWidgetObjects->clear();
    for(unsigned int i=0;i<objects.size();i++){
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
    for(unsigned int i=0;i<joints.size();i++){
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
    if(pd_selected->getJoint()!=joint_selected) return;
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

void MainWindow::updateControlPDManipulators()
{
    ui->widgetPhysics->setProportionalKs(Vec4(ui->xksPdProp->value(),ui->yksPdProp->value(),ui->zksPdProp->value()));
    ui->widgetPhysics->setProportionalKd(Vec4(ui->xkdPdProp->value(),ui->ykdPdProp->value(),ui->zkdPdProp->value()));
}

void MainWindow::updateKsGeral(Vec4 ks)
{
    ui->xksPdProp->setValue(ks.x());
    ui->yksPdProp->setValue(ks.y());
    ui->zksPdProp->setValue(ks.z());
}

void MainWindow::updateKdGeral(Vec4 kd)
{
    printf("\nInside: %.3f %.3f %.3f",kd.x(),kd.y(),kd.z());
    ui->xkdPdProp->setValue(kd.x());
    ui->ykdPdProp->setValue(kd.y());
    ui->zkdPdProp->setValue(kd.z());
}

void MainWindow::updateBalancePD(Vec4 ksT, Vec4 kdT, Vec4 ksF, Vec4 kdF, Vec4 kmomlin,Vec4 kmomang)
{
    ui->xkdForBal->setValue(kdF.x());
    ui->ykdForBal->setValue(kdF.y());
    ui->zkdForBal->setValue(kdF.z());

    ui->xksForBal->setValue(ksF.x());
    ui->yksForBal->setValue(ksF.y());
    ui->zksForBal->setValue(ksF.z());

    ui->xksTqBal->setValue(ksT.x());
    ui->yksTqBal->setValue(ksT.y());
    ui->zksTqBal->setValue(ksT.z());

    ui->xkdTqBal->setValue(kdT.x());
    ui->ykdTqBal->setValue(kdT.y());
    ui->zkdTqBal->setValue(kdT.z());

    ui->xkmomlinBal->setValue(kmomlin.x());
    ui->ykmomlinBal->setValue(kmomlin.y());
    ui->zkmomlinBal->setValue(kmomlin.z());

    ui->xkmomangBal->setValue(kmomang.x());
    ui->ykmomangBal->setValue(kmomang.y());
    ui->zkmomangBal->setValue(kmomang.z());


}

void MainWindow::updateBalanceConeFriction(float m, float angle, float radius, float height)
{
    ui->coneM->setValue(m);
    ui->coneAngle->setValue((int)angle);
    ui->coneHeight->setValue(height);
    ui->coneRadius->setValue(radius);
}

void MainWindow::updateBalanceLocomotion()
{
    Vec4 kvel = Vec4(ui->xLocVel->value(),ui->yLocVel->value(),ui->zLocVel->value());
    Vec4 kdist = Vec4(ui->xLocDist->value(),ui->yLocDist->value(),ui->zLocDist->value());

    ui->widgetPhysics->setKVelocityLocomotion(kvel);
    ui->widgetPhysics->setKDistanceLocomotion(kdist);

}

void MainWindow::updateAngleAnchor()
{
    Vec4 val(ui->angleBalBodyx->value(),ui->angleBalBodyy->value(),ui->angleBalBodyz->value());
    ui->widgetPhysics->setAngleBodyBalance(val);
}

void MainWindow::updateControlBalance()
{
    Vec4 ksT,kdT,ksF,kdF,kmomlin,kmomang;
    ksT.setVec4(ui->xksTqBal->value(),ui->yksTqBal->value(),ui->zksTqBal->value());
    kdT.setVec4(ui->xkdTqBal->value(),ui->ykdTqBal->value(),ui->zkdTqBal->value());
    ksF.setVec4(ui->xksForBal->value(),ui->yksForBal->value(),ui->zksForBal->value());
    kdF.setVec4(ui->xkdForBal->value(),ui->ykdForBal->value(),ui->zkdForBal->value());
    kmomlin.setVec4(ui->xkmomlinBal->value(),ui->ykmomlinBal->value(),ui->zkmomlinBal->value());
    kmomang.setVec4(ui->xkmomangBal->value(),ui->ykmomangBal->value(),ui->zkmomangBal->value());

    ui->widgetPhysics->setBalanceControl(ksT,kdT,ksF,kdF,kmomlin,kmomang);

}

void MainWindow::infoSelectedJoint(Joint * joint)
{

    joint_selected = joint;
    if(joint==NULL){
        ui->infoPD->setVisible(false);
        return;
    }
    ControlPD *pd = NULL;
    for(unsigned int i=0;i<joint->getCharacter()->getControllersPD().size();i++)
        if(joint==joint->getCharacter()->getControllersPD().at(i)->getJoint())
            pd = joint->getCharacter()->getControllersPD().at(i);

    if (pd==NULL){
        ui->infoPD->setVisible(false);
        return;
    }
    ui->infoPD->setVisible(true);
    Vec4 ks = pd->getKs();
    Vec4 kd = pd->getKd();
    Vec4 ksp = pd->getProportionalKs();
    Vec4 kdp = pd->getProportionalKd();
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
    ui->xksPdApply->setText(QString("%1").arg(ks.x()*ksp.x()));
    ui->yksPdApply->setText(QString("%1").arg(ks.y()*ksp.y()));
    ui->zksPdApply->setText(QString("%1").arg(ks.z()*ksp.z()));
    ui->xkdPdApply->setText(QString("%1").arg(kd.x()*kdp.x()));
    ui->ykdPdApply->setText(QString("%1").arg(kd.y()*kdp.y()));
    ui->zkdPdApply->setText(QString("%1").arg(kd.z()*kdp.z()));
}

void MainWindow::updateSelectedObject()
{
    if (obj_selected==NULL) return;
    Vec4 pos(ui->posx->value(),ui->posy->value(),ui->posz->value());
    Vec4 scale(ui->scalex->value(),ui->scaley->value(),ui->scalez->value());
    Vec4 euler(ui->oeulerx->value(),ui->oeulery->value(),ui->oeulerz->value());
    obj_selected->setPosition(pos);
    obj_selected->setProperties(scale);
    obj_selected->setRotation(Quaternion(euler));
    obj_selected->setFMass(ui->mass->value());


//    obj_selected->updatePhysics();
//    //printf("%s\n",obj_selected->getName().toLocal8Bit().data());
//    Vec4 axis; dReal angle;
//    Quaternion q(euler);
//    q.toAxisAngle(&axis,&angle);
    //printf("(%.4f %.4f %.4f %.4f)",angle,axis.x(),axis.y(),axis.z());

}

void MainWindow::updateControlPDPositional()
{
    if (obj_selected==NULL) return;
    Vec4 target(ui->posx_target->value(),ui->posy_target->value(),ui->posz_target->value());
    Vec4 ks(ui->posx_ks->value(),ui->posx_ks->value(),ui->posx_ks->value());
    Vec4 kd(ui->posx_kd->value(),ui->posx_kd->value(),ui->posx_kd->value());
    obj_selected->setTarget(target);
    obj_selected->setKs(ks);
    obj_selected->setKd(kd);

}

void MainWindow::showSelectedObject(int i)
{
    obj_selected = ui->widgetPhysics->getObject(i);
    if (obj_selected==NULL) return;
    Vec4 originalPos = obj_selected->getPosition();
    Vec4 euler       = obj_selected->getRotation().toEuler();
    Vec4 scale       = obj_selected->getProperties();
    float mass       = obj_selected->getFMass();
    ui->posx->setValue(originalPos.x());
    ui->posy->setValue(originalPos.y());
    ui->posz->setValue(originalPos.z());
    ui->oeulerx->setValue(euler.x());
    ui->oeulery->setValue(euler.y());
    ui->oeulerz->setValue(euler.z());
    ui->scalex->setValue(scale.x());
    ui->scaley->setValue(scale.y());
    ui->scalez->setValue(scale.z());
    ui->mass->setValue(mass);
    ui->isFoot->setChecked(obj_selected->getFoot());
    ui->isBodyBalance->setChecked(obj_selected->getBodyBalance());
    //controle PD posicional
    Vec4 target = obj_selected->getTarget();
    Vec4 ks = obj_selected->getKs();
    Vec4 kd = obj_selected->getKd();
    ui->posx_target->setValue(target.x());
    ui->posy_target->setValue(target.y());
    ui->posz_target->setValue(target.z());
    ui->posx_ks->setValue(ks.x());
    ui->posy_ks->setValue(ks.y());
    ui->posz_ks->setValue(ks.z());
    ui->posx_kd->setValue(kd.x());
    ui->posy_kd->setValue(kd.y());
    ui->posz_kd->setValue(kd.z());
    ui->show_effector->setChecked(obj_selected->isShowEffector());
    ui->show_target->setChecked(obj_selected->isShowTarget());
    ui->enable_cpdp->setChecked(obj_selected->isEnableCPDP());
    ui->hasCup->setChecked(obj_selected->hasCoffeeCup());


}

void MainWindow::setGravity()
{
    Vec4 g(ui->gravx->value(),ui->gravy->value(),ui->gravz->value());
    ui->widgetPhysics->setGravityParameters(g);
    ui->widgetPhysics->setGravity(ui->enableGravity->isChecked());

}

void MainWindow::checkFoot(bool b)
{
    if (obj_selected!=NULL) obj_selected->setFoot(b);
}

void MainWindow::checkBodyBalance(bool b)
{
    if (obj_selected!=NULL) obj_selected->setBodyBalance(b);
}

void MainWindow::checkShowEffector(bool b)
{
    if (obj_selected!=NULL)    obj_selected->setShowEffector(b);
}

void MainWindow::checkShowTarget(bool b)
{
    if (obj_selected!=NULL) obj_selected->setShowTarget(b);
}

void MainWindow::checkFrameFootLeft(bool b)
{
    int frame = ui->frameEdit->value();
    if (ui->widgetPhysics->pushMotionCapture()!=NULL)
        ui->widgetPhysics->pushMotionCapture()->getFrameMotion(frame)->setFootLeftGround(b);
}

void MainWindow::checkFrameFootRight(bool b)
{
    int frame = ui->frameEdit->value();
    if (ui->widgetPhysics->pushMotionCapture()!=NULL)
        ui->widgetPhysics->pushMotionCapture()->getFrameMotion(frame)->setFootRightGround(b);
}

void MainWindow::showPropertiesFootFrame(int i)
{
    if (ui->widgetPhysics->pushMotionCapture()!=NULL){
        ui->checkFrameFootLeft->setChecked(ui->widgetPhysics->pushMotionCapture()->getFrameMotion(i)->getFootLeftGround());
        ui->checkFrameFootRight->setChecked(ui->widgetPhysics->pushMotionCapture()->getFrameMotion(i)->getFootRightGround());
    }
}

void MainWindow::checkEnabledCPDP(bool b)
{

    if (obj_selected!=NULL) obj_selected->setEnableCPDP(b);
}

void MainWindow::checkHasCoffeeCup(bool b)
{
    if (obj_selected!=NULL) obj_selected->setCoffeeCup(b);
}

void MainWindow::applyForce2Object()
{
    Vec4 force = Vec4(ui->forcex->value(),ui->forcey->value(),ui->forcez->value());
    ui->widgetPhysics->applyForce(force);
//    ui->forcex->setValue(0.0);
//    ui->forcey->setValue(0.0);
    //    ui->forcez->setValue(0.0);
}

void MainWindow::setMaxTimeLine(int v)
{
    ui->timeLineMotion->setMaximum(v);
    ui->frameEdit->setMaximum(v);
}

void MainWindow::adjustTolerance(double t)
{
    Sensor::setTolerance(t);
}

void MainWindow::plusFrameEdition()
{
    ui->frameEdit->setValue(ui->frameEdit->value()+1);
}

void MainWindow::minusFrameEdition()
{
    ui->frameEdit->setValue(ui->frameEdit->value()-1);
}


void MainWindow::on_actionPlay_Pause_Simulation_triggered()
{
    ui->widgetPhysics->simulationPlayPause();
}

void MainWindow::on_actionRestart_Simulation_triggered()
{
    ui->widgetPhysics->simulationRestart();
}

void MainWindow::on_actionOpen_Model_activated()
{
    ui->widgetPhysics->stopSimulation();
    QString mfile = QFileDialog::getOpenFileName(this,"Open Model","../models/");
    if(!mfile.isEmpty()){
        ui->widgetPhysics->loadScene(mfile);
        ui->enableGravity->setChecked(false);
        ui->gravx->setValue(0);
        ui->gravy->setValue(0);
        ui->gravz->setValue(0);
        ui->groupBoxCone->setVisible(true);
    }
    ui->widgetPhysics->startSimulation();
}

//abrir arquivo de captura de movimento
void MainWindow::on_actionOpen_MoCap_triggered()
{
    ui->widgetPhysics->stopSimulation();
    QString mfile = QFileDialog::getOpenFileName(this,"Load Motion Capture","../mot/");
    if(!mfile.isEmpty()){
        ui->widgetPhysics->loadMotionCapture(mfile);
    }
    ui->widgetPhysics->startSimulation();
}

void MainWindow::on_actionSave_Model_triggered()
{
    ui->widgetPhysics->stopSimulation();
    QString mfile = QFileDialog::getSaveFileName(this,"Save Scene","../models/");
    if(!mfile.isEmpty()) ui->widgetPhysics->saveCharacter(mfile);
     ui->widgetPhysics->startSimulation();
}

void MainWindow::on_btnStart_clicked()
{
    ui->widgetPhysics->setPlayback(false);
}

void MainWindow::on_btnPause_clicked()
{
    ui->widgetPhysics->setPlayback(true);
}

void MainWindow::on_btnRestart_clicked()
{
    ui->widgetPhysics->restartMotion();
}

void MainWindow::on_saveEditingFrame_clicked()
{
    ui->widgetPhysics->stopSimulation();
    QString mfile = QFileDialog::getSaveFileName(this,"Save Config Frames","../framesMotCap/");
    if(!mfile.isEmpty()) ui->widgetPhysics->saveFramesConfig(mfile);
     ui->widgetPhysics->startSimulation();
}

void MainWindow::on_loadEditedFrames_clicked()
{
    ui->widgetPhysics->stopSimulation();
    QString mfile = QFileDialog::getOpenFileName(this,"Load Config Motion Capture","../framesMotCap/");
    if(!mfile.isEmpty()){
        ui->widgetPhysics->loadFramesConfig(mfile);
        showPropertiesFootFrame(0);
    }
    ui->widgetPhysics->startSimulation();
}

void MainWindow::on_actionSave_Simulation_triggered()
{
    ui->widgetPhysics->stopSimulation();
    QString mfile = QFileDialog::getSaveFileName(this,"Save Simulation Configuration","../simulation/");
    if(!mfile.isEmpty()) ui->widgetPhysics->saveSimulationParameters(mfile);
     ui->widgetPhysics->startSimulation();
}

void MainWindow::on_actionOpen_Simulation_triggered()
{
    ui->widgetPhysics->stopSimulation();
    QString mfile = QFileDialog::getOpenFileName(this,"Load Simulation Physics","../simulation/");
    if(!mfile.isEmpty()){
        ui->widgetPhysics->loadSimulationParameters(mfile);
        ui->groupBoxCone->setVisible(true);
    }
    ui->widgetPhysics->startSimulation();
}
