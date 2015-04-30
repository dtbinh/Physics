/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
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
    QWidget *layoutWidget5;
    QHBoxLayout *horizontalLayout_42;
    QLabel *label_39;
    QDoubleSpinBox *stepsInterpolation;
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
    QWidget *tab_3;
    QWidget *layoutWidget_2;
    QHBoxLayout *horizontalLayout_40;
    QLabel *label_25;
    QDoubleSpinBox *xkDis;
    QDoubleSpinBox *ykDis;
    QDoubleSpinBox *zkDis;
    QWidget *layoutWidget_3;
    QHBoxLayout *horizontalLayout_41;
    QLabel *label_38;
    QDoubleSpinBox *xkFor;
    QDoubleSpinBox *ykFor;
    QDoubleSpinBox *zkFor;
    QWidget *layoutWidget6;
    QHBoxLayout *horizontalLayout_43;
    QLabel *label_40;
    QDoubleSpinBox *densityBall;
    QWidget *layoutWidget7;
    QHBoxLayout *horizontalLayout_44;
    QLabel *label_41;
    QDoubleSpinBox *velocityBall;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuControl;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(658, 780);
        actionPlay_Pause_Simulation = new QAction(MainWindow);
        actionPlay_Pause_Simulation->setObjectName(QStringLiteral("actionPlay_Pause_Simulation"));
        actionRestart_Simulation = new QAction(MainWindow);
        actionRestart_Simulation->setObjectName(QStringLiteral("actionRestart_Simulation"));
        actionOpen_Model = new QAction(MainWindow);
        actionOpen_Model->setObjectName(QStringLiteral("actionOpen_Model"));
        actionOpen_MoCap = new QAction(MainWindow);
        actionOpen_MoCap->setObjectName(QStringLiteral("actionOpen_MoCap"));
        actionSave_Model = new QAction(MainWindow);
        actionSave_Model->setObjectName(QStringLiteral("actionSave_Model"));
        actionSave_Simulation = new QAction(MainWindow);
        actionSave_Simulation->setObjectName(QStringLiteral("actionSave_Simulation"));
        actionOpen_Simulation = new QAction(MainWindow);
        actionOpen_Simulation->setObjectName(QStringLiteral("actionOpen_Simulation"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        widgetPhysics = new GLWidget(splitter);
        widgetPhysics->setObjectName(QStringLiteral("widgetPhysics"));
        widgetPhysics->setAutoFillBackground(false);
        splitter->addWidget(widgetPhysics);
        widgetOpt = new QWidget(splitter);
        widgetOpt->setObjectName(QStringLiteral("widgetOpt"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widgetOpt->sizePolicy().hasHeightForWidth());
        widgetOpt->setSizePolicy(sizePolicy);
        widgetOpt->setMaximumSize(QSize(350, 16777215));
        gridLayout_2 = new QGridLayout(widgetOpt);
        gridLayout_2->setSpacing(0);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        Options = new QTabWidget(widgetOpt);
        Options->setObjectName(QStringLiteral("Options"));
        Options->setLayoutDirection(Qt::LeftToRight);
        Options->setTabPosition(QTabWidget::West);
        Options->setTabsClosable(false);
        Options->setMovable(true);
        tabParSim = new QWidget();
        tabParSim->setObjectName(QStringLiteral("tabParSim"));
        layoutWidget = new QWidget(tabParSim);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(0, 10, 311, 29));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        stepSim = new QSpinBox(layoutWidget);
        stepSim->setObjectName(QStringLiteral("stepSim"));
        stepSim->setMinimum(1);
        stepSim->setMaximum(1000);
        stepSim->setValue(67);

        horizontalLayout->addWidget(stepSim);

        enableGravity = new QCheckBox(tabParSim);
        enableGravity->setObjectName(QStringLiteral("enableGravity"));
        enableGravity->setGeometry(QRect(0, 70, 131, 22));
        layoutWidget1 = new QWidget(tabParSim);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(0, 40, 310, 29));
        horizontalLayout_10 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        horizontalLayout_10->setContentsMargins(0, 0, 0, 0);
        label_11 = new QLabel(layoutWidget1);
        label_11->setObjectName(QStringLiteral("label_11"));

        horizontalLayout_10->addWidget(label_11);

        gravx = new QDoubleSpinBox(layoutWidget1);
        gravx->setObjectName(QStringLiteral("gravx"));
        gravx->setDecimals(1);
        gravx->setMinimum(-99);
        gravx->setMaximum(99);

        horizontalLayout_10->addWidget(gravx);

        gravy = new QDoubleSpinBox(layoutWidget1);
        gravy->setObjectName(QStringLiteral("gravy"));
        gravy->setDecimals(1);
        gravy->setMinimum(-99);
        gravy->setMaximum(99);
        gravy->setValue(0);

        horizontalLayout_10->addWidget(gravy);

        gravz = new QDoubleSpinBox(layoutWidget1);
        gravz->setObjectName(QStringLiteral("gravz"));
        gravz->setDecimals(1);
        gravz->setMinimum(-99);
        gravz->setMaximum(99);

        horizontalLayout_10->addWidget(gravz);

        checkBox = new QCheckBox(tabParSim);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setGeometry(QRect(0, 100, 281, 16));
        kdtoks = new QCheckBox(tabParSim);
        kdtoks->setObjectName(QStringLiteral("kdtoks"));
        kdtoks->setGeometry(QRect(0, 130, 201, 22));
        checkMesh = new QCheckBox(tabParSim);
        checkMesh->setObjectName(QStringLiteral("checkMesh"));
        checkMesh->setGeometry(QRect(0, 160, 181, 22));
        infoShow = new QCheckBox(tabParSim);
        infoShow->setObjectName(QStringLiteral("infoShow"));
        infoShow->setGeometry(QRect(0, 190, 97, 22));
        Options->addTab(tabParSim, QString());
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        groupBox_3 = new QGroupBox(tab);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(9, 9, 301, 271));
        QFont font;
        font.setPointSize(9);
        font.setBold(true);
        font.setWeight(75);
        groupBox_3->setFont(font);
        groupBox_3->setStyleSheet(QLatin1String("QGroupBox::title { \n"
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
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        gridLayout_7->setContentsMargins(2, 2, 2, 2);
        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        label_12 = new QLabel(groupBox_3);
        label_12->setObjectName(QStringLiteral("label_12"));

        horizontalLayout_11->addWidget(label_12);

        xksTqBal = new QDoubleSpinBox(groupBox_3);
        xksTqBal->setObjectName(QStringLiteral("xksTqBal"));
        xksTqBal->setMinimum(-500);
        xksTqBal->setMaximum(500);

        horizontalLayout_11->addWidget(xksTqBal);

        yksTqBal = new QDoubleSpinBox(groupBox_3);
        yksTqBal->setObjectName(QStringLiteral("yksTqBal"));
        yksTqBal->setMinimum(-500);
        yksTqBal->setMaximum(500);

        horizontalLayout_11->addWidget(yksTqBal);

        zksTqBal = new QDoubleSpinBox(groupBox_3);
        zksTqBal->setObjectName(QStringLiteral("zksTqBal"));
        zksTqBal->setMinimum(-500);
        zksTqBal->setMaximum(500);

        horizontalLayout_11->addWidget(zksTqBal);


        gridLayout_7->addLayout(horizontalLayout_11, 0, 0, 1, 1);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        label_13 = new QLabel(groupBox_3);
        label_13->setObjectName(QStringLiteral("label_13"));

        horizontalLayout_12->addWidget(label_13);

        xkdTqBal = new QDoubleSpinBox(groupBox_3);
        xkdTqBal->setObjectName(QStringLiteral("xkdTqBal"));
        xkdTqBal->setMinimum(-500);
        xkdTqBal->setMaximum(500);

        horizontalLayout_12->addWidget(xkdTqBal);

        ykdTqBal = new QDoubleSpinBox(groupBox_3);
        ykdTqBal->setObjectName(QStringLiteral("ykdTqBal"));
        ykdTqBal->setMinimum(-500);
        ykdTqBal->setMaximum(500);

        horizontalLayout_12->addWidget(ykdTqBal);

        zkdTqBal = new QDoubleSpinBox(groupBox_3);
        zkdTqBal->setObjectName(QStringLiteral("zkdTqBal"));
        zkdTqBal->setMinimum(-500);
        zkdTqBal->setMaximum(500);

        horizontalLayout_12->addWidget(zkdTqBal);


        gridLayout_7->addLayout(horizontalLayout_12, 1, 0, 1, 1);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        label_14 = new QLabel(groupBox_3);
        label_14->setObjectName(QStringLiteral("label_14"));

        horizontalLayout_13->addWidget(label_14);

        xksForBal = new QDoubleSpinBox(groupBox_3);
        xksForBal->setObjectName(QStringLiteral("xksForBal"));
        xksForBal->setMinimum(-9999);
        xksForBal->setMaximum(9999);

        horizontalLayout_13->addWidget(xksForBal);

        yksForBal = new QDoubleSpinBox(groupBox_3);
        yksForBal->setObjectName(QStringLiteral("yksForBal"));
        yksForBal->setMinimum(-9999);
        yksForBal->setMaximum(9999);

        horizontalLayout_13->addWidget(yksForBal);

        zksForBal = new QDoubleSpinBox(groupBox_3);
        zksForBal->setObjectName(QStringLiteral("zksForBal"));
        zksForBal->setMinimum(-9999);
        zksForBal->setMaximum(9999);

        horizontalLayout_13->addWidget(zksForBal);


        gridLayout_7->addLayout(horizontalLayout_13, 2, 0, 1, 1);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setSpacing(6);
        horizontalLayout_14->setObjectName(QStringLiteral("horizontalLayout_14"));
        label_15 = new QLabel(groupBox_3);
        label_15->setObjectName(QStringLiteral("label_15"));

        horizontalLayout_14->addWidget(label_15);

        xkdForBal = new QDoubleSpinBox(groupBox_3);
        xkdForBal->setObjectName(QStringLiteral("xkdForBal"));
        xkdForBal->setMinimum(-999);
        xkdForBal->setMaximum(999);

        horizontalLayout_14->addWidget(xkdForBal);

        ykdForBal = new QDoubleSpinBox(groupBox_3);
        ykdForBal->setObjectName(QStringLiteral("ykdForBal"));
        ykdForBal->setMinimum(-500);
        ykdForBal->setMaximum(500);

        horizontalLayout_14->addWidget(ykdForBal);

        zkdForBal = new QDoubleSpinBox(groupBox_3);
        zkdForBal->setObjectName(QStringLiteral("zkdForBal"));
        zkdForBal->setMinimum(-500);
        zkdForBal->setMaximum(500);

        horizontalLayout_14->addWidget(zkdForBal);


        gridLayout_7->addLayout(horizontalLayout_14, 3, 0, 1, 1);

        horizontalLayout_19 = new QHBoxLayout();
        horizontalLayout_19->setSpacing(6);
        horizontalLayout_19->setObjectName(QStringLiteral("horizontalLayout_19"));
        label_18 = new QLabel(groupBox_3);
        label_18->setObjectName(QStringLiteral("label_18"));

        horizontalLayout_19->addWidget(label_18);

        xkmomlinBal = new QDoubleSpinBox(groupBox_3);
        xkmomlinBal->setObjectName(QStringLiteral("xkmomlinBal"));
        xkmomlinBal->setMinimum(-500);
        xkmomlinBal->setMaximum(500);

        horizontalLayout_19->addWidget(xkmomlinBal);

        ykmomlinBal = new QDoubleSpinBox(groupBox_3);
        ykmomlinBal->setObjectName(QStringLiteral("ykmomlinBal"));
        ykmomlinBal->setMinimum(-500);
        ykmomlinBal->setMaximum(500);

        horizontalLayout_19->addWidget(ykmomlinBal);

        zkmomlinBal = new QDoubleSpinBox(groupBox_3);
        zkmomlinBal->setObjectName(QStringLiteral("zkmomlinBal"));
        zkmomlinBal->setMinimum(-500);
        zkmomlinBal->setMaximum(500);

        horizontalLayout_19->addWidget(zkmomlinBal);


        gridLayout_7->addLayout(horizontalLayout_19, 4, 0, 1, 1);

        horizontalLayout_37 = new QHBoxLayout();
        horizontalLayout_37->setSpacing(6);
        horizontalLayout_37->setObjectName(QStringLiteral("horizontalLayout_37"));
        label_35 = new QLabel(groupBox_3);
        label_35->setObjectName(QStringLiteral("label_35"));

        horizontalLayout_37->addWidget(label_35);

        xkmomangBal = new QDoubleSpinBox(groupBox_3);
        xkmomangBal->setObjectName(QStringLiteral("xkmomangBal"));
        xkmomangBal->setMinimum(-500);
        xkmomangBal->setMaximum(500);

        horizontalLayout_37->addWidget(xkmomangBal);

        ykmomangBal = new QDoubleSpinBox(groupBox_3);
        ykmomangBal->setObjectName(QStringLiteral("ykmomangBal"));
        ykmomangBal->setMinimum(-500);
        ykmomangBal->setMaximum(500);

        horizontalLayout_37->addWidget(ykmomangBal);

        zkmomangBal = new QDoubleSpinBox(groupBox_3);
        zkmomangBal->setObjectName(QStringLiteral("zkmomangBal"));
        zkmomangBal->setMinimum(-500);
        zkmomangBal->setMaximum(500);

        horizontalLayout_37->addWidget(zkmomangBal);


        gridLayout_7->addLayout(horizontalLayout_37, 5, 0, 1, 1);

        horizontalLayout_20 = new QHBoxLayout();
        horizontalLayout_20->setSpacing(6);
        horizontalLayout_20->setObjectName(QStringLiteral("horizontalLayout_20"));
        checkMomentum = new QCheckBox(groupBox_3);
        checkMomentum->setObjectName(QStringLiteral("checkMomentum"));
        checkMomentum->setChecked(true);

        horizontalLayout_20->addWidget(checkMomentum);

        checkTorqueBal = new QCheckBox(groupBox_3);
        checkTorqueBal->setObjectName(QStringLiteral("checkTorqueBal"));
        checkTorqueBal->setChecked(true);

        horizontalLayout_20->addWidget(checkTorqueBal);

        checkForceBal = new QCheckBox(groupBox_3);
        checkForceBal->setObjectName(QStringLiteral("checkForceBal"));
        checkForceBal->setChecked(true);

        horizontalLayout_20->addWidget(checkForceBal);


        gridLayout_7->addLayout(horizontalLayout_20, 6, 0, 1, 1);

        horizontalLayout_21 = new QHBoxLayout();
        horizontalLayout_21->setSpacing(6);
        horizontalLayout_21->setObjectName(QStringLiteral("horizontalLayout_21"));
        label_20 = new QLabel(groupBox_3);
        label_20->setObjectName(QStringLiteral("label_20"));

        horizontalLayout_21->addWidget(label_20);

        angleBalBodyx = new QSpinBox(groupBox_3);
        angleBalBodyx->setObjectName(QStringLiteral("angleBalBodyx"));
        angleBalBodyx->setMinimum(-361);
        angleBalBodyx->setMaximum(361);

        horizontalLayout_21->addWidget(angleBalBodyx);

        angleBalBodyy = new QSpinBox(groupBox_3);
        angleBalBodyy->setObjectName(QStringLiteral("angleBalBodyy"));
        angleBalBodyy->setMinimum(-361);
        angleBalBodyy->setMaximum(361);

        horizontalLayout_21->addWidget(angleBalBodyy);

        angleBalBodyz = new QSpinBox(groupBox_3);
        angleBalBodyz->setObjectName(QStringLiteral("angleBalBodyz"));
        angleBalBodyz->setMinimum(-361);
        angleBalBodyz->setMaximum(361);

        horizontalLayout_21->addWidget(angleBalBodyz);


        gridLayout_7->addLayout(horizontalLayout_21, 8, 0, 1, 1);

        groupBox_4 = new QGroupBox(tab);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(10, 287, 299, 84));
        groupBox_4->setFont(font);
        groupBox_4->setStyleSheet(QLatin1String("QGroupBox::title { \n"
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
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setSpacing(6);
        horizontalLayout_15->setObjectName(QStringLiteral("horizontalLayout_15"));
        label_16 = new QLabel(groupBox_4);
        label_16->setObjectName(QStringLiteral("label_16"));

        horizontalLayout_15->addWidget(label_16);

        xksPdProp = new QDoubleSpinBox(groupBox_4);
        xksPdProp->setObjectName(QStringLiteral("xksPdProp"));
        xksPdProp->setMinimum(-500);
        xksPdProp->setMaximum(500);
        xksPdProp->setValue(1);

        horizontalLayout_15->addWidget(xksPdProp);

        yksPdProp = new QDoubleSpinBox(groupBox_4);
        yksPdProp->setObjectName(QStringLiteral("yksPdProp"));
        yksPdProp->setMinimum(-500);
        yksPdProp->setMaximum(500);
        yksPdProp->setValue(1);

        horizontalLayout_15->addWidget(yksPdProp);

        zksPdProp = new QDoubleSpinBox(groupBox_4);
        zksPdProp->setObjectName(QStringLiteral("zksPdProp"));
        zksPdProp->setMinimum(-500);
        zksPdProp->setMaximum(500);
        zksPdProp->setValue(1);

        horizontalLayout_15->addWidget(zksPdProp);


        gridLayout_8->addLayout(horizontalLayout_15, 0, 0, 1, 1);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setSpacing(6);
        horizontalLayout_16->setObjectName(QStringLiteral("horizontalLayout_16"));
        label_17 = new QLabel(groupBox_4);
        label_17->setObjectName(QStringLiteral("label_17"));

        horizontalLayout_16->addWidget(label_17);

        xkdPdProp = new QDoubleSpinBox(groupBox_4);
        xkdPdProp->setObjectName(QStringLiteral("xkdPdProp"));
        xkdPdProp->setMinimum(-500);
        xkdPdProp->setMaximum(500);
        xkdPdProp->setSingleStep(0.1);
        xkdPdProp->setValue(1);

        horizontalLayout_16->addWidget(xkdPdProp);

        ykdPdProp = new QDoubleSpinBox(groupBox_4);
        ykdPdProp->setObjectName(QStringLiteral("ykdPdProp"));
        ykdPdProp->setMinimum(-500);
        ykdPdProp->setMaximum(500);
        ykdPdProp->setSingleStep(0.1);
        ykdPdProp->setValue(1);

        horizontalLayout_16->addWidget(ykdPdProp);

        zkdPdProp = new QDoubleSpinBox(groupBox_4);
        zkdPdProp->setObjectName(QStringLiteral("zkdPdProp"));
        zkdPdProp->setMinimum(-500);
        zkdPdProp->setMaximum(500);
        zkdPdProp->setSingleStep(0.1);
        zkdPdProp->setValue(1);

        horizontalLayout_16->addWidget(zkdPdProp);


        gridLayout_8->addLayout(horizontalLayout_16, 1, 0, 1, 1);

        groupBoxCone = new QGroupBox(tab);
        groupBoxCone->setObjectName(QStringLiteral("groupBoxCone"));
        groupBoxCone->setGeometry(QRect(10, 430, 301, 241));
        groupBoxCone->setFont(font);
        groupBoxCone->setStyleSheet(QLatin1String("QGroupBox::title { \n"
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
        layoutWidget2->setObjectName(QStringLiteral("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(3, 3, 291, 236));
        verticalLayout_3 = new QVBoxLayout(layoutWidget2);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(1);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_27 = new QHBoxLayout();
        horizontalLayout_27->setSpacing(6);
        horizontalLayout_27->setObjectName(QStringLiteral("horizontalLayout_27"));
        label_28 = new QLabel(layoutWidget2);
        label_28->setObjectName(QStringLiteral("label_28"));

        horizontalLayout_27->addWidget(label_28);

        coneM = new QDoubleSpinBox(layoutWidget2);
        coneM->setObjectName(QStringLiteral("coneM"));
        coneM->setMaximum(2000);
        coneM->setSingleStep(2);
        coneM->setValue(20);

        horizontalLayout_27->addWidget(coneM);


        verticalLayout_2->addLayout(horizontalLayout_27);

        horizontalLayout_29 = new QHBoxLayout();
        horizontalLayout_29->setSpacing(6);
        horizontalLayout_29->setObjectName(QStringLiteral("horizontalLayout_29"));
        label_30 = new QLabel(layoutWidget2);
        label_30->setObjectName(QStringLiteral("label_30"));

        horizontalLayout_29->addWidget(label_30);

        coneHeight = new QDoubleSpinBox(layoutWidget2);
        coneHeight->setObjectName(QStringLiteral("coneHeight"));
        coneHeight->setMaximum(500);
        coneHeight->setSingleStep(0.01);
        coneHeight->setValue(0.03);

        horizontalLayout_29->addWidget(coneHeight);


        verticalLayout_2->addLayout(horizontalLayout_29);

        horizontalLayout_28 = new QHBoxLayout();
        horizontalLayout_28->setSpacing(6);
        horizontalLayout_28->setObjectName(QStringLiteral("horizontalLayout_28"));
        label_19 = new QLabel(layoutWidget2);
        label_19->setObjectName(QStringLiteral("label_19"));

        horizontalLayout_28->addWidget(label_19);

        coneRadius = new QDoubleSpinBox(layoutWidget2);
        coneRadius->setObjectName(QStringLiteral("coneRadius"));
        coneRadius->setMaximum(500);
        coneRadius->setSingleStep(0.01);
        coneRadius->setValue(0.07);

        horizontalLayout_28->addWidget(coneRadius);


        verticalLayout_2->addLayout(horizontalLayout_28);

        horizontalLayout_30 = new QHBoxLayout();
        horizontalLayout_30->setSpacing(6);
        horizontalLayout_30->setObjectName(QStringLiteral("horizontalLayout_30"));
        label_29 = new QLabel(layoutWidget2);
        label_29->setObjectName(QStringLiteral("label_29"));

        horizontalLayout_30->addWidget(label_29);

        coneAngle = new QDoubleSpinBox(layoutWidget2);
        coneAngle->setObjectName(QStringLiteral("coneAngle"));
        coneAngle->setMinimum(-180);
        coneAngle->setMaximum(180);
        coneAngle->setSingleStep(5);
        coneAngle->setValue(45);

        horizontalLayout_30->addWidget(coneAngle);


        verticalLayout_2->addLayout(horizontalLayout_30);


        verticalLayout_3->addLayout(verticalLayout_2);

        horizontalLayout_23 = new QHBoxLayout();
        horizontalLayout_23->setSpacing(6);
        horizontalLayout_23->setObjectName(QStringLiteral("horizontalLayout_23"));
        label_24 = new QLabel(layoutWidget2);
        label_24->setObjectName(QStringLiteral("label_24"));

        horizontalLayout_23->addWidget(label_24);

        coneLimit = new QSpinBox(layoutWidget2);
        coneLimit->setObjectName(QStringLiteral("coneLimit"));
        coneLimit->setMaximum(100);

        horizontalLayout_23->addWidget(coneLimit);


        verticalLayout_3->addLayout(horizontalLayout_23);

        FootLeft = new QFrame(layoutWidget2);
        FootLeft->setObjectName(QStringLiteral("FootLeft"));
        horizontalLayout_25 = new QHBoxLayout(FootLeft);
        horizontalLayout_25->setSpacing(1);
        horizontalLayout_25->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_25->setObjectName(QStringLiteral("horizontalLayout_25"));
        horizontalLayout_25->setContentsMargins(2, 2, 2, 2);
        label_26 = new QLabel(FootLeft);
        label_26->setObjectName(QStringLiteral("label_26"));

        horizontalLayout_25->addWidget(label_26);

        coneFoot1 = new QSlider(FootLeft);
        coneFoot1->setObjectName(QStringLiteral("coneFoot1"));
        coneFoot1->setMaximum(100);
        coneFoot1->setPageStep(5);
        coneFoot1->setValue(100);
        coneFoot1->setOrientation(Qt::Horizontal);

        horizontalLayout_25->addWidget(coneFoot1);

        conePerc1 = new QLabel(FootLeft);
        conePerc1->setObjectName(QStringLiteral("conePerc1"));

        horizontalLayout_25->addWidget(conePerc1);


        verticalLayout_3->addWidget(FootLeft);

        FootRight = new QFrame(layoutWidget2);
        FootRight->setObjectName(QStringLiteral("FootRight"));
        horizontalLayout_26 = new QHBoxLayout(FootRight);
        horizontalLayout_26->setSpacing(1);
        horizontalLayout_26->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_26->setObjectName(QStringLiteral("horizontalLayout_26"));
        horizontalLayout_26->setContentsMargins(2, 2, 2, 2);
        label_27 = new QLabel(FootRight);
        label_27->setObjectName(QStringLiteral("label_27"));

        horizontalLayout_26->addWidget(label_27);

        coneFoot2 = new QSlider(FootRight);
        coneFoot2->setObjectName(QStringLiteral("coneFoot2"));
        coneFoot2->setMaximum(100);
        coneFoot2->setPageStep(5);
        coneFoot2->setValue(100);
        coneFoot2->setOrientation(Qt::Horizontal);

        horizontalLayout_26->addWidget(coneFoot2);

        conePerc2 = new QLabel(FootRight);
        conePerc2->setObjectName(QStringLiteral("conePerc2"));

        horizontalLayout_26->addWidget(conePerc2);


        verticalLayout_3->addWidget(FootRight);

        groupBox_7 = new QGroupBox(tab);
        groupBox_7->setObjectName(QStringLiteral("groupBox_7"));
        groupBox_7->setGeometry(QRect(10, 380, 301, 44));
        groupBox_7->setFont(font);
        groupBox_7->setStyleSheet(QLatin1String("QGroupBox::title { \n"
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
        gridLayout_11->setObjectName(QStringLiteral("gridLayout_11"));
        horizontalLayout_24 = new QHBoxLayout();
        horizontalLayout_24->setSpacing(2);
        horizontalLayout_24->setObjectName(QStringLiteral("horizontalLayout_24"));
        checkGRF = new QCheckBox(groupBox_7);
        checkGRF->setObjectName(QStringLiteral("checkGRF"));
        checkGRF->setChecked(true);

        horizontalLayout_24->addWidget(checkGRF);

        checkShadow = new QCheckBox(groupBox_7);
        checkShadow->setObjectName(QStringLiteral("checkShadow"));

        horizontalLayout_24->addWidget(checkShadow);

        checkShowChara = new QCheckBox(groupBox_7);
        checkShowChara->setObjectName(QStringLiteral("checkShowChara"));
        checkShowChara->setChecked(true);

        horizontalLayout_24->addWidget(checkShowChara);

        checkWireChara = new QCheckBox(groupBox_7);
        checkWireChara->setObjectName(QStringLiteral("checkWireChara"));

        horizontalLayout_24->addWidget(checkWireChara);


        gridLayout_11->addLayout(horizontalLayout_24, 0, 0, 1, 1);

        Options->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        btnStart = new QPushButton(tab_2);
        btnStart->setObjectName(QStringLiteral("btnStart"));
        btnStart->setGeometry(QRect(10, 20, 98, 27));
        btnPause = new QPushButton(tab_2);
        btnPause->setObjectName(QStringLiteral("btnPause"));
        btnPause->setGeometry(QRect(110, 20, 98, 27));
        btnRestart = new QPushButton(tab_2);
        btnRestart->setObjectName(QStringLiteral("btnRestart"));
        btnRestart->setGeometry(QRect(210, 20, 98, 27));
        timeLineMotion = new QSlider(tab_2);
        timeLineMotion->setObjectName(QStringLiteral("timeLineMotion"));
        timeLineMotion->setGeometry(QRect(10, 70, 291, 29));
        timeLineMotion->setPageStep(1);
        timeLineMotion->setOrientation(Qt::Horizontal);
        layoutWidget3 = new QWidget(tab_2);
        layoutWidget3->setObjectName(QStringLiteral("layoutWidget3"));
        layoutWidget3->setGeometry(QRect(90, 100, 136, 19));
        horizontalLayout_22 = new QHBoxLayout(layoutWidget3);
        horizontalLayout_22->setSpacing(6);
        horizontalLayout_22->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_22->setObjectName(QStringLiteral("horizontalLayout_22"));
        horizontalLayout_22->setContentsMargins(0, 0, 0, 0);
        iframe = new QLabel(layoutWidget3);
        iframe->setObjectName(QStringLiteral("iframe"));

        horizontalLayout_22->addWidget(iframe);

        label_23 = new QLabel(layoutWidget3);
        label_23->setObjectName(QStringLiteral("label_23"));

        horizontalLayout_22->addWidget(label_23);

        nframe = new QLabel(layoutWidget3);
        nframe->setObjectName(QStringLiteral("nframe"));

        horizontalLayout_22->addWidget(nframe);

        groupBox_11 = new QGroupBox(tab_2);
        groupBox_11->setObjectName(QStringLiteral("groupBox_11"));
        groupBox_11->setGeometry(QRect(10, 230, 301, 221));
        groupBox_11->setFont(font);
        groupBox_11->setStyleSheet(QLatin1String("QGroupBox::title { \n"
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
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        horizontalLayout_36 = new QHBoxLayout();
        horizontalLayout_36->setSpacing(6);
        horizontalLayout_36->setObjectName(QStringLiteral("horizontalLayout_36"));
        label_34 = new QLabel(groupBox_11);
        label_34->setObjectName(QStringLiteral("label_34"));

        horizontalLayout_36->addWidget(label_34);

        frameEdit = new QSpinBox(groupBox_11);
        frameEdit->setObjectName(QStringLiteral("frameEdit"));

        horizontalLayout_36->addWidget(frameEdit);


        verticalLayout_4->addLayout(horizontalLayout_36);

        checkShowEditingFrame = new QCheckBox(groupBox_11);
        checkShowEditingFrame->setObjectName(QStringLiteral("checkShowEditingFrame"));

        verticalLayout_4->addWidget(checkShowEditingFrame);

        checkFrameFootLeft = new QCheckBox(groupBox_11);
        checkFrameFootLeft->setObjectName(QStringLiteral("checkFrameFootLeft"));
        checkFrameFootLeft->setChecked(true);

        verticalLayout_4->addWidget(checkFrameFootLeft);

        checkFrameFootRight = new QCheckBox(groupBox_11);
        checkFrameFootRight->setObjectName(QStringLiteral("checkFrameFootRight"));
        checkFrameFootRight->setChecked(true);

        verticalLayout_4->addWidget(checkFrameFootRight);

        saveEditingFrame = new QPushButton(groupBox_11);
        saveEditingFrame->setObjectName(QStringLiteral("saveEditingFrame"));

        verticalLayout_4->addWidget(saveEditingFrame);

        loadEditedFrames = new QPushButton(groupBox_11);
        loadEditedFrames->setObjectName(QStringLiteral("loadEditedFrames"));

        verticalLayout_4->addWidget(loadEditedFrames);

        layoutWidget4 = new QWidget(tab_2);
        layoutWidget4->setObjectName(QStringLiteral("layoutWidget4"));
        layoutWidget4->setGeometry(QRect(20, 140, 274, 29));
        horizontalLayout_35 = new QHBoxLayout(layoutWidget4);
        horizontalLayout_35->setSpacing(6);
        horizontalLayout_35->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_35->setObjectName(QStringLiteral("horizontalLayout_35"));
        horizontalLayout_35->setContentsMargins(0, 0, 0, 0);
        label_22 = new QLabel(layoutWidget4);
        label_22->setObjectName(QStringLiteral("label_22"));

        horizontalLayout_35->addWidget(label_22);

        sensorTolerance = new QDoubleSpinBox(layoutWidget4);
        sensorTolerance->setObjectName(QStringLiteral("sensorTolerance"));
        sensorTolerance->setDecimals(4);
        sensorTolerance->setMaximum(999);
        sensorTolerance->setSingleStep(0.01);
        sensorTolerance->setValue(0.01);

        horizontalLayout_35->addWidget(sensorTolerance);

        groupBox_12 = new QGroupBox(tab_2);
        groupBox_12->setObjectName(QStringLiteral("groupBox_12"));
        groupBox_12->setGeometry(QRect(10, 470, 301, 112));
        groupBox_12->setFont(font);
        groupBox_12->setStyleSheet(QLatin1String("QGroupBox::title { \n"
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
        gridLayout_15->setObjectName(QStringLiteral("gridLayout_15"));
        mocapCyclic = new QCheckBox(groupBox_12);
        mocapCyclic->setObjectName(QStringLiteral("mocapCyclic"));
        mocapCyclic->setChecked(true);

        gridLayout_15->addWidget(mocapCyclic, 0, 0, 1, 1);

        horizontalLayout_38 = new QHBoxLayout();
        horizontalLayout_38->setSpacing(6);
        horizontalLayout_38->setObjectName(QStringLiteral("horizontalLayout_38"));
        label_36 = new QLabel(groupBox_12);
        label_36->setObjectName(QStringLiteral("label_36"));

        horizontalLayout_38->addWidget(label_36);

        beginClycle = new QSpinBox(groupBox_12);
        beginClycle->setObjectName(QStringLiteral("beginClycle"));

        horizontalLayout_38->addWidget(beginClycle);


        gridLayout_15->addLayout(horizontalLayout_38, 1, 0, 1, 1);

        horizontalLayout_39 = new QHBoxLayout();
        horizontalLayout_39->setSpacing(6);
        horizontalLayout_39->setObjectName(QStringLiteral("horizontalLayout_39"));
        label_37 = new QLabel(groupBox_12);
        label_37->setObjectName(QStringLiteral("label_37"));

        horizontalLayout_39->addWidget(label_37);

        endClycle = new QSpinBox(groupBox_12);
        endClycle->setObjectName(QStringLiteral("endClycle"));

        horizontalLayout_39->addWidget(endClycle);


        gridLayout_15->addLayout(horizontalLayout_39, 2, 0, 1, 1);

        checkShowMocap = new QCheckBox(tab_2);
        checkShowMocap->setObjectName(QStringLiteral("checkShowMocap"));
        checkShowMocap->setGeometry(QRect(20, 200, 151, 22));
        checkShowMocap->setChecked(true);
        checkScreenShot = new QCheckBox(tab_2);
        checkScreenShot->setObjectName(QStringLiteral("checkScreenShot"));
        checkScreenShot->setGeometry(QRect(20, 120, 251, 22));
        layoutWidget5 = new QWidget(tab_2);
        layoutWidget5->setObjectName(QStringLiteral("layoutWidget5"));
        layoutWidget5->setGeometry(QRect(20, 170, 271, 29));
        horizontalLayout_42 = new QHBoxLayout(layoutWidget5);
        horizontalLayout_42->setSpacing(6);
        horizontalLayout_42->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_42->setObjectName(QStringLiteral("horizontalLayout_42"));
        horizontalLayout_42->setContentsMargins(0, 0, 0, 0);
        label_39 = new QLabel(layoutWidget5);
        label_39->setObjectName(QStringLiteral("label_39"));

        horizontalLayout_42->addWidget(label_39);

        stepsInterpolation = new QDoubleSpinBox(layoutWidget5);
        stepsInterpolation->setObjectName(QStringLiteral("stepsInterpolation"));
        stepsInterpolation->setDecimals(2);
        stepsInterpolation->setMinimum(0);
        stepsInterpolation->setMaximum(100000);

        horizontalLayout_42->addWidget(stepsInterpolation);

        Options->addTab(tab_2, QString());
        tabManipulatorSim = new QWidget();
        tabManipulatorSim->setObjectName(QStringLiteral("tabManipulatorSim"));
        gridLayout_12 = new QGridLayout(tabManipulatorSim);
        gridLayout_12->setSpacing(6);
        gridLayout_12->setContentsMargins(11, 11, 11, 11);
        gridLayout_12->setObjectName(QStringLiteral("gridLayout_12"));
        listWidgetObjects = new QListWidget(tabManipulatorSim);
        listWidgetObjects->setObjectName(QStringLiteral("listWidgetObjects"));

        gridLayout_12->addWidget(listWidgetObjects, 0, 0, 1, 1);

        groupBox_9 = new QGroupBox(tabManipulatorSim);
        groupBox_9->setObjectName(QStringLiteral("groupBox_9"));
        groupBox_9->setFont(font);
        groupBox_9->setStyleSheet(QLatin1String("QGroupBox::title { \n"
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
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        horizontalLayout_31 = new QHBoxLayout();
        horizontalLayout_31->setSpacing(6);
        horizontalLayout_31->setObjectName(QStringLiteral("horizontalLayout_31"));
        enable_cpdp = new QCheckBox(groupBox_9);
        enable_cpdp->setObjectName(QStringLiteral("enable_cpdp"));

        horizontalLayout_31->addWidget(enable_cpdp);

        show_effector = new QCheckBox(groupBox_9);
        show_effector->setObjectName(QStringLiteral("show_effector"));

        horizontalLayout_31->addWidget(show_effector);

        show_target = new QCheckBox(groupBox_9);
        show_target->setObjectName(QStringLiteral("show_target"));

        horizontalLayout_31->addWidget(show_target);


        gridLayout_6->addLayout(horizontalLayout_31, 0, 0, 1, 1);

        horizontalLayout_32 = new QHBoxLayout();
        horizontalLayout_32->setSpacing(6);
        horizontalLayout_32->setObjectName(QStringLiteral("horizontalLayout_32"));
        label_31 = new QLabel(groupBox_9);
        label_31->setObjectName(QStringLiteral("label_31"));

        horizontalLayout_32->addWidget(label_31);

        posx_target = new QDoubleSpinBox(groupBox_9);
        posx_target->setObjectName(QStringLiteral("posx_target"));
        posx_target->setDecimals(2);
        posx_target->setMinimum(-1000);
        posx_target->setMaximum(1000);
        posx_target->setSingleStep(0.05);

        horizontalLayout_32->addWidget(posx_target);

        posy_target = new QDoubleSpinBox(groupBox_9);
        posy_target->setObjectName(QStringLiteral("posy_target"));
        posy_target->setDecimals(2);
        posy_target->setMinimum(-1000);
        posy_target->setMaximum(1000);
        posy_target->setSingleStep(0.05);

        horizontalLayout_32->addWidget(posy_target);

        posz_target = new QDoubleSpinBox(groupBox_9);
        posz_target->setObjectName(QStringLiteral("posz_target"));
        posz_target->setDecimals(2);
        posz_target->setMinimum(-1000);
        posz_target->setMaximum(1000);
        posz_target->setSingleStep(0.05);

        horizontalLayout_32->addWidget(posz_target);


        gridLayout_6->addLayout(horizontalLayout_32, 1, 0, 1, 1);

        horizontalLayout_33 = new QHBoxLayout();
        horizontalLayout_33->setSpacing(6);
        horizontalLayout_33->setObjectName(QStringLiteral("horizontalLayout_33"));
        label_32 = new QLabel(groupBox_9);
        label_32->setObjectName(QStringLiteral("label_32"));

        horizontalLayout_33->addWidget(label_32);

        posx_ks = new QDoubleSpinBox(groupBox_9);
        posx_ks->setObjectName(QStringLiteral("posx_ks"));
        posx_ks->setDecimals(2);
        posx_ks->setMinimum(-1000);
        posx_ks->setMaximum(1000);
        posx_ks->setSingleStep(0.1);

        horizontalLayout_33->addWidget(posx_ks);

        posy_ks = new QDoubleSpinBox(groupBox_9);
        posy_ks->setObjectName(QStringLiteral("posy_ks"));
        posy_ks->setDecimals(2);
        posy_ks->setMinimum(-1000);
        posy_ks->setMaximum(1000);
        posy_ks->setSingleStep(0.1);

        horizontalLayout_33->addWidget(posy_ks);

        posz_ks = new QDoubleSpinBox(groupBox_9);
        posz_ks->setObjectName(QStringLiteral("posz_ks"));
        posz_ks->setDecimals(2);
        posz_ks->setMinimum(-1000);
        posz_ks->setMaximum(1000);
        posz_ks->setSingleStep(0.1);

        horizontalLayout_33->addWidget(posz_ks);


        gridLayout_6->addLayout(horizontalLayout_33, 2, 0, 1, 1);

        horizontalLayout_34 = new QHBoxLayout();
        horizontalLayout_34->setSpacing(6);
        horizontalLayout_34->setObjectName(QStringLiteral("horizontalLayout_34"));
        label_33 = new QLabel(groupBox_9);
        label_33->setObjectName(QStringLiteral("label_33"));

        horizontalLayout_34->addWidget(label_33);

        posx_kd = new QDoubleSpinBox(groupBox_9);
        posx_kd->setObjectName(QStringLiteral("posx_kd"));
        posx_kd->setDecimals(2);
        posx_kd->setMinimum(-1000);
        posx_kd->setMaximum(1000);
        posx_kd->setSingleStep(0.1);

        horizontalLayout_34->addWidget(posx_kd);

        posy_kd = new QDoubleSpinBox(groupBox_9);
        posy_kd->setObjectName(QStringLiteral("posy_kd"));
        posy_kd->setDecimals(2);
        posy_kd->setMinimum(-1000);
        posy_kd->setMaximum(1000);
        posy_kd->setSingleStep(0.1);

        horizontalLayout_34->addWidget(posy_kd);

        posz_kd = new QDoubleSpinBox(groupBox_9);
        posz_kd->setObjectName(QStringLiteral("posz_kd"));
        posz_kd->setDecimals(2);
        posz_kd->setMinimum(-1000);
        posz_kd->setMaximum(1000);
        posz_kd->setSingleStep(0.1);

        horizontalLayout_34->addWidget(posz_kd);


        gridLayout_6->addLayout(horizontalLayout_34, 3, 0, 1, 1);


        gridLayout_12->addWidget(groupBox_9, 2, 0, 1, 1);

        groupBox_2 = new QGroupBox(tabManipulatorSim);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setFont(font);
        groupBox_2->setStyleSheet(QLatin1String("QGroupBox::title { \n"
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
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setHorizontalSpacing(2);
        gridLayout_3->setVerticalSpacing(0);
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        label_9 = new QLabel(groupBox_2);
        label_9->setObjectName(QStringLiteral("label_9"));

        horizontalLayout_8->addWidget(label_9);

        scalex = new QDoubleSpinBox(groupBox_2);
        scalex->setObjectName(QStringLiteral("scalex"));

        horizontalLayout_8->addWidget(scalex);

        scaley = new QDoubleSpinBox(groupBox_2);
        scaley->setObjectName(QStringLiteral("scaley"));

        horizontalLayout_8->addWidget(scaley);

        scalez = new QDoubleSpinBox(groupBox_2);
        scalez->setObjectName(QStringLiteral("scalez"));

        horizontalLayout_8->addWidget(scalez);


        gridLayout_3->addLayout(horizontalLayout_8, 2, 0, 1, 1);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QStringLiteral("label_7"));

        horizontalLayout_6->addWidget(label_7);

        posx = new QDoubleSpinBox(groupBox_2);
        posx->setObjectName(QStringLiteral("posx"));
        posx->setDecimals(3);
        posx->setMinimum(-1000);
        posx->setMaximum(1000);

        horizontalLayout_6->addWidget(posx);

        posy = new QDoubleSpinBox(groupBox_2);
        posy->setObjectName(QStringLiteral("posy"));
        posy->setDecimals(3);
        posy->setMinimum(-1000);
        posy->setMaximum(1000);

        horizontalLayout_6->addWidget(posy);

        posz = new QDoubleSpinBox(groupBox_2);
        posz->setObjectName(QStringLiteral("posz"));
        posz->setDecimals(3);
        posz->setMinimum(-1000);
        posz->setMaximum(1000);

        horizontalLayout_6->addWidget(posz);


        gridLayout_3->addLayout(horizontalLayout_6, 1, 0, 1, 1);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        label_10 = new QLabel(groupBox_2);
        label_10->setObjectName(QStringLiteral("label_10"));

        horizontalLayout_9->addWidget(label_10);

        mass = new QDoubleSpinBox(groupBox_2);
        mass->setObjectName(QStringLiteral("mass"));

        horizontalLayout_9->addWidget(mass);


        gridLayout_3->addLayout(horizontalLayout_9, 5, 0, 1, 1);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName(QStringLiteral("label_8"));

        horizontalLayout_7->addWidget(label_8);

        oeulerx = new QSpinBox(groupBox_2);
        oeulerx->setObjectName(QStringLiteral("oeulerx"));
        oeulerx->setMinimum(-360);
        oeulerx->setMaximum(360);

        horizontalLayout_7->addWidget(oeulerx);

        oeulery = new QSpinBox(groupBox_2);
        oeulery->setObjectName(QStringLiteral("oeulery"));
        oeulery->setMinimum(-360);
        oeulery->setMaximum(360);

        horizontalLayout_7->addWidget(oeulery);

        oeulerz = new QSpinBox(groupBox_2);
        oeulerz->setObjectName(QStringLiteral("oeulerz"));
        oeulerz->setMinimum(-360);
        oeulerz->setMaximum(360);

        horizontalLayout_7->addWidget(oeulerz);


        gridLayout_3->addLayout(horizontalLayout_7, 3, 0, 1, 1);

        isFoot = new QCheckBox(groupBox_2);
        isFoot->setObjectName(QStringLiteral("isFoot"));

        gridLayout_3->addWidget(isFoot, 6, 0, 1, 1);

        isBodyBalance = new QCheckBox(groupBox_2);
        isBodyBalance->setObjectName(QStringLiteral("isBodyBalance"));

        gridLayout_3->addWidget(isBodyBalance, 7, 0, 1, 1);


        gridLayout_12->addWidget(groupBox_2, 3, 0, 1, 1);

        groupBox = new QGroupBox(tabManipulatorSim);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setFont(font);
        groupBox->setStyleSheet(QLatin1String("QGroupBox::title { \n"
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
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_5->addWidget(label_2, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        forcex = new QDoubleSpinBox(groupBox);
        forcex->setObjectName(QStringLiteral("forcex"));
        forcex->setMaximumSize(QSize(75, 16777215));
        forcex->setMinimum(-10000);
        forcex->setMaximum(10000);
        forcex->setSingleStep(10);

        horizontalLayout_2->addWidget(forcex);

        forcey = new QDoubleSpinBox(groupBox);
        forcey->setObjectName(QStringLiteral("forcey"));
        forcey->setMaximumSize(QSize(75, 16777215));
        forcey->setMinimum(-10000);
        forcey->setMaximum(10000);
        forcey->setSingleStep(10);

        horizontalLayout_2->addWidget(forcey);

        forcez = new QDoubleSpinBox(groupBox);
        forcez->setObjectName(QStringLiteral("forcez"));
        forcez->setMaximumSize(QSize(75, 16777215));
        forcez->setMinimum(-10000);
        forcez->setMaximum(10000);
        forcez->setSingleStep(10);

        horizontalLayout_2->addWidget(forcez);

        applyForce = new QPushButton(groupBox);
        applyForce->setObjectName(QStringLiteral("applyForce"));
        applyForce->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_2->addWidget(applyForce);


        gridLayout_5->addLayout(horizontalLayout_2, 1, 0, 1, 1);


        gridLayout_12->addWidget(groupBox, 4, 0, 1, 1);

        groupBox_10 = new QGroupBox(tabManipulatorSim);
        groupBox_10->setObjectName(QStringLiteral("groupBox_10"));
        groupBox_10->setFont(font);
        groupBox_10->setStyleSheet(QLatin1String("QGroupBox::title { \n"
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
        gridLayout_13->setObjectName(QStringLiteral("gridLayout_13"));
        hasCup = new QCheckBox(groupBox_10);
        hasCup->setObjectName(QStringLiteral("hasCup"));

        gridLayout_13->addWidget(hasCup, 0, 0, 1, 1);


        gridLayout_12->addWidget(groupBox_10, 1, 0, 1, 1);

        Options->addTab(tabManipulatorSim, QString());
        controlpd = new QWidget();
        controlpd->setObjectName(QStringLiteral("controlpd"));
        gridLayout_10 = new QGridLayout(controlpd);
        gridLayout_10->setSpacing(6);
        gridLayout_10->setContentsMargins(11, 11, 11, 11);
        gridLayout_10->setObjectName(QStringLiteral("gridLayout_10"));
        listWidgetJoints = new QListWidget(controlpd);
        listWidgetJoints->setObjectName(QStringLiteral("listWidgetJoints"));

        gridLayout_10->addWidget(listWidgetJoints, 0, 0, 1, 1);

        groupBox_5 = new QGroupBox(controlpd);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        groupBox_5->setFont(font);
        groupBox_5->setStyleSheet(QLatin1String("QGroupBox::title { \n"
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
        gridLayout_9->setObjectName(QStringLiteral("gridLayout_9"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_3 = new QLabel(groupBox_5);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_3->addWidget(label_3);

        ksx = new QDoubleSpinBox(groupBox_5);
        ksx->setObjectName(QStringLiteral("ksx"));
        ksx->setMinimum(-1000);
        ksx->setMaximum(1000);
        ksx->setSingleStep(0.1);

        horizontalLayout_3->addWidget(ksx);

        ksy = new QDoubleSpinBox(groupBox_5);
        ksy->setObjectName(QStringLiteral("ksy"));
        ksy->setMinimum(-1000);
        ksy->setMaximum(1000);
        ksy->setSingleStep(0.1);

        horizontalLayout_3->addWidget(ksy);

        ksz = new QDoubleSpinBox(groupBox_5);
        ksz->setObjectName(QStringLiteral("ksz"));
        ksz->setMinimum(-1000);
        ksz->setMaximum(1000);
        ksz->setSingleStep(0.1);

        horizontalLayout_3->addWidget(ksz);


        gridLayout_9->addLayout(horizontalLayout_3, 0, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_4 = new QLabel(groupBox_5);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_4->addWidget(label_4);

        kdx = new QDoubleSpinBox(groupBox_5);
        kdx->setObjectName(QStringLiteral("kdx"));
        kdx->setMinimum(-1000);
        kdx->setMaximum(1000);
        kdx->setSingleStep(0.1);

        horizontalLayout_4->addWidget(kdx);

        kdy = new QDoubleSpinBox(groupBox_5);
        kdy->setObjectName(QStringLiteral("kdy"));
        kdy->setMinimum(-1000);
        kdy->setMaximum(1000);
        kdy->setSingleStep(0.1);

        horizontalLayout_4->addWidget(kdy);

        kdz = new QDoubleSpinBox(groupBox_5);
        kdz->setObjectName(QStringLiteral("kdz"));
        kdz->setMinimum(-1000);
        kdz->setMaximum(1000);
        kdz->setSingleStep(0.1);

        horizontalLayout_4->addWidget(kdz);


        gridLayout_9->addLayout(horizontalLayout_4, 1, 0, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_5 = new QLabel(groupBox_5);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_5->addWidget(label_5);

        eulerx = new QDoubleSpinBox(groupBox_5);
        eulerx->setObjectName(QStringLiteral("eulerx"));
        eulerx->setMinimum(-360);
        eulerx->setMaximum(360);

        horizontalLayout_5->addWidget(eulerx);

        eulery = new QDoubleSpinBox(groupBox_5);
        eulery->setObjectName(QStringLiteral("eulery"));
        eulery->setMinimum(-360);
        eulery->setMaximum(360);

        horizontalLayout_5->addWidget(eulery);

        eulerz = new QDoubleSpinBox(groupBox_5);
        eulerz->setObjectName(QStringLiteral("eulerz"));
        eulerz->setMinimum(-360);
        eulerz->setMaximum(360);

        horizontalLayout_5->addWidget(eulerz);


        gridLayout_9->addLayout(horizontalLayout_5, 2, 0, 1, 1);

        enablepd = new QCheckBox(groupBox_5);
        enablepd->setObjectName(QStringLiteral("enablepd"));

        gridLayout_9->addWidget(enablepd, 3, 0, 1, 1);


        gridLayout_10->addWidget(groupBox_5, 1, 0, 1, 1);

        groupBox_6 = new QGroupBox(controlpd);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));
        groupBox_6->setFont(font);
        groupBox_6->setStyleSheet(QLatin1String("QGroupBox::title { \n"
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
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setSpacing(6);
        horizontalLayout_17->setObjectName(QStringLiteral("horizontalLayout_17"));
        label_6 = new QLabel(groupBox_6);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_17->addWidget(label_6);

        xksPdApply = new QLabel(groupBox_6);
        xksPdApply->setObjectName(QStringLiteral("xksPdApply"));

        horizontalLayout_17->addWidget(xksPdApply);

        yksPdApply = new QLabel(groupBox_6);
        yksPdApply->setObjectName(QStringLiteral("yksPdApply"));

        horizontalLayout_17->addWidget(yksPdApply);

        zksPdApply = new QLabel(groupBox_6);
        zksPdApply->setObjectName(QStringLiteral("zksPdApply"));

        horizontalLayout_17->addWidget(zksPdApply);


        gridLayout_4->addLayout(horizontalLayout_17, 0, 0, 1, 1);

        horizontalLayout_18 = new QHBoxLayout();
        horizontalLayout_18->setSpacing(6);
        horizontalLayout_18->setObjectName(QStringLiteral("horizontalLayout_18"));
        label_21 = new QLabel(groupBox_6);
        label_21->setObjectName(QStringLiteral("label_21"));

        horizontalLayout_18->addWidget(label_21);

        xkdPdApply = new QLabel(groupBox_6);
        xkdPdApply->setObjectName(QStringLiteral("xkdPdApply"));

        horizontalLayout_18->addWidget(xkdPdApply);

        ykdPdApply = new QLabel(groupBox_6);
        ykdPdApply->setObjectName(QStringLiteral("ykdPdApply"));

        horizontalLayout_18->addWidget(ykdPdApply);

        zkdPdApply = new QLabel(groupBox_6);
        zkdPdApply->setObjectName(QStringLiteral("zkdPdApply"));

        horizontalLayout_18->addWidget(zkdPdApply);


        gridLayout_4->addLayout(horizontalLayout_18, 1, 0, 1, 1);


        gridLayout_10->addWidget(groupBox_6, 2, 0, 1, 1);

        infoPD = new QFrame(controlpd);
        infoPD->setObjectName(QStringLiteral("infoPD"));
        infoPD->setFrameShape(QFrame::NoFrame);
        infoPD->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(infoPD);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);

        gridLayout_10->addWidget(infoPD, 3, 0, 1, 1);

        Options->addTab(controlpd, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        layoutWidget_2 = new QWidget(tab_3);
        layoutWidget_2->setObjectName(QStringLiteral("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(10, 70, 306, 29));
        horizontalLayout_40 = new QHBoxLayout(layoutWidget_2);
        horizontalLayout_40->setSpacing(6);
        horizontalLayout_40->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_40->setObjectName(QStringLiteral("horizontalLayout_40"));
        horizontalLayout_40->setContentsMargins(0, 0, 0, 0);
        label_25 = new QLabel(layoutWidget_2);
        label_25->setObjectName(QStringLiteral("label_25"));

        horizontalLayout_40->addWidget(label_25);

        xkDis = new QDoubleSpinBox(layoutWidget_2);
        xkDis->setObjectName(QStringLiteral("xkDis"));
        xkDis->setMinimum(-500);
        xkDis->setMaximum(500);

        horizontalLayout_40->addWidget(xkDis);

        ykDis = new QDoubleSpinBox(layoutWidget_2);
        ykDis->setObjectName(QStringLiteral("ykDis"));
        ykDis->setMinimum(-500);
        ykDis->setMaximum(500);

        horizontalLayout_40->addWidget(ykDis);

        zkDis = new QDoubleSpinBox(layoutWidget_2);
        zkDis->setObjectName(QStringLiteral("zkDis"));
        zkDis->setMinimum(-500);
        zkDis->setMaximum(500);

        horizontalLayout_40->addWidget(zkDis);

        layoutWidget_3 = new QWidget(tab_3);
        layoutWidget_3->setObjectName(QStringLiteral("layoutWidget_3"));
        layoutWidget_3->setGeometry(QRect(10, 20, 308, 29));
        horizontalLayout_41 = new QHBoxLayout(layoutWidget_3);
        horizontalLayout_41->setSpacing(6);
        horizontalLayout_41->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_41->setObjectName(QStringLiteral("horizontalLayout_41"));
        horizontalLayout_41->setContentsMargins(0, 0, 0, 0);
        label_38 = new QLabel(layoutWidget_3);
        label_38->setObjectName(QStringLiteral("label_38"));

        horizontalLayout_41->addWidget(label_38);

        xkFor = new QDoubleSpinBox(layoutWidget_3);
        xkFor->setObjectName(QStringLiteral("xkFor"));
        xkFor->setMinimum(-500);
        xkFor->setMaximum(500);

        horizontalLayout_41->addWidget(xkFor);

        ykFor = new QDoubleSpinBox(layoutWidget_3);
        ykFor->setObjectName(QStringLiteral("ykFor"));
        ykFor->setMinimum(-500);
        ykFor->setMaximum(500);

        horizontalLayout_41->addWidget(ykFor);

        zkFor = new QDoubleSpinBox(layoutWidget_3);
        zkFor->setObjectName(QStringLiteral("zkFor"));
        zkFor->setMinimum(-500);
        zkFor->setMaximum(500);

        horizontalLayout_41->addWidget(zkFor);

        layoutWidget6 = new QWidget(tab_3);
        layoutWidget6->setObjectName(QStringLiteral("layoutWidget6"));
        layoutWidget6->setGeometry(QRect(10, 130, 182, 29));
        horizontalLayout_43 = new QHBoxLayout(layoutWidget6);
        horizontalLayout_43->setSpacing(6);
        horizontalLayout_43->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_43->setObjectName(QStringLiteral("horizontalLayout_43"));
        horizontalLayout_43->setContentsMargins(0, 0, 0, 0);
        label_40 = new QLabel(layoutWidget6);
        label_40->setObjectName(QStringLiteral("label_40"));

        horizontalLayout_43->addWidget(label_40);

        densityBall = new QDoubleSpinBox(layoutWidget6);
        densityBall->setObjectName(QStringLiteral("densityBall"));
        densityBall->setMinimum(-500);
        densityBall->setMaximum(500);
        densityBall->setValue(0.5);

        horizontalLayout_43->addWidget(densityBall);

        layoutWidget7 = new QWidget(tab_3);
        layoutWidget7->setObjectName(QStringLiteral("layoutWidget7"));
        layoutWidget7->setGeometry(QRect(10, 170, 185, 29));
        horizontalLayout_44 = new QHBoxLayout(layoutWidget7);
        horizontalLayout_44->setSpacing(6);
        horizontalLayout_44->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_44->setObjectName(QStringLiteral("horizontalLayout_44"));
        horizontalLayout_44->setContentsMargins(0, 0, 0, 0);
        label_41 = new QLabel(layoutWidget7);
        label_41->setObjectName(QStringLiteral("label_41"));

        horizontalLayout_44->addWidget(label_41);

        velocityBall = new QDoubleSpinBox(layoutWidget7);
        velocityBall->setObjectName(QStringLiteral("velocityBall"));
        velocityBall->setMinimum(-500);
        velocityBall->setMaximum(500);
        velocityBall->setValue(5);

        horizontalLayout_44->addWidget(velocityBall);

        Options->addTab(tab_3, QString());

        gridLayout_2->addWidget(Options, 0, 0, 1, 1);

        splitter->addWidget(widgetOpt);

        gridLayout->addWidget(splitter, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 658, 25));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuControl = new QMenu(menuBar);
        menuControl->setObjectName(QStringLiteral("menuControl"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
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

        Options->setCurrentIndex(5);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Physics Simulator", 0));
        actionPlay_Pause_Simulation->setText(QApplication::translate("MainWindow", "Play/Pause", 0));
        actionPlay_Pause_Simulation->setShortcut(QApplication::translate("MainWindow", "Space", 0));
        actionRestart_Simulation->setText(QApplication::translate("MainWindow", "Restart", 0));
        actionRestart_Simulation->setShortcut(QApplication::translate("MainWindow", "Ctrl+R", 0));
        actionOpen_Model->setText(QApplication::translate("MainWindow", "Open Model", 0));
        actionOpen_Model->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0));
        actionOpen_MoCap->setText(QApplication::translate("MainWindow", "Open MoCap", 0));
        actionOpen_MoCap->setShortcut(QApplication::translate("MainWindow", "Ctrl+M", 0));
        actionSave_Model->setText(QApplication::translate("MainWindow", "Save Model", 0));
        actionSave_Model->setShortcut(QApplication::translate("MainWindow", "Shift+M", 0));
        actionSave_Simulation->setText(QApplication::translate("MainWindow", "Save Simulation", 0));
        actionSave_Simulation->setShortcut(QApplication::translate("MainWindow", "Shift+S", 0));
        actionOpen_Simulation->setText(QApplication::translate("MainWindow", "Open Simulation", 0));
        label->setText(QApplication::translate("MainWindow", "Steps:", 0));
        enableGravity->setText(QApplication::translate("MainWindow", "Enable Gravity", 0));
        label_11->setText(QApplication::translate("MainWindow", "Gravity", 0));
        checkBox->setText(QApplication::translate("MainWindow", "Update Simultaneously", 0));
        kdtoks->setText(QApplication::translate("MainWindow", "Kd = 1/10ks Automaticaly", 0));
        checkMesh->setText(QApplication::translate("MainWindow", "Render with Mesh", 0));
        infoShow->setText(QApplication::translate("MainWindow", "Show Info", 0));
        Options->setTabText(Options->indexOf(tabParSim), QApplication::translate("MainWindow", "Parameters Simulation", 0));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Balance Control", 0));
        label_12->setText(QApplication::translate("MainWindow", "Tq Ks:", 0));
        label_13->setText(QApplication::translate("MainWindow", "Tq Kd:", 0));
        label_14->setText(QApplication::translate("MainWindow", "Fc Ks", 0));
        label_15->setText(QApplication::translate("MainWindow", "Fc kd:", 0));
        label_18->setText(QApplication::translate("MainWindow", "Mm L:", 0));
        label_35->setText(QApplication::translate("MainWindow", "Mm A:", 0));
        checkMomentum->setText(QApplication::translate("MainWindow", "Momentum", 0));
        checkTorqueBal->setText(QApplication::translate("MainWindow", "Torque", 0));
        checkForceBal->setText(QApplication::translate("MainWindow", "Force", 0));
        label_20->setText(QApplication::translate("MainWindow", "Angle", 0));
        groupBox_4->setTitle(QApplication::translate("MainWindow", "PD control Proportional", 0));
        label_16->setText(QApplication::translate("MainWindow", "Ks:", 0));
        label_17->setText(QApplication::translate("MainWindow", "kd:", 0));
        groupBoxCone->setTitle(QApplication::translate("MainWindow", "Cone Friction Foots", 0));
        label_28->setText(QApplication::translate("MainWindow", "Module:", 0));
        label_30->setText(QApplication::translate("MainWindow", "Height:", 0));
        label_19->setText(QApplication::translate("MainWindow", "Radius:", 0));
        label_29->setText(QApplication::translate("MainWindow", "Angle:", 0));
        label_24->setText(QApplication::translate("MainWindow", "Limit Percent:", 0));
        label_26->setText(QApplication::translate("MainWindow", "foot 1", 0));
        conePerc1->setText(QApplication::translate("MainWindow", "%", 0));
        label_27->setText(QApplication::translate("MainWindow", "foot 2", 0));
        conePerc2->setText(QApplication::translate("MainWindow", "%", 0));
        groupBox_7->setTitle(QApplication::translate("MainWindow", "Character Draw", 0));
        checkGRF->setText(QApplication::translate("MainWindow", "GRF", 0));
        checkShadow->setText(QApplication::translate("MainWindow", "Shadow", 0));
        checkShowChara->setText(QApplication::translate("MainWindow", "Chara", 0));
        checkWireChara->setText(QApplication::translate("MainWindow", " Wire", 0));
        Options->setTabText(Options->indexOf(tab), QApplication::translate("MainWindow", "General Controller", 0));
        btnStart->setText(QApplication::translate("MainWindow", "Start", 0));
        btnPause->setText(QApplication::translate("MainWindow", "Pause", 0));
        btnRestart->setText(QApplication::translate("MainWindow", "Restart", 0));
        iframe->setText(QApplication::translate("MainWindow", "frame", 0));
        label_23->setText(QApplication::translate("MainWindow", "/", 0));
        nframe->setText(QApplication::translate("MainWindow", "totalframe", 0));
        groupBox_11->setTitle(QApplication::translate("MainWindow", "Editing Frames", 0));
        label_34->setText(QApplication::translate("MainWindow", "Frame", 0));
        checkShowEditingFrame->setText(QApplication::translate("MainWindow", "Show Editing", 0));
        checkFrameFootLeft->setText(QApplication::translate("MainWindow", "Foot Left in Ground", 0));
        checkFrameFootRight->setText(QApplication::translate("MainWindow", "Foot Right in Ground", 0));
        saveEditingFrame->setText(QApplication::translate("MainWindow", "Save Editing Frames", 0));
        loadEditedFrames->setText(QApplication::translate("MainWindow", "Load Edited Frames", 0));
        label_22->setText(QApplication::translate("MainWindow", "Adjust Sensor Tollerance:", 0));
        groupBox_12->setTitle(QApplication::translate("MainWindow", "Frame Period", 0));
        mocapCyclic->setText(QApplication::translate("MainWindow", "Cyclic", 0));
        label_36->setText(QApplication::translate("MainWindow", "Begin", 0));
        label_37->setText(QApplication::translate("MainWindow", "End", 0));
        checkShowMocap->setText(QApplication::translate("MainWindow", "Show MoCap", 0));
        checkScreenShot->setText(QApplication::translate("MainWindow", "ScreenShots", 0));
        label_39->setText(QApplication::translate("MainWindow", "Steps Interpolation:", 0));
        Options->setTabText(Options->indexOf(tab_2), QApplication::translate("MainWindow", "Motion Config", 0));
        groupBox_9->setTitle(QApplication::translate("MainWindow", "Control PD Linear", 0));
        enable_cpdp->setText(QApplication::translate("MainWindow", "enabled", 0));
        show_effector->setText(QApplication::translate("MainWindow", "effector", 0));
        show_target->setText(QApplication::translate("MainWindow", "target", 0));
        label_31->setText(QApplication::translate("MainWindow", "Target", 0));
        label_32->setText(QApplication::translate("MainWindow", "ks", 0));
        label_33->setText(QApplication::translate("MainWindow", "kd", 0));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Properties Default", 0));
        label_9->setText(QApplication::translate("MainWindow", "Dim", 0));
        label_7->setText(QApplication::translate("MainWindow", "Pos", 0));
        label_10->setText(QApplication::translate("MainWindow", "Mass", 0));
        label_8->setText(QApplication::translate("MainWindow", "Euler", 0));
        isFoot->setText(QApplication::translate("MainWindow", "is foot", 0));
        isBodyBalance->setText(QApplication::translate("MainWindow", "is body balance", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "Actuators", 0));
        label_2->setText(QApplication::translate("MainWindow", "Force:", 0));
        applyForce->setText(QApplication::translate("MainWindow", "Apply", 0));
        groupBox_10->setTitle(QApplication::translate("MainWindow", "Coffee Cup", 0));
        hasCup->setText(QApplication::translate("MainWindow", "has Coffee Cup", 0));
        Options->setTabText(Options->indexOf(tabManipulatorSim), QApplication::translate("MainWindow", "Manipulators Objects", 0));
        groupBox_5->setTitle(QApplication::translate("MainWindow", "Control PD", 0));
        label_3->setText(QApplication::translate("MainWindow", "ks", 0));
        label_4->setText(QApplication::translate("MainWindow", "kd", 0));
        label_5->setText(QApplication::translate("MainWindow", "Angle", 0));
        enablepd->setText(QApplication::translate("MainWindow", "Enabled", 0));
        groupBox_6->setTitle(QApplication::translate("MainWindow", "Control PD Applied", 0));
        label_6->setText(QApplication::translate("MainWindow", "Ks", 0));
        xksPdApply->setText(QApplication::translate("MainWindow", "val1", 0));
        yksPdApply->setText(QApplication::translate("MainWindow", "val2", 0));
        zksPdApply->setText(QApplication::translate("MainWindow", "val3", 0));
        label_21->setText(QApplication::translate("MainWindow", "Kd", 0));
        xkdPdApply->setText(QApplication::translate("MainWindow", "val1", 0));
        ykdPdApply->setText(QApplication::translate("MainWindow", "val2", 0));
        zkdPdApply->setText(QApplication::translate("MainWindow", "val3", 0));
        Options->setTabText(Options->indexOf(controlpd), QApplication::translate("MainWindow", "Manipulator Joints", 0));
        label_25->setText(QApplication::translate("MainWindow", "Dis K:", 0));
        label_38->setText(QApplication::translate("MainWindow", "Vel K:", 0));
        label_40->setText(QApplication::translate("MainWindow", "Mass Balls:", 0));
        label_41->setText(QApplication::translate("MainWindow", "Velocity Balls:", 0));
        Options->setTabText(Options->indexOf(tab_3), QApplication::translate("MainWindow", "Robustez", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
        menuControl->setTitle(QApplication::translate("MainWindow", "Simulation", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
