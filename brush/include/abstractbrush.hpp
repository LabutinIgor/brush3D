#ifndef ABSTRACTBRUSH_H
#define ABSTRACTBRUSH_H

#include <vector>
#include <glm/glm.hpp>

#include "matrix.hpp"
#include "idsstorage.hpp"
#include "objectmodel.hpp"
#include "brushstroke.hpp"

namespace Brush {
    class AbstractBrush {
    public:
        AbstractBrush(const ObjectModel& objectModel, TextureStorage& textureStorage);
        virtual ~AbstractBrush();
        virtual BrushStroke
        paint(const glm::i32vec2& point, const glm::mat4x4& matrixModelView, const glm::mat4x4& matrixProjection,
              const IdsStorage& idsStorage) = 0;
        virtual BrushStroke paint(const glm::i32vec2& previousPoint, const glm::i32vec2& currentPoint,
                                  const glm::mat4x4& matrixModelView, const glm::mat4x4& matrixProjection,
                                  const IdsStorage& idsStorage);
        void setRadius(float radius);
        float getRadius() const;
        void setColor(const glm::u8vec3& color);
        glm::u8vec3 getColor() const;

    protected:
        const ObjectModel& objectModel_;
        TextureStorage& textureStorage_;

    private:
        float radius_;
        glm::u8vec3 color_;

        AbstractBrush(const AbstractBrush& other);
        AbstractBrush& operator=(const AbstractBrush& other);
    };
}

#endif // ABSTRACTBRUSH_H
