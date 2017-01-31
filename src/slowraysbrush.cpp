#include "slowraysbrush.h"

SlowRaysBrush::SlowRaysBrush(std::vector<Vertex> vertices, QImage* textureImage) : AbstractBrush(vertices, textureImage) {
}

void SlowRaysBrush::paintRound(QVector2D centerOfRound, float radius,
                               std::vector<std::pair<QPoint, std::pair<QColor, QColor>>> &diff) {
    float radiusSquare = radius * radius;
    centerOfRound.setX(centerOfRound.x() * textureImage->width());
    centerOfRound.setY(centerOfRound.y() * textureImage->height());

    int minX = fmax(0.0, round(centerOfRound.x() - radius + 0.5));
    int maxX = fmin(textureImage->width() - 1, round(centerOfRound.x() + radius - 0.5));

    for (int x = minX; x <= maxX; x++) {
        float dx = x - centerOfRound.x();
        float dy = sqrt(radiusSquare - dx * dx);
        int minY = fmax(0.0, round(centerOfRound.y() - dy));
        int maxY = fmin(textureImage->height() - 1, round(centerOfRound.y() + dy));

        for (int y = minY; y <= maxY; y++) {
            diff.push_back({QPoint(x, y), {textureImage->pixelColor(x, y), QColor(255, 0, 0)}});
            textureImage->setPixelColor(QPoint(x, y), color);
        }
    }
}

std::vector<std::pair<QPoint, std::pair<QColor, QColor>>>
        SlowRaysBrush::paint(QPoint point, QMatrix4x4 matrixModelView, QMatrix4x4 matrixProjection, QPoint screenSize) {
    std::vector<std::pair<QPoint, std::pair<QColor, QColor>>> diff;
    QVector2D centerPoint(2.0 * point.x() / screenSize.x() - 1, 2.0 * (screenSize.y() - point.y()) / screenSize.y() - 1);
    for (int dx = -30; dx <= 30; dx++) {
        for (int dy = -30; dy <= 30; dy++) {
            if (dx * dx + dy * dy <= 30) {
                paintSmallRound(QVector2D(centerPoint.x() + dx / 300.0, centerPoint.y() + dy / 300.0), matrixModelView, matrixProjection, diff);
            }
        }
    }
    return diff;
}

void SlowRaysBrush::paintSmallRound(QVector2D point, QMatrix4x4 matrixModelView, QMatrix4x4 matrixProjection,
                                    std::vector<std::pair<QPoint, std::pair<QColor, QColor>>> &diff) {
    int intersectedTriangleId = -1;
    float minDistanceToIntersection = 0;
    QVector2D intersectionPoint;
    for (size_t i = 0; i < vertices.size() / 3; i++) {
        QVector3D vector1 = QVector3D(matrixModelView * QVector4D(vertices[3 * i].position(), 1.0));
        QVector3D vector2 = QVector3D(matrixModelView * QVector4D(vertices[3 * i + 1].position(), 1.0));
        QVector3D vector3 = QVector3D(matrixModelView * QVector4D(vertices[3 * i + 2].position(), 1.0));
        QVector3D rayVector = fromScreenCoordinates(point, matrixProjection);

        QVector3D *currentIntersectionPoint = Geometry::intersectRayAndTriangle(vector1, vector2, vector3, rayVector);

        if (currentIntersectionPoint != nullptr) {
            float currentDistanceToIntersection = QVector3D::dotProduct(*currentIntersectionPoint, *currentIntersectionPoint);
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
