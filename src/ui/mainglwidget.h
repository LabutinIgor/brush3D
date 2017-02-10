#ifndef MAINGLWIDGET_H
#define MAINGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QQuaternion>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>
#include <QString>
#include <QFileDialog>
#include <QOpenGLFramebufferObject>
#include <cmath>
#include <vector>

#include "vertexforbuffer.h"
#include "src/controller.hpp"

class MainGLWidget : public QOpenGLWidget,
                     protected QOpenGLFunctions {
Q_OBJECT

public:
    MainGLWidget(QWidget *parent);

public slots:

    void loadObjHandler();

    void loadTextureHandler();

protected:
    void initializeGL() override;

    void resizeGL(int width, int height) override;

    void paintGL() override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void wheelEvent(QWheelEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

    void keyReleaseEvent(QKeyEvent *event) override;

protected slots:

    void teardownGL();

private:
    Controller *controller;
    QOpenGLBuffer *vertexBuffer = 0;
    QOpenGLVertexArrayObject *arrayObject = 0;
    QOpenGLShaderProgram *program = 0;
    QOpenGLFramebufferObject *frameBuffer = 0;
    QOpenGLShaderProgram *programForIds = 0;
    std::vector<VertexForBuffer> vertices;
    QOpenGLTexture *texture = 0;
    QImage *textureImage = 0;

    int matrixID;
    int programForIdsMatrixID;
    QMatrix4x4 projectionMatrix;

    void loadShaders(const char *vertexShaderName, const char *fragmentShaderName);

    void loadShadersForIds(const char *vertexShaderName, const char *fragmentShaderName);

    void initializeObj();

    void setTexture();
};

#endif // MAINGLWIDGET_H
