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

#include <iostream>
#include <math.h>
#include <vector>

#include "vertex.h"
#include "brush.h"

class MainGLWidget : public QOpenGLWidget,
        protected QOpenGLFunctions
{
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
    void mouseMoveEvent (QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

protected slots:
    void teardownGL();

private:
    QOpenGLBuffer *vertexBuffer = 0;
    QOpenGLVertexArrayObject *arrayObject = 0;
    QOpenGLShaderProgram *program = 0;
    std::vector<Vertex> vertices;
    QOpenGLTexture *texture = 0;
    QImage *textureImage;
    Brush *brush;

    int matrixID;
    QMatrix4x4 rotationMatrix;
    QMatrix4x4 scaleMatrix;
    QMatrix4x4 viewMatrix;
    QMatrix4x4 projectionMatrix;
    QPoint previousMousePosition;
    bool mousePressed = false;
    bool isBrashActive = false;
    double scaleCoefficient = 0;

    void loadShaders(const char *vertexShaderName, const char *fragmentShaderName);
    void loadObj(const char *fileName);
    void loadTexture(const char *fileName);
    void updateTransformMatrix(QPoint mousePosition);
    void initializeObj();
    void initializeBrush();
    void setViewMatrixForObj();
    void setTexture();
};

#endif // MAINGLWIDGET_H
