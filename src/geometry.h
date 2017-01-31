#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QVector2D>
#include <QMatrix4x4>
#include <iostream>
#include <math.h>

class Geometry
{
public:
    static double getMinY(QVector2D point1, QVector2D point2, QVector2D point3, double x);
    static double getMaxY(QVector2D point1, QVector2D point2, QVector2D point3, double x);
    static double getMinIntersectionPoint(QVector2D point1, QVector2D point2, double x);
    static double getMaxIntersectionPoint(QVector2D point1, QVector2D point2, double x);
    static double getIntersectionPoint(QVector2D point1, QVector2D point2, double x);
    static QVector3D *intersectRayAndTriangle(QVector3D p1, QVector3D p2, QVector3D p3, QVector3D ray);
    static QVector2D getPointInUVCoordinates(QVector3D p1, QVector3D p2, QVector3D p,
                                             QVector2D uv1, QVector2D uv2, QVector2D uv3);
    static QVector3D getPointFromUVCoordinates(QVector2D* pointsUV, QVector3D* points, QVector2D pointUV);
    static QPoint toScreenCoordinates(QVector3D point, QMatrix4x4 projection, QPoint screenSize);
    static QVector3D *intersectRayAndPlane(QVector3D p1, QVector3D p2, QVector3D p3, QVector3D ray);
    static bool isPointInTriangle(QVector2D point, QVector2D v1, QVector2D v2, QVector2D v3);
    static std::vector<QVector2D> getSegmentsIntersectionPoints(QVector2D *segment1, QVector2D *segment2);
    static std::vector<QVector2D*> intersectTriangles(QVector2D *triangle1, QVector2D *triangle2);
};

#endif // GEOMETRY_H
