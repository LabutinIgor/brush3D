#include "controller.h"

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include "tiny_obj_loader.h"

Controller::Controller() {
    rotationMatrix_.setToIdentity();
    scaleMatrix_.setToIdentity();
    viewMatrix_.setToIdentity();
}

void Controller::loadObj(const char *fileName) {
    verticesForBuffer_.clear();
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

    for (size_t s = 0; s < shapes.size(); ++s) {
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); ++f) {
            int fv = shapes[s].mesh.num_face_vertices[f];
            tinyobj::index_t idx = shapes[s].mesh.indices[index_offset];
            VertexForBuffer firstVertex = VertexForBuffer(vertexFromTinyobj(attrib.vertices, attrib.texcoords,
                                                                            idx.vertex_index, idx.texcoord_index, 0));
            for (size_t v = 1; v < (size_t) fv - 1; ++v) {
                tinyobj::index_t idxCurrent = shapes[s].mesh.indices[index_offset + v];
                tinyobj::index_t idxNext = shapes[s].mesh.indices[index_offset + v + 1];
                firstVertex.setId(glm::vec3(triangleId % 256, (triangleId / 256) % 256, (triangleId / 256 / 256) % 256));
                verticesForBuffer_.push_back(firstVertex);
                verticesForBuffer_.push_back(vertexFromTinyobj(attrib.vertices, attrib.texcoords,
                                                              idxCurrent.vertex_index, idxCurrent.texcoord_index, triangleId));
                verticesForBuffer_.push_back(vertexFromTinyobj(attrib.vertices, attrib.texcoords,
                                                              idxNext.vertex_index, idxNext.texcoord_index, triangleId++));
            }
            index_offset += fv;
            shapes[s].mesh.material_ids[f];
        }
    }

    uint32_t verticesNumber = verticesForBuffer_.size();
    objectModel_ = ObjectModel(verticesNumber, verticesNumber / 3);

    for (uint32_t i = 0; i < verticesForBuffer_.size(); ++i) {
        objectModel_.setVertex(i, verticesForBuffer_[i].position(), verticesForBuffer_[i].uv());
    }

    setViewMatrixForObj();
    scaleCoefficient_ = 0;
}

VertexForBuffer Controller::vertexFromTinyobj(const std::vector<float> &vertices, const std::vector<float> &texcoords,
                                              uint32_t vId, uint32_t tId, uint32_t triangleId) {
    return VertexForBuffer(glm::vec3(vertices[3 * vId + 0],
                                     vertices[3 * vId + 1],
                                     vertices[3 * vId + 2]),
                           glm::vec2(texcoords[2 * tId + 0],
                                     texcoords[2 * tId + 1]),
                           glm::vec3(triangleId % 256, (triangleId / 256) % 256, (triangleId / 256 / 256) % 256));
}

void Controller::setViewMatrixForObj() {
    double maxZ = 1;
    for (auto v : verticesForBuffer_) {
        glm::vec3 position = v.position();
        maxZ = fmax(maxZ, position.z);
    }
    viewMatrix_.lookAt(
                QVector3D(0, 0, 10 * maxZ),
                QVector3D(0, 0, 0),
                QVector3D(0, 1, 0)
    );
}

void Controller::updateRotationMatrix() {
    double dx = previousMousePosition_.x() - mousePosition_.x();
    double dy = previousMousePosition_.y() - mousePosition_.y();

    QMatrix4x4 rot;
    rot.setToIdentity();
    rot.rotate(dx * 0.1f, 0, -1, 0);
    rot.rotate(dy * 0.1f, -1, 0, 0);
    rotationMatrix_ = rot * rotationMatrix_;
    previousMousePosition_ = mousePosition_;
}

void Controller::loadTextureImage(const char *fileName) {
    QImage image(fileName);
    textureImage_ = QImage(image.mirrored());
}

void Controller::initializeBrush() {
    if (brush_ != 0) {
        delete brush_;
    }
    size_t w = textureImage_.width();
    size_t h = textureImage_.height();
    textureStorage_ = TextureStorage(w, h);
    for (size_t x = 0; x < w; ++x) {
        for (size_t y = 0; y < h; ++y) {
            QColor color = textureImage_.pixelColor(x, y);
            textureStorage_.setColor(x, y, glm::u8vec3(color.red(), color.green(), color.blue()));
        }
    }
    brush_ = new PixelsFastBrush(objectModel_, textureStorage_);
    brush_->setRadius(10.0);
}

void Controller::mousePressed(const QPoint& position) {
    previousMousePosition_ = position;
    mousePosition_ = position;
    isMousePressed_ = true;
    if (isBrashActive_) {
        beginBrushStroke(position);
        isBrushUpdated_ = true;
    }
}

void Controller::mouseMoved(const QPoint& position) {
    mousePosition_ = position;
    if (isBrashActive_) {
        continueBrushStroke(position);
        isBrushUpdated_ = true;
    }
}

