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
    void paintExample();
    void setRadius(double radius);
    QImage *getTextureImage();

private:
    double radius = 10;
    std::vector<Vertex> vertices;
    QImage *textureImage;

    void paintTriangle(QVector2D point1, QVector2D point2, QVector2D point3);
};

#endif // BRUSH_H
