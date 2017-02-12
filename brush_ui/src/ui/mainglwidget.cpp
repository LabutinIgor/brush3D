#include "mainglwidget.h"

MainGLWidget::MainGLWidget(QWidget* parent) : QOpenGLWidget(parent) {
    controller = new Controller();
}

void MainGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    connect(context(), SIGNAL(aboutToBeDestroyed()), this, SLOT(teardownGL()), Qt::DirectConnection);
    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    loadShaders("../resources/shaders/vshader.glsl",
                "../resources/shaders/fshader.glsl");
    loadShadersForIds("../resources/shaders/vshaderForIds.glsl",
                      "../resources/shaders/fshaderForIds.glsl");
    matrixID = program->uniformLocation("matrix");
    programForIdsMatrixID = programForIds->uniformLocation("matrix");

    controller->loadObj("/Users/igorl/Documents/au/project/qt_repo/objViewer/brush_ui/resources/objs/cube.obj");
    vertices = controller->getVertices();
    initializeObj();
    controller->loadTextureImage(
            "/Users/igorl/Documents/au/project/qt_repo/objViewer/brush_ui/resources/textures/simple_texture.bmp");
    controller->initializeBrush();
    textureImage = controller->getTextureFromBrush();
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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        programForIds->bind();
        arrayObject->bind();
        QMatrix4x4 transformMatrix = controller->getProjectionMatrix() * controller->getModelViewMatrix();
        programForIds->setUniformValue(programForIdsMatrixID, transformMatrix);

        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        arrayObject->release();
        programForIds->release();

        QImage* idsStorage = new QImage(frameBuffer->toImage());
        controller->setIdsStorage(idsStorage);

        frameBuffer->release();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        program->bind();
        arrayObject->bind();
        program->setUniformValue(matrixID, transformMatrix);
        if (texture != 0) {
            if (controller->getIsBrushUpdated()) {
                textureImage = controller->getTextureFromBrush();
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

void MainGLWidget::mouseMoveEvent(QMouseEvent* event) {
    controller->mouseMoved(event->pos());
}

void MainGLWidget::mousePressEvent(QMouseEvent* event) {
    controller->mousePressed(event->pos());
}

void MainGLWidget::mouseReleaseEvent(QMouseEvent* event) {
    controller->mouseReleased(event->pos());
}

void MainGLWidget::keyPressEvent(QKeyEvent* event) {
    controller->keyPressed(event->key());
}

void MainGLWidget::keyReleaseEvent(QKeyEvent* event) {
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
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Obj Files (*.obj)"));
    if (fileName != "") {
        controller->loadObj(fileName.toStdString().c_str());
        vertices = controller->getVertices();
        initializeObj();
        controller->initializeBrush();
    }
}

void MainGLWidget::loadTextureHandler() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Textures (*.bmp *.png *.jpg)"));
    if (fileName != "") {
        controller->loadTextureImage(fileName.toStdString().c_str());
        controller->initializeBrush();
        textureImage = controller->getTextureFromBrush();
        setTexture();
    }
}

void MainGLWidget::loadShaders(const char* vertexShaderName, const char* fragmentShaderName) {
    program = new QOpenGLShaderProgram();
    program->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderName);
    program->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderName);
    program->bindAttributeLocation("vertex", 0);
    program->bindAttributeLocation("texCoord", 1);
    program->link();
}

void MainGLWidget::loadShadersForIds(const char* vertexShaderName, const char* fragmentShaderName) {
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
    vertexBuffer->allocate(&vertices[0], sizeof(VertexForBuffer) * vertices.size());

    arrayObject = new QOpenGLVertexArrayObject();
    arrayObject->create();
    arrayObject->bind();
    program->enableAttributeArray(0);
    program->enableAttributeArray(1);
    program->setAttributeBuffer(0, GL_FLOAT, VertexForBuffer::positionOffset(), VertexForBuffer::PositionTupleSize,
                                VertexForBuffer::stride());
    program->setAttributeBuffer(1, GL_FLOAT, VertexForBuffer::uvOffset(), VertexForBuffer::uvTupleSize,
                                VertexForBuffer::stride());

    programForIds->enableAttributeArray(0);
    programForIds->enableAttributeArray(2);
    programForIds->setAttributeBuffer(0, GL_FLOAT, VertexForBuffer::positionOffset(),
                                      VertexForBuffer::PositionTupleSize, VertexForBuffer::stride());
    programForIds->setAttributeBuffer(2, GL_FLOAT, VertexForBuffer::idOffset(), VertexForBuffer::idTupleSize,
                                      VertexForBuffer::stride());
}

void MainGLWidget::setTexture() {
    if (texture != 0) {
        texture->destroy();
    }
    texture = new QOpenGLTexture(*textureImage);
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
}
