#include "abstractbrush.h"

AbstractBrush::AbstractBrush(std::vector<Vertex> vertices, QImage* textureImage)
{
    this->vertices = vertices;
    this->textureImage = textureImage;
}

void AbstractBrush::setRadius(double radius) {
    this->radius = radius;
}

void AbstractBrush::setColor(QColor color) {
    this->color = color;
}

QImage *AbstractBrush::getTextureImage() {
    return textureImage;
}

glm::vec3 AbstractBrush::fromScreenCoordinates(glm::vec2 point, glm::mat4x4 matrixProjection) {
    return glm::vec3(point.x / matrixProjection[0][0],
                     point.y / matrixProjection[1][1],
                     -1.0);
}

void AbstractBrush::setIdsBuffer(QImage *idsBuffer) {
    this->idsBuffer = idsBuffer;
}

std::vector<std::pair<glm::i32vec2, std::pair<QColor, QColor>>>
        AbstractBrush::paint(glm::i32vec2 previousPoint, glm::i32vec2 lastPoint,
              glm::mat4x4 matrixModelView, glm::mat4x4 projection, glm::i32vec2 screenSize) {
    std::vector<std::pair<glm::i32vec2, std::pair<QColor, QColor>>> diff;

    glm::vec2 d(previousPoint - lastPoint);
    int cntRounds = glm::length(d);
    for (int i = 0; i < cntRounds; i++) {
        glm::i32vec2 currentPoint = previousPoint + (lastPoint - previousPoint) * i / cntRounds;
        auto currentPointDiff = paint(currentPoint, matrixModelView, projection, screenSize);
        diff.insert(std::end(diff), std::begin(currentPointDiff), std::end(currentPointDiff));
    }

    return diff;
}
