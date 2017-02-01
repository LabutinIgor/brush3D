#include "controller.h"

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include "tiny_obj_loader.h"

Controller::Controller() {
    rotationMatrix.setToIdentity();
    scaleMatrix.setToIdentity();
    viewMatrix.setToIdentity();
}

void Controller::loadObj(const char *fileName) {
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
    int triangleId = 0;

    for (size_t s = 0; s < shapes.size(); s++) {
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            int fv = shapes[s].mesh.num_face_vertices[f];
            tinyobj::index_t idx = shapes[s].mesh.indices[index_offset];
            Vertex firstVertex = Vertex(glm::vec3(attrib.vertices[3 * idx.vertex_index + 0],
                                                  attrib.vertices[3 * idx.vertex_index + 1],
                                                  attrib.vertices[3 * idx.vertex_index + 2]),
                                        glm::vec2(attrib.texcoords[2 * idx.texcoord_index + 0],
                                                  attrib.texcoords[2 * idx.texcoord_index + 1]),
                                        glm::vec3(0, 0, 0));
            for (size_t v = 1; v < (size_t) fv - 1; v++) {
                tinyobj::index_t idxCurrent = shapes[s].mesh.indices[index_offset + v];
                tinyobj::index_t idxNext = shapes[s].mesh.indices[index_offset + v + 1];
                firstVertex.setId(glm::vec3(triangleId % 256, (triangleId / 256) % 256, (triangleId / 256 / 256) % 256));
                vertices.push_back(firstVertex);
                vertices.push_back(Vertex(glm::vec3(attrib.vertices[3 * idxCurrent.vertex_index + 0],
                                                    attrib.vertices[3 * idxCurrent.vertex_index + 1],
                                                    attrib.vertices[3 * idxCurrent.vertex_index + 2]),
                                          glm::vec2(attrib.texcoords[2 * idxCurrent.texcoord_index + 0],
                                                    attrib.texcoords[2 * idxCurrent.texcoord_index + 1]),
                                          glm::vec3(triangleId % 256, (triangleId / 256) % 256, (triangleId / 256 / 256) % 256)));

                vertices.push_back(Vertex(glm::vec3(attrib.vertices[3 * idxNext.vertex_index + 0],
                                                    attrib.vertices[3 * idxNext.vertex_index + 1],
                                                    attrib.vertices[3 * idxNext.vertex_index + 2]),
                                          glm::vec2(attrib.texcoords[2 * idxNext.texcoord_index + 0],
                                                    attrib.texcoords[2 * idxNext.texcoord_index + 1]),
                                          glm::vec3(triangleId % 256, (triangleId / 256) % 256, (triangleId / 256 / 256) % 256)));
                triangleId++;
            }
            index_offset += fv;
            shapes[s].mesh.material_ids[f];
        }
    }

    setViewMatrixForObj();
    scaleCoefficient = 0;
}

void Controller::setViewMatrixForObj() {
    double maxZ = 1;
    for (auto v : vertices) {
        glm::vec3 position = v.position();
        maxZ = fmax(maxZ, position.z);
    }
    viewMatrix.lookAt(
                QVector3D(0, 0, 10 * maxZ),
                QVector3D(0, 0, 0),
                QVector3D(0, 1, 0)
    );
}

void Controller::updateRotationMatrix() {
    double dx = previousMousePosition.x() - mousePosition.x();
    double dy = previousMousePosition.y() - mousePosition.y();

    QMatrix4x4 rot;
    rot.setToIdentity();
    rot.rotate(dx * 0.1f, 0, -1, 0);
    rot.rotate(dy * 0.1f, -1, 0, 0);
    rotationMatrix = rot * rotationMatrix;
    previousMousePosition = mousePosition;
}

void Controller::loadTextureImage(const char *fileName) {
    if (textureImage != 0) {
        delete textureImage;
    }
    QImage image(fileName);
    textureImage = new QImage(image.mirrored());
}

void Controller::initializeBrush() {
    int w = textureImage->width();
    int h = textureImage->height();
    Color* colorData = new Color[w * h];
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            QColor color = textureImage->pixelColor(x, y);
            colorData[h * x + y] = Color(color.red(), color.green(), color.blue());
        }
    }
    TextureStorage* textureStorage = new TextureStorage(w, h, colorData);

    if (brush != 0) {
        delete brush;
    }
    brush = new PixelsFastBrush(vertices, textureStorage);
    brush->setRadius(10.0);
}

void Controller::mousePressed(QPoint position) {
    previousMousePosition = position;
    mousePosition = position;
    isMousePressed = true;
    if (isBrashActive) {
        beginBrushStroke(position);
        isBrushUpdated = true;
    }
}

