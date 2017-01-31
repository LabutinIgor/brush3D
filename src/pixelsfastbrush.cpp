#include "pixelsfastbrush.h"

PixelsFastBrush::PixelsFastBrush(std::vector<Vertex> vertices, QImage* textureImage) : AbstractBrush(vertices, textureImage) {
}


std::vector<std::pair<QPoint, std::pair<QColor, QColor>>>
    PixelsFastBrush::paint(QPoint point, QMatrix4x4 matrixModelView, QMatrix4x4 projection, QPoint screenSize) {
    std::vector<std::pair<QPoint, std::pair<QColor, QColor>>> diff;

    auto ids = getIntersectedTrianglesIds(point, screenSize);
    for (size_t id : ids) {
        paintTriangle(id, matrixModelView, projection, screenSize, point, diff);
    }

    return diff;
}

void PixelsFastBrush::paintTriangle(size_t id, QMatrix4x4 matrixModelView, QMatrix4x4 projection, QPoint screenSize, QPoint brushCenter,
                                    std::vector<std::pair<QPoint, std::pair<QColor, QColor>>> &diff) {
    QVector2D pointsUV[3] = {vertices[3 * id].uv(), vertices[3 * id + 1].uv(), vertices[3 * id + 2].uv()};
    QVector3D points[3] = {QVector3D(matrixModelView * QVector4D(vertices[3 * id].position(), 1.0)),
                           QVector3D(matrixModelView * QVector4D(vertices[3 * id + 1].position(), 1.0)),
                           QVector3D(matrixModelView * QVector4D(vertices[3 * id + 2].position(), 1.0))};

    int minX = fmax(0, round(fmin(pointsUV[0].x(), fmin(pointsUV[1].x(), pointsUV[2].x())) * textureImage->width()));
    int maxX = fmin(textureImage->width() - 1, round(fmax(pointsUV[0].x(), fmax(pointsUV[1].x(), pointsUV[2].x()))
                                               * textureImage->width()));

    for (int x = minX; x <= maxX; x++) {
        int minY = fmax(0, round(Geometry::getMinY(pointsUV[0], pointsUV[1], pointsUV[2],
                                                   x / (1.0 * textureImage->width()))
                           * textureImage->height()));
        int maxY = fmin(textureImage->height() - 1, round(Geometry::getMaxY(pointsUV[0], pointsUV[1], pointsUV[2],
                                                                            x / (1.0 * textureImage->width()))
                                                    * textureImage->height()));

        for (int y = minY; y <= maxY; y++) {
            QVector3D point = Geometry::getPointFromUVCoordinates(pointsUV, points,
                                                                  QVector2D(x / (1.0 * textureImage->width()),
                                                                            y / (1.0 * textureImage->height())));
            QPoint screenPoint(Geometry::toScreenCoordinates(point, projection, screenSize));
            if (screenPoint.x() >= 0 && screenPoint.y() >= 0 && screenPoint.x() < screenSize.x() && screenPoint.y() < screenSize.y()
                    && QPoint::dotProduct(brushCenter - screenPoint, brushCenter - screenPoint) < radius * radius
                    && (getIdByPixel(screenPoint) == id
                        || getIdByPixel(QPoint(screenPoint.x() + 1, screenPoint.y())) == id
                        || getIdByPixel(QPoint(screenPoint.x() - 1, screenPoint.y())) == id
                        || getIdByPixel(QPoint(screenPoint.x(), screenPoint.y() + 1)) == id
                        || getIdByPixel(QPoint(screenPoint.x(), screenPoint.y() - 1)) == id)) {
                diff.push_back({QPoint(x, y), {textureImage->pixelColor(x, y), color}});
                textureImage->setPixelColor(QPoint(x, y), color);
            }
        }
    }
}

std::unordered_set<size_t> PixelsFastBrush::getIntersectedTrianglesIds(QPoint brushCenter, QPoint screenSize) {
    std::unordered_set<size_t> ids;

    for (int dx = -radius; dx <= radius; dx++) {
        for (int dy = -radius; dy <= radius; dy++) {
            if (dx * dx + dy * dy <= radius * radius) {
                QPoint point(brushCenter.x() + dx, brushCenter.y() + dy);
                if (point.x() >= 0 && point.y() >= 0 && point.x() < screenSize.x() && point.y() < screenSize.y()
                        && QColor(idsBuffer->pixel(point)).blue() < 255) {
                    ids.insert(getIdByPixel(point));
                }
            }
        }
    }

    return ids;
}

size_t PixelsFastBrush::getIdByPixel(QPoint point) {
    QColor color(idsBuffer->pixel(point));
    return color.red() + 256 * color.green() + 256 * 256 * color.blue();
}
