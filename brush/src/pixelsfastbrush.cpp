#include "include/pixelsfastbrush.hpp"
#include "src/details/utils.hpp"

namespace Brush {
    PixelsFastBrush::PixelsFastBrush(const ObjectModel& objectModel, TextureStorage& textureStorage)
            : AbstractBrush(objectModel, textureStorage), pixelsUvOfTriangle_(objectModel.getFacesNumber()),
              vertexFromUv_(textureStorage.getWidth(), textureStorage.getHeight()) {
        for (IdType faceId = 0; faceId < objectModel.getFacesNumber(); ++faceId) {
            precalcForFace(faceId);
        }
    }

    void PixelsFastBrush::precalcForFace(IdType faceId) {
        size_t w = textureStorage_.getWidth();
        size_t h = textureStorage_.getHeight();
        Face face(objectModel_, faceId);
        uint32_t minX = static_cast<uint32_t>(fmax(0, Utils::getMinUvX(face) * w));
        uint32_t maxX = static_cast<uint32_t>(fmin(Utils::getMaxUvX(face) * w, w - 1));

        for (uint32_t x = minX; x <= maxX; ++x) {
            float xUv = static_cast<float>(x / (1.0 * w));
            uint32_t minY = static_cast<uint32_t>(fmax(0, Utils::getMinY(face, xUv) * h));
            uint32_t maxY = static_cast<uint32_t>(fmin(h - 1, Utils::getMaxY(face, xUv) * h));

            for (uint32_t y = minY; y <= maxY; ++y) {
                float yUv = static_cast<float>(y / (1.0 * h));
                glm::vec3 point = Utils::getPointFromUVCoordinates(face.getUvs(), face.getPositions(),
                                                                   glm::vec2(xUv, yUv));
                vertexFromUv_.setValue(x, y, point);
                pixelsUvOfTriangle_[faceId].push_back(glm::u32vec2(x, y));
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

    void PixelsFastBrush::paintTriangle(IdType id, const glm::mat4x4& matrixModelView,
                                        const glm::mat4x4& matrixProjection,
                                        const glm::i32vec2& brushCenter, const IdsStorage& idsStorage,
                                        BrushStroke& diff) {
        for (std::vector<glm::u32vec2>::iterator it = pixelsUvOfTriangle_[id].begin();
             it != pixelsUvOfTriangle_[id].end(); it++) {
            glm::u32vec2 pixel = *it;

            glm::vec3 point(matrixModelView * glm::vec4(vertexFromUv_.getValue(pixel), 1.0));
            glm::vec2 screenPoint(Utils::toScreenCoordinates(point, matrixProjection, idsStorage.getScreenSize()));
            glm::i32vec2 storagePoint(idsStorage.fromScreenCoord(screenPoint));
            if (Utils::isInside(screenPoint, idsStorage.getScreenSize())
                && Utils::isInsideRound(screenPoint, brushCenter, getRadius())
                && isVisible(storagePoint, id, idsStorage)) {
                diff.add(ColorChange(pixel, textureStorage_.getValue(pixel), getColor()));
                textureStorage_.setValue(pixel, getColor());
            }
        }
    }

    bool PixelsFastBrush::isVisible(const glm::vec2& storagePoint, IdType faceIdFromStorage,
                                    const IdsStorage& idsStorage) const {
        return Utils::hasNeighbourWithId(idsStorage, storagePoint, faceIdFromStorage)
               || objectModel_.areAdjacentFaces(faceIdFromStorage, idsStorage.getValue(storagePoint));
    }

    std::unordered_set<IdType> PixelsFastBrush::calculateIntersectedTrianglesIds(const glm::mat4x4& matrixModelView,
                                                                                 const glm::i32vec2& brushCenter,
                                                                                 const IdsStorage& idsStorage) const {
        std::unordered_set<IdType> ids;
        glm::vec2 centerPoint(brushCenter);
        glm::vec2 vectorR(getRadius(), getRadius());
        glm::i32vec2 leftPoint = idsStorage.fromScreenCoord(centerPoint - vectorR);
        glm::i32vec2 rightPoint = idsStorage.fromScreenCoord(centerPoint + vectorR);
        uint32_t minX = static_cast<uint32_t>(fmax(0, leftPoint.x));
        uint32_t maxX = static_cast<uint32_t>(fmax(0, fmin(idsStorage.getWidth(), rightPoint.x)));
        uint32_t minY = static_cast<uint32_t>(fmax(0, leftPoint.y));
        uint32_t maxY = static_cast<uint32_t>(fmax(0, fmin(idsStorage.getWidth(), rightPoint.y)));
        for (uint32_t x = minX; x < maxX; x++) {
            for (uint32_t y = minY; y < maxY; y++) {
                glm::i32vec2 point(x, y);
                if (Utils::isInsideRound(idsStorage.toScreenCoord(point), brushCenter, getRadius())
                    && hasVisibleTriangleAtPoint(point, matrixModelView, idsStorage)) {
                    ids.insert(idsStorage.getValue(point));
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