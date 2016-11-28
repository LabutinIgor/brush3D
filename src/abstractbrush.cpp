#include "abstractbrush.h"

AbstractBrush::AbstractBrush(std::vector<Vertex> vertices, QImage* textureImage)
{
    this->vertices = vertices;
    this->textureImage = textureImage;
}

void AbstractBrush::setRadius(double radius) {
    this->radius = radius;
}

QImage *AbstractBrush::getTextureImage() {
    return textureImage;
}

QVector3D AbstractBrush::fromScreenCoordinates(QVector2D point, QMatrix4x4 matrixProjection) {
    return QVector3D(point.x() / matrixProjection.column(0).x(),
                     point.y() / matrixProjection.column(1).y(),
                     -1.0);
}
