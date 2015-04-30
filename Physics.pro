#-------------------------------------------------
#
# Project created by QtCreator 2013-02-22T22:02:09
#
#-------------------------------------------------



QT       += core gui opengl widgets

TARGET = ODESys
TEMPLATE = app
DEFINES += \
           SHADERS_ENABLED \
           GL_GLEXT_PROTOTYPES \

SOURCES += main.cpp\
        mainwindow.cpp \
    math/vec4.cpp \
    graphics/draw.cpp \
    interface/glwidget.cpp \
    physics/physics.cpp \
    scene/scene.cpp \
    scene/object.cpp \
    scene/character.cpp \
    scene/joint.cpp \
    graphics/mesh.cpp \
    extra/material.cpp \
    math/matrix4x4.cpp \
    math/quaternion.cpp \
    graphics/plane.cpp \
    control/control.cpp \
    physics/functions.cpp \
    extra/utils.cpp \
    math/matrix.cpp \
    control/balance.cpp \
    math/vecn.cpp \
    scene/grf.cpp \
    mocap/frame.cpp \
    mocap/mocap.cpp \
    control/sensor.cpp \
    mocap/framequat.cpp \
    interface/camera.cpp \
    extra/ObjMesh.cpp \
    interpolation/interpolation.cpp \
    graphics/shadows.cpp \
    graphics/imageloader.cpp \
    extra/screenshot.cpp \
    extra/text.cpp \
    math/vector3d.cpp \
    extra/tga.c \
    extra/texture.c \
    math/ray.cpp

HEADERS  += mainwindow.h \
    math/vec4.h \
    graphics/draw.h \
    interface/glwidget.h \
    physics/physics.h \
    scene/scene.h \
    math/quaternion.h \
    math/matrix4x4.h \
    scene/object.h \
    scene/character.h \
    scene/joint.h \
    extra/material.h \
    graphics/plane.h \
    control/control.h \
    physics/functions.h \
    extra/utils.h \
    math/matrix.h \
    control/balance.h \
    math/vecn.h \
    scene/grf.h \
    mocap/frame.h \
    mocap/mocap.h \
    control/sensor.h \
    mocap/framequat.h \
    interface/camera.h \
    extra/ObjMesh.h \
    interpolation/interpolation.h \
    graphics/shadows.h \
    graphics/imageloader.h \
    extra/screenshot.h \
    extra/text.h \
    math/vector3d.h \
    extra/tga.h \
    extra/texture.h \
    math/ray.h

FORMS    += mainwindow.ui

LIBS += -lode -lGLU -lglut -lGL -lgsl -lgslcblas -fopenmp -lGLEW

CONFIG += release

QT += xml

QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS +=  -fopenmp
