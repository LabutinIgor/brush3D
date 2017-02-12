#ifndef BRUSHUTILS_H
#define BRUSHUTILS_H

#include <glm/glm.hpp>

#include "face.hpp"
#include "matrix.hpp"

namespace Brush {
    namespace BrushUtils {
        double_t getMinUvX(const Face &face);

        double_t getMaxUvX(const Face &face);

        double_t getMinY(const Face &face, double_t x);

        double_t getMaxY(const Face &face, double_t x);

        double_t getMinIntersectionPoint(const glm::vec2 &point1, const glm::vec2 &point2, double_t x);

        double_t getMaxIntersectionPoint(const glm::vec2 &point1, const glm::vec2 &point2, double_t x);

        double_t getIntersectionPoint(const glm::vec2 &point1, const glm::vec2 &point2, double_t x);

        bool isInside(const glm::i32vec2 &point, const glm::i32vec2 &rectangle);

        bool hasNeighbourWithId(const IdsStorage &idsStorage, const glm::i32vec2 &point, IdType id);

        const double_t EPS = 1e-5;
    }
}

#endif // BRUSHUTILS_H
