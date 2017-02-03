#ifndef TEXTURESTORAGE_H
#define TEXTURESTORAGE_H

#include <glm.hpp>

class TextureStorage {
public:
    TextureStorage(uint32_t width, uint32_t height, glm::u8vec3* texture);
    ~TextureStorage();
    uint32_t getWidth();
    uint32_t getHeight();
    glm::u8vec3 getColor(uint32_t row, uint32_t col);
    glm::u8vec3 getColor(glm::u32vec2 point);
    void setColor(uint32_t row, uint32_t col, glm::u8vec3 color);
    void setColor(glm::u32vec2 point, glm::u8vec3 color);

private:
    uint32_t width;
    uint32_t height;
    glm::u8vec3* values;
};

#endif // TEXTURESTORAGE_H
