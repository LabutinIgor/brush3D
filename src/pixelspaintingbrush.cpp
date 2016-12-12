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

std::vector<std::pair<QPoint, std::pair<QColor, QColor>>>
        PixelsPaintingBrush::paint(QPoint previousPoint, QPoint lastPoint,
              QMatrix4x4 matrixModelView, QMatrix4x4 projection, QPoint screenSize) {
    std::vector<std::pair<QPoint, std::pair<QColor, QColor>>> diff;

    int cntRounds = sqrt(QPoint::dotProduct(previousPoint - lastPoint, previousPoint - lastPoint));
    for (int i = 0; i < cntRounds; i++) {
        QPoint currentPoint = previousPoint + (lastPoint - previousPoint) * i / cntRounds;
        auto currentPointDiff = paint(currentPoint, matrixModelView, projection, screenSize);
        diff.insert(std::end(diff), std::begin(currentPointDiff), std::end(currentPointDiff));
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
        std::vector<QVector2D*> triangles = Geometry::intersectTriangles(triangle1, triangle2);

        for (QVector2D *triangle : triangles) {
            paintTriangle(triangle, diff);
        }
    }
}

/*
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
*/

void PixelsPaintingBrush::paintTriangle(QVector2D *points, std::vector<std::pair<QPoint, std::pair<QColor, QColor>>> &diff) {
    int minX = round(fmin(points[0].x(), fmin(points[1].x(), points[2].x())) * textureImage->width());
    int maxX = round(fmax(points[0].x(), fmax(points[1].x(), points[2].x())) * textureImage->width());

    for (int x = minX; x <= maxX; x++) {
        int minY = round(Geometry::getMinY(points[0], points[1], points[2], x / (1.0 * textureImage->width()))
                         * textureImage->height());
        int maxY = round(Geometry::getMaxY(points[0], points[1], points[2], x / (1.0 * textureImage->width()))
                         * textureImage->height());

        for (int y = minY; y <= maxY; y++) {
            diff.push_back({QPoint(x, y), {textureImage->pixelColor(x, y), QColor(255, 0, 0)}});
            textureImage->setPixelColor(QPoint(x, y), QColor(255, 0, 0));
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

/*
    for (size_t i = 0; i < vertices.size() / 3; i++) {
        QVector3D vector1 = QVector3D(matrixModelView * QVector4D(vertices[3 * i].position(), 1.0));
        QVector3D vector2 = QVector3D(matrixModelView * QVector4D(vertices[3 * i + 1].position(), 1.0));
        QVector3D vector3 = QVector3D(matrixModelView * QVector4D(vertices[3 * i + 2].position(), 1.0));
        if (QVector3D::crossProduct(vector2 - vector1, vector3 - vector1).z() > 0) {
            ids.push_back(i);
        }
    }
*/
    return ids;
}
