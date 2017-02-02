#include "pixelsfastbrush.h"

PixelsFastBrush::PixelsFastBrush(ObjectModel* objectModel, TextureStorage* TextureStorage) : AbstractBrush(objectModel, TextureStorage) {
}


std::vector<std::pair<glm::i32vec2, std::pair<Color, Color>>>
    PixelsFastBrush::paint(glm::i32vec2 point, glm::mat4x4 matrixModelView, glm::mat4x4 projection, glm::i32vec2 screenSize) {
    std::vector<std::pair<glm::i32vec2, std::pair<Color, Color>>> diff;

    auto ids = getIntersectedTrianglesIds(point, screenSize);
    for (size_t id : ids) {
        paintTriangle(id, matrixModelView, projection, screenSize, point, diff);
    }

    return diff;
}

void PixelsFastBrush::paintTriangle(size_t id, glm::mat4x4 matrixModelView, glm::mat4x4 projection, glm::i32vec2 screenSize, glm::i32vec2 brushCenter,
                                    std::vector<std::pair<glm::i32vec2, std::pair<Color, Color>>> &diff) {
    glm::vec2* pointsUV = objectModel->getFace(id).getUvTriangle();
    glm::vec3* points = objectModel->getFace(id).getCoordinatesTriangle();
    for (int i = 0; i < 3; i++) {
        points[i] = glm::vec3(matrixModelView * glm::vec4(points[i], 1.0));
    }

    int minX = fmax(0, round(fmin(pointsUV[0].x, fmin(pointsUV[1].x, pointsUV[2].x)) * textureStorage->getWidth()));
    int maxX = fmin(textureStorage->getWidth() - 1, round(fmax(pointsUV[0].x, fmax(pointsUV[1].x, pointsUV[2].x))
                                               * textureStorage->getWidth()));

    for (int x = minX; x <= maxX; x++) {
        int minY = fmax(0, round(Geometry::getMinY(pointsUV[0], pointsUV[1], pointsUV[2],
                                                   x / (1.0 * textureStorage->getWidth()))
                           * textureStorage->getHeight()));
        int maxY = fmin(textureStorage->getHeight() - 1, round(Geometry::getMaxY(pointsUV[0], pointsUV[1], pointsUV[2],
                                                                            x / (1.0 * textureStorage->getWidth()))
                                                    * textureStorage->getHeight()));

        for (int y = minY; y <= maxY; y++) {
            glm::vec3 point = Geometry::getPointFromUVCoordinates(pointsUV, points,
                                                                  glm::vec2(x / (1.0 * textureStorage->getWidth()),
                                                                            y / (1.0 * textureStorage->getHeight())));
            glm::i32vec2 screenPoint(Geometry::toScreenCoordinates(point, projection, screenSize));
            if (screenPoint.x >= 0 && screenPoint.y >= 0 && screenPoint.x < screenSize.x && screenPoint.y < screenSize.y
                    && glm::length(glm::vec2(brushCenter - screenPoint)) < radius
                    && (idsStorage->getId(screenPoint) == id
                        || idsStorage->getId(glm::i32vec2(screenPoint.x + 1, screenPoint.y)) == id
                        || idsStorage->getId(glm::i32vec2(screenPoint.x - 1, screenPoint.y)) == id
                        || idsStorage->getId(glm::i32vec2(screenPoint.x, screenPoint.y + 1)) == id
                        || idsStorage->getId(glm::i32vec2(screenPoint.x, screenPoint.y - 1)) == id)) {
                diff.push_back({glm::i32vec2(x, y), {textureStorage->getColor(x, y), color}});
                textureStorage->setColor(x, y, color);
            }
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
                        && idsStorage->getId(point.x, point.y) < objectModel->getFacesNumber()) {
                    ids.insert(idsStorage->getId(point.x, point.y));
                }
            }
        }
    }

    return ids;
}
