#include "mainglwidget.h"
#include <iostream>

MainGLWidget::MainGLWidget(QWidget *parent) : QOpenGLWidget(parent) {
}

void MainGLWidget::initializeGL() {
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

    GLuint vertexArrayID;
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    initShaderProgram(":/vshader.glsl", ":/fshader.glsl");

    static const GLfloat g_vertex_buffer_data[] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            0.0f,  1.0f, 0.0f,
    };

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), &g_vertex_buffer_data, GL_STATIC_DRAW);

    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

    viewMatrix.setToIdentity();
    viewMatrix.lookAt(
            QVector3D(0, 0, 10),
            QVector3D(0, 0, 0),
            QVector3D(0, 1, 0)
    );
    modelMatrix.setToIdentity();
}

void MainGLWidget::resizeGL(int width, int height) {
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(45.0f, width / float(height), 0.01f, 100.0f);
}

void MainGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);

    QMatrix4x4 matrixMVP = projectionMatrix * viewMatrix * modelMatrix;

    glUniformMatrix4fv(0, 1, GL_FALSE, matrixMVP.constData());

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

    glDrawArrays(GL_TRIANGLES, 0, (GLsizei) 3);

    glDisableVertexAttribArray(0);
}

void MainGLWidget::initShaderProgram(const QString &vertexShaderPath,
                                     const QString &fragmentShaderPath) {
    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderPath)) {
        std::cerr << "failed to add vertex shader\n";
        exit(-1);
    }

    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderPath)) {
        std::cerr <<"failed to add fragment shader\n";
        exit(-1);
    }

    if (!shaderProgram.link()) {
        std::cerr <<"failed to link shaders\n";
        exit(-1);
    }
}
