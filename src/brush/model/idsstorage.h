#ifndef IDSSTORAGE_H
#define IDSSTORAGE_H

#include <vector>
#include <cstdint>
#include <glm.hpp>

typedef uint32_t IdType;

class IdsStorage {
public:
    IdsStorage();
    IdsStorage(uint32_t width, uint32_t height);
    uint32_t getWidth() const;
    uint32_t getHeight() const;
    IdType getId(uint32_t row, uint32_t col) const;
    IdType getId(glm::i32vec2 point) const;
    void setId(uint32_t row, uint32_t col, IdType id);
    bool hasNeighbourWithId(glm::i32vec2 point, uint32_t id) const;

private:
    uint32_t width;
    uint32_t height;
    std::vector<IdType> ids;
};

#endif // IDSSTORAGE_H
