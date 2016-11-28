#include "pixelspaintingbrush.h"

PixelsPaintingBrush::PixelsPaintingBrush(std::vector<Vertex> vertices, QImage* textureImage) : AbstractBrush(vertices, textureImage) {
}

void PixelsPaintingBrush::paint(QPoint point, QMatrix4x4 matrixModelView, QMatrix4x4 matrixProjection, QPoint screenSize) {
    for (int dx = -radius; dx <= radius; dx++) {
        for (int dy = -radius; dy <= radius; dy++) {
            if (dx * dx + dy * dy <= radius * radius) {
                paintPixel(QPoint(point.x() + dx, point.y() + dy), matrixModelView, matrixProjection, screenSize);
            }
        }
    }
}

void PixelsPaintingBrush::paintIntersectionWithPyramid(std::vector<size_t> intersectedTrianglesIds,
                                         QVector3D ray1, QVector3D ray2, QVector3D ray3, QMatrix4x4 matrixModelView) {
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

        paintTrianglesIntersection(vertices[3 * i].uv(),
                                   vertices[3 * i + 1].uv(),
                                   vertices[3 * i + 2].uv(),
                                   projectedRayInUV1,
                                   projectedRayInUV2,
                                   projectedRayInUV3);
    }
}

void PixelsPaintingBrush::paintTrianglesIntersection(QVector2D point1, QVector2D point2, QVector2D point3,
                                       QVector2D point4, QVector2D point5, QVector2D point6) {
    int minX = round(fmin(point1.x(), fmin(point2.x(), point3.x())) * textureImage->width());
    int maxX = round(fmax(point1.x(), fmax(point2.x(), point3.x())) * textureImage->width());

    for (int x = minX; x <= maxX; x++) {
        int minY = round(Geometry::getMinY(point1, point2, point3, x / (1.0 * textureImage->width()))
                         * textureImage->height());
        int maxY = round(Geometry::getMaxY(point1, point2, point3, x / (1.0 * textureImage->width()))
                         * textureImage->height());

        for (int y = minY; y <= maxY; y++) {
            if (Geometry::isPointInTriangle(QVector2D(x / (1.0 * textureImage->width()),
                                                      y / (1.0 * textureImage->height())),
                                          point4, point5, point6)) {
                textureImage->setPixelColor(QPoint(x, y), QColor(255, 0, 0));
            }
        }
    }
}

void PixelsPaintingBrush::paintPixel(QPoint point, QMatrix4x4 matrixModelView, QMatrix4x4 projection, QPoint screenSize) {
    std::vector<size_t> intersectedTrianglesIds = getIntersectedTrianglesIds(point, matrixModelView, projection, screenSize);
    if (intersectedTrianglesIds.empty()) {
        return;
    }
    QVector2D centerPoint(2.0 * point.x() / screenSize.x() - 1, 2.0 * (screenSize.y() - point.y()) / screenSize.y() - 1);
    QVector2D pixelSize(1.5 / screenSize.x(), 1.5 / screenSize.y());

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

    paintIntersectionWithPyramid(intersectedTrianglesIds, ray1, ray2, ray3, matrixModelView);
    paintIntersectionWithPyramid(intersectedTrianglesIds, ray2, ray3, ray4, matrixModelView);
}

std::vector<size_t> PixelsPaintingBrush::getIntersectedTrianglesIds(QPoint point, QMatrix4x4 matrixModelView, QMatrix4x4 projection, QPoint screenSize) {
    std::vector<size_t> ids;
    for (size_t i = 0; i < vertices.size() / 3; i++) {
        ids.push_back(i);
    }
    return ids;
}
