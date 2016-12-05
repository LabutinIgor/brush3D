#ifndef SLOWRAYSBRUSH_H
#define SLOWRAYSBRUSH_H

#include <QOpenGLTexture>
#include <vector>

#include "geometry.h"
#include "vertex.h"
#include "abstractbrush.h"


class SlowRaysBrush : public AbstractBrush {
public:
    SlowRaysBrush(std::vector<Vertex> vertices, QImage* textureImage);
    virtual std::vector<std::pair<QPoint, std::pair<QColor, QColor>>>
        paint(QPoint point, QMatrix4x4 matrixModelView, QMatrix4x4 projection, QPoint screenSize);
    virtual std::vector<std::pair<QPoint, std::pair<QColor, QColor>>>
            paint(QPoint previousPoint, QPoint currentPoint,
                  QMatrix4x4 matrixModelView, QMatrix4x4 projection, QPoint screenSize);

private:
    void paintRound(QVector2D centerOfRound, float radius, std::vector<std::pair<QPoint, std::pair<QColor, QColor>>> &diff);
    void paintSmallRound(QVector2D point, QMatrix4x4 matrixModelView, QMatrix4x4 matrixProjection,
                         std::vector<std::pair<QPoint, std::pair<QColor, QColor>>> &diff);
};

#endif // SLOWRAYSBRUSH_H
