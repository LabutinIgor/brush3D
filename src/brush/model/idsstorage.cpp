#include "idsstorage.h"

IdsStorage::IdsStorage() {
}

IdsStorage::IdsStorage(size_t width, size_t height) : width_(width), height_(height), ids_(width * height) {
}

size_t IdsStorage::getWidth() const {
    return width_;
}

size_t IdsStorage::getHeight() const {
    return height_;
}

glm::i32vec2 IdsStorage::getSize() const {
    return glm::i32vec2(getWidth(), getHeight());
}

IdType IdsStorage::getId(size_t row, size_t col) const {
    return ids_[row * height_ + col];
}

IdType IdsStorage::getId(const glm::u32vec2& point) const {
    return ids_[point.x * height_ + point.y];
}

void IdsStorage::setId(size_t row, size_t col, IdType id) {
    ids_[row * height_ + col] = id;
}

bool IdsStorage::hasNeighbourWithId(const glm::i32vec2& point, IdType id) const {
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            glm::i32vec2 newPoint = glm::i32vec2(point.x + dx, point.y + dy);
            if (newPoint.x >= 0 && newPoint.y >= 0 && newPoint.x < (int32_t) width_ && newPoint.y < (int32_t) height_ && getId(newPoint) == id) {
                return true;
            }
        }
    }
    return false;
}
