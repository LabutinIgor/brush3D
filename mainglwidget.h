#ifndef MAINGLWIDGET_H
#define MAINGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QtOpenGL>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>

#include <iostream>

class MainGLWidget : public QOpenGLWidget
{
public:
    MainGLWidget(QWidget *parent);

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

private:
    void initShaderProgram( const QString& vertexShaderPath,
                               const QString& fragmentShaderPath );

    QOpenGLShaderProgram shaderProgram;
    QMatrix4x4 projectionMatrix, viewMatrix, modelMatrix;
    GLuint vertexBuffer;

};

#endif // MAINGLWIDGET_H
