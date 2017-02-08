#ifndef BRUSHUTILS_H
#define BRUSHUTILS_H

#include <glm.hpp>

#include "face.h"
#include "matrix.h"

namespace BrushUtils {
    double_t getMinUvX(const Face& face);
    double_t getMaxUvX(const Face& face);
    double_t getMinY(const Face& face, double_t x);
    double_t getMaxY(const Face& face, double_t x);
    double_t getMinIntersectionPoint(const glm::vec2& point1, const glm::vec2& point2, double_t x);
    double_t getMaxIntersectionPoint(const glm::vec2& point1, const glm::vec2& point2, double_t x);
    double_t getIntersectionPoint(const glm::vec2& point1, const glm::vec2& point2, double_t x);
    bool isInside(const glm::i32vec2& point, const glm::i32vec2& rectangle);
    bool hasNeighbourWithId(const IdsStorage& idsStorage, const glm::i32vec2& point, IdType id);
    constexpr double_t EPS = 1e-5;
}

#endif // BRUSHUTILS_H
