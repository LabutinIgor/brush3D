#ifndef TEXTURESTORAGE_H
#define TEXTURESTORAGE_H

#include "color.h"

class TextureStorage {
public:
    TextureStorage(uint32_t width, uint32_t height, Color* texture);
    uint32_t getWidth();
    uint32_t getHeight();
    Color getColor(uint32_t row, uint32_t col);
    void setColor(uint32_t row, uint32_t col, Color color);

private:
    uint32_t width;
    uint32_t height;
    Color* values;
};

#endif // TEXTURESTORAGE_H
