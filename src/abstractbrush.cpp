#include "abstractbrush.h"

AbstractBrush::AbstractBrush(std::vector<Vertex> vertices, QImage* textureImage)
{
    this->vertices = vertices;
    this->textureImage = textureImage;
}

void AbstractBrush::setRadius(double radius) {
    this->radius = radius;
}

void AbstractBrush::setColor(QColor color) {
    this->color = color;
}

QImage *AbstractBrush::getTextureImage() {
    return textureImage;
}

QVector3D AbstractBrush::fromScreenCoordinates(QVector2D point, QMatrix4x4 matrixProjection) {
    return QVector3D(point.x() / matrixProjection.column(0).x(),
                     point.y() / matrixProjection.column(1).y(),
                     -1.0);
}

void AbstractBrush::setIdsBuffer(QImage *idsBuffer) {
    this->idsBuffer = idsBuffer;
}

std::vector<std::pair<QPoint, std::pair<QColor, QColor>>>
        AbstractBrush::paint(QPoint previousPoint, QPoint lastPoint,
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
