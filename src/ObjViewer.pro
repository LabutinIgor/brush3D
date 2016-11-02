#-------------------------------------------------
#
# Project created by QtCreator 2016-10-24T18:42:48
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ObjViewer
TEMPLATE = app


SOURCES += main.cpp \
    mainwindow.cpp \
    mainglwidget.cpp \
    objloader.cpp

HEADERS += mainwindow.h \
    mainglwidget.h \
    vertex.h

FORMS   += mainwindow.ui

INCLUDEPATH += tinyobjloader/

RESOURCES += \
    shaders.qrc

OTHER_FILES += \
    vshader.glsl \
    fshader.glsl \
    cube.obj

DISTFILES += \
    cube.obj
