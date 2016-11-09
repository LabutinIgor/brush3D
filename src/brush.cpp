#include "brush.h"

Brush::Brush(std::vector<Vertex> vertices, QImage* textureImage) {
    this->vertices = vertices;
    this->textureImage = textureImage;
}

void Brush::paintExample() {
    paintTriangle(vertices[0].uv(),
                  vertices[1].uv(),
                  vertices[2].uv());
}

void Brush::paintTriangle(QVector2D point1, QVector2D point2, QVector2D point3) {
    int minX = round(fmin(point1.x(), fmin(point2.x(), point3.x())) * textureImage->height());
    int maxX = round(fmax(point1.x(), fmax(point2.x(), point3.x())) * textureImage->height());

    for (int x = minX; x < maxX; x++) {
        int minY = round(Geometry::getMinY(point1, point2, point3, x / (1.0 * textureImage->height()))
                         * textureImage->width());
        int maxY = round(Geometry::getMaxY(point1, point2, point3, x / (1.0 * textureImage->height()))
                         * textureImage->width());

        for (int y = minY; y < maxY; y++) {
            textureImage->setPixelColor(QPoint(x, y), QColor(255, 0, 0));
        }
    }
}

void Brush::paint(QVector2D point, QMatrix4x4 matrixModelView, QMatrix4x4 matrixProjection) {
    //std::cerr << point.x() << " " << point.y() << "\n";
    float fX = matrixProjection.column(0).x();
    float fY = matrixProjection.column(1).y();
    int intersectedTriangleId = -1;
    float minDistanceToIntersection = 0;
    for (size_t i = 0; i < vertices.size() / 3; i++) {
        QVector3D vector1 = QVector3D(matrixModelView * QVector4D(vertices[3 * i].position(), 1.0));
        QVector3D vector2 = QVector3D(matrixModelView * QVector4D(vertices[3 * i + 1].position(), 1.0));
        QVector3D vector3 = QVector3D(matrixModelView * QVector4D(vertices[3 * i + 2].position(), 1.0));
        QVector3D rayVector = QVector3D(point.x() / fX, point.y() / fY, -1.0);
        QVector3D *intersectionPoint = Geometry::intersectRayAndTriangle(vector1, vector2, vector3, rayVector);
        if (intersectionPoint != nullptr) {
            float currentDistanceToIntersection = QVector3D::dotProduct(*intersectionPoint, *intersectionPoint);
            if (intersectedTriangleId == -1 || currentDistanceToIntersection < minDistanceToIntersection) {
                intersectedTriangleId = i;
                minDistanceToIntersection = currentDistanceToIntersection;
            }
        }
    }

    if (intersectedTriangleId != -1) {
        paintTriangle(vertices[3 * intersectedTriangleId].uv(),
                      vertices[3 * intersectedTriangleId + 1].uv(),
                      vertices[3 * intersectedTriangleId + 2].uv());
    }
}

void Brush::setRadius(double radius) {
    this->radius = radius;
}

QImage *Brush::getTextureImage() {
    return textureImage;
}
