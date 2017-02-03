#include "abstractbrush.h"

AbstractBrush::AbstractBrush(const ObjectModel& objectModel, const TextureStorage textureStorage) :
    objectModel(objectModel), textureStorage(textureStorage) {
}

AbstractBrush::~AbstractBrush() {
}

void AbstractBrush::setRadius(double radius) {
    this->radius = radius;
}

void AbstractBrush::setColor(glm::u8vec3 color) {
    this->color = color;
}

TextureStorage& AbstractBrush::getTextureStorage() {
    return textureStorage;
}

void AbstractBrush::setIdsStorage(IdsStorage idsStorage) {
    this->idsStorage = idsStorage;
}

BrushStroke AbstractBrush::paint(glm::i32vec2 previousPoint, glm::i32vec2 lastPoint,
                                  glm::mat4x4 matrixModelView, glm::mat4x4 projection, glm::i32vec2 screenSize) {
    BrushStroke diff;

    glm::vec2 d(previousPoint - lastPoint);
    int cntRounds = glm::length(d);
    for (int i = 0; i < cntRounds; i++) {
        glm::i32vec2 currentPoint = previousPoint + (lastPoint - previousPoint) * i / cntRounds;
        auto currentPointDiff = paint(currentPoint, matrixModelView, projection, screenSize);
        diff.addAll(currentPointDiff);
    }

    return diff;
}
