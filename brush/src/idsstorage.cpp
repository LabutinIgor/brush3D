
#include <iostream>
#include "idsstorage.hpp"

namespace Brush {
    IdsStorage::IdsStorage() {
    }

    IdsStorage::IdsStorage(const glm::u32vec2& screenSize, const glm::u32vec2& storageSize,
                           const glm::u32vec2& firstPoint, const glm::vec2& lastPoint) : screenSize_(screenSize),
                                                                                         firstPoint_(firstPoint),
                                                                                         matrix_(storageSize.x,
                                                                                                 storageSize.y) {
        scale_ = glm::vec2(storageSize.x * 1.0 / (lastPoint.x - firstPoint.x),
                           (storageSize.y * 1.0 / lastPoint.y - firstPoint.y));
    }

    size_t IdsStorage::getWidth() const {
        return matrix_.getWidth();
    }

    size_t IdsStorage::getHeight() const {
        return matrix_.getHeight();
    }

    glm::i32vec2 IdsStorage::getSize() const {
        return matrix_.getSize();
    }

    IdType IdsStorage::getValue(size_t row, size_t col) const {
        return matrix_.getValue(row, col);
    }

    IdType IdsStorage::getValue(const glm::u32vec2& point) const {
        return matrix_.getValue(point);
    }

    void IdsStorage::setValue(size_t row, size_t col, const IdType& value) {
        matrix_.setValue(row, col, value);
    }

    void IdsStorage::setValue(const glm::u32vec2& point, const IdType& value) {
        return matrix_.setValue(point, value);
    }

    glm::i32vec2 IdsStorage::getScreenSize() const {
        return screenSize_;
    }

    glm::i32vec2 IdsStorage::fromScreenCoord(const glm::vec2& point) const {
        glm::i32vec2 p(point - static_cast<glm::vec2>(firstPoint_));
        return glm::i32vec2(p.x * scale_.x, p.y * scale_.y);
    }

    glm::vec2 IdsStorage::toScreenCoord(const glm::i32vec2& point) const {
        glm::vec2 p(point.x / scale_.x, point.y / scale_.y);
        return p + static_cast<glm::vec2>(firstPoint_);
    }
}