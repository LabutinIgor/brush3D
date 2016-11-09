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
    static QVector3D *intersectRayAndTriangle(QVector3D p1, QVector3D p2, QVector3D p3, QVector3D p);
};

#endif // GEOMETRY_H
