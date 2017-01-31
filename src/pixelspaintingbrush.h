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
    virtual std::vector<std::pair<glm::i32vec2, std::pair<QColor, QColor>>>
        paint(glm::i32vec2 point, glm::mat4x4 matrixModelView, glm::mat4x4 projection, glm::i32vec2 screenSize);

private:
    void paintPixel(glm::i32vec2 point, glm::mat4x4 matrixModelView, glm::mat4x4 projection, glm::i32vec2 screenSize,
                    std::vector<std::pair<glm::i32vec2, std::pair<QColor, QColor>>> &diff);
    std::vector<size_t> getIntersectedTrianglesIds(glm::i32vec2 point, glm::mat4x4 matrixModelView, glm::mat4x4 projection, glm::i32vec2 screenSize);
    void paintTriangle(glm::vec2 point1, glm::vec2 point2, glm::vec2 point3,
                       std::vector<std::pair<glm::i32vec2, std::pair<QColor, QColor>>> &diff);
    void paintIntersectionWithPyramid(std::vector<size_t> intersectedTrianglesIds,
                                      glm::vec3 ray1, glm::vec3 ray2, glm::vec3 ray3, glm::mat4x4 matrixModelView,
                                      std::vector<std::pair<glm::i32vec2, std::pair<QColor, QColor>>> &diff);
    void paintTriangle(glm::vec2 *points, std::vector<std::pair<glm::i32vec2, std::pair<QColor, QColor>>> &diff);
};

#endif // PIXELSPAINTINGBRUSH_H
