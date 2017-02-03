#include "idsstorage.h"

IdsStorage::IdsStorage() {
}

IdsStorage::IdsStorage(uint32_t width, uint32_t height) : width(width), height(height) {
    ids.resize(width * height);
}

uint32_t IdsStorage::getWidth() const {
    return width;
}

uint32_t IdsStorage::getHeight() const {
    return height;
}

IdType IdsStorage::getId(uint32_t row, uint32_t col) const {
    return ids[row * height + col];
}

IdType IdsStorage::getId(glm::i32vec2 point) const {
    return ids[point.x * height + point.y];
}

void IdsStorage::setId(uint32_t row, uint32_t col, IdType id) {
    ids[row * height + col] = id;
}

bool IdsStorage::hasNeighbourWithId(glm::i32vec2 point, uint32_t id) const {
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            glm::i32vec2 newPoint = glm::i32vec2(point.x + dx, point.y + dy);
            if (newPoint.x >= 0 && newPoint.y >= 0 && newPoint.x < (int32_t) width && newPoint.y < (int32_t) height && getId(newPoint) == id) {
                return true;
            }
        }
    }
    return false;
}
