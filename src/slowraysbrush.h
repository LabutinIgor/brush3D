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
    virtual std::vector<std::pair<glm::i32vec2, std::pair<QColor, QColor>>>
        paint(glm::i32vec2 point, glm::mat4x4 matrixModelView, glm::mat4x4 projection, glm::i32vec2 screenSize);

private:
    void paintRound(glm::vec2 centerOfRound, float radius, std::vector<std::pair<glm::i32vec2, std::pair<QColor, QColor>>> &diff);
    void paintSmallRound(glm::vec2 point, glm::mat4x4 matrixModelView, glm::mat4x4 matrixProjection,
                         std::vector<std::pair<glm::i32vec2, std::pair<QColor, QColor>>> &diff);
};

#endif // SLOWRAYSBRUSH_H
