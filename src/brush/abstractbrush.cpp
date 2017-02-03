#include "abstractbrush.h"

AbstractBrush::AbstractBrush(const ObjectModel& objectModel, const TextureStorage textureStorage) :
    objectModel_(objectModel), textureStorage_(textureStorage) {
}

AbstractBrush::~AbstractBrush() {
}

void AbstractBrush::setRadius(double radius) {
    this->radius_ = radius;
}

void AbstractBrush::setColor(const glm::u8vec3& color) {
    this->color_ = color;
}

TextureStorage& AbstractBrush::getTextureStorage() {
    return textureStorage_;
}

void AbstractBrush::setIdsStorage(const IdsStorage& idsStorage) {
    this->idsStorage_ = idsStorage;
}

BrushStroke AbstractBrush::paint(const glm::i32vec2& previousPoint, const glm::i32vec2& lastPoint,
                                  const glm::mat4x4& matrixModelView, const glm::mat4x4& projection) {
    BrushStroke diff;

    glm::vec2 d(previousPoint - lastPoint);
    int cntRounds = glm::length(d);
    for (int i = 0; i < cntRounds; i++) {
        glm::i32vec2 currentPoint = previousPoint + (lastPoint - previousPoint) * i / cntRounds;
        auto currentPointDiff = paint(currentPoint, matrixModelView, projection);
        diff.addAll(currentPointDiff);
    }

    return diff;
}
