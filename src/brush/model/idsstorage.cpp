#include "idsstorage.h"

IdsStorage::IdsStorage(uint32_t width, uint32_t height, IdType* ids) : width(width), height(height), ids(ids) {
}

uint32_t IdsStorage::getWidth() {
    return width;
}

uint32_t IdsStorage::getHeight() {
    return height;
}

IdType IdsStorage::getId(uint32_t row, uint32_t col) {
    return ids[row * height + col];
}

IdType IdsStorage::getId(glm::i32vec2 point) {
    return ids[point.x * height + point.y];
}

bool IdsStorage::hasNeighbourWithId(glm::i32vec2 point, uint32_t id) {
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
