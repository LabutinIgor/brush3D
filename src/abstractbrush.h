#ifndef ABSTRACTBRUSH_H
#define ABSTRACTBRUSH_H

#include <QOpenGLTexture>
#include <QColor>
#include <vector>

#include "geometry.h"
#include "vertex.h"
#include "glm.hpp"

class AbstractBrush
{
public:
    AbstractBrush(std::vector<Vertex> vertices, QImage* textureImage);
    virtual std::vector<std::pair<glm::i32vec2, std::pair<QColor, QColor>>>
        paint(glm::i32vec2 point, glm::mat4x4 matrixModelView, glm::mat4x4 projection, glm::i32vec2 screenSize) = 0;
    virtual std::vector<std::pair<glm::i32vec2, std::pair<QColor, QColor>>>
            paint(glm::i32vec2 previousPoint, glm::i32vec2 currentPoint,
                  glm::mat4x4 matrixModelView, glm::mat4x4 projection, glm::i32vec2 screenSize);
    void setRadius(double radius);
    void setColor(QColor color);
    void setIdsBuffer(QImage *idsBuffer);
    QImage *getTextureImage();

protected:
    double radius = 10;
    QColor color = QColor(255, 0, 0);
    std::vector<Vertex> vertices;
    QImage *textureImage;
    QImage *idsBuffer = 0;

    glm::vec3 fromScreenCoordinates(glm::vec2 point, glm::mat4x4 matrixProjection);
};

#endif // ABSTRACTBRUSH_H
