#include "pixelsfastbrush.h"

PixelsFastBrush::PixelsFastBrush(const ObjectModel& objectModel, const TextureStorage& textureStorage)
        : AbstractBrush(objectModel, textureStorage), pixelsUvOfTriangle_(objectModel.getFacesNumber()),
          vertexFromUv_(textureStorage.getWidth() * textureStorage.getHeight())  {

    size_t w = textureStorage.getWidth();
    size_t h = textureStorage.getHeight();
    for (IdType faceId = 0; faceId < objectModel.getFacesNumber(); ++faceId) {
        Face face(objectModel, faceId);

        int minX = face.getUv(0).x * w;
        int maxX = face.getUv(0).x * w;
        for (size_t i = 0; i < face.getSize(); ++i) {
            minX = round(fmin(minX, face.getUv(i).x * w));
            maxX = round(fmax(maxX, face.getUv(i).x * w));
        }
        minX = fmax(0, minX);
        maxX = fmin(maxX, w - 1);

        for (int x = minX; x <= maxX; ++x) {
            float xUv = x / (1.0 * w);

            int minY = fmax(0, round(Geometry::getMinY(face.getUvs(), xUv) * h));
            int maxY = fmin(h - 1, round(Geometry::getMaxY(face.getUvs(), xUv) * h));

            for (int y = minY; y <= maxY; ++y) {
                float yUv = y / (1.0 * h);

                glm::vec3 point = Geometry::getPointFromUVCoordinates(face.getUvs(), face.getPositions(), glm::vec2(xUv, yUv));
                vertexFromUv_[x * h + y] = point;
                pixelsUvOfTriangle_[faceId].push_back(glm::u32vec2(x, y));
            }
        }
    }
}

BrushStroke PixelsFastBrush::paint(const glm::i32vec2& point, const glm::mat4x4& matrixModelView, const glm::mat4x4& projection,
                                   const IdsStorage& idsStorage) {
    BrushStroke diff;
    auto ids = calculateIntersectedTrianglesIds(matrixModelView, point, idsStorage);
    for (size_t id : ids) {
        paintTriangle(id, matrixModelView, projection, point, idsStorage, diff);
    }
    return diff;
}

void PixelsFastBrush::paintTriangle(IdType id, const glm::mat4x4& matrixModelView, const glm::mat4x4& projection, const glm::i32vec2& brushCenter,
                                    const IdsStorage& idsStorage, BrushStroke &diff) {
    for (glm::u32vec2 pixel : pixelsUvOfTriangle_[id]) {
        glm::vec3 point(matrixModelView * glm::vec4(vertexFromUv_[pixel.x * textureStorage_.getHeight() + pixel.y], 1.0));

        glm::i32vec2 screenPoint(Geometry::toScreenCoordinates(point, projection, idsStorage.getSize()));

        if (isInside(screenPoint, idsStorage.getSize())
                && isInsideBrush(screenPoint, brushCenter)
                && isVisible(screenPoint, id, idsStorage)) {
            diff.add(ColorChange(pixel, textureStorage_.getColor(pixel), getColor()));
            textureStorage_.setColor(pixel, getColor());
        }
    }
}

bool PixelsFastBrush::isVisible(const glm::vec2& screenPoint, IdType faceIdFromStorage, const IdsStorage& idsStorage) const {
    return idsStorage.hasNeighbourWithId(screenPoint, faceIdFromStorage)
            || objectModel_.areAdjacentFaces(faceIdFromStorage, idsStorage.getId(screenPoint));
}

bool PixelsFastBrush::isInside(const glm::i32vec2& point, const glm::i32vec2& rectangle) const {
    return point.x >= 0 && point.y >= 0 && point.x < rectangle.x && point.y < rectangle.y;
}

std::unordered_set<IdType> PixelsFastBrush::calculateIntersectedTrianglesIds(const glm::mat4x4& matrixModelView,
                                                                             const glm::i32vec2& brushCenter, const IdsStorage& idsStorage) const {
    std::unordered_set<IdType> ids;
    for (int dx = -getRadius(); dx <= getRadius(); ++dx) {
        for (int dy = -getRadius(); dy <= getRadius(); ++dy) {
            if (dx * dx + dy * dy <= getRadius() * getRadius()) {
                glm::i32vec2 point(brushCenter.x + dx, brushCenter.y + dy);
                if (hasVisibleTriangleAtPoint(point, matrixModelView, idsStorage)) {
                    ids.insert(idsStorage.getId(point));
                }
            }
        }
    }
    return ids;
}

bool PixelsFastBrush::hasVisibleTriangleAtPoint(const glm::vec2& point, const glm::mat4x4& matrixModelView,
                                                const IdsStorage& idsStorage) const {
    if (isInside(point, idsStorage.getSize())) {
        size_t id = idsStorage.getId(point);
        return (id < objectModel_.getFacesNumber()
                && calculateFaceAngleCos(Face(objectModel_, id), matrixModelView) < MIN_ANGLE_COS);
    }
    return false;
}

float_t PixelsFastBrush::calculateFaceAngleCos(Face face, const glm::mat4x4& matrixModelView) const {
    std::vector<glm::vec3> points(3);
    for (int i = 0; i < 3; ++i) {
        points[i] = glm::vec3(matrixModelView * glm::vec4(face.getPosition(i), 1.0));
    }
    glm::vec3 normal = glm::cross(points[1] - points[0], points[2] - points[0]);
    normal /= glm::length(normal);

    glm::vec3 faceCenter((points[0] + points[1] + points[2]) / 3.0f);
    faceCenter /= glm::length(faceCenter);

    return glm::dot(normal, faceCenter);
}
