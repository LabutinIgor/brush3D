#include "pixelspaintingbrush.h"

PixelsPaintingBrush::PixelsPaintingBrush(std::vector<Vertex> vertices, QImage* textureImage) : AbstractBrush(vertices, textureImage) {
}

std::vector<std::pair<glm::i32vec2, std::pair<QColor, QColor>>>
        PixelsPaintingBrush::paint(glm::i32vec2 point, glm::mat4x4 matrixModelView, glm::mat4x4 matrixProjection, glm::i32vec2 screenSize) {
    std::vector<std::pair<glm::i32vec2, std::pair<QColor, QColor>>> diff;
    for (int dx = -radius; dx <= radius; dx++) {
        for (int dy = -radius; dy <= radius; dy++) {
            if (dx * dx + dy * dy <= radius * radius) {
                paintPixel(glm::i32vec2(point.x + dx, point.y + dy), matrixModelView, matrixProjection, screenSize, diff);
            }
        }
    }
    return diff;
}

void PixelsPaintingBrush::paintIntersectionWithPyramid(std::vector<size_t> intersectedTrianglesIds,
                                         glm::vec3 ray1, glm::vec3 ray2, glm::vec3 ray3, glm::mat4x4 matrixModelView,
                                                       std::vector<std::pair<glm::i32vec2, std::pair<QColor, QColor>>> &diff) {
    for (int i : intersectedTrianglesIds) {
        glm::vec3 vector1 = glm::vec3(matrixModelView * glm::vec4(vertices[3 * i].position(), 1.0));
        glm::vec3 vector2 = glm::vec3(matrixModelView * glm::vec4(vertices[3 * i + 1].position(), 1.0));
        glm::vec3 vector3 = glm::vec3(matrixModelView * glm::vec4(vertices[3 * i + 2].position(), 1.0));

        glm::vec3 *projectedRay1 = Geometry::intersectRayAndPlane(vector1, vector2, vector3, ray1);
        glm::vec3 *projectedRay2 = Geometry::intersectRayAndPlane(vector1, vector2, vector3, ray2);
        glm::vec3 *projectedRay3 = Geometry::intersectRayAndPlane(vector1, vector2, vector3, ray3);

        if (projectedRay1 == nullptr || projectedRay2 == nullptr || projectedRay3 == nullptr) {
            continue;
        }

        glm::vec2 projectedRayInUV1 = Geometry::getPointInUVCoordinates(vector2 - vector1, vector3 - vector1, *projectedRay1 - vector1,
                                                                        vertices[3 * i].uv(),
                                                                        vertices[3 * i + 1].uv() - vertices[3 * i].uv(),
                                                                        vertices[3 * i + 2].uv() - vertices[3 * i].uv());
        glm::vec2 projectedRayInUV2 = Geometry::getPointInUVCoordinates(vector2 - vector1, vector3 - vector1, *projectedRay2 - vector1,
                                                                        vertices[3 * i].uv(),
                                                                        vertices[3 * i + 1].uv() - vertices[3 * i].uv(),
                                                                        vertices[3 * i + 2].uv() - vertices[3 * i].uv());
        glm::vec2 projectedRayInUV3 = Geometry::getPointInUVCoordinates(vector2 - vector1, vector3 - vector1, *projectedRay3 - vector1,
                                                                        vertices[3 * i].uv(),
                                                                        vertices[3 * i + 1].uv() - vertices[3 * i].uv(),
                                                                        vertices[3 * i + 2].uv() - vertices[3 * i].uv());

        glm::vec2 triangle1[3] = {vertices[3 * i].uv(), vertices[3 * i + 1].uv(), vertices[3 * i + 2].uv()};
        glm::vec2 triangle2[3] = {projectedRayInUV1, projectedRayInUV2, projectedRayInUV3};
        //paintTriangle(triangle2, diff);
        std::vector<glm::vec2*> triangles = Geometry::intersectTriangles(triangle1, triangle2);

        for (glm::vec2 *triangle : triangles) {
            paintTriangle(triangle, diff);
        }
    }
}

void PixelsPaintingBrush::paintTriangle(glm::vec2 *points, std::vector<std::pair<glm::i32vec2, std::pair<QColor, QColor>>> &diff) {
    int minX = fmax(0, round(fmin(points[0].x, fmin(points[1].x, points[2].x)) * textureImage->width()));
    int maxX = fmin(textureImage->width() - 1, round(fmax(points[0].x, fmax(points[1].x, points[2].x)) * textureImage->width()));

    for (int x = minX; x <= maxX; x++) {
        int minY = fmax(0, round(Geometry::getMinY(points[0], points[1], points[2], x / (1.0 * textureImage->width()))
                           * textureImage->height()));
        int maxY = fmin(textureImage->height() - 1, round(Geometry::getMaxY(points[0], points[1], points[2], x / (1.0 * textureImage->width()))
                                                * textureImage->height()));

        for (int y = minY; y <= maxY; y++) {
            diff.push_back({glm::i32vec2(x, y), {textureImage->pixelColor(x, y), QColor(255, 0, 0)}});
            textureImage->setPixelColor(QPoint(x, y), color);
        }
    }
}

void PixelsPaintingBrush::paintPixel(glm::i32vec2 point, glm::mat4x4 matrixModelView, glm::mat4x4 projection, glm::i32vec2 screenSize,
                                     std::vector<std::pair<glm::i32vec2, std::pair<QColor, QColor>>> &diff) {
    std::vector<size_t> intersectedTrianglesIds = getIntersectedTrianglesIds(point, matrixModelView, projection, screenSize);
    if (intersectedTrianglesIds.empty()) {
        return;
    }
    glm::vec2 centerPoint(2.0 * point.x / screenSize.x - 1, 2.0 * (screenSize.y - point.y) / screenSize.y - 1);
    glm::vec2 pixelSize(3.0 / screenSize.x, 3.0 / screenSize.y);

    glm::vec3 ray1 = fromScreenCoordinates(centerPoint - pixelSize,
                                           projection);
    glm::vec3 ray2 = fromScreenCoordinates(glm::vec2(centerPoint.x + pixelSize.x,
                                                     centerPoint.y - pixelSize.x),
                                           projection);
    glm::vec3 ray3 = fromScreenCoordinates(glm::vec2(centerPoint.x - pixelSize.x,
                                                     centerPoint.y + pixelSize.y),
                                           projection);
    glm::vec3 ray4 = fromScreenCoordinates(centerPoint + pixelSize,
                                           projection);

    paintIntersectionWithPyramid(intersectedTrianglesIds, ray1, ray2, ray3, matrixModelView, diff);
    paintIntersectionWithPyramid(intersectedTrianglesIds, ray2, ray3, ray4, matrixModelView, diff);
}

std::vector<size_t> PixelsPaintingBrush::getIntersectedTrianglesIds(glm::i32vec2 point, glm::mat4x4 matrixModelView, glm::mat4x4 projection, glm::i32vec2 screenSize) {
    std::vector<size_t> ids;

    if (point.x >= 0 && point.y >= 0 && point.x < screenSize.x && point.y < screenSize.y
            && QColor(idsBuffer->pixel(point.x, point.y)).blue() < vertices.size() / 3) {
        QColor color = QColor(idsBuffer->pixel(point.x, point.y));
        ids.push_back(color.red() + 256 * color.green() + 256 * 256 * color.blue());
    }

    return ids;
}
