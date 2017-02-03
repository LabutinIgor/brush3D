#include "texturestorage.h"

TextureStorage::TextureStorage() {
}

TextureStorage::TextureStorage(uint32_t width, uint32_t height) : width_(width), height_(height) {
    values_.resize(width * height);
}

uint32_t TextureStorage::getWidth() const {
    return width_;
}

uint32_t TextureStorage::getHeight() const {
    return height_;
}

glm::u8vec3 TextureStorage::getColor(uint32_t row, uint32_t col) const {
    return values_[row * height_ + col];
}

glm::u8vec3 TextureStorage::getColor(const glm::u32vec2& point) const {
    return values_[point.x * height_ + point.y];
}

void TextureStorage::setColor(uint32_t row, uint32_t col, const glm::u8vec3& color) {
    values_[row * height_ + col] = color;
}

void TextureStorage::setColor(const glm::u32vec2& point, const glm::u8vec3& color) {
    values_[point.x * height_ + point.y] = color;
}
