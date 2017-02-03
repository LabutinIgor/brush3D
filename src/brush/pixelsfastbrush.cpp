#include "pixelsfastbrush.h"

PixelsFastBrush::PixelsFastBrush(const ObjectModel& objectModel, const TextureStorage& textureStorage) : AbstractBrush(objectModel, textureStorage) {
    uint32_t w = textureStorage.getWidth();
    uint32_t h = textureStorage.getHeight();
    coordinatesFromUv.resize(w * h);
    pixelsUvOfTriangle.resize(objectModel.getFacesNumber());

    for (uint32_t faceId = 0; faceId < objectModel.getFacesNumber(); faceId++) {
        Face face(objectModel, faceId);

        int minX = fmax(0, round(fmin(face.getUv(0).x, fmin(face.getUv(1).x, face.getUv(2).x)) * textureStorage.getWidth()));
        int maxX = fmin(textureStorage.getWidth() - 1, round(fmax(face.getUv(0).x, fmax(face.getUv(1).x, face.getUv(2).x))
                                                   * textureStorage.getWidth()));

        for (int x = minX; x <= maxX; x++) {
            float xUv = x / (1.0 * textureStorage.getWidth());
            int minY = fmax(0, round(Geometry::getMinY(face.getUvs(), xUv) * textureStorage.getHeight()));
            int maxY = fmin(textureStorage.getHeight() - 1, round(Geometry::getMaxY(face.getUvs(), xUv) * textureStorage.getHeight()));

            for (int y = minY; y <= maxY; y++) {
                float yUv = y / (1.0 * textureStorage.getHeight());
                glm::vec3 point = Geometry::getPointFromUVCoordinates(face.getUvs(), face.getCoordinates(), glm::vec2(xUv, yUv));
                coordinatesFromUv[x * h + y] = point;
                pixelsUvOfTriangle[faceId].push_back(glm::u32vec2(x, y));
            }
        }
    }
}

BrushStroke PixelsFastBrush::paint(glm::i32vec2 point, glm::mat4x4 matrixModelView, glm::mat4x4 projection, glm::i32vec2 screenSize) {
    BrushStroke diff;
    auto ids = getIntersectedTrianglesIds(point, screenSize);
    for (size_t id : ids) {
        paintTriangle(id, matrixModelView, projection, screenSize, point, diff);
    }
    return diff;
}

void PixelsFastBrush::paintTriangle(size_t id, glm::mat4x4 matrixModelView, glm::mat4x4 projection, glm::i32vec2 screenSize, glm::i32vec2 brushCenter,
                                    BrushStroke &diff) {
    for (glm::u32vec2 pixel : pixelsUvOfTriangle[id]) {
        glm::vec3 point(matrixModelView * glm::vec4(coordinatesFromUv[pixel.x * textureStorage.getHeight() + pixel.y], 1.0));
        glm::i32vec2 screenPoint(Geometry::toScreenCoordinates(point, projection, screenSize));

        if (screenPoint.x >= 0 && screenPoint.y >= 0 && screenPoint.x < screenSize.x && screenPoint.y < screenSize.y
                && glm::length(glm::vec2(brushCenter - screenPoint)) < radius
                && ((idsStorage.hasNeighbourWithId(screenPoint, id)) || (objectModel.areAdjacentFaces(id, idsStorage.getId(screenPoint))))) {
            diff.add(ColorChange(pixel, textureStorage.getColor(pixel), color));
            textureStorage.setColor(pixel, color);
        }
    }
}

std::unordered_set<size_t> PixelsFastBrush::getIntersectedTrianglesIds(glm::i32vec2 brushCenter, glm::i32vec2 screenSize) {
    std::unordered_set<size_t> ids;
    for (int dx = -radius; dx <= radius; dx++) {
        for (int dy = -radius; dy <= radius; dy++) {
            if (dx * dx + dy * dy <= radius * radius) {
                glm::i32vec2 point(brushCenter.x + dx, brushCenter.y + dy);
                if (point.x >= 0 && point.y >= 0 && point.x < screenSize.x && point.y < screenSize.y
                        && idsStorage.getId(point.x, point.y) < objectModel.getFacesNumber()) {
                    ids.insert(idsStorage.getId(point.x, point.y));
                }
            }
        }
    }
    return ids;
}
