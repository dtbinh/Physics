#-------------------------------------------------
#
# Project created by QtCreator 2013-02-22T22:02:09
#
#-------------------------------------------------



QT       += core gui opengl widgets openglextensions

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
    math/matrix4x4.cpp \
    math/quaternion.cpp \
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
    extra/ObjMesh.cpp \
    interpolation/interpolation.cpp \
    graphics/shadows.cpp \
    graphics/imageloader.cpp \
    extra/screenshot.cpp \
    extra/text.cpp \
    math/vector3d.cpp \
    extra/tga.c \
    math/ray.cpp \
    control/pose.cpp \
    control/graphicalpose.cpp \
    graphics/shader.cpp \
    graphics/ShaderPrimitives/cube.cpp \
    graphics/ShaderPrimitives/kdgeometryshader.cpp \
    graphics/ShaderPrimitives/texture.cpp \
    graphics/ShaderPrimitives/sampler.cpp \
    graphics/ShaderPrimitives/abstractscene.cpp \
    graphics/ShaderPrimitives/material.cpp \
    graphics/ShaderPrimitives/camera.cpp \
    extra/materialobj.cpp \
    interface/cameraold.cpp \
    graphics/ShaderPrimitives/sphere.cpp \
    graphics/ShaderPrimitives/cylinder.cpp \
    graphics/ShaderPrimitives/plane.cpp \
    graphics/ShaderPrimitives/objloader.cpp \
    graphics/ShaderPrimitives/mesh.cpp \
    graphics/ShaderPrimitives/axisalignedboundingbox.cpp \
    graphics/planeobj.cpp \
    graphics/meshobj.cpp

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
    extra/ObjMesh.h \
    interpolation/interpolation.h \
    graphics/shadows.h \
    graphics/imageloader.h \
    extra/screenshot.h \
    extra/text.h \
    math/vector3d.h \
    extra/tga.h \
    extra/texture.h \
    math/ray.h \
    control/pose.h \
    control/graphicalpose.h \
    graphics/shader.h \
    graphics/ShaderPrimitives/cube.h \
    graphics/ShaderPrimitives/kdgeometryshader.h \
    graphics/ShaderPrimitives/sampler.h \
    graphics/ShaderPrimitives/texture.h \
    graphics/ShaderPrimitives/abstractscene.h \
    graphics/ShaderPrimitives/material.h \
    graphics/ShaderPrimitives/camera.h \
    graphics/ShaderPrimitives/camera_p.h \
    extra/materialobj.h \
    interface/cameraold.h \
    graphics/ShaderPrimitives/sphere.h \
    graphics/ShaderPrimitives/cylinder.h \
    graphics/ShaderPrimitives/plane.h \
    graphics/ShaderPrimitives/objloader.h \
    graphics/ShaderPrimitives/mesh.h \
    graphics/ShaderPrimitives/axisalignedboundingbox.h \
    graphics/planeobj.h \
    graphics/meshobj.h

FORMS    += mainwindow.ui

LIBS += -lode -lGLU -lglut -lGL -lgsl -lgslcblas -fopenmp -lGLEW

CONFIG += release

QT += xml

QMAKE_CXXFLAGS += -fopenmp -ggdb
QMAKE_LFLAGS +=  -fopenmp -ggdb
