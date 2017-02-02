#ifndef TEXTURESTORAGE_H
#define TEXTURESTORAGE_H

#include "color.h"
#include "glm.hpp"

class TextureStorage {
public:
    TextureStorage(uint32_t width, uint32_t height, Color* texture);
    ~TextureStorage();
    uint32_t getWidth();
    uint32_t getHeight();
    Color getColor(uint32_t row, uint32_t col);
    Color getColor(glm::u32vec2 point);
    void setColor(uint32_t row, uint32_t col, Color color);
    void setColor(glm::u32vec2 point, Color color);

private:
    uint32_t width;
    uint32_t height;
    Color* values;
};

#endif // TEXTURESTORAGE_H
