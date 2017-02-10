#ifndef ABSTRACTBRUSH_H
#define ABSTRACTBRUSH_H

#include <vector>
#include <glm.hpp>

#include "matrix.hpp"
#include "objectmodel.hpp"
#include "brushstroke.hpp"

class AbstractBrush {
public:
    AbstractBrush(const ObjectModel &objectModel, TextureStorage &textureStorage);

    AbstractBrush(const AbstractBrush &other) = delete;

    AbstractBrush &operator=(const AbstractBrush &other) = delete;

    virtual ~AbstractBrush();

    virtual BrushStroke
    paint(const glm::i32vec2 &point, const glm::mat4x4 &matrixModelView, const glm::mat4x4 &matrixProjection,
          const IdsStorage &idsStorage) = 0;

    virtual BrushStroke paint(const glm::i32vec2 &previousPoint, const glm::i32vec2 &currentPoint,
                              const glm::mat4x4 &matrixModelView, const glm::mat4x4 &matrixProjection,
                              const IdsStorage &idsStorage);

    bool isInsideBrush(const glm::i32vec2 &screenPoint, const glm::i32vec2 &brushCenter) const;

    void setRadius(double radius);

    double getRadius() const;

    void setColor(const glm::u8vec3 &color);

    glm::u8vec3 getColor() const;

protected:
    const ObjectModel &objectModel_;
    TextureStorage &textureStorage_;

private:
    double radius_ = 10;
    glm::u8vec3 color_ = glm::u8vec3(255, 0, 0);
};

#endif // ABSTRACTBRUSH_H
