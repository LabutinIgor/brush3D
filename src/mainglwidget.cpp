#include "mainglwidget.h"
#include <iomanip>

MainGLWidget::MainGLWidget(QWidget *parent) : QOpenGLWidget(parent) {
    controller = new Controller();
}

void MainGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    connect(context(), SIGNAL(aboutToBeDestroyed()), this, SLOT(teardownGL()), Qt::DirectConnection);
    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));

    glEnable(GL_CULL_FACE);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    loadShaders(":/resources/shaders/vshader.glsl", ":/resources/shaders/fshader.glsl");
    loadShadersForIds(":/resources/shaders/vshaderForIds.glsl", ":/resources/shaders/fshaderForIds.glsl");
    matrixID = program->uniformLocation("matrix");
    programForIdsMatrixID = programForIds->uniformLocation("matrix");

    controller->loadObj("/Users/igorl/Documents/au/project/qt_repo/objViewer/resources/cube.obj");
    vertices = controller->getVertices();
    initializeObj();
    controller->loadTextureImage("/Users/igorl/Documents/au/project/qt_repo/objViewer/resources/simple_texture.bmp");
    controller->initializeBrush();
    textureImage = controller->getTextureImageFromBrush();
    setTexture();

    setFocusPolicy(Qt::StrongFocus);
}

void MainGLWidget::resizeGL(int width, int height) {
    frameBuffer = new QOpenGLFramebufferObject(width, height);
    controller->updateSize(width, height);
}

void MainGLWidget::paintGL() {
    if (arrayObject != 0) {
        frameBuffer->bind();
        glClear(GL_COLOR_BUFFER_BIT);
        programForIds->bind();
        arrayObject->bind();
        programForIds->setUniformValue(programForIdsMatrixID, controller->getProjectionMatrix() * controller->getModelViewMatrix());
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        arrayObject->release();
        programForIds->release();

        QImage *idsBuffer = new QImage(frameBuffer->toImage());
        controller->setIdsBuffer(idsBuffer);

        frameBuffer->release();

        glClear(GL_COLOR_BUFFER_BIT);
        program->bind();
        arrayObject->bind();
        program->setUniformValue(matrixID, controller->getProjectionMatrix() * controller->getModelViewMatrix());
        if (texture != 0) {
            if (controller->getIsBrushUpdated()) {
                textureImage = controller->getTextureImageFromBrush();
                setTexture();
            }
            texture->bind();
        }

        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        arrayObject->release();
        if (texture != 0) {
            texture->release();
        }
        program->release();
    }
}

void MainGLWidget::mouseMoveEvent (QMouseEvent *event) {
    controller->mouseMoved(event->pos());
}

void MainGLWidget::mousePressEvent(QMouseEvent *event) {
    controller->mousePressed(event->pos());
}

void MainGLWidget::mouseReleaseEvent(QMouseEvent *event) {
    controller->mouseReleased(event->pos());
}

void MainGLWidget::keyPressEvent(QKeyEvent *event) {
    controller->keyPressed(event->key());
}

void MainGLWidget::keyReleaseEvent(QKeyEvent *event) {
    controller->keyReleased(event->key());
}

void MainGLWidget::wheelEvent(QWheelEvent* event) {
    controller->wheelScrolled(event->delta());
}

void MainGLWidget::teardownGL() {
    arrayObject->destroy();
    vertexBuffer->destroy();
    delete program;
}

void MainGLWidget::loadObjHandler() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
                                                    tr("Obj Files (*.obj)"));
    if (fileName != "") {
        controller->loadObj(fileName.toStdString().c_str());
        vertices = controller->getVertices();
        initializeObj();
        controller->initializeBrush();
    }
}

void MainGLWidget::loadTextureHandler() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
                                                    tr("Textures (*.bmp *.png *.jpg)"));
    if (fileName != "") {
        controller->loadTextureImage(fileName.toStdString().c_str());
        controller->initializeBrush();
        textureImage = controller->getTextureImageFromBrush();
        setTexture();
    }
}

void MainGLWidget::loadShaders(const char *vertexShaderName, const char *fragmentShaderName) {
    program = new QOpenGLShaderProgram();
    program->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderName);
    program->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderName);
    program->bindAttributeLocation("vertex", 0);
    program->bindAttributeLocation("texCoord", 1);
    program->link();
}

void MainGLWidget::loadShadersForIds(const char *vertexShaderName, const char *fragmentShaderName) {
    programForIds = new QOpenGLShaderProgram();
    programForIds->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderName);
    programForIds->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderName);
    programForIds->bindAttributeLocation("vertex", 0);
    programForIds->bindAttributeLocation("id", 2);
    programForIds->link();
}

void MainGLWidget::initializeObj() {
    if (vertexBuffer != 0) {
        vertexBuffer->destroy();
        arrayObject->destroy();
    }

    vertexBuffer = new QOpenGLBuffer();
    vertexBuffer->create();
    vertexBuffer->bind();
    vertexBuffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vertexBuffer->allocate(&vertices[0], sizeof(Vertex) * vertices.size());

    arrayObject = new QOpenGLVertexArrayObject();
    arrayObject->create();
    arrayObject->bind();
    program->enableAttributeArray(0);
    program->enableAttributeArray(1);
    program->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
    program->setAttributeBuffer(1, GL_FLOAT, Vertex::uvOffset(), Vertex::uvTupleSize, Vertex::stride());

    programForIds->enableAttributeArray(0);
    programForIds->enableAttributeArray(2);
    programForIds->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
    programForIds->setAttributeBuffer(2, GL_FLOAT, Vertex::idOffset(), Vertex::idTupleSize, Vertex::stride());
}

void MainGLWidget::setTexture() {
    if (texture != 0) {
        texture->destroy();
    }
    texture = new QOpenGLTexture(*textureImage);
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
}
