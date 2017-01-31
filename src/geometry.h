#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QMatrix4x4>
#include <iostream>
#include <math.h>
#include <glm.hpp>

class Geometry
{
public:
    static double getMinY(glm::vec2 point1, glm::vec2 point2, glm::vec2 point3, double x);
    static double getMaxY(glm::vec2 point1, glm::vec2 point2, glm::vec2 point3, double x);
    static double getMinIntersectionPoint(glm::vec2 point1, glm::vec2 point2, double x);
    static double getMaxIntersectionPoint(glm::vec2 point1, glm::vec2 point2, double x);
    static double getIntersectionPoint(glm::vec2 point1, glm::vec2 point2, double x);
    static glm::vec3 *intersectRayAndTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 ray);
    static glm::vec2 getPointInUVCoordinates(glm::vec3 p1, glm::vec3 p2, glm::vec3 p,
                                             glm::vec2 uv1, glm::vec2 uv2, glm::vec2 uv3);
    static glm::vec3 getPointFromUVCoordinates(glm::vec2* pointsUV, glm::vec3* points, glm::vec2 pointUV);
    static glm::i32vec2 toScreenCoordinates(glm::vec3 point, glm::mat4x4 projection, glm::i32vec2 screenSize);
    static glm::vec3 *intersectRayAndPlane(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 ray);
    static bool isPointInTriangle(glm::vec2 point, glm::vec2 v1, glm::vec2 v2, glm::vec2 v3);
    static std::vector<glm::vec2> getSegmentsIntersectionPoints(glm::vec2 *segment1, glm::vec2 *segment2);
    static std::vector<glm::vec2*> intersectTriangles(glm::vec2 *triangle1, glm::vec2 *triangle2);
};

#endif // GEOMETRY_H
