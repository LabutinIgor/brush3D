#ifndef PIXELSFASTBRUSH_H
#define PIXELSFASTBRUSH_H

#include <vector>
#include <unordered_set>

#include "geometry.h"
#include "abstractbrush.h"

class PixelsFastBrush : public AbstractBrush {
public:
    PixelsFastBrush(const ObjectModel& objectModel, const TextureStorage& textureStorage);
    virtual BrushStroke paint(glm::i32vec2 point, glm::mat4x4 matrixModelView, glm::mat4x4 projection, glm::i32vec2 screenSize);

private:
    std::vector<std::vector<glm::u32vec2>> pixelsUvOfTriangle;
    std::vector<glm::vec3> coordinatesFromUv;

    std::unordered_set<size_t> getIntersectedTrianglesIds(glm::i32vec2 brushCenter, glm::i32vec2 screenSize);
    void paintTriangle(size_t id, glm::mat4x4 matrixModelView, glm::mat4x4 projection, glm::i32vec2 screenSize, glm::i32vec2 brushCenter,
                       BrushStroke &diff);

};

#endif // PIXELSFASTBRUSH_H
