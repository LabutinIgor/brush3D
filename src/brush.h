#ifndef BRUSH_H
#define BRUSH_H

#include <QOpenGLTexture>
#include <vector>

#include "geometry.h"
#include "vertex.h"
#include "abstractbrush.h"


class Brush : public AbstractBrush {
public:
    Brush(std::vector<Vertex> vertices, QImage* textureImage);
    virtual void paint(QPoint point, QMatrix4x4 matrixModelView, QMatrix4x4 projection, QPoint screenSize);

private:
    void paintTriangle(QVector2D point1, QVector2D point2, QVector2D point3);
    void paintIntersectionWithPyramid(std::vector<size_t> intersectedTrianglesIds,
                                      QVector3D ray1, QVector3D ray2, QVector3D ray3, QMatrix4x4 matrixModelView);
    void paintTrianglesIntersection(QVector2D point1, QVector2D point2, QVector2D point3,
                                           QVector2D point4, QVector2D point5, QVector2D point6);
};

#endif // BRUSH_H
