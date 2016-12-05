#include "brush.h"

Brush::Brush(std::vector<Vertex> vertices, QImage* textureImage) : AbstractBrush(vertices, textureImage) {
}

void Brush::paintTriangle(QVector2D *points, std::vector<std::pair<QPoint, std::pair<QColor, QColor>>> &diff) {
    int minX = round(fmin(points[0].x(), fmin(points[1].x(), points[2].x())) * textureImage->width());
    int maxX = round(fmax(points[0].x(), fmax(points[1].x(), points[2].x())) * textureImage->width());

    for (int x = fmax(0, minX); x <= fmin(textureImage->width(), maxX); x++) {
        int minY = round(Geometry::getMinY(points[0], points[1], points[2], x / (1.0 * textureImage->width()))
                         * textureImage->height());
        int maxY = round(Geometry::getMaxY(points[0], points[1], points[2], x / (1.0 * textureImage->width()))
                         * textureImage->height());

        for (int y = fmax(0, minY); y <= fmin(textureImage->height(), maxY); y++) {
            diff.push_back({QPoint(x, y), {textureImage->pixelColor(x, y), QColor(255, 0, 0)}});
            textureImage->setPixelColor(QPoint(x, y), QColor(255, 0, 0));
        }
    }
}

std::vector<std::pair<QPoint, std::pair<QColor, QColor>>>
        Brush::paint(QPoint point, QMatrix4x4 matrixModelView, QMatrix4x4 matrixProjection, QPoint screenSize) {
    std::vector<std::pair<QPoint, std::pair<QColor, QColor>>> diff;
    QVector2D centerPoint(2.0 * point.x() / screenSize.x() - 1, 2.0 * (screenSize.y() - point.y()) / screenSize.y() - 1);
    QVector3D centerRay = fromScreenCoordinates(centerPoint, matrixProjection);

    const int POINTS_CNT = 4;
    for (int i = 0; i < POINTS_CNT; i++) {
        QVector3D ray1 = fromScreenCoordinates(QVector2D(centerPoint.x() + radius * cos(2 * M_PI * i / POINTS_CNT) / textureImage->width(),
                                                         centerPoint.y() + radius * sin(2 * M_PI * i / POINTS_CNT) / textureImage->height()),
                                                 matrixProjection);
        QVector3D ray2 = fromScreenCoordinates(QVector2D(centerPoint.x() + radius * cos(2 * M_PI * (i + 1) / POINTS_CNT) / textureImage->width(),
                                                         centerPoint.y() + radius * sin(2 * M_PI * (i + 1) / POINTS_CNT) / textureImage->height()),
                                                 matrixProjection);

        std::vector<size_t> ids;
        for (size_t i = 0; i < vertices.size() / 3; i++) {
            QVector3D vector1 = QVector3D(matrixModelView * QVector4D(vertices[3 * i].position(), 1.0));
            QVector3D vector2 = QVector3D(matrixModelView * QVector4D(vertices[3 * i + 1].position(), 1.0));
            QVector3D vector3 = QVector3D(matrixModelView * QVector4D(vertices[3 * i + 2].position(), 1.0));
            if (QVector3D::crossProduct(vector2 - vector1, vector3 - vector1).z() > 1e-3) {
                ids.push_back(i);
            }
        }
        paintIntersectionWithPyramid(ids, centerRay, ray1, ray2, matrixModelView, diff);
    }
    return diff;
}

std::vector<std::pair<QPoint, std::pair<QColor, QColor>>>
        Brush::paint(QPoint previousPoint, QPoint lastPoint,
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

void Brush::paintIntersectionWithPyramid(std::vector<size_t> intersectedTrianglesIds,
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
