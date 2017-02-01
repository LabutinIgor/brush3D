#include "slowraysbrush.h"

SlowRaysBrush::SlowRaysBrush(std::vector<Vertex> vertices, TextureStorage* textureStorage) : AbstractBrush(vertices, textureStorage) {
}

void SlowRaysBrush::paintRound(glm::vec2 centerOfRound, float radius,
                               std::vector<std::pair<glm::i32vec2, std::pair<Color, Color>>> &diff) {
    float radiusSquare = radius * radius;
    centerOfRound.x *= textureStorage->getWidth();
    centerOfRound.y *= textureStorage->getHeight();

    int minX = fmax(0.0, round(centerOfRound.x - radius + 0.5));
    int maxX = fmin(textureStorage->getWidth() - 1, round(centerOfRound.x + radius - 0.5));

    for (int x = minX; x <= maxX; x++) {
        float dx = x - centerOfRound.x;
        float dy = sqrt(radiusSquare - dx * dx);
        int minY = fmax(0.0, round(centerOfRound.y - dy));
        int maxY = fmin(textureStorage->getHeight() - 1, round(centerOfRound.y + dy));

        for (int y = minY; y <= maxY; y++) {
            diff.push_back({glm::i32vec2(x, y), {textureStorage->getColor(x, y), color}});
            textureStorage->setColor(x, y, color);
        }
    }
}

std::vector<std::pair<glm::i32vec2, std::pair<Color, Color>>>
        SlowRaysBrush::paint(glm::i32vec2 point, glm::mat4x4 matrixModelView, glm::mat4x4 matrixProjection, glm::i32vec2 screenSize) {
    std::vector<std::pair<glm::i32vec2, std::pair<Color, Color>>> diff;
    glm::vec2 centerPoint(2.0 * point.x / screenSize.x - 1, 2.0 * (screenSize.y - point.y) / screenSize.y - 1);
    for (int dx = -30; dx <= 30; dx++) {
        for (int dy = -30; dy <= 30; dy++) {
            if (dx * dx + dy * dy <= 30) {
                paintSmallRound(glm::vec2(centerPoint.x + dx / 300.0, centerPoint.y + dy / 300.0), matrixModelView, matrixProjection, diff);
            }
        }
    }
    return diff;
}

void SlowRaysBrush::paintSmallRound(glm::vec2 point, glm::mat4x4 matrixModelView, glm::mat4x4 matrixProjection,
                                    std::vector<std::pair<glm::i32vec2, std::pair<Color, Color>>> &diff) {
    int intersectedTriangleId = -1;
    float minDistanceToIntersection = 0;
    glm::vec2 intersectionPoint;
    for (size_t i = 0; i < vertices.size() / 3; i++) {
        glm::vec3 vector1 = glm::vec3(matrixModelView * glm::vec4(vertices[3 * i].position(), 1.0));
        glm::vec3 vector2 = glm::vec3(matrixModelView * glm::vec4(vertices[3 * i + 1].position(), 1.0));
        glm::vec3 vector3 = glm::vec3(matrixModelView * glm::vec4(vertices[3 * i + 2].position(), 1.0));
        glm::vec3 rayVector = fromScreenCoordinates(point, matrixProjection);

        glm::vec3 *currentIntersectionPoint = Geometry::intersectRayAndTriangle(vector1, vector2, vector3, rayVector);

        if (currentIntersectionPoint != nullptr) {
            float currentDistanceToIntersection = glm::dot(*currentIntersectionPoint, *currentIntersectionPoint);
            if (intersectedTriangleId == -1 || currentDistanceToIntersection < minDistanceToIntersection) {
                intersectedTriangleId = i;
                minDistanceToIntersection = currentDistanceToIntersection;
                intersectionPoint = Geometry::getPointInUVCoordinates(vector2 - vector1,
                                                                      vector3 - vector1,
                                                                      *currentIntersectionPoint - vector1,
                                                                      vertices[3 * i].uv(),
                                                                      vertices[3 * i + 1].uv() - vertices[3 * i].uv(),
                                                                      vertices[3 * i + 2].uv() - vertices[3 * i].uv());
            }
        }
    }

    if (intersectedTriangleId != -1) {
        paintRound(intersectionPoint, radius / 5, diff);
    }
}
