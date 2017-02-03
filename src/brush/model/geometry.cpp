#include "geometry.h"


double Geometry::getMinY(const std::vector<glm::vec2>& triangle, double x) {
    return fmin(getMinIntersectionPoint(triangle[0], triangle[1], x),
                fmin(getMinIntersectionPoint(triangle[1], triangle[2], x),
                     getMinIntersectionPoint(triangle[0], triangle[2], x)));
}

double Geometry::getMaxY(const std::vector<glm::vec2>& triangle, double x) {
    return fmax(getMaxIntersectionPoint(triangle[0], triangle[1], x),
                fmax(getMaxIntersectionPoint(triangle[1], triangle[2], x),
                     getMaxIntersectionPoint(triangle[0], triangle[2], x)));
}

double Geometry::getMinIntersectionPoint(const glm::vec2& point1, const glm::vec2& point2, double x) {
    if (fabs(point1.x - point2.x) < 1e-5) {
        if (fabs(point1.x - x) > 1e-5) {
            return 1;
        } else {
            return fmin(point1.y, point2.y);
        }
    } else {
        double y = getIntersectionPoint(point1, point2, x);
        if (y < fmin(point1.y, point2.y) || y > fmax(point1.y, point2.y)) {
            return 1;
        } else {
            return y;
        }
    }
}

double Geometry::getMaxIntersectionPoint(const glm::vec2& point1, const glm::vec2& point2, double x) {
    if (fabs(point1.x - point2.x) < 1e-5) {
        if (fabs(point1.x - x) > 1e-5) {
            return 0;
        } else {
            return fmax(point1.y, point2.y);
        }
    } else {
        double y = getIntersectionPoint(point1, point2, x);
        if (y < fmin(point1.y, point2.y) || y > fmax(point1.y, point2.y)) {
            return 0;
        } else {
            return y;
        }
    }
}

double Geometry::getIntersectionPoint(const glm::vec2& point1, const glm::vec2& point2, double x) {
    double intersectionY = point1.y + (x - point1.x) * (point2.y - point1.y) / (point2.x - point1.x);
    return fmax(0.0, fmin(1.0, intersectionY));
}

glm::vec3 Geometry::getPointFromUVCoordinates(const std::vector<glm::vec2>& pointsUV, const std::vector<glm::vec3>& points, const glm::vec2& pointUV) {
    glm::vec2 uvVector1 = pointsUV[1] - pointsUV[0];
    glm::vec2 uvVector2 = pointsUV[2] - pointsUV[0];
    glm::vec3 vector1 = points[1] - points[0];
    glm::vec3 vector2 = points[2] - points[0];
    glm::vec2 point = pointUV - pointsUV[0];

    float d = uvVector1.x * uvVector2.y - uvVector1.y * uvVector2.x;
    float c1 = (point.x * uvVector2.y - point.y * uvVector2.x) / d;
    float c2 = (uvVector1.x * point.y - uvVector1.y * point.x) / d;

    return points[0] + c1 * vector1 + c2 * vector2;
}

glm::i32vec2 Geometry::toScreenCoordinates(const glm::vec3& point, const glm::mat4x4& projection, const glm::i32vec2& screenSize) {
    glm::vec4 homogeneousCoordinates(projection * glm::vec4(point, 1.0));
    glm::vec3 projectedPoint(homogeneousCoordinates.x / homogeneousCoordinates.w,
                             homogeneousCoordinates.y / homogeneousCoordinates.w,
                             homogeneousCoordinates.z / homogeneousCoordinates.w);
    return glm::i32vec2(screenSize.x * (projectedPoint.x + 1) / 2,
                  screenSize.y * (1 - projectedPoint.y) / 2);
}

glm::vec3 Geometry::fromScreenCoordinates(const glm::vec2& point, const glm::mat4x4& matrixProjection) {
    return glm::vec3(point.x / matrixProjection[0][0],
                     point.y / matrixProjection[1][1],
                     -1.0);
}
