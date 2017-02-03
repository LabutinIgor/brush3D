#include "texturestorage.h"

TextureStorage::TextureStorage(uint32_t width, uint32_t height, glm::u8vec3* values) : width(width), height(height), values(values)  {
}

TextureStorage::~TextureStorage() {
    delete[] values;
}

uint32_t TextureStorage::getWidth() {
    return width;
}

uint32_t TextureStorage::getHeight() {
    return height;
}

glm::u8vec3 TextureStorage::getColor(uint32_t row, uint32_t col) {
    return values[row * height + col];
}

glm::u8vec3 TextureStorage::getColor(glm::u32vec2 point) {
    return values[point.x * height + point.y];
}

void TextureStorage::setColor(uint32_t row, uint32_t col, glm::u8vec3 color) {
    values[row * height + col] = color;
}

void TextureStorage::setColor(glm::u32vec2 point, glm::u8vec3 color) {
    values[point.x * height + point.y] = color;
}
