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
    virtual std::vector<std::pair<glm::i32vec2, std::pair<QColor, QColor>>>
        paint(glm::i32vec2 point, glm::mat4x4 matrixModelView, glm::mat4x4 projection, glm::i32vec2 screenSize);

private:
    void paintTriangle(glm::vec2 *points, std::vector<std::pair<glm::i32vec2, std::pair<QColor, QColor>>> &diff);
    void paintIntersectionWithPyramid(std::vector<size_t> intersectedTrianglesIds,
                                      glm::vec3 ray1, glm::vec3 ray2, glm::vec3 ray3, glm::mat4x4 matrixModelView,
                                      std::vector<std::pair<glm::i32vec2, std::pair<QColor, QColor>>> &diff);
};

#endif // BRUSH_H
