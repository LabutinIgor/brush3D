#include "brush.h"

Brush::Brush(std::vector<Vertex> vertices, QImage* textureImage) : AbstractBrush(vertices, textureImage) {
}

void Brush::paintTriangle(glm::vec2 *points, std::vector<std::pair<glm::i32vec2, std::pair<QColor, QColor>>> &diff) {
    int minX = round(fmin(points[0].x, fmin(points[1].x, points[2].x)) * textureImage->width());
    int maxX = round(fmax(points[0].x, fmax(points[1].x, points[2].x)) * textureImage->width());

    for (int x = fmax(0, minX); x <= fmin(textureImage->width(), maxX); x++) {
        int minY = round(Geometry::getMinY(points[0], points[1], points[2], x / (1.0 * textureImage->width()))
                         * textureImage->height());
        int maxY = round(Geometry::getMaxY(points[0], points[1], points[2], x / (1.0 * textureImage->width()))
                         * textureImage->height());

        for (int y = fmax(0, minY); y <= fmin(textureImage->height(), maxY); y++) {
            diff.push_back({glm::i32vec2(x, y), {textureImage->pixelColor(x, y), color}});
            textureImage->setPixelColor(QPoint(x, y), color);
        }
    }
}

std::vector<std::pair<glm::i32vec2, std::pair<QColor, QColor>>>
        Brush::paint(glm::i32vec2 point, glm::mat4x4 matrixModelView, glm::mat4x4 matrixProjection, glm::i32vec2 screenSize) {
    std::vector<std::pair<glm::i32vec2, std::pair<QColor, QColor>>> diff;
    glm::vec2 centerPoint(2.0 * point.x / screenSize.x - 1, 2.0 * (screenSize.y - point.y) / screenSize.y - 1);
    glm::vec3 centerRay = fromScreenCoordinates(centerPoint, matrixProjection);

    const int POINTS_CNT = 4;
    for (int i = 0; i < POINTS_CNT; i++) {
        glm::vec3 ray1 = fromScreenCoordinates(glm::vec2(centerPoint.x + radius * cos(2 * M_PI * i / POINTS_CNT) / textureImage->width(),
                                                         centerPoint.y + radius * sin(2 * M_PI * i / POINTS_CNT) / textureImage->height()),
                                                 matrixProjection);
        glm::vec3 ray2 = fromScreenCoordinates(glm::vec2(centerPoint.x + radius * cos(2 * M_PI * (i + 1) / POINTS_CNT) / textureImage->width(),
                                                         centerPoint.y + radius * sin(2 * M_PI * (i + 1) / POINTS_CNT) / textureImage->height()),
                                                 matrixProjection);

        std::vector<size_t> ids;

        for (size_t i = 0; i < vertices.size() / 3; i++) {
            glm::vec3 vector1 = glm::vec3(matrixModelView * glm::vec4(vertices[3 * i].position(), 1.0));
            glm::vec3 vector2 = glm::vec3(matrixModelView * glm::vec4(vertices[3 * i + 1].position(), 1.0));
            glm::vec3 vector3 = glm::vec3(matrixModelView * glm::vec4(vertices[3 * i + 2].position(), 1.0));
            if (glm::cross(vector2 - vector1, vector3 - vector1).z > 1e-3) {
                ids.push_back(i);
            }
        }

        paintIntersectionWithPyramid(ids, centerRay, ray1, ray2, matrixModelView, diff);
    }
    return diff;
}

void Brush::paintIntersectionWithPyramid(std::vector<size_t> intersectedTrianglesIds,
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
        std::vector<glm::vec2*> triangles = Geometry::intersectTriangles(triangle1, triangle2);

        for (glm::vec2 *triangle : triangles) {
            paintTriangle(triangle, diff);
        }
    }
}
