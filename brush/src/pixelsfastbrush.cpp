#include "include/pixelsfastbrush.hpp"
#include "src/details/utils.hpp"

namespace Brush {
    PixelsFastBrush::PixelsFastBrush(const ObjectModel& objectModel, TextureStorage& textureStorage)
            : AbstractBrush(objectModel, textureStorage), pixelsUvOfTriangle_(objectModel.getFacesNumber()),
              vertexFromUv_(textureStorage.getWidth(), textureStorage.getHeight()) {
        size_t w = textureStorage.getWidth();
        size_t h = textureStorage.getHeight();

        for (IdType faceId = 0; faceId < objectModel.getFacesNumber(); ++faceId) {
            Face face(objectModel, faceId);
            uint32_t minX = static_cast<uint32_t>(fmax(0, Utils::getMinUvX(face) * w));
            uint32_t maxX = static_cast<uint32_t>(fmin(Utils::getMaxUvX(face) * w, w - 1));

            for (uint32_t x = minX; x <= maxX; ++x) {
                float_t xUv = static_cast<float_t>(x / (1.0 * w));
                uint32_t minY = static_cast<uint32_t>(fmax(0, Utils::getMinY(face, xUv) * h));
                uint32_t maxY = static_cast<uint32_t>(fmin(h - 1, Utils::getMaxY(face, xUv) * h));

                for (uint32_t y = minY; y <= maxY; ++y) {
                    float_t yUv = static_cast<float_t >(y / (1.0 * h));
                    glm::vec3 point = Utils::getPointFromUVCoordinates(face.getUvs(), face.getPositions(),
                                                                       glm::vec2(xUv, yUv));
                    vertexFromUv_.setValue(x, y, point);
                    pixelsUvOfTriangle_[faceId].push_back(glm::u32vec2(x, y));
                }
            }
        }
    }

    BrushStroke PixelsFastBrush::paint(const glm::i32vec2& point, const glm::mat4x4& matrixModelView,
                                       const glm::mat4x4& matrixProjection, const IdsStorage& idsStorage) {
        BrushStroke diff;
        std::unordered_set<IdType> ids = calculateIntersectedTrianglesIds(matrixModelView, point, idsStorage);
        for (std::unordered_set<IdType>::iterator it = ids.begin(); it != ids.end(); it++) {
            paintTriangle(*it, matrixModelView, matrixProjection, point, idsStorage, diff);
        }
        return diff;
    }

    void
    PixelsFastBrush::paintTriangle(IdType id, const glm::mat4x4& matrixModelView, const glm::mat4x4& matrixProjection,
                                   const glm::i32vec2& brushCenter, const IdsStorage& idsStorage, BrushStroke& diff) {
        for (std::vector<glm::u32vec2>::iterator it = pixelsUvOfTriangle_[id].begin();
             it != pixelsUvOfTriangle_[id].end(); it++) {
            glm::u32vec2 pixel = *it;

            glm::vec3 point(matrixModelView * glm::vec4(vertexFromUv_.getValue(pixel), 1.0));
            glm::i32vec2 screenPoint(Utils::toScreenCoordinates(point, matrixProjection, idsStorage.getSize()));

            if (Utils::isInside(screenPoint, idsStorage.getSize())
                && isInsideBrush(screenPoint, brushCenter)
                && isVisible(screenPoint, id, idsStorage)) {
                diff.add(ColorChange(pixel, textureStorage_.getValue(pixel), getColor()));
                textureStorage_.setValue(pixel, getColor());
            }
        }
    }

    bool
    PixelsFastBrush::isVisible(const glm::vec2& screenPoint, IdType faceIdFromStorage,
                               const IdsStorage& idsStorage) const {
        return Utils::hasNeighbourWithId(idsStorage, screenPoint, faceIdFromStorage)
               || objectModel_.areAdjacentFaces(faceIdFromStorage, idsStorage.getValue(screenPoint));
    }

    std::unordered_set<IdType> PixelsFastBrush::calculateIntersectedTrianglesIds(const glm::mat4x4& matrixModelView,
                                                                                 const glm::i32vec2& brushCenter,
                                                                                 const IdsStorage& idsStorage) const {
        std::unordered_set<IdType> ids;
        for (int dx = (int) -getRadius(); dx <= getRadius(); ++dx) {
            for (int dy = (int) -getRadius(); dy <= getRadius(); ++dy) {
                if (dx * dx + dy * dy <= getRadius() * getRadius()) {
                    glm::i32vec2 point(brushCenter.x + dx, brushCenter.y + dy);
                    if (hasVisibleTriangleAtPoint(point, matrixModelView, idsStorage)) {
                        ids.insert(idsStorage.getValue(point));
                    }
                }
            }
        }
        return ids;
    }

    bool PixelsFastBrush::hasVisibleTriangleAtPoint(const glm::vec2& point, const glm::mat4x4& matrixModelView,
                                                    const IdsStorage& idsStorage) const {
        if (Utils::isInside(point, idsStorage.getSize())) {
            size_t id = idsStorage.getValue(point);
            return (id < objectModel_.getFacesNumber()
                    && Utils::calculateFaceAngleCos(Face(objectModel_, id), matrixModelView) < MIN_ANGLE_COS);
        }
        return false;
    }
}