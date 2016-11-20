#ifndef BRUSH_H
#define BRUSH_H

#include <QOpenGLTexture>
#include <vector>

#include "geometry.h"
#include "vertex.h"


class Brush {
public:
    Brush(std::vector<Vertex> vertices, QImage* textureImage);
    void paint(QVector2D point, QMatrix4x4 matrixModelView, QMatrix4x4 projection);
    void paintSlow(QVector2D point, QMatrix4x4 matrixModelView, QMatrix4x4 projection);

    void setRadius(double radius);
    QImage *getTextureImage();

private:
    double radius = 10;
    std::vector<Vertex> vertices;
    QImage *textureImage;

    void paintTriangle(QVector2D point1, QVector2D point2, QVector2D point3);
    void paintRound(QVector2D centerOfRound, float radius);
    void paintSmallRound(QVector2D point, QMatrix4x4 matrixModelView, QMatrix4x4 matrixProjection);
    QVector3D fromScreenCoordinates(QVector2D point, QMatrix4x4 matrixProjection);
    void paintIntersectionWithPyramid(QVector3D ray1, QVector3D ray2, QVector3D ray3, QMatrix4x4 matrixModelView);
    void paintTrianglesIntersection(QVector2D point1, QVector2D point2, QVector2D point3,
                                           QVector2D point4, QVector2D point5, QVector2D point6);
};

#endif // BRUSH_H
