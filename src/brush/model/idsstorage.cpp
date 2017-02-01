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
