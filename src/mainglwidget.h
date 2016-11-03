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
#include "vertex.h"
#include "transform3d.h"

#include <iostream>
#include <vector>

class MainGLWidget : public QOpenGLWidget,
        protected QOpenGLFunctions
{
    Q_OBJECT

public:
    MainGLWidget(QWidget *parent);

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;
    void mouseMoveEvent (QMouseEvent * event) override;
    void mousePressEvent(QMouseEvent * event) override;
    void mouseReleaseEvent(QMouseEvent * event) override;

protected slots:
    void teardownGL();

private:
    QOpenGLBuffer m_vertex;
    QOpenGLVertexArrayObject m_object;
    QOpenGLShaderProgram *m_program;
    std::vector<Vertex> vertices;
    QOpenGLTexture *texture;

    int u_matrix;
    int u_texture;
    QMatrix4x4 m_modelMatrix;
    QMatrix4x4 m_viewMatrix;
    QMatrix4x4 m_projectionMatrix;
    QPoint previousMousePosition;
    bool mousePressed = false;

    void loadShaders(const char* vertexShaderName, const char* fragmentShaderName);
    void loadObj(const char* fileName);
    void loadTexture(const char* fileName);
    void updateTransformMatrix(QPoint mousePosition);

    void printVersionInformation();
};

#endif // MAINGLWIDGET_H