void Controller::mouseMoved(QPoint position) {
    mousePosition = position;
    if (isBrashActive) {
        continueBrushStroke(position);
        isBrushUpdated = true;
    }
}

void Controller::mouseReleased(QPoint position) {
    mousePosition = position;
    if (isBrashActive) {
        endBrushStroke(position);
        isBrushUpdated = true;
    }
    isMousePressed = false;
}

void Controller::keyPressed(int key) {
    if (key == Qt::Key_Control) {
        isBrashActive = true;
        if (isMousePressed) {
            beginBrushStroke(mousePosition);
        }
    } else {
        if (key == Qt::Key_Z) {
            brushHistory.undo(brush->getTextureStorage());
            isBrushUpdated = true;
        } else {
            if (key == Qt::Key_Y) {
                brushHistory.redo(brush->getTextureStorage());
                isBrushUpdated = true;
            }
        }
    }
}

void Controller::keyReleased(int key) {
    if (key == Qt::Key_Control) {
        if (isMousePressed) {
            previousMousePosition = mousePosition;
            endBrushStroke(mousePosition);
            isBrushUpdated = true;
        }
        isBrashActive = false;
    }
}

void Controller::wheelScrolled(int delta) {
    scaleCoefficient += delta * 0.001;
}

bool Controller::getIsBrushUpdated() {
    return isBrushUpdated;
}

void Controller::setIdsStorage(QImage *idsBuffer) {
    int w = idsBuffer->width();
    int h = idsBuffer->height();
    IdType* idsData = new IdType[w * h];
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            QColor color = idsBuffer->pixelColor(x, y);
            idsData[h * x + y] = color.red() + color.green() * 256 + color.blue() * 256 * 256;
        }
    }
    IdsStorage* idsStorage = new IdsStorage(w, h, idsData);

    brush->setIdsStorage(idsStorage);
}

void Controller::beginBrushStroke(QPoint point) {
    auto firstStrokePart = brush->paint(glm::i32vec2(point.x(), point.y()),
                                        fromQMatrix(getModelViewMatrix()),
                                        fromQMatrix(projectionMatrix),
                                        glm::i32vec2(screenSize.x(), screenSize.y()));
    currentStroke = BrushStroke(firstStrokePart);
    lastPointOfStroke = point;
}

void Controller::continueBrushStroke(QPoint point) {
    auto strokePart = brush->paint(glm::i32vec2(lastPointOfStroke.x(), lastPointOfStroke.y()),
                                   glm::i32vec2(point.x(), point.y()),
                                   fromQMatrix(getModelViewMatrix()),
                                   fromQMatrix(projectionMatrix),
                                   glm::i32vec2(screenSize.x(), screenSize.y()));
    currentStroke.add(strokePart);
    lastPointOfStroke = point;
}

void Controller::endBrushStroke(QPoint point) {
    auto strokePart = brush->paint(glm::i32vec2(lastPointOfStroke.x(), lastPointOfStroke.y()),
                                   glm::i32vec2(point.x(), point.y()),
                                   fromQMatrix(getModelViewMatrix()),
                                   fromQMatrix(projectionMatrix),
                                   glm::i32vec2(screenSize.x(), screenSize.y()));
    currentStroke.add(strokePart);
    brushHistory.addStroke(currentStroke);
}

glm::mat4x4 Controller::fromQMatrix(QMatrix4x4 qmat) {
    float const* data = qmat.constData();
    return glm::mat4x4(data[0],data[1], data[2], data[3],
                       data[4],data[5], data[6], data[7],
                       data[8],data[9], data[10], data[11],
                       data[12],data[13], data[14], data[15]);
}

void Controller::updateSize(int width, int height) {
    screenSize = QPoint(width, height);
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(45.0f, width / float(height), 0.0f, 1000.0f);
}

QMatrix4x4 Controller::getModelViewMatrix() {
    scaleMatrix.setToIdentity();
    scaleMatrix.scale(exp(scaleCoefficient));

    if (isMousePressed && !isBrashActive) {
        updateRotationMatrix();
    }
    return viewMatrix * rotationMatrix * scaleMatrix;
}

QMatrix4x4 Controller::getProjectionMatrix() {
    return projectionMatrix;
}

std::vector<Vertex> Controller::getVertices() {
    return vertices;
}

QImage *Controller::getTextureFromBrush() {
    isBrushUpdated = false;

    TextureStorage* textureStorage = brush->getTextureStorage();

    for (uint32_t x = 0; x < textureStorage->getWidth(); x++) {
        for (uint32_t y = 0; y < textureStorage->getHeight(); y++) {
            Color color = textureStorage->getColor(x, y);
            textureImage->setPixelColor(x, y, QColor(color.getR(), color.getG(), color.getB()));
        }
    }

    return textureImage;
}
