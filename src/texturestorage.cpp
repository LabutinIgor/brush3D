#include "texturestorage.h"

TextureStorage::TextureStorage(uint32_t width, uint32_t height, Color* values) : width(width), height(height), values(values)  {
}

uint32_t TextureStorage::getWidth() {
    return width;
}

uint32_t TextureStorage::getHeight() {
    return height;
}

Color TextureStorage::getColor(uint32_t row, uint32_t col) {
    return values[row * height + col];
}

void TextureStorage::setColor(uint32_t row, uint32_t col, Color color) {
    values[row * height + col] = color;
}
