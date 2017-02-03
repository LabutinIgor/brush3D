#ifndef TEXTURESTORAGE_H
#define TEXTURESTORAGE_H

#include <vector>
#include <glm.hpp>

class TextureStorage {
public:
    TextureStorage();
    TextureStorage(uint32_t width, uint32_t height);
    uint32_t getWidth() const;
    uint32_t getHeight() const;
    glm::u8vec3 getColor(uint32_t row, uint32_t col) const;
    glm::u8vec3 getColor(glm::u32vec2 point) const;
    void setColor(uint32_t row, uint32_t col, glm::u8vec3 color);
    void setColor(glm::u32vec2 point, glm::u8vec3 color);

private:
    uint32_t width;
    uint32_t height;
    std::vector<glm::u8vec3> values;
};

#endif // TEXTURESTORAGE_H
