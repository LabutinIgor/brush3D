#include "include/abstractbrush.hpp"

#include "src/details/utils.hpp"
#include "src/details/face.hpp"

namespace Brush {
    AbstractBrush::AbstractBrush(const ObjectModel& objectModel, TextureStorage& textureStorage) :
            objectModel_(objectModel), textureStorage_(textureStorage), radius_(10), color_(glm::u8vec3(255, 0, 0)) {
    }

    AbstractBrush::~AbstractBrush() {
    }

    void AbstractBrush::setRadius(float radius) {
        this->radius_ = radius;
    }

    void AbstractBrush::setColor(const glm::u8vec3& color) {
        this->color_ = color;
    }

    float AbstractBrush::getRadius() const {
        return radius_;
    }

    glm::u8vec3 AbstractBrush::getColor() const {
        return color_;
    }

    BrushStroke AbstractBrush::paint(const glm::i32vec2& previousPoint, const glm::i32vec2& lastPoint,
                                     const glm::mat4x4& matrixModelView, const glm::mat4x4& matrixProjection,
                                     const IdsStorage& idsStorage) {
        BrushStroke diff;

        glm::vec2 d(previousPoint - lastPoint);
        int cntRounds = glm::length(d);
        for (int i = 0; i < cntRounds; ++i) {
            glm::i32vec2 currentPoint = previousPoint + (lastPoint - previousPoint) * i / cntRounds;
            BrushStroke currentPointDiff = paint(currentPoint, matrixModelView, matrixProjection, idsStorage);
            diff.add(currentPointDiff);
        }

        return diff;
    }
}
