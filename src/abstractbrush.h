#ifndef ABSTRACTBRUSH_H
#define ABSTRACTBRUSH_H

#include <QOpenGLTexture>
#include <vector>

#include "geometry.h"
#include "vertex.h"

class AbstractBrush
{
public:
    AbstractBrush(std::vector<Vertex> vertices, QImage* textureImage);
    virtual std::vector<std::pair<QPoint, std::pair<QColor, QColor>>>
        paint(QPoint point, QMatrix4x4 matrixModelView, QMatrix4x4 projection, QPoint screenSize) = 0;
    virtual std::vector<std::pair<QPoint, std::pair<QColor, QColor>>>
            paint(QPoint previousPoint, QPoint currentPoint,
                  QMatrix4x4 matrixModelView, QMatrix4x4 projection, QPoint screenSize) = 0;
    void setRadius(double radius);
    QImage *getTextureImage();

protected:
    double radius = 10;
    std::vector<Vertex> vertices;
    QImage *textureImage;

    QVector3D fromScreenCoordinates(QVector2D point, QMatrix4x4 matrixProjection);
};

#endif // ABSTRACTBRUSH_H
