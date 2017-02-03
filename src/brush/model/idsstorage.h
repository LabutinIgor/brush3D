#ifndef IDSSTORAGE_H
#define IDSSTORAGE_H

#include <vector>
#include <cstdint>
#include <glm.hpp>

typedef uint32_t IdType;

class IdsStorage {
public:
    IdsStorage();
    IdsStorage(size_t width, size_t height);
    uint32_t getWidth() const;
    uint32_t getHeight() const;
    IdType getId(size_t row, size_t col) const;
    IdType getId(const glm::u32vec2& point) const;
    void setId(size_t row, size_t col, IdType id);
    bool hasNeighbourWithId(const glm::i32vec2& point, size_t id) const;

private:
    uint32_t width_;
    uint32_t height_;
    std::vector<IdType> ids_;
};

#endif // IDSSTORAGE_H
