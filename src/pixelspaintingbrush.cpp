#include "pixelspaintingbrush.h"

PixelsPaintingBrush::PixelsPaintingBrush(std::vector<Vertex> vertices, QImage* textureImage) : AbstractBrush(vertices, textureImage) {
}

std::vector<std::pair<QPoint, std::pair<QColor, QColor>>>
        PixelsPaintingBrush::paint(QPoint point, QMatrix4x4 matrixModelView, QMatrix4x4 matrixProjection, QPoint screenSize) {
    std::vector<std::pair<QPoint, std::pair<QColor, QColor>>> diff;
    for (int dx = -radius; dx <= radius; dx++) {
        for (int dy = -radius; dy <= radius; dy++) {
            if (dx * dx + dy * dy <= radius * radius) {
                paintPixel(QPoint(point.x() + dx, point.y() + dy), matrixModelView, matrixProjection, screenSize, diff);
            }
        }
    }
    return diff;
}

void PixelsPaintingBrush::paintIntersectionWithPyramid(std::vector<size_t> intersectedTrianglesIds,
                                         QVector3D ray1, QVector3D ray2, QVector3D ray3, QMatrix4x4 matrixModelView,
                                                       std::vector<std::pair<QPoint, std::pair<QColor, QColor>>> &diff) {
    for (int i : intersectedTrianglesIds) {
        QVector3D vector1 = QVector3D(matrixModelView * QVector4D(vertices[3 * i].position(), 1.0));
        QVector3D vector2 = QVector3D(matrixModelView * QVector4D(vertices[3 * i + 1].position(), 1.0));
        QVector3D vector3 = QVector3D(matrixModelView * QVector4D(vertices[3 * i + 2].position(), 1.0));

        QVector3D *projectedRay1 = Geometry::intersectRayAndPlane(vector1, vector2, vector3, ray1);
        QVector3D *projectedRay2 = Geometry::intersectRayAndPlane(vector1, vector2, vector3, ray2);
        QVector3D *projectedRay3 = Geometry::intersectRayAndPlane(vector1, vector2, vector3, ray3);

        if (projectedRay1 == nullptr || projectedRay2 == nullptr || projectedRay3 == nullptr) {
            continue;
        }

        QVector2D projectedRayInUV1 = Geometry::getPointInUVCoordinates(vector2 - vector1, vector3 - vector1, *projectedRay1 - vector1,
                                                                        vertices[3 * i].uv(),
                                                                        vertices[3 * i + 1].uv() - vertices[3 * i].uv(),
                                                                        vertices[3 * i + 2].uv() - vertices[3 * i].uv());
        QVector2D projectedRayInUV2 = Geometry::getPointInUVCoordinates(vector2 - vector1, vector3 - vector1, *projectedRay2 - vector1,
                                                                        vertices[3 * i].uv(),
                                                                        vertices[3 * i + 1].uv() - vertices[3 * i].uv(),
                                                                        vertices[3 * i + 2].uv() - vertices[3 * i].uv());
        QVector2D projectedRayInUV3 = Geometry::getPointInUVCoordinates(vector2 - vector1, vector3 - vector1, *projectedRay3 - vector1,
                                                                        vertices[3 * i].uv(),
                                                                        vertices[3 * i + 1].uv() - vertices[3 * i].uv(),
                                                                        vertices[3 * i + 2].uv() - vertices[3 * i].uv());

        QVector2D triangle1[3] = {vertices[3 * i].uv(), vertices[3 * i + 1].uv(), vertices[3 * i + 2].uv()};
        QVector2D triangle2[3] = {projectedRayInUV1, projectedRayInUV2, projectedRayInUV3};
        //paintTriangle(triangle2, diff);
        std::vector<QVector2D*> triangles = Geometry::intersectTriangles(triangle1, triangle2);

        for (QVector2D *triangle : triangles) {
            paintTriangle(triangle, diff);
        }
    }
}

void PixelsPaintingBrush::paintTriangle(QVector2D *points, std::vector<std::pair<QPoint, std::pair<QColor, QColor>>> &diff) {
    int minX = fmax(0, round(fmin(points[0].x(), fmin(points[1].x(), points[2].x())) * textureImage->width()));
    int maxX = fmin(textureImage->width() - 1, round(fmax(points[0].x(), fmax(points[1].x(), points[2].x())) * textureImage->width()));

    for (int x = minX; x <= maxX; x++) {
        int minY = fmax(0, round(Geometry::getMinY(points[0], points[1], points[2], x / (1.0 * textureImage->width()))
                           * textureImage->height()));
        int maxY = fmin(textureImage->height() - 1, round(Geometry::getMaxY(points[0], points[1], points[2], x / (1.0 * textureImage->width()))
                                                * textureImage->height()));

        for (int y = minY; y <= maxY; y++) {
            diff.push_back({QPoint(x, y), {textureImage->pixelColor(x, y), QColor(255, 0, 0)}});
            textureImage->setPixelColor(QPoint(x, y), color);
        }
    }
}

void PixelsPaintingBrush::paintPixel(QPoint point, QMatrix4x4 matrixModelView, QMatrix4x4 projection, QPoint screenSize,
                                     std::vector<std::pair<QPoint, std::pair<QColor, QColor>>> &diff) {
    std::vector<size_t> intersectedTrianglesIds = getIntersectedTrianglesIds(point, matrixModelView, projection, screenSize);
    if (intersectedTrianglesIds.empty()) {
        return;
    }
    QVector2D centerPoint(2.0 * point.x() / screenSize.x() - 1, 2.0 * (screenSize.y() - point.y()) / screenSize.y() - 1);
    QVector2D pixelSize(3.0 / screenSize.x(), 3.0 / screenSize.y());

    QVector3D ray1 = fromScreenCoordinates(centerPoint - pixelSize,
                                           projection);
    QVector3D ray2 = fromScreenCoordinates(QVector2D(centerPoint.x() + pixelSize.x(),
                                                     centerPoint.y() - pixelSize.x()),
                                           projection);
    QVector3D ray3 = fromScreenCoordinates(QVector2D(centerPoint.x() - pixelSize.x(),
                                                     centerPoint.y() + pixelSize.y()),
                                           projection);
    QVector3D ray4 = fromScreenCoordinates(centerPoint + pixelSize,
                                           projection);

    paintIntersectionWithPyramid(intersectedTrianglesIds, ray1, ray2, ray3, matrixModelView, diff);
    paintIntersectionWithPyramid(intersectedTrianglesIds, ray2, ray3, ray4, matrixModelView, diff);
}

std::vector<size_t> PixelsPaintingBrush::getIntersectedTrianglesIds(QPoint point, QMatrix4x4 matrixModelView, QMatrix4x4 projection, QPoint screenSize) {
    std::vector<size_t> ids;

    if (point.x() >= 0 && point.y() >= 0 && point.x() < screenSize.x() && point.y() < screenSize.y()
            && QColor(idsBuffer->pixel(point)).blue() < vertices.size() / 3) {
        QColor color = QColor(idsBuffer->pixel(point));
        ids.push_back(color.red() + 256 * color.green() + 256 * 256 * color.blue());
    }

    return ids;
}
