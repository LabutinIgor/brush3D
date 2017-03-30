#ifndef BRUSHUTILS_H
#define BRUSHUTILS_H

#include <glm/glm.hpp>

#include "face.hpp"
#include "matrix.hpp"
#include "idsstorage.hpp"

namespace Brush {
    namespace Utils {
        float getMinUvX(const Face& face);
        float getMaxUvX(const Face& face);
        float getMinY(const Face& face, float x);
        float getMaxY(const Face& face, float x);
        float getMinIntersectionPoint(const glm::vec2& point1, const glm::vec2& point2, float x);
        float getMaxIntersectionPoint(const glm::vec2& point1, const glm::vec2& point2, float x);
        float getIntersectionPoint(const glm::vec2& point1, const glm::vec2& point2, float x);
        bool isInside(const glm::i32vec2& point, const glm::i32vec2& rectangle);
        bool isInsideRound(const glm::i32vec2& point, const glm::i32vec2& center, const float radius);
        bool hasNeighbourWithId(const IdsStorage& idsStorage, const glm::i32vec2& point, IdType id);
        glm::vec3 getPointFromUVCoordinates(const std::vector<glm::vec2>& pointsUV,
                                            const std::vector<glm::vec3>& points, const glm::vec2& pointUV);
        glm::vec2 toScreenCoordinates(const glm::vec3& point, const glm::mat4x4& matrixProjection,
                                      const glm::i32vec2& screenSize);
        float calculateFaceAngleCos(const Face& face, const glm::mat4x4& matrixModelView);

        const float EPS = 1e-5;
    }
}

#endif // BRUSHUTILS_H
