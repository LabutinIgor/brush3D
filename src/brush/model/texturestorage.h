#ifndef TEXTURESTORAGE_H
#define TEXTURESTORAGE_H

#include <vector>
#include <glm.hpp>

class TextureStorage {
public:
    TextureStorage();
    TextureStorage(size_t width, size_t height);
    size_t getWidth() const;
    size_t getHeight() const;
    glm::u8vec3 getColor(size_t row, size_t col) const;
    glm::u8vec3 getColor(const glm::u32vec2& point) const;
    void setColor(size_t row, size_t col, const glm::u8vec3& color);
    void setColor(const glm::u32vec2& point, const glm::u8vec3& color);

private:
    size_t width_;
    size_t height_;
    std::vector<glm::u8vec3> values_;
};

#endif // TEXTURESTORAGE_H
