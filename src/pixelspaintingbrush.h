#ifndef PIXELSPAINTINGBRUSH_H
#define PIXELSPAINTINGBRUSH_H

#include <QOpenGLTexture>
#include <vector>

#include "geometry.h"
#include "vertex.h"
#include "abstractbrush.h"

class PixelsPaintingBrush : public AbstractBrush {
public:
    PixelsPaintingBrush(std::vector<Vertex> vertices, QImage* textureImage);
    virtual std::vector<std::pair<QPoint, std::pair<QColor, QColor>>>
        paint(QPoint point, QMatrix4x4 matrixModelView, QMatrix4x4 projection, QPoint screenSize);
    virtual std::vector<std::pair<QPoint, std::pair<QColor, QColor>>>
            paint(QPoint previousPoint, QPoint currentPoint,
                  QMatrix4x4 matrixModelView, QMatrix4x4 projection, QPoint screenSize);

private:
    void paintPixel(QPoint point, QMatrix4x4 matrixModelView, QMatrix4x4 projection, QPoint screenSize,
                    std::vector<std::pair<QPoint, std::pair<QColor, QColor>>> &diff);
    std::vector<size_t> getIntersectedTrianglesIds(QPoint point, QMatrix4x4 matrixModelView, QMatrix4x4 projection, QPoint screenSize);
    void paintTriangle(QVector2D point1, QVector2D point2, QVector2D point3,
                       std::vector<std::pair<QPoint, std::pair<QColor, QColor>>> &diff);
    void paintIntersectionWithPyramid(std::vector<size_t> intersectedTrianglesIds,
                                      QVector3D ray1, QVector3D ray2, QVector3D ray3, QMatrix4x4 matrixModelView,
                                      std::vector<std::pair<QPoint, std::pair<QColor, QColor>>> &diff);
    void paintTriangle(QVector2D *points, std::vector<std::pair<QPoint, std::pair<QColor, QColor>>> &diff);
//    void paintTrianglesIntersection(QVector2D point1, QVector2D point2, QVector2D point3,
//                                           QVector2D point4, QVector2D point5, QVector2D point6);
};

#endif // PIXELSPAINTINGBRUSH_H
