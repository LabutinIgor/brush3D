#ifndef ABSTRACTBRUSH_H
#define ABSTRACTBRUSH_H

#include <vector>
#include <glm.hpp>

#include "geometry.h"
#include "texturestorage.h"
#include "idsstorage.h"
#include "objectmodel.h"
#include "face.h"
#include "brushstroke.h"

class AbstractBrush {
public:
    AbstractBrush(const ObjectModel& objectModel, const TextureStorage& textureStorage);
    AbstractBrush(const AbstractBrush& other) = delete;
    AbstractBrush& operator=(const AbstractBrush& other) = delete;
    virtual ~AbstractBrush();
    virtual BrushStroke paint(const glm::i32vec2& point, const glm::mat4x4& matrixModelView, const glm::mat4x4& projection,
                              const IdsStorage& idsStorage) = 0;
    virtual BrushStroke paint(const glm::i32vec2& previousPoint, const glm::i32vec2& currentPoint,
                              const glm::mat4x4& matrixModelView, const glm::mat4x4& projection,
                              const IdsStorage& idsStorage);
    void setRadius(double radius);
    double getRadius() const;
    void setColor(const glm::u8vec3& color);
    glm::u8vec3 getColor() const;
    TextureStorage& getTextureStorage();

protected:
    const ObjectModel& objectModel_;
    TextureStorage textureStorage_;

private:
    double radius_ = 10;
    glm::u8vec3 color_ = glm::u8vec3(255, 0, 0);
};

#endif // ABSTRACTBRUSH_H
