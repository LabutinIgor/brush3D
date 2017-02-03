#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <cmath>
#include <glm.hpp>

class Geometry {
public:
    static double getMinY(const std::vector<glm::vec2>& triangle, double x);
    static double getMaxY(const std::vector<glm::vec2>& triangle, double x);
    static double getMinIntersectionPoint(const glm::vec2& point1, const glm::vec2& point2, double x);
    static double getMaxIntersectionPoint(const glm::vec2& point1, const glm::vec2& point2, double x);
    static double getIntersectionPoint(const glm::vec2& point1, const glm::vec2& point2, double x);
    static glm::vec3 getPointFromUVCoordinates(const std::vector<glm::vec2>& pointsUV, const std::vector<glm::vec3>& points, const glm::vec2& pointUV);
    static glm::i32vec2 toScreenCoordinates(const glm::vec3& point, const glm::mat4x4& projection, const glm::i32vec2& screenSize);
    static glm::vec3 fromScreenCoordinates(const glm::vec2& point, const glm::mat4x4& matrixProjection);
};

#endif // GEOMETRY_H
