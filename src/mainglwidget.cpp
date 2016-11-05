#include "mainglwidget.h"

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include "tiny_obj_loader.h"

MainGLWidget::MainGLWidget(QWidget *parent) : QOpenGLWidget(parent) {
    m_rotationMatrix.setToIdentity();
    m_scaleMatrix.setToIdentity();
    m_viewMatrix.setToIdentity();
}

void MainGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    connect(context(), SIGNAL(aboutToBeDestroyed()), this, SLOT(teardownGL()), Qt::DirectConnection);
    connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));

    glEnable(GL_CULL_FACE);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    loadShaders(":/resources/shaders/vshader.glsl", ":/resources/shaders/fshader.glsl");
    u_matrix = m_program->uniformLocation("matrix");
}

void MainGLWidget::resizeGL(int width, int height) {
    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(45.0f, width / float(height), 0.0f, 1000.0f);
}

void MainGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (m_object != 0) {
        m_program->bind();
        m_object->bind();
        if (mousePressed) {
            updateTransformMatrix(mapFromGlobal(QCursor::pos()));
        }
        m_scaleMatrix.setToIdentity();
        m_scaleMatrix.scale(exp(scaleCoefficient));
        m_program->setUniformValue(u_matrix, m_projectionMatrix * m_viewMatrix * m_rotationMatrix * m_scaleMatrix);
        if (texture != 0) {
            texture->bind();
        }
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        m_object->release();
        if (texture != 0) {
            texture->release();
        }
        m_program->release();
    }
}

void MainGLWidget::mouseMoveEvent (QMouseEvent * event) {
    previousMousePosition = event->pos();
}

void MainGLWidget::mousePressEvent(QMouseEvent* event) {
    previousMousePosition = event->pos();
    mousePressed = true;
}

void MainGLWidget::mouseReleaseEvent(QMouseEvent* event) {
    previousMousePosition = event->pos();
    mousePressed = false;
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
    m_rotationMatrix = rot * m_rotationMatrix;
    previousMousePosition = mousePosition;
}

void MainGLWidget::teardownGL() {
    m_object->destroy();
    m_vertex->destroy();
    delete m_program;
}

void MainGLWidget::loadObjHandler() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
                                                    tr("Obj Files (*.obj)"));
    if (fileName != "") {
        loadObj(fileName.toStdString().c_str());
        initializeObj();
    }
}

void MainGLWidget::loadTextureHandler() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
                                                    tr("Textures (*.bmp *.png *.jpg)"));
    if (fileName != "") {
        loadTexture(fileName.toStdString().c_str());
    }
}

void MainGLWidget::loadShaders(const char *vertexShaderName, const char *fragmentShaderName) {
    m_program = new QOpenGLShaderProgram();
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderName);
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderName);
    m_program->bindAttributeLocation("vertex", 0);
    m_program->bindAttributeLocation("texCoord", 1);
    m_program->link();
    m_program->bind();
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
    if (m_vertex != 0) {
        m_vertex->destroy();
        m_object->destroy();
    }

    m_vertex = new QOpenGLBuffer();
    m_vertex->create();
    m_vertex->bind();
    m_vertex->setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertex->allocate(&vertices[0], sizeof(Vertex) * vertices.size());

    m_object = new QOpenGLVertexArrayObject();
    m_object->create();
    m_object->bind();
    m_program->enableAttributeArray(0);
    m_program->enableAttributeArray(1);
    m_program->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
    m_program->setAttributeBuffer(1, GL_FLOAT, Vertex::uvOffset(), Vertex::uvTupleSize, Vertex::stride());

    setViewMatrixForObj();
    scaleCoefficient = 0;
}

void MainGLWidget::setViewMatrixForObj() {
    double maxZ = 0;
    for (auto v : vertices) {
        QVector3D position = v.position();
        maxZ = fmax(maxZ, position.z());
    }
    m_viewMatrix.lookAt(
                QVector3D(0, 0, 20 * maxZ),
                QVector3D(0, 0, 0),
                QVector3D(0, 1, 0)
    );
}

void MainGLWidget::loadTexture(const char *fileName) {
    if (texture != 0) {
        texture->destroy();
    }
    QImage img(fileName);
    texture = new QOpenGLTexture(img.mirrored());
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
}
