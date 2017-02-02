#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QMatrix4x4>
#include <iostream>
#include <math.h>
#include <glm.hpp>

class Geometry
{
public:
    static double getMinY(glm::vec2* triangle, double x);
    static double getMaxY(glm::vec2* triangle, double x);
    static double getMinIntersectionPoint(glm::vec2 point1, glm::vec2 point2, double x);
    static double getMaxIntersectionPoint(glm::vec2 point1, glm::vec2 point2, double x);
    static double getIntersectionPoint(glm::vec2 point1, glm::vec2 point2, double x);
    static glm::vec3 getPointFromUVCoordinates(glm::vec2* pointsUV, glm::vec3* points, glm::vec2 pointUV);
    static glm::i32vec2 toScreenCoordinates(glm::vec3 point, glm::mat4x4 projection, glm::i32vec2 screenSize);
    static glm::vec3 fromScreenCoordinates(glm::vec2 point, glm::mat4x4 matrixProjection);
};

#endif // GEOMETRY_H
