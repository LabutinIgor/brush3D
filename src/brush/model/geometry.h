#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <cmath>
#include <glm.hpp>

namespace Geometry {
    double getMinY(const std::vector<glm::vec2>& triangle, double x);
    double getMaxY(const std::vector<glm::vec2>& triangle, double x);
    double getMinIntersectionPoint(const glm::vec2& point1, const glm::vec2& point2, double x);
    double getMaxIntersectionPoint(const glm::vec2& point1, const glm::vec2& point2, double x);
    double getIntersectionPoint(const glm::vec2& point1, const glm::vec2& point2, double x);
    glm::vec3 getPointFromUVCoordinates(const std::vector<glm::vec2>& pointsUV, const std::vector<glm::vec3>& points, const glm::vec2& pointUV);
    glm::i32vec2 toScreenCoordinates(const glm::vec3& point, const glm::mat4x4& projection, const glm::i32vec2& screenSize);
    glm::vec3 fromScreenCoordinates(const glm::vec2& point, const glm::mat4x4& matrixProjection);
    constexpr double EPS = 1e-5;
}

#endif // GEOMETRY_H
