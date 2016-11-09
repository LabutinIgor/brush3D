#-------------------------------------------------
#
# Project created by QtCreator 2016-10-24T18:42:48
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ObjViewer
TEMPLATE = app


SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/mainglwidget.cpp \
    src/brush.cpp \
    src/geometry.cpp

HEADERS += src/mainwindow.h \
    src/mainglwidget.h \
    src/vertex.h \
    src/brush.h \
    src/geometry.h

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

RESOURCES += \
    resources.qrc

DISTFILES += \
    .gitignore

