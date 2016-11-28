#include "brush.h"

Brush::Brush(std::vector<Vertex> vertices, QImage* textureImage) : AbstractBrush(vertices, textureImage) {
}

void Brush::paintTriangle(QVector2D point1, QVector2D point2, QVector2D point3) {
    int minX = round(fmin(point1.x(), fmin(point2.x(), point3.x())) * textureImage->width());
    int maxX = round(fmax(point1.x(), fmax(point2.x(), point3.x())) * textureImage->width());

    for (int x = minX; x <= maxX; x++) {
        int minY = round(Geometry::getMinY(point1, point2, point3, x / (1.0 * textureImage->width()))
                         * textureImage->height());
        int maxY = round(Geometry::getMaxY(point1, point2, point3, x / (1.0 * textureImage->width()))
                         * textureImage->height());

        for (int y = minY; y <= maxY; y++) {
            textureImage->setPixelColor(QPoint(x, y), QColor(255, 0, 0));
        }
    }
}

void Brush::paint(QPoint point, QMatrix4x4 matrixModelView, QMatrix4x4 matrixProjection, QPoint screenSize) {
    QVector2D centerPoint(2.0 * point.x() / screenSize.x() - 1, 2.0 * (screenSize.y() - point.y()) / screenSize.y() - 1);
    QVector3D centerRay = fromScreenCoordinates(centerPoint, matrixProjection);

    const int POINTS_CNT = 6;
    for (int i = 0; i < POINTS_CNT; i++) {
        QVector3D ray1 = fromScreenCoordinates(QVector2D(centerPoint.x() + radius * cos(2 * M_PI * i / POINTS_CNT) / textureImage->width(),
                                                         centerPoint.y() + radius * sin(2 * M_PI * i / POINTS_CNT) / textureImage->height()),
                                                 matrixProjection);
        QVector3D ray2 = fromScreenCoordinates(QVector2D(centerPoint.x() + radius * cos(2 * M_PI * (i + 1) / POINTS_CNT) / textureImage->width(),
                                                         centerPoint.y() + radius * sin(2 * M_PI * (i + 1) / POINTS_CNT) / textureImage->height()),
                                                 matrixProjection);

        std::vector<size_t> ids;
        for (size_t i = 0; i < vertices.size() / 3; i++) {
            ids.push_back(i);
        }
        paintIntersectionWithPyramid(ids, centerRay, ray1, ray2, matrixModelView);
    }
}

void Brush::paintIntersectionWithPyramid(std::vector<size_t> intersectedTrianglesIds,
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

void Brush::paintTrianglesIntersection(QVector2D point1, QVector2D point2, QVector2D point3,
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
