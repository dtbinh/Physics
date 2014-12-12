/****************************************************************************
** Meta object code from reading C++ file 'glwidget.h'
**
** Created: Tue Dec 9 14:59:45 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../interface/glwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'glwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GLWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      76,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      19,       // signalCount

 // signals: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x05,
      46,    9,    9,    9, 0x05,
      80,    9,    9,    9, 0x05,
      99,    9,    9,    9, 0x05,
     124,  118,    9,    9, 0x05,
     175,  171,    9,    9, 0x05,
     218,    9,    9,    9, 0x05,
     236,    9,    9,    9, 0x05,
     260,    9,    9,    9, 0x05,
     282,    9,    9,    9, 0x05,
     302,    9,    9,    9, 0x05,
     322,    9,    9,    9, 0x05,
     341,    9,    9,    9, 0x05,
     361,    9,    9,    9, 0x05,
     381,    9,    9,    9, 0x05,
     407,    9,    9,    9, 0x05,
     433,  431,    9,    9, 0x05,
     450,    9,    9,    9, 0x05,
     473,    9,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
     498,  496,    9,    9, 0x0a,
     518,  496,    9,    9, 0x0a,
     538,  496,    9,    9, 0x0a,
     562,  557,    9,    9, 0x0a,
     589,  557,    9,    9, 0x0a,
     615,  557,    9,    9, 0x0a,
     645,  641,    9,    9, 0x0a,
     663,    9,    9,    9, 0x0a,
     679,  496,    9,    9, 0x0a,
     708,  702,    9,    9, 0x0a,
     729,  431,    9,    9, 0x0a,
     749,  431,    9,    9, 0x0a,
     767,  641,    9,    9, 0x0a,
     791,  557,    9,    9, 0x0a,
     825,  557,    9,    9, 0x0a,
     865,  859,    9,    9, 0x0a,
     889,    9,    9,    9, 0x0a,
     906,    9,    9,    9, 0x0a,
     930,  924,    9,    9, 0x0a,
     949,  947,    9,    9, 0x0a,
     976,  947,    9,    9, 0x0a,
    1009,  947,    9,    9, 0x0a,
    1045,  496,    9,    9, 0x0a,
    1065, 1062,    9,    9, 0x0a,
    1092, 1089,    9,    9, 0x0a,
    1148, 1116,    9,    9, 0x0a,
    1197,  496,    9,    9, 0x0a,
    1226,  496,    9,    9, 0x0a,
    1254,  641,    9,    9, 0x0a,
    1282,  431,    9,    9, 0x0a,
    1308,  496,    9,    9, 0x0a,
    1339,  496,    9,    9, 0x0a,
    1365,  641,    9,    9, 0x0a,
    1383,  641,    9,    9, 0x0a,
    1400,  641,    9,    9, 0x0a,
    1422,  641,    9,    9, 0x0a,
    1444,  641,    9,    9, 0x0a,
    1467, 1465,    9,    9, 0x0a,
    1496, 1465,    9,    9, 0x0a,
    1546,    9, 1525,    9, 0x0a,
    1583,    9, 1563,    9, 0x0a,
    1603, 1599,    9,    9, 0x0a,
    1634, 1599, 1626,    9, 0x0a,
    1649,  496,    9,    9, 0x0a,
    1669, 1599,    9,    9, 0x0a,
    1691,  557,    9,    9, 0x0a,
    1710,  557,    9,    9, 0x0a,
    1738, 1733,    9,    9, 0x0a,
    1761,  496,    9,    9, 0x0a,
    1781,  496,    9,    9, 0x0a,
    1798,  496,    9,    9, 0x0a,
    1817,    9,    9,    9, 0x0a,
    1829,    9,    9,    9, 0x0a,
    1852, 1846,    9,    9, 0x0a,
    1875,    9,    9,    9, 0x0a,
    1897,    9,    9,    9, 0x0a,
    1917,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_GLWidget[] = {
    "GLWidget\0\0updateObjects(std::vector<Object*>)\0"
    "updateJoints(std::vector<Joint*>)\0"
    "updateKsProp(Vec4)\0updateKdProp(Vec4)\0"
    ",,,,,\0updateBalancePD(Vec4,Vec4,Vec4,Vec4,Vec4,Vec4)\0"
    ",,,\0updateBalanceCone(float,float,float,float)\0"
    "showJoint(Joint*)\0motionCurrentFrame(int)\0"
    "motionTotalFrame(int)\0setSliderFoot1(int)\0"
    "setSliderFoot2(int)\0plusFrameEdition()\0"
    "minusFrameEdition()\0setForceCharacter()\0"
    "setBeginClycleWidget(int)\0"
    "setEndClycleWidget(int)\0v\0setGravity(Vec4)\0"
    "setEnableGravity(bool)\0setAngleDirection(int)\0"
    "b\0setScreenShot(bool)\0setRenderMesh(bool)\0"
    "setShowInfos(bool)\0file\0"
    "loadMotionCapture(QString)\0"
    "loadFramesConfig(QString)\0"
    "saveFramesConfig(QString)\0val\0"
    "setPlayback(bool)\0restartMotion()\0"
    "showEditingFrame(bool)\0frame\0"
    "setEditingFrame(int)\0setBeginClycle(int)\0"
    "setEndClycle(int)\0setToleranceCOM(double)\0"
    "saveSimulationParameters(QString)\0"
    "loadSimulationParameters(QString)\0"
    "steps\0SimStepsSimulation(int)\0"
    "stopSimulation()\0startSimulation()\0"
    "force\0applyForce(Vec4)\0g\0"
    "setGravityParameters(Vec4)\0"
    "setSimbiconForceParameters(Vec4)\0"
    "setSimbiconDistanceParameters(Vec4)\0"
    "setGravity(bool)\0ks\0setProportionalKs(Vec4)\0"
    "kd\0setProportionalKd(Vec4)\0"
    "ksT,kdT,ksF,kdF,kmomlin,kmomang\0"
    "setBalanceControl(Vec4,Vec4,Vec4,Vec4,Vec4,Vec4)\0"
    "setEnableTorqueBalance(bool)\0"
    "setEnableForceBalance(bool)\0"
    "setCompensationBalance(int)\0"
    "setAngleBodyBalance(Vec4)\0"
    "setEnableMomentumBalance(bool)\0"
    "setKsRelationshipKs(bool)\0setLimitCone(int)\0"
    "setMCone(double)\0setRadiusCone(double)\0"
    "setHeightCone(double)\0setAngleCone(double)\0"
    "k\0setKVelocityLocomotion(Vec4)\0"
    "setKDistanceLocomotion(Vec4)\0"
    "std::vector<Object*>\0getObjectsList()\0"
    "std::vector<Joint*>\0getJointsList()\0"
    "row\0setObjectSelected(int)\0Object*\0"
    "getObject(int)\0showCharacter(bool)\0"
    "setJointSelected(int)\0loadScene(QString)\0"
    "saveCharacter(QString)\0wire\0"
    "setWireCharacter(bool)\0setShowShadow(bool)\0"
    "setShowGRF(bool)\0setShowMoCap(bool)\0"
    "drawScene()\0drawParameters()\0value\0"
    "setAlphaCharacter(int)\0simulationPlayPause()\0"
    "simulationRestart()\0simStep()\0"
};

void GLWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GLWidget *_t = static_cast<GLWidget *>(_o);
        switch (_id) {
        case 0: _t->updateObjects((*reinterpret_cast< std::vector<Object*>(*)>(_a[1]))); break;
        case 1: _t->updateJoints((*reinterpret_cast< std::vector<Joint*>(*)>(_a[1]))); break;
        case 2: _t->updateKsProp((*reinterpret_cast< Vec4(*)>(_a[1]))); break;
        case 3: _t->updateKdProp((*reinterpret_cast< Vec4(*)>(_a[1]))); break;
        case 4: _t->updateBalancePD((*reinterpret_cast< Vec4(*)>(_a[1])),(*reinterpret_cast< Vec4(*)>(_a[2])),(*reinterpret_cast< Vec4(*)>(_a[3])),(*reinterpret_cast< Vec4(*)>(_a[4])),(*reinterpret_cast< Vec4(*)>(_a[5])),(*reinterpret_cast< Vec4(*)>(_a[6]))); break;
        case 5: _t->updateBalanceCone((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3])),(*reinterpret_cast< float(*)>(_a[4]))); break;
        case 6: _t->showJoint((*reinterpret_cast< Joint*(*)>(_a[1]))); break;
        case 7: _t->motionCurrentFrame((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->motionTotalFrame((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->setSliderFoot1((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->setSliderFoot2((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->plusFrameEdition(); break;
        case 12: _t->minusFrameEdition(); break;
        case 13: _t->setForceCharacter(); break;
        case 14: _t->setBeginClycleWidget((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->setEndClycleWidget((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->setGravity((*reinterpret_cast< Vec4(*)>(_a[1]))); break;
        case 17: _t->setEnableGravity((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 18: _t->setAngleDirection((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->setScreenShot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 20: _t->setRenderMesh((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 21: _t->setShowInfos((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 22: _t->loadMotionCapture((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 23: _t->loadFramesConfig((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 24: _t->saveFramesConfig((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 25: _t->setPlayback((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 26: _t->restartMotion(); break;
        case 27: _t->showEditingFrame((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 28: _t->setEditingFrame((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 29: _t->setBeginClycle((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 30: _t->setEndClycle((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 31: _t->setToleranceCOM((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 32: _t->saveSimulationParameters((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 33: _t->loadSimulationParameters((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 34: _t->SimStepsSimulation((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 35: _t->stopSimulation(); break;
        case 36: _t->startSimulation(); break;
        case 37: _t->applyForce((*reinterpret_cast< Vec4(*)>(_a[1]))); break;
        case 38: _t->setGravityParameters((*reinterpret_cast< Vec4(*)>(_a[1]))); break;
        case 39: _t->setSimbiconForceParameters((*reinterpret_cast< Vec4(*)>(_a[1]))); break;
        case 40: _t->setSimbiconDistanceParameters((*reinterpret_cast< Vec4(*)>(_a[1]))); break;
        case 41: _t->setGravity((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 42: _t->setProportionalKs((*reinterpret_cast< Vec4(*)>(_a[1]))); break;
        case 43: _t->setProportionalKd((*reinterpret_cast< Vec4(*)>(_a[1]))); break;
        case 44: _t->setBalanceControl((*reinterpret_cast< Vec4(*)>(_a[1])),(*reinterpret_cast< Vec4(*)>(_a[2])),(*reinterpret_cast< Vec4(*)>(_a[3])),(*reinterpret_cast< Vec4(*)>(_a[4])),(*reinterpret_cast< Vec4(*)>(_a[5])),(*reinterpret_cast< Vec4(*)>(_a[6]))); break;
        case 45: _t->setEnableTorqueBalance((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 46: _t->setEnableForceBalance((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 47: _t->setCompensationBalance((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 48: _t->setAngleBodyBalance((*reinterpret_cast< Vec4(*)>(_a[1]))); break;
        case 49: _t->setEnableMomentumBalance((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 50: _t->setKsRelationshipKs((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 51: _t->setLimitCone((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 52: _t->setMCone((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 53: _t->setRadiusCone((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 54: _t->setHeightCone((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 55: _t->setAngleCone((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 56: _t->setKVelocityLocomotion((*reinterpret_cast< Vec4(*)>(_a[1]))); break;
        case 57: _t->setKDistanceLocomotion((*reinterpret_cast< Vec4(*)>(_a[1]))); break;
        case 58: { std::vector<Object*> _r = _t->getObjectsList();
            if (_a[0]) *reinterpret_cast< std::vector<Object*>*>(_a[0]) = _r; }  break;
        case 59: { std::vector<Joint*> _r = _t->getJointsList();
            if (_a[0]) *reinterpret_cast< std::vector<Joint*>*>(_a[0]) = _r; }  break;
        case 60: _t->setObjectSelected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 61: { Object* _r = _t->getObject((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< Object**>(_a[0]) = _r; }  break;
        case 62: _t->showCharacter((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 63: _t->setJointSelected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 64: _t->loadScene((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 65: _t->saveCharacter((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 66: _t->setWireCharacter((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 67: _t->setShowShadow((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 68: _t->setShowGRF((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 69: _t->setShowMoCap((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 70: _t->drawScene(); break;
        case 71: _t->drawParameters(); break;
        case 72: _t->setAlphaCharacter((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 73: _t->simulationPlayPause(); break;
        case 74: _t->simulationRestart(); break;
        case 75: _t->simStep(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData GLWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GLWidget::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_GLWidget,
      qt_meta_data_GLWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GLWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GLWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GLWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GLWidget))
        return static_cast<void*>(const_cast< GLWidget*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int GLWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 76)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 76;
    }
    return _id;
}

// SIGNAL 0
void GLWidget::updateObjects(std::vector<Object*> _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GLWidget::updateJoints(std::vector<Joint*> _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void GLWidget::updateKsProp(Vec4 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void GLWidget::updateKdProp(Vec4 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void GLWidget::updateBalancePD(Vec4 _t1, Vec4 _t2, Vec4 _t3, Vec4 _t4, Vec4 _t5, Vec4 _t6)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void GLWidget::updateBalanceCone(float _t1, float _t2, float _t3, float _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void GLWidget::showJoint(Joint * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void GLWidget::motionCurrentFrame(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void GLWidget::motionTotalFrame(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void GLWidget::setSliderFoot1(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void GLWidget::setSliderFoot2(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void GLWidget::plusFrameEdition()
{
    QMetaObject::activate(this, &staticMetaObject, 11, 0);
}

// SIGNAL 12
void GLWidget::minusFrameEdition()
{
    QMetaObject::activate(this, &staticMetaObject, 12, 0);
}

// SIGNAL 13
void GLWidget::setForceCharacter()
{
    QMetaObject::activate(this, &staticMetaObject, 13, 0);
}

// SIGNAL 14
void GLWidget::setBeginClycleWidget(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void GLWidget::setEndClycleWidget(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void GLWidget::setGravity(Vec4 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void GLWidget::setEnableGravity(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}

// SIGNAL 18
void GLWidget::setAngleDirection(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 18, _a);
}
QT_END_MOC_NAMESPACE
