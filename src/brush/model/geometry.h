#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <cmath>
#include <glm.hpp>

#include "face.h"

namespace Geometry {
    glm::vec3 getPointFromUVCoordinates(const std::vector<glm::vec2>& pointsUV, const std::vector<glm::vec3>& points, const glm::vec2& pointUV);
    glm::i32vec2 toScreenCoordinates(const glm::vec3& point, const glm::mat4x4& matrixProjection, const glm::i32vec2& screenSize);
    glm::vec3 fromScreenCoordinates(const glm::vec2& point, const glm::mat4x4& matrixProjection);
    float_t calculateFaceAngleCos(const Face& face, const glm::mat4x4& matrixModelView);
    constexpr double EPS = 1e-5;
}

#endif // GEOMETRY_H
