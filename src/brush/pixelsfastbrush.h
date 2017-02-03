#ifndef PIXELSFASTBRUSH_H
#define PIXELSFASTBRUSH_H

#include <vector>
#include <unordered_set>

#include "geometry.h"
#include "abstractbrush.h"

class PixelsFastBrush : public AbstractBrush {
public:
    PixelsFastBrush(const ObjectModel& objectModel, const TextureStorage& textureStorage);
    virtual BrushStroke paint(const glm::i32vec2& point, const glm::mat4x4& matrixModelView, const glm::mat4x4& projection);

private:
    std::vector<std::vector<glm::u32vec2>> pixelsUvOfTriangle_;
    std::vector<glm::vec3> coordinatesFromUv_;

    std::unordered_set<size_t> getIntersectedTrianglesIds(const glm::i32vec2& brushCenter);
    void paintTriangle(size_t id, const glm::mat4x4& matrixModelView, const glm::mat4x4& projection,
                       const glm::i32vec2& brushCenter, BrushStroke &diff);

};

#endif // PIXELSFASTBRUSH_H
