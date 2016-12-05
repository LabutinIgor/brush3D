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
    virtual std::vector<std::pair<QPoint, std::pair<QColor, QColor>>>
        paint(QPoint point, QMatrix4x4 matrixModelView, QMatrix4x4 projection, QPoint screenSize);
    virtual std::vector<std::pair<QPoint, std::pair<QColor, QColor>>>
            paint(QPoint previousPoint, QPoint currentPoint,
                  QMatrix4x4 matrixModelView, QMatrix4x4 projection, QPoint screenSize);

private:
    void paintTriangle(QVector2D *points, std::vector<std::pair<QPoint, std::pair<QColor, QColor>>> &diff);
    void paintIntersectionWithPyramid(std::vector<size_t> intersectedTrianglesIds,
                                      QVector3D ray1, QVector3D ray2, QVector3D ray3, QMatrix4x4 matrixModelView,
                                      std::vector<std::pair<QPoint, std::pair<QColor, QColor>>> &diff);
};

#endif // BRUSH_H
