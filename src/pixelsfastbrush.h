#ifndef PIXELSFASTBRUSH_H
#define PIXELSFASTBRUSH_H


#include <QOpenGLTexture>
#include <vector>
#include <unordered_set>

#include "geometry.h"
#include "vertex.h"
#include "abstractbrush.h"


class PixelsFastBrush : public AbstractBrush {
public:
    PixelsFastBrush(std::vector<Vertex> vertices, QImage* textureImage);
    virtual std::vector<std::pair<QPoint, std::pair<QColor, QColor>>>
        paint(QPoint point, QMatrix4x4 matrixModelView, QMatrix4x4 projection, QPoint screenSize);

private:
    std::unordered_set<size_t> getIntersectedTrianglesIds(QPoint brushCenter, QPoint screenSize);
    void paintTriangle(size_t id, QMatrix4x4 matrixModelView, QMatrix4x4 projection, QPoint screenSize, QPoint brushCenter,
                       std::vector<std::pair<QPoint, std::pair<QColor, QColor>>> &diff);
    size_t getIdByPixel(QPoint point);

};

#endif // PIXELSFASTBRUSH_H
