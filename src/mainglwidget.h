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
#include "vertex.h"
#include <QDebug>
#include <QString>
#include <QFileDialog>
#include <iostream>
#include <math.h>
#include <vector>

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
    void mouseMoveEvent (QMouseEvent * event) override;
    void mousePressEvent(QMouseEvent * event) override;
    void mouseReleaseEvent(QMouseEvent * event) override;
    void wheelEvent(QWheelEvent * event) override;

protected slots:
    void teardownGL();

private:
    QOpenGLBuffer *m_vertex = 0;
    QOpenGLVertexArrayObject *m_object = 0;
    QOpenGLShaderProgram *m_program = 0;
    std::vector<Vertex> vertices;
    QOpenGLTexture *texture = 0;

    int u_matrix;
    int u_texture;
    QMatrix4x4 m_rotationMatrix;
    QMatrix4x4 m_scaleMatrix;
    QMatrix4x4 m_viewMatrix;
    QMatrix4x4 m_projectionMatrix;
    QPoint previousMousePosition;
    bool mousePressed = false;
    double scaleCoefficient = 0;

    void loadShaders(const char* vertexShaderName, const char* fragmentShaderName);
    void loadObj(const char* fileName);
    void loadTexture(const char* fileName);
    void updateTransformMatrix(QPoint mousePosition);
    void initializeObj();
    void setViewMatrixForObj();
};

#endif // MAINGLWIDGET_H
