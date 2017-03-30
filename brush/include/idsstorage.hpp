#ifndef BRUSHGUI_IDSSTORAGE_H
#define BRUSHGUI_IDSSTORAGE_H

#include <cstdint>

#include "matrix.hpp"

namespace Brush {
    typedef uint32_t IdType;

    class IdsStorage {
    public:
        IdsStorage();
        IdsStorage(const glm::u32vec2& screenSize, const glm::u32vec2& storageSize, const glm::u32vec2& firstPoint,
                   const glm::vec2& lastPoint);
        size_t getWidth() const;
        size_t getHeight() const;
        glm::i32vec2 getSize() const;
        IdType getValue(size_t row, size_t col) const;
        IdType getValue(const glm::u32vec2& point) const;
        void setValue(size_t row, size_t col, const IdType& value);
        void setValue(const glm::u32vec2& point, const IdType& value);
        glm::i32vec2 getScreenSize() const;
        glm::i32vec2 fromScreenCoord(const glm::vec2& point) const;
        glm::vec2 toScreenCoord(const glm::i32vec2& point) const;
    private:
        glm::u32vec2 screenSize_;
        glm::u32vec2 firstPoint_;
        glm::vec2 scale_;
        Matrix <IdType> matrix_;
    };
}


#endif //BRUSHGUI_IDSSTORAGE_H
