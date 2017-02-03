#include "controller.h"

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include "tiny_obj_loader.h"

Controller::Controller() {
    rotationMatrix.setToIdentity();
    scaleMatrix.setToIdentity();
    viewMatrix.setToIdentity();
}

void Controller::loadObj(const char *fileName) {
    verticesForBuffer.clear();
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
    uint32_t triangleId = 0;

    for (size_t s = 0; s < shapes.size(); s++) {
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            int fv = shapes[s].mesh.num_face_vertices[f];
            tinyobj::index_t idx = shapes[s].mesh.indices[index_offset];
            VertexForBuffer firstVertex = VertexForBuffer(vertexFromTinyobj(attrib.vertices, attrib.texcoords,
                                                                            idx.vertex_index, idx.texcoord_index, 0));
            for (size_t v = 1; v < (size_t) fv - 1; v++) {
                tinyobj::index_t idxCurrent = shapes[s].mesh.indices[index_offset + v];
                tinyobj::index_t idxNext = shapes[s].mesh.indices[index_offset + v + 1];
                firstVertex.setId(glm::vec3(triangleId % 256, (triangleId / 256) % 256, (triangleId / 256 / 256) % 256));
                verticesForBuffer.push_back(firstVertex);
                verticesForBuffer.push_back(vertexFromTinyobj(attrib.vertices, attrib.texcoords,
                                                              idxCurrent.vertex_index, idxCurrent.texcoord_index, triangleId));
                verticesForBuffer.push_back(vertexFromTinyobj(attrib.vertices, attrib.texcoords,
                                                              idxNext.vertex_index, idxNext.texcoord_index, triangleId++));
            }
            index_offset += fv;
            shapes[s].mesh.material_ids[f];
        }
    }

    uint32_t verticesNumber = verticesForBuffer.size();
    objectModel = ObjectModel(verticesNumber, verticesNumber / 3);

    for (uint32_t i = 0; i < verticesForBuffer.size(); i++) {
        objectModel.setVertex(i, verticesForBuffer[i].position(), verticesForBuffer[i].uv());
    }

    setViewMatrixForObj();
    scaleCoefficient = 0;
}

VertexForBuffer Controller::vertexFromTinyobj(const std::vector<float> &vertices, const std::vector<float> &texcoords, uint32_t vId, uint32_t tId, uint32_t triangleId) {
    return VertexForBuffer(glm::vec3(vertices[3 * vId + 0],
                                     vertices[3 * vId + 1],
                                     vertices[3 * vId + 2]),
                           glm::vec2(texcoords[2 * tId + 0],
                                     texcoords[2 * tId + 1]),
                           glm::vec3(triangleId % 256, (triangleId / 256) % 256, (triangleId / 256 / 256) % 256));
}

void Controller::setViewMatrixForObj() {
    double maxZ = 1;
    for (auto v : verticesForBuffer) {
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
    if (brush != 0) {
        delete brush;
    }
    size_t w = textureImage->width();
    size_t h = textureImage->height();
    textureStorage = TextureStorage(w, h);
    for (size_t x = 0; x < w; x++) {
        for (size_t y = 0; y < h; y++) {
            QColor color = textureImage->pixelColor(x, y);
            textureStorage.setColor(x, y, glm::u8vec3(color.red(), color.green(), color.blue()));
        }
    }
    brush = new PixelsFastBrush(objectModel, textureStorage);
    brush->setRadius(10.0);
}

void Controller::mousePressed(const QPoint& position) {
    previousMousePosition = position;
    mousePosition = position;
    isMousePressed = true;
    if (isBrashActive) {
        beginBrushStroke(position);
        isBrushUpdated = true;
    }
}

void Controller::mouseMoved(const QPoint& position) {
    mousePosition = position;
    if (isBrashActive) {
        continueBrushStroke(position);
        isBrushUpdated = true;
    }
}

void Controller::mouseReleased(const QPoint& position) {
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
    size_t w = idsBuffer->width();
    size_t h = idsBuffer->height();
    IdsStorage idsStorage(w, h);
    for (size_t x = 0; x < w; x++) {
        for (size_t y = 0; y < h; y++) {
            QColor color = idsBuffer->pixelColor(x, y);
            idsStorage.setId(x, y, color.red() + color.green() * 256 + color.blue() * 256 * 256);
        }
    }

    brush->setIdsStorage(idsStorage);
}

void Controller::beginBrushStroke(const QPoint& point) {
    auto firstStrokePart = brush->paint(glm::i32vec2(point.x(), point.y()),
                                        fromQMatrix(getModelViewMatrix()),
                                        fromQMatrix(projectionMatrix));
    currentStroke = BrushStroke(firstStrokePart);
    lastPointOfStroke = point;
}

void Controller::continueBrushStroke(const QPoint& point) {
    auto strokePart = brush->paint(glm::i32vec2(lastPointOfStroke.x(), lastPointOfStroke.y()),
                                   glm::i32vec2(point.x(), point.y()),
                                   fromQMatrix(getModelViewMatrix()),
                                   fromQMatrix(projectionMatrix));
    currentStroke.addAll(strokePart);
    lastPointOfStroke = point;
}

void Controller::endBrushStroke(const QPoint& point) {
    auto strokePart = brush->paint(glm::i32vec2(lastPointOfStroke.x(), lastPointOfStroke.y()),
                                   glm::i32vec2(point.x(), point.y()),
                                   fromQMatrix(getModelViewMatrix()),
                                   fromQMatrix(projectionMatrix));
    currentStroke.addAll(strokePart);
    brushHistory.addStroke(currentStroke);
}

glm::mat4x4 Controller::fromQMatrix(const QMatrix4x4& qmat) {
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

const QMatrix4x4& Controller::getProjectionMatrix() {
    return projectionMatrix;
}

const std::vector<VertexForBuffer>& Controller::getVertices() {
    return verticesForBuffer;
}

QImage *Controller::getTextureFromBrush() {
    isBrushUpdated = false;

    TextureStorage& textureStorage = brush->getTextureStorage();

    for (size_t x = 0; x < textureStorage.getWidth(); x++) {
        for (size_t y = 0; y < textureStorage.getHeight(); y++) {
            glm::u8vec3 color = textureStorage.getColor(x, y);
            textureImage->setPixelColor(x, y, QColor(color.r, color.g, color.b));
        }
    }

    return textureImage;
}
