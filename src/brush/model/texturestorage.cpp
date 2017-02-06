#include "texturestorage.h"

TextureStorage::TextureStorage() {
}

TextureStorage::TextureStorage(size_t width, size_t height) : width_(width), height_(height), values_(width * height) {
}

size_t TextureStorage::getWidth() const {
    return width_;
}

size_t TextureStorage::getHeight() const {
    return height_;
}

glm::u8vec3 TextureStorage::getColor(size_t row, size_t col) const {
    return values_[row * height_ + col];
}

glm::u8vec3 TextureStorage::getColor(const glm::u32vec2& point) const {
    return values_[point.x * height_ + point.y];
}

void TextureStorage::setColor(size_t row, size_t col, const glm::u8vec3& color) {
    values_[row * height_ + col] = color;
}

void TextureStorage::setColor(const glm::u32vec2& point, const glm::u8vec3& color) {
    values_[point.x * height_ + point.y] = color;
}
