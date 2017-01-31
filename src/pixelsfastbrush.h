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
    virtual std::vector<std::pair<glm::i32vec2, std::pair<QColor, QColor>>>
        paint(glm::i32vec2 point, glm::mat4x4 matrixModelView, glm::mat4x4 projection, glm::i32vec2 screenSize);

private:
    std::unordered_set<size_t> getIntersectedTrianglesIds(glm::i32vec2 brushCenter, glm::i32vec2 screenSize);
    void paintTriangle(size_t id, glm::mat4x4 matrixModelView, glm::mat4x4 projection, glm::i32vec2 screenSize, glm::i32vec2 brushCenter,
                       std::vector<std::pair<glm::i32vec2, std::pair<QColor, QColor>>> &diff);
    size_t getIdByPixel(glm::i32vec2 point);

};

#endif // PIXELSFASTBRUSH_H