void Controller::mouseReleased(const QPoint& position) {
    mousePosition_ = position;
    if (isBrashActive_) {
        endBrushStroke(position);
        isBrushUpdated_ = true;
    }
    isMousePressed_ = false;
}

void Controller::keyPressed(int key) {
    if (key == Qt::Key_Control) {
        isBrashActive_ = true;
        if (isMousePressed_) {
            beginBrushStroke(mousePosition_);
        }
    } else {
        if (key == Qt::Key_Z) {
            brushHistory_.undo(brush_->getTextureStorage());
            isBrushUpdated_ = true;
        } else {
            if (key == Qt::Key_Y) {
                brushHistory_.redo(brush_->getTextureStorage());
                isBrushUpdated_ = true;
            }
        }
    }
}

void Controller::keyReleased(int key) {
    if (key == Qt::Key_Control) {
        if (isMousePressed_) {
            previousMousePosition_ = mousePosition_;
            endBrushStroke(mousePosition_);
            isBrushUpdated_ = true;
        }
        isBrashActive_ = false;
    }
}

void Controller::wheelScrolled(int delta) {
    scaleCoefficient_ += delta * 0.001;
}

bool Controller::getIsBrushUpdated() {
    return isBrushUpdated_;
}

void Controller::setIdsStorage(QImage *idsBuffer) {
    size_t w = idsBuffer->width();
    size_t h = idsBuffer->height();
    idsStorage_ = IdsStorage(w, h);
    for (size_t x = 0; x < w; ++x) {
        for (size_t y = 0; y < h; ++y) {
            QColor color = idsBuffer->pixelColor(x, y);
            idsStorage_.setId(x, y, color.red() + color.green() * 256 + color.blue() * 256 * 256);
        }
    }
}

void Controller::beginBrushStroke(const QPoint& point) {
    auto firstStrokePart = brush_->paint(glm::i32vec2(point.x(), point.y()),
                                        fromQMatrix(getModelViewMatrix()),
                                        fromQMatrix(projectionMatrix_),
                                        idsStorage_);
    currentStroke_ = BrushStroke(firstStrokePart);
    lastPointOfStroke_ = point;
}

void Controller::continueBrushStroke(const QPoint& point) {
    auto strokePart = brush_->paint(glm::i32vec2(lastPointOfStroke_.x(), lastPointOfStroke_.y()),
                                   glm::i32vec2(point.x(), point.y()),
                                   fromQMatrix(getModelViewMatrix()),
                                   fromQMatrix(projectionMatrix_),
                                   idsStorage_);
    currentStroke_.add(strokePart);
    lastPointOfStroke_ = point;
}

void Controller::endBrushStroke(const QPoint& point) {
    auto strokePart = brush_->paint(glm::i32vec2(lastPointOfStroke_.x(), lastPointOfStroke_.y()),
                                   glm::i32vec2(point.x(), point.y()),
                                   fromQMatrix(getModelViewMatrix()),
                                   fromQMatrix(projectionMatrix_),
                                   idsStorage_);
    currentStroke_.add(strokePart);
    brushHistory_.addStroke(currentStroke_);
}

glm::mat4x4 Controller::fromQMatrix(const QMatrix4x4& qmat) {
    float const* data = qmat.constData();
    return glm::mat4x4(data[0],  data[1],  data[2],  data[3],
                       data[4],  data[5],  data[6],  data[7],
                       data[8],  data[9],  data[10], data[11],
                       data[12], data[13], data[14], data[15]);
}

void Controller::updateSize(int width, int height) {
    projectionMatrix_.setToIdentity();
    projectionMatrix_.perspective(45.0f, width / float(height), 0.0f, 1000.0f);
}

QMatrix4x4 Controller::getModelViewMatrix() {
    scaleMatrix_.setToIdentity();
    scaleMatrix_.scale(exp(scaleCoefficient_));

    if (isMousePressed_ && !isBrashActive_) {
        updateRotationMatrix();
    }
    return viewMatrix_ * rotationMatrix_ * scaleMatrix_;
}

const QMatrix4x4& Controller::getProjectionMatrix() {
    return projectionMatrix_;
}

const std::vector<VertexForBuffer>& Controller::getVertices() {
    return verticesForBuffer_;
}

QImage *Controller::getTextureFromBrush() {
    isBrushUpdated_ = false;

    TextureStorage& textureStorage_ = brush_->getTextureStorage();

    for (size_t x = 0; x < textureStorage_.getWidth(); ++x) {
        for (size_t y = 0; y < textureStorage_.getHeight(); ++y) {
            glm::u8vec3 color = textureStorage_.getColor(x, y);
            textureImage_.setPixelColor(x, y, QColor(color.r, color.g, color.b));
        }
    }

    return &textureImage_;
}
