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

    for (size_t s = 0; s < shapes.size(); s++) {
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            int fv = shapes[s].mesh.num_face_vertices[f];
            tinyobj::index_t idx = shapes[s].mesh.indices[index_offset];
            Vertex firstVertex = Vertex(QVector3D(attrib.vertices[3 * idx.vertex_index + 0],
                                                  attrib.vertices[3 * idx.vertex_index + 1],
                                                  attrib.vertices[3 * idx.vertex_index + 2]),
                                        QVector2D(attrib.texcoords[2 * idx.texcoord_index + 0],
                                                  attrib.texcoords[2 * idx.texcoord_index + 1]));
            for (size_t v = 1; v < (size_t) fv - 1; v++) {
                tinyobj::index_t idxCurrent = shapes[s].mesh.indices[index_offset + v];
                tinyobj::index_t idxNext = shapes[s].mesh.indices[index_offset + v + 1];
                vertices.push_back(firstVertex);
                vertices.push_back(Vertex(QVector3D(attrib.vertices[3 * idxCurrent.vertex_index + 0],
                                          attrib.vertices[3 * idxCurrent.vertex_index + 1],
                                          attrib.vertices[3 * idxCurrent.vertex_index + 2]),
                                QVector2D(attrib.texcoords[2 * idxCurrent.texcoord_index + 0],
                                          attrib.texcoords[2 * idxCurrent.texcoord_index + 1])));

                vertices.push_back(Vertex(QVector3D(attrib.vertices[3 * idxNext.vertex_index + 0],
                                          attrib.vertices[3 * idxNext.vertex_index + 1],
                                          attrib.vertices[3 * idxNext.vertex_index + 2]),
                                QVector2D(attrib.texcoords[2 * idxNext.texcoord_index + 0],
                                          attrib.texcoords[2 * idxNext.texcoord_index + 1])));
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
        QVector3D position = v.position();
        maxZ = fmax(maxZ, position.z());
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
    brush = new Brush(vertices, textureImage);
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
    } else {
        if (key == Qt::Key_Z) {
            brushHistory.undo(brush->getTextureImage());
            isBrushUpdated = true;
        } else {
            if (key == Qt::Key_Y) {
                brushHistory.redo(brush->getTextureImage());
                isBrushUpdated = true;
            }
        }
    }
}

void Controller::keyReleased(int key) {
    if (key == Qt::Key_Control) {
        isBrashActive = false;
    }
}

void Controller::wheelScrolled(int delta) {
    scaleCoefficient += delta * 0.001;
}

bool Controller::getIsBrushUpdated() {
    return isBrushUpdated;
}

void Controller::beginBrushStroke(QPoint point) {
    auto firstStrokePart = brush->paint(point, getModelViewMatrix(), projectionMatrix, screenSize);
    currentStroke = BrushStroke(firstStrokePart);
    lastPointOfStroke = point;
}

void Controller::continueBrushStroke(QPoint point) {
    auto strokePart = brush->paint(lastPointOfStroke, point, getModelViewMatrix(), projectionMatrix, screenSize);
    currentStroke.add(strokePart);
    lastPointOfStroke = point;
}

void Controller::endBrushStroke(QPoint point) {
    auto strokePart = brush->paint(lastPointOfStroke, point, getModelViewMatrix(), projectionMatrix, screenSize);
    currentStroke.add(strokePart);
    brushHistory.addStroke(currentStroke);
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

QImage *Controller::getTextureImageFromBrush() {
    isBrushUpdated = false;
    return brush->getTextureImage();
}
