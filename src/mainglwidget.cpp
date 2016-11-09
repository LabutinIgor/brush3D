#include "mainglwidget.h"

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include "tiny_obj_loader.h"

MainGLWidget::MainGLWidget(QWidget *parent) : QOpenGLWidget(parent) {
    rotationMatrix.setToIdentity();
    scaleMatrix.setToIdentity();
    viewMatrix.setToIdentity();
}

void MainGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    connect(context(), SIGNAL(aboutToBeDestroyed()), this, SLOT(teardownGL()), Qt::DirectConnection);
    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));

    glEnable(GL_CULL_FACE);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    loadShaders(":/resources/shaders/vshader.glsl", ":/resources/shaders/fshader.glsl");
    matrixID = program->uniformLocation("matrix");

    loadObj("/Users/igorl/Documents/au/project/qt_repo/objViewer/resources/cube.obj");
    initializeObj();
    loadTexture("/Users/igorl/Documents/au/project/qt_repo/objViewer/resources/simple_texture.bmp");

    initializeBrush();

    setFocusPolicy(Qt::StrongFocus);
}

void MainGLWidget::resizeGL(int width, int height) {
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(45.0f, width / float(height), 0.0f, 1000.0f);
    //std::cerr << projectionMatrix.column(i << "\n";
}

void MainGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (arrayObject != 0) {
        program->bind();
        arrayObject->bind();
        if (mousePressed && !isBrashActive) {
            updateTransformMatrix(mapFromGlobal(QCursor::pos()));
        }
        scaleMatrix.setToIdentity();
        scaleMatrix.scale(exp(scaleCoefficient));
        program->setUniformValue(matrixID, projectionMatrix * viewMatrix * rotationMatrix * scaleMatrix);
        if (texture != 0) {
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
    previousMousePosition = event->pos();
    if (isBrashActive) {
        brush->paint(QVector2D(event->pos().x() * 2.0 / width() - 1,
                               (height() - event->pos().y()) * 2.0 / height() - 1),
                     viewMatrix * rotationMatrix * scaleMatrix, projectionMatrix);
        textureImage = brush->getTextureImage();
        setTexture();
    }
}

void MainGLWidget::mousePressEvent(QMouseEvent *event) {
    previousMousePosition = event->pos();
    mousePressed = true;
}

void MainGLWidget::mouseReleaseEvent(QMouseEvent *event) {
    previousMousePosition = event->pos();
    mousePressed = false;
}

void MainGLWidget::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Control) {
        isBrashActive = true;
    }
}

void MainGLWidget::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Control) {
        isBrashActive = false;
    }
}

void MainGLWidget::wheelEvent(QWheelEvent* event) {
    scaleCoefficient += event->delta() * 0.001;
}

void MainGLWidget::updateTransformMatrix(QPoint mousePosition) {
    double dx = previousMousePosition.x() - mousePosition.x();
    double dy = previousMousePosition.y() - mousePosition.y();

    QMatrix4x4 rot;
    rot.setToIdentity();
    rot.rotate(dx * 0.1f, 0, -1, 0);
    rot.rotate(dy * 0.1f, -1, 0, 0);
    rotationMatrix = rot * rotationMatrix;
    previousMousePosition = mousePosition;
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
        loadObj(fileName.toStdString().c_str());
        initializeObj();
        initializeBrush();
    }
}

void MainGLWidget::loadTextureHandler() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
                                                    tr("Textures (*.bmp *.png *.jpg)"));
    if (fileName != "") {
        loadTexture(fileName.toStdString().c_str());
        initializeBrush();
    }
}

void MainGLWidget::loadShaders(const char *vertexShaderName, const char *fragmentShaderName) {
    program = new QOpenGLShaderProgram();
    program->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderName);
    program->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderName);
    program->bindAttributeLocation("vertex", 0);
    program->bindAttributeLocation("texCoord", 1);
    program->link();
    program->bind();
}

void MainGLWidget::loadObj(const char *fileName) {
    vertices.clear();
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, fileName);
    if (!err.empty()) {
        std::cerr << err << std::endl;
    }
    if (!ret) {
        exit(1);
    }

    for (size_t s = 0; s < shapes.size(); s++) {
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            int fv = shapes[s].mesh.num_face_vertices[f];
            for (size_t v = 0; v < (size_t) fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                float vx = attrib.vertices[3 * idx.vertex_index + 0];
                float vy = attrib.vertices[3 * idx.vertex_index + 1];
                float vz = attrib.vertices[3 * idx.vertex_index + 2];
                //float nx = attrib.normals[3 * idx.normal_index + 0];
                //float ny = attrib.normals[3 * idx.normal_index + 1];
                //float nz = attrib.normals[3 * idx.normal_index + 2];
                float tx = attrib.texcoords[2 * idx.texcoord_index + 0];
                float ty = attrib.texcoords[2 * idx.texcoord_index + 1];
                vertices.push_back(Vertex(QVector3D(vx, vy, vz), QVector2D(tx, ty)));
            }
            index_offset += fv;
            shapes[s].mesh.material_ids[f];
        }
    }
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

    setViewMatrixForObj();
    scaleCoefficient = 0;
}

void MainGLWidget::initializeBrush() {
    brush = new Brush(vertices, textureImage);
    brush->paintExample();
    textureImage = brush->getTextureImage();
    setTexture();
}

void MainGLWidget::setViewMatrixForObj() {
    double maxZ = 1;
    for (auto v : vertices) {
        QVector3D position = v.position();
        maxZ = fmax(maxZ, position.z());
    }
    viewMatrix.lookAt(
                QVector3D(0, 0, 10 * maxZ),
                QVector3D(0, 0, 0),
                QVector3D(0, 1, 0)
    );
}

void MainGLWidget::loadTexture(const char *fileName) {
    if (texture != 0) {
        texture->destroy();
    }
    QImage image(fileName);
    textureImage = new QImage(image.mirrored());
    setTexture();
}

void MainGLWidget::setTexture() {
    texture = new QOpenGLTexture(*textureImage);
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
}
