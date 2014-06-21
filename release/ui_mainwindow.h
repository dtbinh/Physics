/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Wed Apr 23 11:00:16 2014
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QSpinBox>
#include <QtGui/QSplitter>
#include <QtGui/QTabWidget>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <interface/glwidget.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionPlay_Pause_Simulation;
    QAction *actionRestart_Simulation;
    QAction *actionOpen_Model;
    QAction *actionOpen_MoCap;
    QAction *actionSave_Model;
    QAction *actionSave_Simulation;
    QAction *actionOpen_Simulation;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QSplitter *splitter;
    GLWidget *widgetPhysics;
    QWidget *widgetOpt;
    QGridLayout *gridLayout_2;
    QTabWidget *Options;
    QWidget *tabParSim;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpinBox *stepSim;
    QCheckBox *enableGravity;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_11;
    QDoubleSpinBox *gravx;
    QDoubleSpinBox *gravy;
    QDoubleSpinBox *gravz;
    QCheckBox *checkBox;
    QCheckBox *kdtoks;
    QCheckBox *checkMesh;
    QCheckBox *infoShow;
    QWidget *tab;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_7;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_12;
    QDoubleSpinBox *xksTqBal;
    QDoubleSpinBox *yksTqBal;
    QDoubleSpinBox *zksTqBal;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_13;
    QDoubleSpinBox *xkdTqBal;
    QDoubleSpinBox *ykdTqBal;
    QDoubleSpinBox *zkdTqBal;
    QHBoxLayout *horizontalLayout_13;
    QLabel *label_14;
    QDoubleSpinBox *xksForBal;
    QDoubleSpinBox *yksForBal;
    QDoubleSpinBox *zksForBal;
    QHBoxLayout *horizontalLayout_14;
    QLabel *label_15;
    QDoubleSpinBox *xkdForBal;
    QDoubleSpinBox *ykdForBal;
    QDoubleSpinBox *zkdForBal;
    QHBoxLayout *horizontalLayout_19;
    QLabel *label_18;
    QDoubleSpinBox *xkmomlinBal;
    QDoubleSpinBox *ykmomlinBal;
    QDoubleSpinBox *zkmomlinBal;
    QHBoxLayout *horizontalLayout_37;
    QLabel *label_35;
    QDoubleSpinBox *xkmomangBal;
    QDoubleSpinBox *ykmomangBal;
    QDoubleSpinBox *zkmomangBal;
    QHBoxLayout *horizontalLayout_20;
    QCheckBox *checkMomentum;
    QCheckBox *checkTorqueBal;
    QCheckBox *checkForceBal;
    QHBoxLayout *horizontalLayout_21;
    QLabel *label_20;
    QSpinBox *angleBalBodyx;
    QSpinBox *angleBalBodyy;
    QSpinBox *angleBalBodyz;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_8;
    QHBoxLayout *horizontalLayout_15;
    QLabel *label_16;
    QDoubleSpinBox *xksPdProp;
    QDoubleSpinBox *yksPdProp;
    QDoubleSpinBox *zksPdProp;
    QHBoxLayout *horizontalLayout_16;
    QLabel *label_17;
    QDoubleSpinBox *xkdPdProp;
    QDoubleSpinBox *ykdPdProp;
    QDoubleSpinBox *zkdPdProp;
    QGroupBox *groupBoxCone;
    QWidget *layoutWidget2;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_27;
    QLabel *label_28;
    QDoubleSpinBox *coneM;
    QHBoxLayout *horizontalLayout_29;
    QLabel *label_30;
    QDoubleSpinBox *coneHeight;
    QHBoxLayout *horizontalLayout_28;
    QLabel *label_19;
    QDoubleSpinBox *coneRadius;
    QHBoxLayout *horizontalLayout_30;
    QLabel *label_29;
    QDoubleSpinBox *coneAngle;
    QHBoxLayout *horizontalLayout_23;
    QLabel *label_24;
    QSpinBox *coneLimit;
    QFrame *FootLeft;
    QHBoxLayout *horizontalLayout_25;
    QLabel *label_26;
    QSlider *coneFoot1;
    QLabel *conePerc1;
    QFrame *FootRight;
    QHBoxLayout *horizontalLayout_26;
    QLabel *label_27;
    QSlider *coneFoot2;
    QLabel *conePerc2;
    QGroupBox *groupBox_7;
    QGridLayout *gridLayout_11;
    QHBoxLayout *horizontalLayout_24;
    QCheckBox *checkGRF;
    QCheckBox *checkShadow;
    QCheckBox *checkShowChara;
    QCheckBox *checkWireChara;
    QWidget *tab_2;
    QPushButton *btnStart;
    QPushButton *btnPause;
    QPushButton *btnRestart;
    QSlider *timeLineMotion;
    QWidget *layoutWidget3;
    QHBoxLayout *horizontalLayout_22;
    QLabel *iframe;
    QLabel *label_23;
    QLabel *nframe;
    QGroupBox *groupBox_11;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_36;
    QLabel *label_34;
    QSpinBox *frameEdit;
    QCheckBox *checkShowEditingFrame;
    QCheckBox *checkFrameFootLeft;
    QCheckBox *checkFrameFootRight;
    QPushButton *saveEditingFrame;
    QPushButton *loadEditedFrames;
    QWidget *layoutWidget4;
    QHBoxLayout *horizontalLayout_35;
    QLabel *label_22;
    QDoubleSpinBox *sensorTolerance;
    QGroupBox *groupBox_12;
    QGridLayout *gridLayout_15;
    QCheckBox *mocapCyclic;
    QHBoxLayout *horizontalLayout_38;
    QLabel *label_36;
    QSpinBox *beginClycle;
    QHBoxLayout *horizontalLayout_39;
    QLabel *label_37;
    QSpinBox *endClycle;
    QCheckBox *checkShowMocap;
    QCheckBox *checkScreenShot;
    QDoubleSpinBox *toleranciaCOM;
    QWidget *tabManipulatorSim;
    QGridLayout *gridLayout_12;
    QListWidget *listWidgetObjects;
    QGroupBox *groupBox_9;
    QGridLayout *gridLayout_6;
    QHBoxLayout *horizontalLayout_31;
    QCheckBox *enable_cpdp;
    QCheckBox *show_effector;
    QCheckBox *show_target;
    QHBoxLayout *horizontalLayout_32;
    QLabel *label_31;
    QDoubleSpinBox *posx_target;
    QDoubleSpinBox *posy_target;
    QDoubleSpinBox *posz_target;
    QHBoxLayout *horizontalLayout_33;
    QLabel *label_32;
    QDoubleSpinBox *posx_ks;
    QDoubleSpinBox *posy_ks;
    QDoubleSpinBox *posz_ks;
    QHBoxLayout *horizontalLayout_34;
    QLabel *label_33;
    QDoubleSpinBox *posx_kd;
    QDoubleSpinBox *posy_kd;
    QDoubleSpinBox *posz_kd;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_3;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_9;
    QDoubleSpinBox *scalex;
    QDoubleSpinBox *scaley;
    QDoubleSpinBox *scalez;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_7;
    QDoubleSpinBox *posx;
    QDoubleSpinBox *posy;
    QDoubleSpinBox *posz;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_10;
    QDoubleSpinBox *mass;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_8;
    QSpinBox *oeulerx;
    QSpinBox *oeulery;
    QSpinBox *oeulerz;
    QCheckBox *isFoot;
    QCheckBox *isBodyBalance;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_5;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_2;
    QDoubleSpinBox *forcex;
    QDoubleSpinBox *forcey;
    QDoubleSpinBox *forcez;
    QPushButton *applyForce;
    QGroupBox *groupBox_10;
    QGridLayout *gridLayout_13;
    QCheckBox *hasCup;
    QWidget *controlpd;
    QGridLayout *gridLayout_10;
    QListWidget *listWidgetJoints;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout_9;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QDoubleSpinBox *ksx;
    QDoubleSpinBox *ksy;
    QDoubleSpinBox *ksz;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QDoubleSpinBox *kdx;
    QDoubleSpinBox *kdy;
    QDoubleSpinBox *kdz;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_5;
    QDoubleSpinBox *eulerx;
    QDoubleSpinBox *eulery;
    QDoubleSpinBox *eulerz;
    QCheckBox *enablepd;
    QGroupBox *groupBox_6;
    QGridLayout *gridLayout_4;
    QHBoxLayout *horizontalLayout_17;
    QLabel *label_6;
    QLabel *xksPdApply;
    QLabel *yksPdApply;
    QLabel *zksPdApply;
    QHBoxLayout *horizontalLayout_18;
    QLabel *label_21;
    QLabel *xkdPdApply;
    QLabel *ykdPdApply;
    QLabel *zkdPdApply;
    QFrame *infoPD;
    QVBoxLayout *verticalLayout;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuControl;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(658, 780);
        actionPlay_Pause_Simulation = new QAction(MainWindow);
        actionPlay_Pause_Simulation->setObjectName(QString::fromUtf8("actionPlay_Pause_Simulation"));
        actionRestart_Simulation = new QAction(MainWindow);
        actionRestart_Simulation->setObjectName(QString::fromUtf8("actionRestart_Simulation"));
        actionOpen_Model = new QAction(MainWindow);
        actionOpen_Model->setObjectName(QString::fromUtf8("actionOpen_Model"));
        actionOpen_MoCap = new QAction(MainWindow);
        actionOpen_MoCap->setObjectName(QString::fromUtf8("actionOpen_MoCap"));
        actionSave_Model = new QAction(MainWindow);
        actionSave_Model->setObjectName(QString::fromUtf8("actionSave_Model"));
        actionSave_Simulation = new QAction(MainWindow);
        actionSave_Simulation->setObjectName(QString::fromUtf8("actionSave_Simulation"));
        actionOpen_Simulation = new QAction(MainWindow);
        actionOpen_Simulation->setObjectName(QString::fromUtf8("actionOpen_Simulation"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        widgetPhysics = new GLWidget(splitter);
        widgetPhysics->setObjectName(QString::fromUtf8("widgetPhysics"));
        widgetPhysics->setAutoFillBackground(false);
        splitter->addWidget(widgetPhysics);
        widgetOpt = new QWidget(splitter);
        widgetOpt->setObjectName(QString::fromUtf8("widgetOpt"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widgetOpt->sizePolicy().hasHeightForWidth());
        widgetOpt->setSizePolicy(sizePolicy);
        widgetOpt->setMaximumSize(QSize(350, 16777215));
        gridLayout_2 = new QGridLayout(widgetOpt);
        gridLayout_2->setSpacing(0);
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        Options = new QTabWidget(widgetOpt);
        Options->setObjectName(QString::fromUtf8("Options"));
        Options->setLayoutDirection(Qt::LeftToRight);
        Options->setTabPosition(QTabWidget::West);
        Options->setTabsClosable(false);
        Options->setMovable(true);
        tabParSim = new QWidget();
        tabParSim->setObjectName(QString::fromUtf8("tabParSim"));
        layoutWidget = new QWidget(tabParSim);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(0, 10, 311, 29));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        stepSim = new QSpinBox(layoutWidget);
        stepSim->setObjectName(QString::fromUtf8("stepSim"));
        stepSim->setMinimum(1);
        stepSim->setMaximum(1000);
        stepSim->setValue(67);

        horizontalLayout->addWidget(stepSim);

        enableGravity = new QCheckBox(tabParSim);
        enableGravity->setObjectName(QString::fromUtf8("enableGravity"));
        enableGravity->setGeometry(QRect(0, 70, 131, 22));
        layoutWidget1 = new QWidget(tabParSim);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(0, 40, 310, 29));
        horizontalLayout_10 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        horizontalLayout_10->setContentsMargins(0, 0, 0, 0);
        label_11 = new QLabel(layoutWidget1);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        horizontalLayout_10->addWidget(label_11);

        gravx = new QDoubleSpinBox(layoutWidget1);
        gravx->setObjectName(QString::fromUtf8("gravx"));
        gravx->setDecimals(1);
        gravx->setMinimum(-99);
        gravx->setMaximum(99);

        horizontalLayout_10->addWidget(gravx);

        gravy = new QDoubleSpinBox(layoutWidget1);
        gravy->setObjectName(QString::fromUtf8("gravy"));
        gravy->setDecimals(1);
        gravy->setMinimum(-99);
        gravy->setMaximum(99);
        gravy->setValue(0);

        horizontalLayout_10->addWidget(gravy);

        gravz = new QDoubleSpinBox(layoutWidget1);
        gravz->setObjectName(QString::fromUtf8("gravz"));
        gravz->setDecimals(1);
        gravz->setMinimum(-99);
        gravz->setMaximum(99);

        horizontalLayout_10->addWidget(gravz);

        checkBox = new QCheckBox(tabParSim);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(0, 100, 281, 16));
        kdtoks = new QCheckBox(tabParSim);
        kdtoks->setObjectName(QString::fromUtf8("kdtoks"));
        kdtoks->setGeometry(QRect(0, 130, 201, 22));
        checkMesh = new QCheckBox(tabParSim);
        checkMesh->setObjectName(QString::fromUtf8("checkMesh"));
        checkMesh->setGeometry(QRect(0, 160, 181, 22));
        infoShow = new QCheckBox(tabParSim);
        infoShow->setObjectName(QString::fromUtf8("infoShow"));
        infoShow->setGeometry(QRect(0, 190, 97, 22));
        Options->addTab(tabParSim, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        groupBox_3 = new QGroupBox(tab);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(9, 9, 301, 271));
        QFont font;
        font.setPointSize(9);
        font.setBold(true);
        font.setWeight(75);
        groupBox_3->setFont(font);
        groupBox_3->setStyleSheet(QString::fromUtf8("QGroupBox::title { \n"
"   \n"
"     subcontrol-position: top left; /* position at the top left*/ \n"
"  padding: 2px 13px;\n"
"   margin-top: -5px;\n"
"    \n"
" }\n"
"\n"
"QGroupBox{\n"
"   border: 1px solid rgb(182, 181, 179);\n"
"  border-radius: 5px;\n"
"}"));
        gridLayout_7 = new QGridLayout(groupBox_3);
        gridLayout_7->setSpacing(2);
        gridLayout_7->setContentsMargins(2, 2, 2, 2);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        label_12 = new QLabel(groupBox_3);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        horizontalLayout_11->addWidget(label_12);

        xksTqBal = new QDoubleSpinBox(groupBox_3);
        xksTqBal->setObjectName(QString::fromUtf8("xksTqBal"));
        xksTqBal->setMinimum(-500);
        xksTqBal->setMaximum(500);

        horizontalLayout_11->addWidget(xksTqBal);

        yksTqBal = new QDoubleSpinBox(groupBox_3);
        yksTqBal->setObjectName(QString::fromUtf8("yksTqBal"));
        yksTqBal->setMinimum(-500);
        yksTqBal->setMaximum(500);

        horizontalLayout_11->addWidget(yksTqBal);

        zksTqBal = new QDoubleSpinBox(groupBox_3);
        zksTqBal->setObjectName(QString::fromUtf8("zksTqBal"));
        zksTqBal->setMinimum(-500);
        zksTqBal->setMaximum(500);

        horizontalLayout_11->addWidget(zksTqBal);


        gridLayout_7->addLayout(horizontalLayout_11, 0, 0, 1, 1);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        label_13 = new QLabel(groupBox_3);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        horizontalLayout_12->addWidget(label_13);

        xkdTqBal = new QDoubleSpinBox(groupBox_3);
        xkdTqBal->setObjectName(QString::fromUtf8("xkdTqBal"));
        xkdTqBal->setMinimum(-500);
        xkdTqBal->setMaximum(500);

        horizontalLayout_12->addWidget(xkdTqBal);

        ykdTqBal = new QDoubleSpinBox(groupBox_3);
        ykdTqBal->setObjectName(QString::fromUtf8("ykdTqBal"));
        ykdTqBal->setMinimum(-500);
        ykdTqBal->setMaximum(500);

        horizontalLayout_12->addWidget(ykdTqBal);

        zkdTqBal = new QDoubleSpinBox(groupBox_3);
        zkdTqBal->setObjectName(QString::fromUtf8("zkdTqBal"));
        zkdTqBal->setMinimum(-500);
        zkdTqBal->setMaximum(500);

        horizontalLayout_12->addWidget(zkdTqBal);


        gridLayout_7->addLayout(horizontalLayout_12, 1, 0, 1, 1);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        label_14 = new QLabel(groupBox_3);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        horizontalLayout_13->addWidget(label_14);

        xksForBal = new QDoubleSpinBox(groupBox_3);
        xksForBal->setObjectName(QString::fromUtf8("xksForBal"));
        xksForBal->setMinimum(-9999);
        xksForBal->setMaximum(9999);

        horizontalLayout_13->addWidget(xksForBal);

        yksForBal = new QDoubleSpinBox(groupBox_3);
        yksForBal->setObjectName(QString::fromUtf8("yksForBal"));
        yksForBal->setMinimum(-9999);
        yksForBal->setMaximum(9999);

        horizontalLayout_13->addWidget(yksForBal);

        zksForBal = new QDoubleSpinBox(groupBox_3);
        zksForBal->setObjectName(QString::fromUtf8("zksForBal"));
        zksForBal->setMinimum(-9999);
        zksForBal->setMaximum(9999);

        horizontalLayout_13->addWidget(zksForBal);


        gridLayout_7->addLayout(horizontalLayout_13, 2, 0, 1, 1);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setSpacing(6);
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        label_15 = new QLabel(groupBox_3);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        horizontalLayout_14->addWidget(label_15);

        xkdForBal = new QDoubleSpinBox(groupBox_3);
        xkdForBal->setObjectName(QString::fromUtf8("xkdForBal"));
        xkdForBal->setMinimum(-999);
        xkdForBal->setMaximum(999);

        horizontalLayout_14->addWidget(xkdForBal);

        ykdForBal = new QDoubleSpinBox(groupBox_3);
        ykdForBal->setObjectName(QString::fromUtf8("ykdForBal"));
        ykdForBal->setMinimum(-500);
        ykdForBal->setMaximum(500);

        horizontalLayout_14->addWidget(ykdForBal);

        zkdForBal = new QDoubleSpinBox(groupBox_3);
        zkdForBal->setObjectName(QString::fromUtf8("zkdForBal"));
        zkdForBal->setMinimum(-500);
        zkdForBal->setMaximum(500);

        horizontalLayout_14->addWidget(zkdForBal);


        gridLayout_7->addLayout(horizontalLayout_14, 3, 0, 1, 1);

        horizontalLayout_19 = new QHBoxLayout();
        horizontalLayout_19->setSpacing(6);
        horizontalLayout_19->setObjectName(QString::fromUtf8("horizontalLayout_19"));
        label_18 = new QLabel(groupBox_3);
        label_18->setObjectName(QString::fromUtf8("label_18"));

        horizontalLayout_19->addWidget(label_18);

        xkmomlinBal = new QDoubleSpinBox(groupBox_3);
        xkmomlinBal->setObjectName(QString::fromUtf8("xkmomlinBal"));
        xkmomlinBal->setMinimum(-500);
        xkmomlinBal->setMaximum(500);

        horizontalLayout_19->addWidget(xkmomlinBal);

        ykmomlinBal = new QDoubleSpinBox(groupBox_3);
        ykmomlinBal->setObjectName(QString::fromUtf8("ykmomlinBal"));
        ykmomlinBal->setMinimum(-500);
        ykmomlinBal->setMaximum(500);

        horizontalLayout_19->addWidget(ykmomlinBal);

        zkmomlinBal = new QDoubleSpinBox(groupBox_3);
        zkmomlinBal->setObjectName(QString::fromUtf8("zkmomlinBal"));
        zkmomlinBal->setMinimum(-500);
        zkmomlinBal->setMaximum(500);

        horizontalLayout_19->addWidget(zkmomlinBal);


        gridLayout_7->addLayout(horizontalLayout_19, 4, 0, 1, 1);

        horizontalLayout_37 = new QHBoxLayout();
        horizontalLayout_37->setSpacing(6);
        horizontalLayout_37->setObjectName(QString::fromUtf8("horizontalLayout_37"));
        label_35 = new QLabel(groupBox_3);
        label_35->setObjectName(QString::fromUtf8("label_35"));

        horizontalLayout_37->addWidget(label_35);

        xkmomangBal = new QDoubleSpinBox(groupBox_3);
        xkmomangBal->setObjectName(QString::fromUtf8("xkmomangBal"));
        xkmomangBal->setMinimum(-500);
        xkmomangBal->setMaximum(500);

        horizontalLayout_37->addWidget(xkmomangBal);

        ykmomangBal = new QDoubleSpinBox(groupBox_3);
        ykmomangBal->setObjectName(QString::fromUtf8("ykmomangBal"));
        ykmomangBal->setMinimum(-500);
        ykmomangBal->setMaximum(500);

        horizontalLayout_37->addWidget(ykmomangBal);

        zkmomangBal = new QDoubleSpinBox(groupBox_3);
        zkmomangBal->setObjectName(QString::fromUtf8("zkmomangBal"));
        zkmomangBal->setMinimum(-500);
        zkmomangBal->setMaximum(500);

        horizontalLayout_37->addWidget(zkmomangBal);


        gridLayout_7->addLayout(horizontalLayout_37, 5, 0, 1, 1);

        horizontalLayout_20 = new QHBoxLayout();
        horizontalLayout_20->setSpacing(6);
        horizontalLayout_20->setObjectName(QString::fromUtf8("horizontalLayout_20"));
        checkMomentum = new QCheckBox(groupBox_3);
        checkMomentum->setObjectName(QString::fromUtf8("checkMomentum"));
        checkMomentum->setChecked(true);

        horizontalLayout_20->addWidget(checkMomentum);

        checkTorqueBal = new QCheckBox(groupBox_3);
        checkTorqueBal->setObjectName(QString::fromUtf8("checkTorqueBal"));
        checkTorqueBal->setChecked(true);

        horizontalLayout_20->addWidget(checkTorqueBal);

        checkForceBal = new QCheckBox(groupBox_3);
        checkForceBal->setObjectName(QString::fromUtf8("checkForceBal"));
        checkForceBal->setChecked(true);

        horizontalLayout_20->addWidget(checkForceBal);


        gridLayout_7->addLayout(horizontalLayout_20, 6, 0, 1, 1);

        horizontalLayout_21 = new QHBoxLayout();
        horizontalLayout_21->setSpacing(6);
        horizontalLayout_21->setObjectName(QString::fromUtf8("horizontalLayout_21"));
        label_20 = new QLabel(groupBox_3);
        label_20->setObjectName(QString::fromUtf8("label_20"));

        horizontalLayout_21->addWidget(label_20);

        angleBalBodyx = new QSpinBox(groupBox_3);
        angleBalBodyx->setObjectName(QString::fromUtf8("angleBalBodyx"));
        angleBalBodyx->setMinimum(-361);
        angleBalBodyx->setMaximum(361);

        horizontalLayout_21->addWidget(angleBalBodyx);

        angleBalBodyy = new QSpinBox(groupBox_3);
        angleBalBodyy->setObjectName(QString::fromUtf8("angleBalBodyy"));
        angleBalBodyy->setMinimum(-361);
        angleBalBodyy->setMaximum(361);

        horizontalLayout_21->addWidget(angleBalBodyy);

        angleBalBodyz = new QSpinBox(groupBox_3);
        angleBalBodyz->setObjectName(QString::fromUtf8("angleBalBodyz"));
        angleBalBodyz->setMinimum(-361);
        angleBalBodyz->setMaximum(361);

        horizontalLayout_21->addWidget(angleBalBodyz);


        gridLayout_7->addLayout(horizontalLayout_21, 8, 0, 1, 1);

        groupBox_4 = new QGroupBox(tab);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(10, 287, 299, 84));
        groupBox_4->setFont(font);
        groupBox_4->setStyleSheet(QString::fromUtf8("QGroupBox::title { \n"
"   \n"
"     subcontrol-position: top left; /* position at the top left*/ \n"
"  padding: 2px 13px;\n"
"   margin-top: -5px;\n"
"    \n"
" }\n"
"\n"
"QGroupBox{\n"
"   border: 1px solid rgb(182, 181, 179);\n"
"  border-radius: 5px;\n"
"}"));
        gridLayout_8 = new QGridLayout(groupBox_4);
        gridLayout_8->setSpacing(6);
        gridLayout_8->setContentsMargins(11, 11, 11, 11);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setSpacing(6);
        horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
        label_16 = new QLabel(groupBox_4);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        horizontalLayout_15->addWidget(label_16);

        xksPdProp = new QDoubleSpinBox(groupBox_4);
        xksPdProp->setObjectName(QString::fromUtf8("xksPdProp"));
        xksPdProp->setMinimum(-500);
        xksPdProp->setMaximum(500);
        xksPdProp->setValue(1);

        horizontalLayout_15->addWidget(xksPdProp);

        yksPdProp = new QDoubleSpinBox(groupBox_4);
        yksPdProp->setObjectName(QString::fromUtf8("yksPdProp"));
        yksPdProp->setMinimum(-500);
        yksPdProp->setMaximum(500);
        yksPdProp->setValue(1);

        horizontalLayout_15->addWidget(yksPdProp);

        zksPdProp = new QDoubleSpinBox(groupBox_4);
        zksPdProp->setObjectName(QString::fromUtf8("zksPdProp"));
        zksPdProp->setMinimum(-500);
        zksPdProp->setMaximum(500);
        zksPdProp->setValue(1);

        horizontalLayout_15->addWidget(zksPdProp);


        gridLayout_8->addLayout(horizontalLayout_15, 0, 0, 1, 1);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setSpacing(6);
        horizontalLayout_16->setObjectName(QString::fromUtf8("horizontalLayout_16"));
        label_17 = new QLabel(groupBox_4);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        horizontalLayout_16->addWidget(label_17);

        xkdPdProp = new QDoubleSpinBox(groupBox_4);
        xkdPdProp->setObjectName(QString::fromUtf8("xkdPdProp"));
        xkdPdProp->setMinimum(-500);
        xkdPdProp->setMaximum(500);
        xkdPdProp->setSingleStep(0.1);
        xkdPdProp->setValue(1);

        horizontalLayout_16->addWidget(xkdPdProp);

        ykdPdProp = new QDoubleSpinBox(groupBox_4);
        ykdPdProp->setObjectName(QString::fromUtf8("ykdPdProp"));
        ykdPdProp->setMinimum(-500);
        ykdPdProp->setMaximum(500);
        ykdPdProp->setSingleStep(0.1);
        ykdPdProp->setValue(1);

        horizontalLayout_16->addWidget(ykdPdProp);

        zkdPdProp = new QDoubleSpinBox(groupBox_4);
        zkdPdProp->setObjectName(QString::fromUtf8("zkdPdProp"));
        zkdPdProp->setMinimum(-500);
        zkdPdProp->setMaximum(500);
        zkdPdProp->setSingleStep(0.1);
        zkdPdProp->setValue(1);

        horizontalLayout_16->addWidget(zkdPdProp);


        gridLayout_8->addLayout(horizontalLayout_16, 1, 0, 1, 1);

        groupBoxCone = new QGroupBox(tab);
        groupBoxCone->setObjectName(QString::fromUtf8("groupBoxCone"));
        groupBoxCone->setGeometry(QRect(10, 430, 301, 241));
        groupBoxCone->setFont(font);
        groupBoxCone->setStyleSheet(QString::fromUtf8("QGroupBox::title { \n"
"   \n"
"     subcontrol-position: top left; /* position at the top left*/ \n"
"  padding: 2px 13px;\n"
"   margin-top: -5px;\n"
"    \n"
" }\n"
"\n"
"QGroupBox{\n"
"   border: 1px solid rgb(182, 181, 179);\n"
"  border-radius: 5px;\n"
"}"));
        layoutWidget2 = new QWidget(groupBoxCone);
        layoutWidget2->setObjectName(QString::fromUtf8("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(3, 3, 291, 236));
        verticalLayout_3 = new QVBoxLayout(layoutWidget2);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(1);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_27 = new QHBoxLayout();
        horizontalLayout_27->setSpacing(6);
        horizontalLayout_27->setObjectName(QString::fromUtf8("horizontalLayout_27"));
        label_28 = new QLabel(layoutWidget2);
        label_28->setObjectName(QString::fromUtf8("label_28"));

        horizontalLayout_27->addWidget(label_28);

        coneM = new QDoubleSpinBox(layoutWidget2);
        coneM->setObjectName(QString::fromUtf8("coneM"));
        coneM->setMaximum(2000);
        coneM->setSingleStep(2);
        coneM->setValue(20);

        horizontalLayout_27->addWidget(coneM);


        verticalLayout_2->addLayout(horizontalLayout_27);

        horizontalLayout_29 = new QHBoxLayout();
        horizontalLayout_29->setSpacing(6);
        horizontalLayout_29->setObjectName(QString::fromUtf8("horizontalLayout_29"));
        label_30 = new QLabel(layoutWidget2);
        label_30->setObjectName(QString::fromUtf8("label_30"));

        horizontalLayout_29->addWidget(label_30);

        coneHeight = new QDoubleSpinBox(layoutWidget2);
        coneHeight->setObjectName(QString::fromUtf8("coneHeight"));
        coneHeight->setMaximum(500);
        coneHeight->setSingleStep(0.01);
        coneHeight->setValue(0.03);

        horizontalLayout_29->addWidget(coneHeight);


        verticalLayout_2->addLayout(horizontalLayout_29);

        horizontalLayout_28 = new QHBoxLayout();
        horizontalLayout_28->setSpacing(6);
        horizontalLayout_28->setObjectName(QString::fromUtf8("horizontalLayout_28"));
        label_19 = new QLabel(layoutWidget2);
        label_19->setObjectName(QString::fromUtf8("label_19"));

        horizontalLayout_28->addWidget(label_19);

        coneRadius = new QDoubleSpinBox(layoutWidget2);
        coneRadius->setObjectName(QString::fromUtf8("coneRadius"));
        coneRadius->setMaximum(500);
        coneRadius->setSingleStep(0.01);
        coneRadius->setValue(0.07);

        horizontalLayout_28->addWidget(coneRadius);


        verticalLayout_2->addLayout(horizontalLayout_28);

        horizontalLayout_30 = new QHBoxLayout();
        horizontalLayout_30->setSpacing(6);
        horizontalLayout_30->setObjectName(QString::fromUtf8("horizontalLayout_30"));
        label_29 = new QLabel(layoutWidget2);
        label_29->setObjectName(QString::fromUtf8("label_29"));

        horizontalLayout_30->addWidget(label_29);

        coneAngle = new QDoubleSpinBox(layoutWidget2);
        coneAngle->setObjectName(QString::fromUtf8("coneAngle"));
        coneAngle->setMinimum(-180);
        coneAngle->setMaximum(180);
        coneAngle->setSingleStep(5);
        coneAngle->setValue(45);

        horizontalLayout_30->addWidget(coneAngle);


        verticalLayout_2->addLayout(horizontalLayout_30);


        verticalLayout_3->addLayout(verticalLayout_2);

        horizontalLayout_23 = new QHBoxLayout();
        horizontalLayout_23->setSpacing(6);
        horizontalLayout_23->setObjectName(QString::fromUtf8("horizontalLayout_23"));
        label_24 = new QLabel(layoutWidget2);
        label_24->setObjectName(QString::fromUtf8("label_24"));

        horizontalLayout_23->addWidget(label_24);

        coneLimit = new QSpinBox(layoutWidget2);
        coneLimit->setObjectName(QString::fromUtf8("coneLimit"));
        coneLimit->setMaximum(100);

        horizontalLayout_23->addWidget(coneLimit);


        verticalLayout_3->addLayout(horizontalLayout_23);

        FootLeft = new QFrame(layoutWidget2);
        FootLeft->setObjectName(QString::fromUtf8("FootLeft"));
        horizontalLayout_25 = new QHBoxLayout(FootLeft);
        horizontalLayout_25->setSpacing(1);
        horizontalLayout_25->setContentsMargins(2, 2, 2, 2);
        horizontalLayout_25->setObjectName(QString::fromUtf8("horizontalLayout_25"));
        label_26 = new QLabel(FootLeft);
        label_26->setObjectName(QString::fromUtf8("label_26"));

        horizontalLayout_25->addWidget(label_26);

        coneFoot1 = new QSlider(FootLeft);
        coneFoot1->setObjectName(QString::fromUtf8("coneFoot1"));
        coneFoot1->setMaximum(100);
        coneFoot1->setPageStep(5);
        coneFoot1->setValue(100);
        coneFoot1->setOrientation(Qt::Horizontal);

        horizontalLayout_25->addWidget(coneFoot1);

        conePerc1 = new QLabel(FootLeft);
        conePerc1->setObjectName(QString::fromUtf8("conePerc1"));

        horizontalLayout_25->addWidget(conePerc1);


        verticalLayout_3->addWidget(FootLeft);

        FootRight = new QFrame(layoutWidget2);
        FootRight->setObjectName(QString::fromUtf8("FootRight"));
        horizontalLayout_26 = new QHBoxLayout(FootRight);
        horizontalLayout_26->setSpacing(1);
        horizontalLayout_26->setContentsMargins(2, 2, 2, 2);
        horizontalLayout_26->setObjectName(QString::fromUtf8("horizontalLayout_26"));
        label_27 = new QLabel(FootRight);
        label_27->setObjectName(QString::fromUtf8("label_27"));

        horizontalLayout_26->addWidget(label_27);

        coneFoot2 = new QSlider(FootRight);
        coneFoot2->setObjectName(QString::fromUtf8("coneFoot2"));
        coneFoot2->setMaximum(100);
        coneFoot2->setPageStep(5);
        coneFoot2->setValue(100);
        coneFoot2->setOrientation(Qt::Horizontal);

        horizontalLayout_26->addWidget(coneFoot2);

        conePerc2 = new QLabel(FootRight);
        conePerc2->setObjectName(QString::fromUtf8("conePerc2"));

        horizontalLayout_26->addWidget(conePerc2);


        verticalLayout_3->addWidget(FootRight);

        groupBox_7 = new QGroupBox(tab);
        groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
        groupBox_7->setGeometry(QRect(10, 380, 301, 44));
        groupBox_7->setFont(font);
        groupBox_7->setStyleSheet(QString::fromUtf8("QGroupBox::title { \n"
"   \n"
"     subcontrol-position: top left; /* position at the top left*/ \n"
"  padding: 2px 13px;\n"
"   margin-top: -5px;\n"
"    \n"
" }\n"
"\n"
"QGroupBox{\n"
"   border: 1px solid rgb(182, 181, 179);\n"
"  border-radius: 5px;\n"
"}"));
        gridLayout_11 = new QGridLayout(groupBox_7);
        gridLayout_11->setSpacing(6);
        gridLayout_11->setContentsMargins(11, 11, 11, 11);
        gridLayout_11->setObjectName(QString::fromUtf8("gridLayout_11"));
        horizontalLayout_24 = new QHBoxLayout();
        horizontalLayout_24->setSpacing(2);
        horizontalLayout_24->setObjectName(QString::fromUtf8("horizontalLayout_24"));
        checkGRF = new QCheckBox(groupBox_7);
        checkGRF->setObjectName(QString::fromUtf8("checkGRF"));
        checkGRF->setChecked(true);

        horizontalLayout_24->addWidget(checkGRF);

        checkShadow = new QCheckBox(groupBox_7);
        checkShadow->setObjectName(QString::fromUtf8("checkShadow"));

        horizontalLayout_24->addWidget(checkShadow);

        checkShowChara = new QCheckBox(groupBox_7);
        checkShowChara->setObjectName(QString::fromUtf8("checkShowChara"));
        checkShowChara->setChecked(true);

        horizontalLayout_24->addWidget(checkShowChara);

        checkWireChara = new QCheckBox(groupBox_7);
        checkWireChara->setObjectName(QString::fromUtf8("checkWireChara"));

        horizontalLayout_24->addWidget(checkWireChara);


        gridLayout_11->addLayout(horizontalLayout_24, 0, 0, 1, 1);

        Options->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        btnStart = new QPushButton(tab_2);
        btnStart->setObjectName(QString::fromUtf8("btnStart"));
        btnStart->setGeometry(QRect(10, 20, 98, 27));
        btnPause = new QPushButton(tab_2);
        btnPause->setObjectName(QString::fromUtf8("btnPause"));
        btnPause->setGeometry(QRect(110, 20, 98, 27));
        btnRestart = new QPushButton(tab_2);
        btnRestart->setObjectName(QString::fromUtf8("btnRestart"));
        btnRestart->setGeometry(QRect(210, 20, 98, 27));
        timeLineMotion = new QSlider(tab_2);
        timeLineMotion->setObjectName(QString::fromUtf8("timeLineMotion"));
        timeLineMotion->setGeometry(QRect(10, 70, 291, 29));
        timeLineMotion->setPageStep(1);
        timeLineMotion->setOrientation(Qt::Horizontal);
        layoutWidget3 = new QWidget(tab_2);
        layoutWidget3->setObjectName(QString::fromUtf8("layoutWidget3"));
        layoutWidget3->setGeometry(QRect(90, 100, 135, 19));
        horizontalLayout_22 = new QHBoxLayout(layoutWidget3);
        horizontalLayout_22->setSpacing(6);
        horizontalLayout_22->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_22->setObjectName(QString::fromUtf8("horizontalLayout_22"));
        horizontalLayout_22->setContentsMargins(0, 0, 0, 0);
        iframe = new QLabel(layoutWidget3);
        iframe->setObjectName(QString::fromUtf8("iframe"));

        horizontalLayout_22->addWidget(iframe);

        label_23 = new QLabel(layoutWidget3);
        label_23->setObjectName(QString::fromUtf8("label_23"));

        horizontalLayout_22->addWidget(label_23);

        nframe = new QLabel(layoutWidget3);
        nframe->setObjectName(QString::fromUtf8("nframe"));

        horizontalLayout_22->addWidget(nframe);

        groupBox_11 = new QGroupBox(tab_2);
        groupBox_11->setObjectName(QString::fromUtf8("groupBox_11"));
        groupBox_11->setGeometry(QRect(10, 230, 301, 221));
        groupBox_11->setFont(font);
        groupBox_11->setStyleSheet(QString::fromUtf8("QGroupBox::title { \n"
"   \n"
"     subcontrol-position: top left; /* position at the top left*/ \n"
"  padding: 2px 13px;\n"
"   margin-top: -5px;\n"
"    \n"
" }\n"
"\n"
"QGroupBox{\n"
"   border: 1px solid rgb(182, 181, 179);\n"
"  border-radius: 5px;\n"
"}"));
        verticalLayout_4 = new QVBoxLayout(groupBox_11);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_36 = new QHBoxLayout();
        horizontalLayout_36->setSpacing(6);
        horizontalLayout_36->setObjectName(QString::fromUtf8("horizontalLayout_36"));
        label_34 = new QLabel(groupBox_11);
        label_34->setObjectName(QString::fromUtf8("label_34"));

        horizontalLayout_36->addWidget(label_34);

        frameEdit = new QSpinBox(groupBox_11);
        frameEdit->setObjectName(QString::fromUtf8("frameEdit"));

        horizontalLayout_36->addWidget(frameEdit);


        verticalLayout_4->addLayout(horizontalLayout_36);

        checkShowEditingFrame = new QCheckBox(groupBox_11);
        checkShowEditingFrame->setObjectName(QString::fromUtf8("checkShowEditingFrame"));

        verticalLayout_4->addWidget(checkShowEditingFrame);

        checkFrameFootLeft = new QCheckBox(groupBox_11);
        checkFrameFootLeft->setObjectName(QString::fromUtf8("checkFrameFootLeft"));
        checkFrameFootLeft->setChecked(true);

        verticalLayout_4->addWidget(checkFrameFootLeft);

        checkFrameFootRight = new QCheckBox(groupBox_11);
        checkFrameFootRight->setObjectName(QString::fromUtf8("checkFrameFootRight"));
        checkFrameFootRight->setChecked(true);

        verticalLayout_4->addWidget(checkFrameFootRight);

        saveEditingFrame = new QPushButton(groupBox_11);
        saveEditingFrame->setObjectName(QString::fromUtf8("saveEditingFrame"));

        verticalLayout_4->addWidget(saveEditingFrame);

        loadEditedFrames = new QPushButton(groupBox_11);
        loadEditedFrames->setObjectName(QString::fromUtf8("loadEditedFrames"));

        verticalLayout_4->addWidget(loadEditedFrames);

        layoutWidget4 = new QWidget(tab_2);
        layoutWidget4->setObjectName(QString::fromUtf8("layoutWidget4"));
        layoutWidget4->setGeometry(QRect(20, 140, 251, 29));
        horizontalLayout_35 = new QHBoxLayout(layoutWidget4);
        horizontalLayout_35->setSpacing(6);
        horizontalLayout_35->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_35->setObjectName(QString::fromUtf8("horizontalLayout_35"));
        horizontalLayout_35->setContentsMargins(0, 0, 0, 0);
        label_22 = new QLabel(layoutWidget4);
        label_22->setObjectName(QString::fromUtf8("label_22"));

        horizontalLayout_35->addWidget(label_22);

        sensorTolerance = new QDoubleSpinBox(layoutWidget4);
        sensorTolerance->setObjectName(QString::fromUtf8("sensorTolerance"));
        sensorTolerance->setDecimals(2);
        sensorTolerance->setMaximum(999);
        sensorTolerance->setSingleStep(0.01);
        sensorTolerance->setValue(50);

        horizontalLayout_35->addWidget(sensorTolerance);

        groupBox_12 = new QGroupBox(tab_2);
        groupBox_12->setObjectName(QString::fromUtf8("groupBox_12"));
        groupBox_12->setGeometry(QRect(10, 470, 301, 112));
        groupBox_12->setFont(font);
        groupBox_12->setStyleSheet(QString::fromUtf8("QGroupBox::title { \n"
"   \n"
"     subcontrol-position: top left; /* position at the top left*/ \n"
"  padding: 2px 13px;\n"
"   margin-top: -5px;\n"
"    \n"
" }\n"
"\n"
"QGroupBox{\n"
"   border: 1px solid rgb(182, 181, 179);\n"
"  border-radius: 5px;\n"
"}"));
        gridLayout_15 = new QGridLayout(groupBox_12);
        gridLayout_15->setSpacing(6);
        gridLayout_15->setContentsMargins(11, 11, 11, 11);
        gridLayout_15->setObjectName(QString::fromUtf8("gridLayout_15"));
        mocapCyclic = new QCheckBox(groupBox_12);
        mocapCyclic->setObjectName(QString::fromUtf8("mocapCyclic"));
        mocapCyclic->setChecked(true);

        gridLayout_15->addWidget(mocapCyclic, 0, 0, 1, 1);

        horizontalLayout_38 = new QHBoxLayout();
        horizontalLayout_38->setSpacing(6);
        horizontalLayout_38->setObjectName(QString::fromUtf8("horizontalLayout_38"));
        label_36 = new QLabel(groupBox_12);
        label_36->setObjectName(QString::fromUtf8("label_36"));

        horizontalLayout_38->addWidget(label_36);

        beginClycle = new QSpinBox(groupBox_12);
        beginClycle->setObjectName(QString::fromUtf8("beginClycle"));

        horizontalLayout_38->addWidget(beginClycle);


        gridLayout_15->addLayout(horizontalLayout_38, 1, 0, 1, 1);

        horizontalLayout_39 = new QHBoxLayout();
        horizontalLayout_39->setSpacing(6);
        horizontalLayout_39->setObjectName(QString::fromUtf8("horizontalLayout_39"));
        label_37 = new QLabel(groupBox_12);
        label_37->setObjectName(QString::fromUtf8("label_37"));

        horizontalLayout_39->addWidget(label_37);

        endClycle = new QSpinBox(groupBox_12);
        endClycle->setObjectName(QString::fromUtf8("endClycle"));

        horizontalLayout_39->addWidget(endClycle);


        gridLayout_15->addLayout(horizontalLayout_39, 2, 0, 1, 1);

        checkShowMocap = new QCheckBox(tab_2);
        checkShowMocap->setObjectName(QString::fromUtf8("checkShowMocap"));
        checkShowMocap->setGeometry(QRect(20, 200, 151, 22));
        checkShowMocap->setChecked(true);
        checkScreenShot = new QCheckBox(tab_2);
        checkScreenShot->setObjectName(QString::fromUtf8("checkScreenShot"));
        checkScreenShot->setGeometry(QRect(20, 120, 251, 22));
        toleranciaCOM = new QDoubleSpinBox(tab_2);
        toleranciaCOM->setObjectName(QString::fromUtf8("toleranciaCOM"));
        toleranciaCOM->setGeometry(QRect(200, 180, 62, 27));
        toleranciaCOM->setDecimals(4);
        toleranciaCOM->setMinimum(-0.0001);
        toleranciaCOM->setMaximum(1000);
        Options->addTab(tab_2, QString());
        tabManipulatorSim = new QWidget();
        tabManipulatorSim->setObjectName(QString::fromUtf8("tabManipulatorSim"));
        gridLayout_12 = new QGridLayout(tabManipulatorSim);
        gridLayout_12->setSpacing(6);
        gridLayout_12->setContentsMargins(11, 11, 11, 11);
        gridLayout_12->setObjectName(QString::fromUtf8("gridLayout_12"));
        listWidgetObjects = new QListWidget(tabManipulatorSim);
        listWidgetObjects->setObjectName(QString::fromUtf8("listWidgetObjects"));

        gridLayout_12->addWidget(listWidgetObjects, 0, 0, 1, 1);

        groupBox_9 = new QGroupBox(tabManipulatorSim);
        groupBox_9->setObjectName(QString::fromUtf8("groupBox_9"));
        groupBox_9->setFont(font);
        groupBox_9->setStyleSheet(QString::fromUtf8("QGroupBox::title { \n"
"   \n"
"     subcontrol-position: top left; /* position at the top left*/ \n"
"  padding: 2px 13px;\n"
"   margin-top: -5px;\n"
"    \n"
" }\n"
"\n"
"QGroupBox{\n"
"   border: 1px solid rgb(182, 181, 179);\n"
"  border-radius: 5px;\n"
"}"));
        gridLayout_6 = new QGridLayout(groupBox_9);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        horizontalLayout_31 = new QHBoxLayout();
        horizontalLayout_31->setSpacing(6);
        horizontalLayout_31->setObjectName(QString::fromUtf8("horizontalLayout_31"));
        enable_cpdp = new QCheckBox(groupBox_9);
        enable_cpdp->setObjectName(QString::fromUtf8("enable_cpdp"));

        horizontalLayout_31->addWidget(enable_cpdp);

        show_effector = new QCheckBox(groupBox_9);
        show_effector->setObjectName(QString::fromUtf8("show_effector"));

        horizontalLayout_31->addWidget(show_effector);

        show_target = new QCheckBox(groupBox_9);
        show_target->setObjectName(QString::fromUtf8("show_target"));

        horizontalLayout_31->addWidget(show_target);


        gridLayout_6->addLayout(horizontalLayout_31, 0, 0, 1, 1);

        horizontalLayout_32 = new QHBoxLayout();
        horizontalLayout_32->setSpacing(6);
        horizontalLayout_32->setObjectName(QString::fromUtf8("horizontalLayout_32"));
        label_31 = new QLabel(groupBox_9);
        label_31->setObjectName(QString::fromUtf8("label_31"));

        horizontalLayout_32->addWidget(label_31);

        posx_target = new QDoubleSpinBox(groupBox_9);
        posx_target->setObjectName(QString::fromUtf8("posx_target"));
        posx_target->setDecimals(2);
        posx_target->setMinimum(-1000);
        posx_target->setMaximum(1000);
        posx_target->setSingleStep(0.05);

        horizontalLayout_32->addWidget(posx_target);

        posy_target = new QDoubleSpinBox(groupBox_9);
        posy_target->setObjectName(QString::fromUtf8("posy_target"));
        posy_target->setDecimals(2);
        posy_target->setMinimum(-1000);
        posy_target->setMaximum(1000);
        posy_target->setSingleStep(0.05);

        horizontalLayout_32->addWidget(posy_target);

        posz_target = new QDoubleSpinBox(groupBox_9);
        posz_target->setObjectName(QString::fromUtf8("posz_target"));
        posz_target->setDecimals(2);
        posz_target->setMinimum(-1000);
        posz_target->setMaximum(1000);
        posz_target->setSingleStep(0.05);

        horizontalLayout_32->addWidget(posz_target);


        gridLayout_6->addLayout(horizontalLayout_32, 1, 0, 1, 1);

        horizontalLayout_33 = new QHBoxLayout();
        horizontalLayout_33->setSpacing(6);
        horizontalLayout_33->setObjectName(QString::fromUtf8("horizontalLayout_33"));
        label_32 = new QLabel(groupBox_9);
        label_32->setObjectName(QString::fromUtf8("label_32"));

        horizontalLayout_33->addWidget(label_32);

        posx_ks = new QDoubleSpinBox(groupBox_9);
        posx_ks->setObjectName(QString::fromUtf8("posx_ks"));
        posx_ks->setDecimals(2);
        posx_ks->setMinimum(-1000);
        posx_ks->setMaximum(1000);
        posx_ks->setSingleStep(0.1);

        horizontalLayout_33->addWidget(posx_ks);

        posy_ks = new QDoubleSpinBox(groupBox_9);
        posy_ks->setObjectName(QString::fromUtf8("posy_ks"));
        posy_ks->setDecimals(2);
        posy_ks->setMinimum(-1000);
        posy_ks->setMaximum(1000);
        posy_ks->setSingleStep(0.1);

        horizontalLayout_33->addWidget(posy_ks);

        posz_ks = new QDoubleSpinBox(groupBox_9);
        posz_ks->setObjectName(QString::fromUtf8("posz_ks"));
        posz_ks->setDecimals(2);
        posz_ks->setMinimum(-1000);
        posz_ks->setMaximum(1000);
        posz_ks->setSingleStep(0.1);

        horizontalLayout_33->addWidget(posz_ks);


        gridLayout_6->addLayout(horizontalLayout_33, 2, 0, 1, 1);

        horizontalLayout_34 = new QHBoxLayout();
        horizontalLayout_34->setSpacing(6);
        horizontalLayout_34->setObjectName(QString::fromUtf8("horizontalLayout_34"));
        label_33 = new QLabel(groupBox_9);
        label_33->setObjectName(QString::fromUtf8("label_33"));

        horizontalLayout_34->addWidget(label_33);

        posx_kd = new QDoubleSpinBox(groupBox_9);
        posx_kd->setObjectName(QString::fromUtf8("posx_kd"));
        posx_kd->setDecimals(2);
        posx_kd->setMinimum(-1000);
        posx_kd->setMaximum(1000);
        posx_kd->setSingleStep(0.1);

        horizontalLayout_34->addWidget(posx_kd);

        posy_kd = new QDoubleSpinBox(groupBox_9);
        posy_kd->setObjectName(QString::fromUtf8("posy_kd"));
        posy_kd->setDecimals(2);
        posy_kd->setMinimum(-1000);
        posy_kd->setMaximum(1000);
        posy_kd->setSingleStep(0.1);

        horizontalLayout_34->addWidget(posy_kd);

        posz_kd = new QDoubleSpinBox(groupBox_9);
        posz_kd->setObjectName(QString::fromUtf8("posz_kd"));
        posz_kd->setDecimals(2);
        posz_kd->setMinimum(-1000);
        posz_kd->setMaximum(1000);
        posz_kd->setSingleStep(0.1);

        horizontalLayout_34->addWidget(posz_kd);


        gridLayout_6->addLayout(horizontalLayout_34, 3, 0, 1, 1);


        gridLayout_12->addWidget(groupBox_9, 2, 0, 1, 1);

        groupBox_2 = new QGroupBox(tabManipulatorSim);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setFont(font);
        groupBox_2->setStyleSheet(QString::fromUtf8("QGroupBox::title { \n"
"   \n"
"     subcontrol-position: top left; /* position at the top left*/ \n"
"  padding: 2px 13px;\n"
"   margin-top: -5px;\n"
"    \n"
" }\n"
"\n"
"QGroupBox{\n"
"   border: 1px solid rgb(182, 181, 179);\n"
"  border-radius: 5px;\n"
"}"));
        gridLayout_3 = new QGridLayout(groupBox_2);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setHorizontalSpacing(2);
        gridLayout_3->setVerticalSpacing(0);
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_9 = new QLabel(groupBox_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        horizontalLayout_8->addWidget(label_9);

        scalex = new QDoubleSpinBox(groupBox_2);
        scalex->setObjectName(QString::fromUtf8("scalex"));

        horizontalLayout_8->addWidget(scalex);

        scaley = new QDoubleSpinBox(groupBox_2);
        scaley->setObjectName(QString::fromUtf8("scaley"));

        horizontalLayout_8->addWidget(scaley);

        scalez = new QDoubleSpinBox(groupBox_2);
        scalez->setObjectName(QString::fromUtf8("scalez"));

        horizontalLayout_8->addWidget(scalez);


        gridLayout_3->addLayout(horizontalLayout_8, 2, 0, 1, 1);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        horizontalLayout_6->addWidget(label_7);

        posx = new QDoubleSpinBox(groupBox_2);
        posx->setObjectName(QString::fromUtf8("posx"));
        posx->setDecimals(3);
        posx->setMinimum(-1000);
        posx->setMaximum(1000);

        horizontalLayout_6->addWidget(posx);

        posy = new QDoubleSpinBox(groupBox_2);
        posy->setObjectName(QString::fromUtf8("posy"));
        posy->setDecimals(3);
        posy->setMinimum(-1000);
        posy->setMaximum(1000);

        horizontalLayout_6->addWidget(posy);

        posz = new QDoubleSpinBox(groupBox_2);
        posz->setObjectName(QString::fromUtf8("posz"));
        posz->setDecimals(3);
        posz->setMinimum(-1000);
        posz->setMaximum(1000);

        horizontalLayout_6->addWidget(posz);


        gridLayout_3->addLayout(horizontalLayout_6, 1, 0, 1, 1);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        label_10 = new QLabel(groupBox_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        horizontalLayout_9->addWidget(label_10);

        mass = new QDoubleSpinBox(groupBox_2);
        mass->setObjectName(QString::fromUtf8("mass"));

        horizontalLayout_9->addWidget(mass);


        gridLayout_3->addLayout(horizontalLayout_9, 5, 0, 1, 1);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        horizontalLayout_7->addWidget(label_8);

        oeulerx = new QSpinBox(groupBox_2);
        oeulerx->setObjectName(QString::fromUtf8("oeulerx"));
        oeulerx->setMinimum(-360);
        oeulerx->setMaximum(360);

        horizontalLayout_7->addWidget(oeulerx);

        oeulery = new QSpinBox(groupBox_2);
        oeulery->setObjectName(QString::fromUtf8("oeulery"));
        oeulery->setMinimum(-360);
        oeulery->setMaximum(360);

        horizontalLayout_7->addWidget(oeulery);

        oeulerz = new QSpinBox(groupBox_2);
        oeulerz->setObjectName(QString::fromUtf8("oeulerz"));
        oeulerz->setMinimum(-360);
        oeulerz->setMaximum(360);

        horizontalLayout_7->addWidget(oeulerz);


        gridLayout_3->addLayout(horizontalLayout_7, 3, 0, 1, 1);

        isFoot = new QCheckBox(groupBox_2);
        isFoot->setObjectName(QString::fromUtf8("isFoot"));

        gridLayout_3->addWidget(isFoot, 6, 0, 1, 1);

        isBodyBalance = new QCheckBox(groupBox_2);
        isBodyBalance->setObjectName(QString::fromUtf8("isBodyBalance"));

        gridLayout_3->addWidget(isBodyBalance, 7, 0, 1, 1);


        gridLayout_12->addWidget(groupBox_2, 3, 0, 1, 1);

        groupBox = new QGroupBox(tabManipulatorSim);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setFont(font);
        groupBox->setStyleSheet(QString::fromUtf8("QGroupBox::title { \n"
"   \n"
"     subcontrol-position: top left; /* position at the top left*/ \n"
"  padding: 2px 13px;\n"
"   margin-top: -5px;\n"
"    \n"
" }\n"
"\n"
"QGroupBox{\n"
"   border: 1px solid rgb(182, 181, 179);\n"
"  border-radius: 5px;\n"
"}"));
        gridLayout_5 = new QGridLayout(groupBox);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_5->addWidget(label_2, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        forcex = new QDoubleSpinBox(groupBox);
        forcex->setObjectName(QString::fromUtf8("forcex"));
        forcex->setMaximumSize(QSize(75, 16777215));
        forcex->setMinimum(-10000);
        forcex->setMaximum(10000);
        forcex->setSingleStep(10);

        horizontalLayout_2->addWidget(forcex);

        forcey = new QDoubleSpinBox(groupBox);
        forcey->setObjectName(QString::fromUtf8("forcey"));
        forcey->setMaximumSize(QSize(75, 16777215));
        forcey->setMinimum(-10000);
        forcey->setMaximum(10000);
        forcey->setSingleStep(10);

        horizontalLayout_2->addWidget(forcey);

        forcez = new QDoubleSpinBox(groupBox);
        forcez->setObjectName(QString::fromUtf8("forcez"));
        forcez->setMaximumSize(QSize(75, 16777215));
        forcez->setMinimum(-10000);
        forcez->setMaximum(10000);
        forcez->setSingleStep(10);

        horizontalLayout_2->addWidget(forcez);

        applyForce = new QPushButton(groupBox);
        applyForce->setObjectName(QString::fromUtf8("applyForce"));
        applyForce->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_2->addWidget(applyForce);


        gridLayout_5->addLayout(horizontalLayout_2, 1, 0, 1, 1);


        gridLayout_12->addWidget(groupBox, 4, 0, 1, 1);

        groupBox_10 = new QGroupBox(tabManipulatorSim);
        groupBox_10->setObjectName(QString::fromUtf8("groupBox_10"));
        groupBox_10->setFont(font);
        groupBox_10->setStyleSheet(QString::fromUtf8("QGroupBox::title { \n"
"   \n"
"     subcontrol-position: top left; /* position at the top left*/ \n"
"  padding: 2px 13px;\n"
"   margin-top: -5px;\n"
"    \n"
" }\n"
"\n"
"QGroupBox{\n"
"   border: 1px solid rgb(182, 181, 179);\n"
"  border-radius: 5px;\n"
"}"));
        gridLayout_13 = new QGridLayout(groupBox_10);
        gridLayout_13->setSpacing(6);
        gridLayout_13->setContentsMargins(11, 11, 11, 11);
        gridLayout_13->setObjectName(QString::fromUtf8("gridLayout_13"));
        hasCup = new QCheckBox(groupBox_10);
        hasCup->setObjectName(QString::fromUtf8("hasCup"));

        gridLayout_13->addWidget(hasCup, 0, 0, 1, 1);


        gridLayout_12->addWidget(groupBox_10, 1, 0, 1, 1);

        Options->addTab(tabManipulatorSim, QString());
        controlpd = new QWidget();
        controlpd->setObjectName(QString::fromUtf8("controlpd"));
        gridLayout_10 = new QGridLayout(controlpd);
        gridLayout_10->setSpacing(6);
        gridLayout_10->setContentsMargins(11, 11, 11, 11);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        listWidgetJoints = new QListWidget(controlpd);
        listWidgetJoints->setObjectName(QString::fromUtf8("listWidgetJoints"));

        gridLayout_10->addWidget(listWidgetJoints, 0, 0, 1, 1);

        groupBox_5 = new QGroupBox(controlpd);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        groupBox_5->setFont(font);
        groupBox_5->setStyleSheet(QString::fromUtf8("QGroupBox::title { \n"
"   \n"
"     subcontrol-position: top left; /* position at the top left*/ \n"
"  padding: 2px 13px;\n"
"   margin-top: -5px;\n"
"    \n"
" }\n"
"\n"
"QGroupBox{\n"
"   border: 1px solid rgb(182, 181, 179);\n"
"  border-radius: 5px;\n"
"}"));
        gridLayout_9 = new QGridLayout(groupBox_5);
        gridLayout_9->setSpacing(6);
        gridLayout_9->setContentsMargins(11, 11, 11, 11);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(groupBox_5);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_3->addWidget(label_3);

        ksx = new QDoubleSpinBox(groupBox_5);
        ksx->setObjectName(QString::fromUtf8("ksx"));
        ksx->setMinimum(-1000);
        ksx->setMaximum(1000);
        ksx->setSingleStep(0.1);

        horizontalLayout_3->addWidget(ksx);

        ksy = new QDoubleSpinBox(groupBox_5);
        ksy->setObjectName(QString::fromUtf8("ksy"));
        ksy->setMinimum(-1000);
        ksy->setMaximum(1000);
        ksy->setSingleStep(0.1);

        horizontalLayout_3->addWidget(ksy);

        ksz = new QDoubleSpinBox(groupBox_5);
        ksz->setObjectName(QString::fromUtf8("ksz"));
        ksz->setMinimum(-1000);
        ksz->setMaximum(1000);
        ksz->setSingleStep(0.1);

        horizontalLayout_3->addWidget(ksz);


        gridLayout_9->addLayout(horizontalLayout_3, 0, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_4 = new QLabel(groupBox_5);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_4->addWidget(label_4);

        kdx = new QDoubleSpinBox(groupBox_5);
        kdx->setObjectName(QString::fromUtf8("kdx"));
        kdx->setMinimum(-1000);
        kdx->setMaximum(1000);
        kdx->setSingleStep(0.1);

        horizontalLayout_4->addWidget(kdx);

        kdy = new QDoubleSpinBox(groupBox_5);
        kdy->setObjectName(QString::fromUtf8("kdy"));
        kdy->setMinimum(-1000);
        kdy->setMaximum(1000);
        kdy->setSingleStep(0.1);

        horizontalLayout_4->addWidget(kdy);

        kdz = new QDoubleSpinBox(groupBox_5);
        kdz->setObjectName(QString::fromUtf8("kdz"));
        kdz->setMinimum(-1000);
        kdz->setMaximum(1000);
        kdz->setSingleStep(0.1);

        horizontalLayout_4->addWidget(kdz);


        gridLayout_9->addLayout(horizontalLayout_4, 1, 0, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_5 = new QLabel(groupBox_5);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_5->addWidget(label_5);

        eulerx = new QDoubleSpinBox(groupBox_5);
        eulerx->setObjectName(QString::fromUtf8("eulerx"));
        eulerx->setMinimum(-360);
        eulerx->setMaximum(360);

        horizontalLayout_5->addWidget(eulerx);

        eulery = new QDoubleSpinBox(groupBox_5);
        eulery->setObjectName(QString::fromUtf8("eulery"));
        eulery->setMinimum(-360);
        eulery->setMaximum(360);

        horizontalLayout_5->addWidget(eulery);

        eulerz = new QDoubleSpinBox(groupBox_5);
        eulerz->setObjectName(QString::fromUtf8("eulerz"));
        eulerz->setMinimum(-360);
        eulerz->setMaximum(360);

        horizontalLayout_5->addWidget(eulerz);


        gridLayout_9->addLayout(horizontalLayout_5, 2, 0, 1, 1);

        enablepd = new QCheckBox(groupBox_5);
        enablepd->setObjectName(QString::fromUtf8("enablepd"));

        gridLayout_9->addWidget(enablepd, 3, 0, 1, 1);


        gridLayout_10->addWidget(groupBox_5, 1, 0, 1, 1);

        groupBox_6 = new QGroupBox(controlpd);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        groupBox_6->setFont(font);
        groupBox_6->setStyleSheet(QString::fromUtf8("QGroupBox::title { \n"
"   \n"
"     subcontrol-position: top left; /* position at the top left*/ \n"
"  padding: 2px 13px;\n"
"   margin-top: -5px;\n"
"    \n"
" }\n"
"\n"
"QGroupBox{\n"
"   border: 1px solid rgb(182, 181, 179);\n"
"  border-radius: 5px;\n"
"}"));
        gridLayout_4 = new QGridLayout(groupBox_6);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setSpacing(6);
        horizontalLayout_17->setObjectName(QString::fromUtf8("horizontalLayout_17"));
        label_6 = new QLabel(groupBox_6);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_17->addWidget(label_6);

        xksPdApply = new QLabel(groupBox_6);
        xksPdApply->setObjectName(QString::fromUtf8("xksPdApply"));

        horizontalLayout_17->addWidget(xksPdApply);

        yksPdApply = new QLabel(groupBox_6);
        yksPdApply->setObjectName(QString::fromUtf8("yksPdApply"));

        horizontalLayout_17->addWidget(yksPdApply);

        zksPdApply = new QLabel(groupBox_6);
        zksPdApply->setObjectName(QString::fromUtf8("zksPdApply"));

        horizontalLayout_17->addWidget(zksPdApply);


        gridLayout_4->addLayout(horizontalLayout_17, 0, 0, 1, 1);

        horizontalLayout_18 = new QHBoxLayout();
        horizontalLayout_18->setSpacing(6);
        horizontalLayout_18->setObjectName(QString::fromUtf8("horizontalLayout_18"));
        label_21 = new QLabel(groupBox_6);
        label_21->setObjectName(QString::fromUtf8("label_21"));

        horizontalLayout_18->addWidget(label_21);

        xkdPdApply = new QLabel(groupBox_6);
        xkdPdApply->setObjectName(QString::fromUtf8("xkdPdApply"));

        horizontalLayout_18->addWidget(xkdPdApply);

        ykdPdApply = new QLabel(groupBox_6);
        ykdPdApply->setObjectName(QString::fromUtf8("ykdPdApply"));

        horizontalLayout_18->addWidget(ykdPdApply);

        zkdPdApply = new QLabel(groupBox_6);
        zkdPdApply->setObjectName(QString::fromUtf8("zkdPdApply"));

        horizontalLayout_18->addWidget(zkdPdApply);


        gridLayout_4->addLayout(horizontalLayout_18, 1, 0, 1, 1);


        gridLayout_10->addWidget(groupBox_6, 2, 0, 1, 1);

        infoPD = new QFrame(controlpd);
        infoPD->setObjectName(QString::fromUtf8("infoPD"));
        infoPD->setFrameShape(QFrame::NoFrame);
        infoPD->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(infoPD);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));

        gridLayout_10->addWidget(infoPD, 3, 0, 1, 1);

        Options->addTab(controlpd, QString());

        gridLayout_2->addWidget(Options, 0, 0, 1, 1);

        splitter->addWidget(widgetOpt);

        gridLayout->addWidget(splitter, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 658, 25));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuControl = new QMenu(menuBar);
        menuControl->setObjectName(QString::fromUtf8("menuControl"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuControl->menuAction());
        menuFile->addAction(actionOpen_Model);
        menuFile->addAction(actionSave_Model);
        menuFile->addAction(actionOpen_MoCap);
        menuFile->addAction(actionOpen_Simulation);
        menuFile->addAction(actionSave_Simulation);
        menuControl->addAction(actionPlay_Pause_Simulation);
        menuControl->addAction(actionRestart_Simulation);

        retranslateUi(MainWindow);

        Options->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Physics Simulator", 0, QApplication::UnicodeUTF8));
        actionPlay_Pause_Simulation->setText(QApplication::translate("MainWindow", "Play/Pause", 0, QApplication::UnicodeUTF8));
        actionPlay_Pause_Simulation->setShortcut(QApplication::translate("MainWindow", "Space", 0, QApplication::UnicodeUTF8));
        actionRestart_Simulation->setText(QApplication::translate("MainWindow", "Restart", 0, QApplication::UnicodeUTF8));
        actionRestart_Simulation->setShortcut(QApplication::translate("MainWindow", "Ctrl+R", 0, QApplication::UnicodeUTF8));
        actionOpen_Model->setText(QApplication::translate("MainWindow", "Open Model", 0, QApplication::UnicodeUTF8));
        actionOpen_Model->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionOpen_MoCap->setText(QApplication::translate("MainWindow", "Open MoCap", 0, QApplication::UnicodeUTF8));
        actionOpen_MoCap->setShortcut(QApplication::translate("MainWindow", "Ctrl+M", 0, QApplication::UnicodeUTF8));
        actionSave_Model->setText(QApplication::translate("MainWindow", "Save Model", 0, QApplication::UnicodeUTF8));
        actionSave_Model->setShortcut(QApplication::translate("MainWindow", "Shift+M", 0, QApplication::UnicodeUTF8));
        actionSave_Simulation->setText(QApplication::translate("MainWindow", "Save Simulation", 0, QApplication::UnicodeUTF8));
        actionSave_Simulation->setShortcut(QApplication::translate("MainWindow", "Shift+S", 0, QApplication::UnicodeUTF8));
        actionOpen_Simulation->setText(QApplication::translate("MainWindow", "Open Simulation", 0, QApplication::UnicodeUTF8));
        actionOpen_Simulation->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Steps:", 0, QApplication::UnicodeUTF8));
        enableGravity->setText(QApplication::translate("MainWindow", "Enable Gravity", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("MainWindow", "Gravity", 0, QApplication::UnicodeUTF8));
        checkBox->setText(QApplication::translate("MainWindow", "Update Simultaneously", 0, QApplication::UnicodeUTF8));
        kdtoks->setText(QApplication::translate("MainWindow", "Kd = 1/10ks Automaticaly", 0, QApplication::UnicodeUTF8));
        checkMesh->setText(QApplication::translate("MainWindow", "Render with Mesh", 0, QApplication::UnicodeUTF8));
        infoShow->setText(QApplication::translate("MainWindow", "Show Info", 0, QApplication::UnicodeUTF8));
        Options->setTabText(Options->indexOf(tabParSim), QApplication::translate("MainWindow", "Parameters Simulation", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Balance Control", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("MainWindow", "Tq Ks:", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("MainWindow", "Tq Kd:", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("MainWindow", "Fc Ks", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("MainWindow", "Fc kd:", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("MainWindow", "Mm L:", 0, QApplication::UnicodeUTF8));
        label_35->setText(QApplication::translate("MainWindow", "Mm A:", 0, QApplication::UnicodeUTF8));
        checkMomentum->setText(QApplication::translate("MainWindow", "Momentum", 0, QApplication::UnicodeUTF8));
        checkTorqueBal->setText(QApplication::translate("MainWindow", "Torque", 0, QApplication::UnicodeUTF8));
        checkForceBal->setText(QApplication::translate("MainWindow", "Force", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("MainWindow", "Angle", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("MainWindow", "PD control Proportional", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("MainWindow", "Ks:", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("MainWindow", "kd:", 0, QApplication::UnicodeUTF8));
        groupBoxCone->setTitle(QApplication::translate("MainWindow", "Cone Friction Foots", 0, QApplication::UnicodeUTF8));
        label_28->setText(QApplication::translate("MainWindow", "Module:", 0, QApplication::UnicodeUTF8));
        label_30->setText(QApplication::translate("MainWindow", "Height:", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("MainWindow", "Radius:", 0, QApplication::UnicodeUTF8));
        label_29->setText(QApplication::translate("MainWindow", "Angle:", 0, QApplication::UnicodeUTF8));
        label_24->setText(QApplication::translate("MainWindow", "Limit Percent:", 0, QApplication::UnicodeUTF8));
        label_26->setText(QApplication::translate("MainWindow", "foot 1", 0, QApplication::UnicodeUTF8));
        conePerc1->setText(QApplication::translate("MainWindow", "%", 0, QApplication::UnicodeUTF8));
        label_27->setText(QApplication::translate("MainWindow", "foot 2", 0, QApplication::UnicodeUTF8));
        conePerc2->setText(QApplication::translate("MainWindow", "%", 0, QApplication::UnicodeUTF8));
        groupBox_7->setTitle(QApplication::translate("MainWindow", "Character Draw", 0, QApplication::UnicodeUTF8));
        checkGRF->setText(QApplication::translate("MainWindow", "GRF", 0, QApplication::UnicodeUTF8));
        checkShadow->setText(QApplication::translate("MainWindow", "Shadow", 0, QApplication::UnicodeUTF8));
        checkShowChara->setText(QApplication::translate("MainWindow", "Chara", 0, QApplication::UnicodeUTF8));
        checkWireChara->setText(QApplication::translate("MainWindow", " Wire", 0, QApplication::UnicodeUTF8));
        Options->setTabText(Options->indexOf(tab), QApplication::translate("MainWindow", "General Controller", 0, QApplication::UnicodeUTF8));
        btnStart->setText(QApplication::translate("MainWindow", "Start", 0, QApplication::UnicodeUTF8));
        btnPause->setText(QApplication::translate("MainWindow", "Pause", 0, QApplication::UnicodeUTF8));
        btnRestart->setText(QApplication::translate("MainWindow", "Restart", 0, QApplication::UnicodeUTF8));
        iframe->setText(QApplication::translate("MainWindow", "frame", 0, QApplication::UnicodeUTF8));
        label_23->setText(QApplication::translate("MainWindow", "/", 0, QApplication::UnicodeUTF8));
        nframe->setText(QApplication::translate("MainWindow", "totalframe", 0, QApplication::UnicodeUTF8));
        groupBox_11->setTitle(QApplication::translate("MainWindow", "Editing Frames", 0, QApplication::UnicodeUTF8));
        label_34->setText(QApplication::translate("MainWindow", "Frame", 0, QApplication::UnicodeUTF8));
        checkShowEditingFrame->setText(QApplication::translate("MainWindow", "Show Editing", 0, QApplication::UnicodeUTF8));
        checkFrameFootLeft->setText(QApplication::translate("MainWindow", "Foot Left in Ground", 0, QApplication::UnicodeUTF8));
        checkFrameFootRight->setText(QApplication::translate("MainWindow", "Foot Right in Ground", 0, QApplication::UnicodeUTF8));
        saveEditingFrame->setText(QApplication::translate("MainWindow", "Save Editing Frames", 0, QApplication::UnicodeUTF8));
        loadEditedFrames->setText(QApplication::translate("MainWindow", "Load Edited Frames", 0, QApplication::UnicodeUTF8));
        label_22->setText(QApplication::translate("MainWindow", "Adjust Sensor Tollerance:", 0, QApplication::UnicodeUTF8));
        groupBox_12->setTitle(QApplication::translate("MainWindow", "Frame Period", 0, QApplication::UnicodeUTF8));
        mocapCyclic->setText(QApplication::translate("MainWindow", "Cyclic", 0, QApplication::UnicodeUTF8));
        label_36->setText(QApplication::translate("MainWindow", "Begin", 0, QApplication::UnicodeUTF8));
        label_37->setText(QApplication::translate("MainWindow", "End", 0, QApplication::UnicodeUTF8));
        checkShowMocap->setText(QApplication::translate("MainWindow", "Show MoCap", 0, QApplication::UnicodeUTF8));
        checkScreenShot->setText(QApplication::translate("MainWindow", "ScreenShots", 0, QApplication::UnicodeUTF8));
        Options->setTabText(Options->indexOf(tab_2), QApplication::translate("MainWindow", "Motion Config", 0, QApplication::UnicodeUTF8));
        groupBox_9->setTitle(QApplication::translate("MainWindow", "Control PD Linear", 0, QApplication::UnicodeUTF8));
        enable_cpdp->setText(QApplication::translate("MainWindow", "enabled", 0, QApplication::UnicodeUTF8));
        show_effector->setText(QApplication::translate("MainWindow", "effector", 0, QApplication::UnicodeUTF8));
        show_target->setText(QApplication::translate("MainWindow", "target", 0, QApplication::UnicodeUTF8));
        label_31->setText(QApplication::translate("MainWindow", "Target", 0, QApplication::UnicodeUTF8));
        label_32->setText(QApplication::translate("MainWindow", "ks", 0, QApplication::UnicodeUTF8));
        label_33->setText(QApplication::translate("MainWindow", "kd", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Properties Default", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("MainWindow", "Dim", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MainWindow", "Pos", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("MainWindow", "Mass", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("MainWindow", "Euler", 0, QApplication::UnicodeUTF8));
        isFoot->setText(QApplication::translate("MainWindow", "is foot", 0, QApplication::UnicodeUTF8));
        isBodyBalance->setText(QApplication::translate("MainWindow", "is body balance", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("MainWindow", "Actuators", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Force:", 0, QApplication::UnicodeUTF8));
        applyForce->setText(QApplication::translate("MainWindow", "Apply", 0, QApplication::UnicodeUTF8));
        groupBox_10->setTitle(QApplication::translate("MainWindow", "Coffee Cup", 0, QApplication::UnicodeUTF8));
        hasCup->setText(QApplication::translate("MainWindow", "has Coffee Cup", 0, QApplication::UnicodeUTF8));
        Options->setTabText(Options->indexOf(tabManipulatorSim), QApplication::translate("MainWindow", "Manipulators Objects", 0, QApplication::UnicodeUTF8));
        groupBox_5->setTitle(QApplication::translate("MainWindow", "Control PD", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "ks", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "kd", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "Angle", 0, QApplication::UnicodeUTF8));
        enablepd->setText(QApplication::translate("MainWindow", "Enabled", 0, QApplication::UnicodeUTF8));
        groupBox_6->setTitle(QApplication::translate("MainWindow", "Control PD Applied", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainWindow", "Ks", 0, QApplication::UnicodeUTF8));
        xksPdApply->setText(QApplication::translate("MainWindow", "val1", 0, QApplication::UnicodeUTF8));
        yksPdApply->setText(QApplication::translate("MainWindow", "val2", 0, QApplication::UnicodeUTF8));
        zksPdApply->setText(QApplication::translate("MainWindow", "val3", 0, QApplication::UnicodeUTF8));
        label_21->setText(QApplication::translate("MainWindow", "Kd", 0, QApplication::UnicodeUTF8));
        xkdPdApply->setText(QApplication::translate("MainWindow", "val1", 0, QApplication::UnicodeUTF8));
        ykdPdApply->setText(QApplication::translate("MainWindow", "val2", 0, QApplication::UnicodeUTF8));
        zkdPdApply->setText(QApplication::translate("MainWindow", "val3", 0, QApplication::UnicodeUTF8));
        Options->setTabText(Options->indexOf(controlpd), QApplication::translate("MainWindow", "Manipulator Joints", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        menuControl->setTitle(QApplication::translate("MainWindow", "Simulation", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
