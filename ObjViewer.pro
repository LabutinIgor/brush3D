#-------------------------------------------------
#
# Project created by QtCreator 2016-10-24T18:42:48
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ObjViewer
TEMPLATE = app

#LIBS += external/tinyobjloader/tiny_obj_loader.h


SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/mainglwidget.cpp \
    src/brush.cpp \
    src/geometry.cpp \
    src/controller.cpp \
    src/abstractbrush.cpp \
    src/slowraysbrush.cpp \
    src/pixelspaintingbrush.cpp \
    src/brushstroke.cpp \
    src/brushhistory.cpp \
    src/pixelsfastbrush.cpp

HEADERS += src/mainwindow.h \
    src/mainglwidget.h \
    src/vertex.h \
    src/brush.h \
    src/geometry.h \
    src/controller.h \
    src/abstractbrush.h \
    src/slowraysbrush.h \
    src/pixelspaintingbrush.h \
    src/brushstroke.h \
    src/brushhistory.h \
    src/pixelsfastbrush.h

FORMS   += src/mainwindow.ui

INCLUDEPATH += \
    src/ \
    external/tinyobjloader/

OTHER_FILES += \
    external/tinyobjloader/tiny_obj_loader.h \
    resources/cube.obj \
    resources/simple_texture.bmp \
    resources/shaders/vshader.glsl \
    resources/shaders/fshader.glsl \
    resources/shaders/vshaderForIds.glsl \
    resources/shaders/fshaderForIds.glsl \

RESOURCES += \
    resources.qrc

DISTFILES += \
    .gitignore \
    fshaderForIds.glsl

