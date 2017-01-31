#include "geometry.h"


double Geometry::getMinY(QVector2D point1, QVector2D point2, QVector2D point3, double x) {
    return fmin(getMinIntersectionPoint(point1, point2, x),
                fmin(getMinIntersectionPoint(point2, point3, x),
                     getMinIntersectionPoint(point1, point3, x)));
}

double Geometry::getMaxY(QVector2D point1, QVector2D point2, QVector2D point3, double x) {
    return fmax(getMaxIntersectionPoint(point1, point2, x),
                fmax(getMaxIntersectionPoint(point2, point3, x),
                     getMaxIntersectionPoint(point1, point3, x)));
}

double Geometry::getMinIntersectionPoint(QVector2D point1, QVector2D point2, double x) {
    if (fabs(point1.x() - point2.x()) < 1e-5) {
        if (fabs(point1.x() - x) > 1e-5) {
            return 1;
        } else {
            return fmin(point1.y(), point2.y());
        }
    } else {
        double y = getIntersectionPoint(point1, point2, x);
        if (y < fmin(point1.y(), point2.y()) || y > fmax(point1.y(), point2.y())) {
            return 1;
        } else {
            return y;
        }
    }
}

double Geometry::getMaxIntersectionPoint(QVector2D point1, QVector2D point2, double x) {
    if (fabs(point1.x() - point2.x()) < 1e-5) {
        if (fabs(point1.x() - x) > 1e-5) {
            return 0;
        } else {
            return fmax(point1.y(), point2.y());
        }
    } else {
        double y = getIntersectionPoint(point1, point2, x);
        if (y < fmin(point1.y(), point2.y()) || y > fmax(point1.y(), point2.y())) {
            return 0;
        } else {
            return y;
        }
    }
}

double Geometry::getIntersectionPoint(QVector2D point1, QVector2D point2, double x) {
    double intersectionY = point1.y() + (x - point1.x()) * (point2.y() - point1.y()) / (point2.x() - point1.x());
    return fmax(0.0, fmin(1.0, intersectionY));
}

QVector2D Geometry::getPointInUVCoordinates(QVector3D p1, QVector3D p2, QVector3D p,
                                            QVector2D uv1, QVector2D uv2, QVector2D uv3) {
    float d = p1.x() * p2.y() - p1.y() * p2.x();
    float c1 = (p.x() * p2.y() - p.y() * p2.x()) / d;
    float c2 = (p1.x() * p.y() - p1.y() * p.x()) / d;
    return uv1 + uv2 * c1 + uv3 * c2;
}

QVector3D Geometry::getPointFromUVCoordinates(QVector2D* pointsUV, QVector3D* points, QVector2D pointUV) {
//    std::cerr << "points:\n";
//    for (int i = 0; i < 3; i++) {
//        std::cerr << points[i].x() << " " << points[i].y() << " " << points[i].z() << "\n";
//    }
    QVector2D uvVector1 = pointsUV[1] - pointsUV[0];
    QVector2D uvVector2 = pointsUV[2] - pointsUV[0];
    QVector3D vector1 = points[1] - points[0];
    QVector3D vector2 = points[2] - points[0];
    pointUV -= pointsUV[0];

    float d = uvVector1.x() * uvVector2.y() - uvVector1.y() * uvVector2.x();
    float c1 = (pointUV.x() * uvVector2.y() - pointUV.y() * uvVector2.x()) / d;
    float c2 = (uvVector1.x() * pointUV.y() - uvVector1.y() * pointUV.x()) / d;

    return points[0] + c1 * vector1 + c2 * vector2;
}

QPoint Geometry::toScreenCoordinates(QVector3D point, QMatrix4x4 projection, QPoint screenSize) {
    QVector4D homogeneousCoordinates(projection * QVector4D(point, 1.0));
    QVector3D projectedPoint(homogeneousCoordinates.x() / homogeneousCoordinates.w(),
                             homogeneousCoordinates.y() / homogeneousCoordinates.w(),
                             homogeneousCoordinates.z() / homogeneousCoordinates.w());
//    std::cerr << "point: " << point.x() << " " << point.y() << " " << point.z() << "\n";
//    std::cerr << "homogeneous: " << homogeneousCoordinates.x() << " " << homogeneousCoordinates.y() << " "
//              << homogeneousCoordinates.z() << " " << homogeneousCoordinates.w() << "\n";
//    std::cerr << "projected: " << projectedPoint.x() << " " << projectedPoint.y() << " " << projectedPoint.z() << "\n";
    return QPoint(screenSize.x() * (projectedPoint.x() + 1) / 2,
                  screenSize.y() * (1 - projectedPoint.y()) / 2);
}

QVector3D *Geometry::intersectRayAndPlane(QVector3D p1, QVector3D p2, QVector3D p3, QVector3D ray) {
    QVector3D u, v, n;      // triangle vectors
    QVector3D w0, w;        // ray vectors
    float r, a, b;          // params to calc ray-plane intersect

    u = p2 - p1;
    v = p3 - p1;
    n = QVector3D::crossProduct(u, v);
    if (n.lengthSquared() < 1e-3) {
        //std::cerr << "degenerate triangle\n";
        return nullptr;
    }

    w0 = -p1;
    a = -QVector3D::dotProduct(n, w0);
    b = QVector3D::dotProduct(n, ray);
    if (fabs(b) < 1e-5) {
        return nullptr;
    }

    r = a / b;
    if (r < 0.0) {
        //std::cerr << "ray goes away from plane\n";
        return nullptr;
    }

    return new QVector3D(r * ray);
}

bool Geometry::isPointInTriangle(QVector2D p, QVector2D v1, QVector2D v2, QVector2D v3) {
    bool b1 = QVector3D::crossProduct((p - v2), (v1 - v2)).z() < 0.0f;
    bool b2 = QVector3D::crossProduct((p - v3), (v2 - v3)).z() < 0.0f;
    bool b3 = QVector3D::crossProduct((p - v1), (v3 - v1)).z() < 0.0f;

    return ((b1 == b2) && (b2 == b3));
}

QVector3D *Geometry::intersectRayAndTriangle(QVector3D p1, QVector3D p2, QVector3D p3, QVector3D ray) {
    QVector3D u, v, n;      // triangle vectors
    QVector3D w0, w;        // ray vectors
    float r, a, b;          // params to calc ray-plane intersect

    u = p2 - p1;
    v = p3 - p1;
    n = QVector3D::crossProduct(u, v);
    if (n.lengthSquared() < 1e-3) {
        //std::cerr << "degenerate triangle\n";
        return nullptr;
    }

    w0 = -p1;
    a = -QVector3D::dotProduct(n, w0);
    b = QVector3D::dotProduct(n, ray);
    if (fabs(b) < 1e-5) {
        return nullptr;
    }

    r = a / b;
    if (r < 0.0) {
        //std::cerr << "ray goes away from plane\n";
        return nullptr;
    }

    QVector3D intersectionPoint = r * ray;

    float uu, uv, vv, wu, wv, d;
    uu = QVector3D::dotProduct(u, u);
    uv = QVector3D::dotProduct(u, v);
    vv = QVector3D::dotProduct(v, v);
    w = intersectionPoint - p1;
    wu = QVector3D::dotProduct(w, u);
    wv = QVector3D::dotProduct(w, v);
    d = uv * uv - uu * vv;

    float s, t;
    s = (uv * wv - vv * wu) / d;
    if (s < 0.0 || s > 1.0) {
        return nullptr;
    }
    t = (uv * wu - uu * wv) / d;
    if (t < 0.0 || (s + t) > 1.0) {
        return nullptr;
    }

    return new QVector3D(intersectionPoint);
}

std::vector<QVector2D> Geometry::getSegmentsIntersectionPoints(QVector2D *segment1, QVector2D *segment2) {
    std::vector<QVector2D> points;
    float a1 = segment1[1].y() - segment1[0].y();
    float b1 = segment1[0].x() - segment1[1].x();
    float c1 = - a1 * segment1[0].x() - b1 * segment1[0].y();

    float a2 = segment2[1].y() - segment2[0].y();
    float b2 = segment2[0].x() - segment2[1].x();
    float c2 = - a2 * segment2[0].x() - b2 * segment2[0].y();

    float d = a1 * b2 - a2 * b1;
    if (fabs(d) < 1e-4) {
        if (fabs(a2 * segment1[0].x() + b2 * segment1[0].y() - c2) < 1e-4) {
            float x_min = fmax(fmin(segment1[0].x(), segment1[1].x()), fmin(segment2[0].x(), segment2[1].x()));
            float y_min = fmax(fmin(segment1[0].y(), segment1[1].y()), fmin(segment2[0].y(), segment2[1].y()));
            float x_max = fmin(fmax(segment1[0].x(), segment1[1].x()), fmax(segment2[0].x(), segment2[1].x()));
            float y_max = fmin(fmax(segment1[0].y(), segment1[1].y()), fmax(segment2[0].y(), segment2[1].y()));

            if (x_min < x_max && y_min < y_max) {
                points.push_back(QVector2D(x_min, y_min));
                points.push_back(QVector2D(x_max, y_max));
            }
        }
        return points;
    }

    float x = (-c1 * b2 + b1 * c2) / d;
    float y = (- a1 * c2 + c1 * a2) / d;

    if (x < fmin(segment1[0].x(), segment1[1].x()) || x < fmin(segment2[0].x(), segment2[1].x()) ||
        x > fmax(segment1[0].x(), segment1[1].x()) || x > fmax(segment2[0].x(), segment2[1].x()) ||
        y < fmin(segment1[0].y(), segment1[1].y()) || y < fmin(segment2[0].y(), segment2[1].y()) ||
        y > fmax(segment1[0].y(), segment1[1].y()) || y > fmax(segment2[0].y(), segment2[1].y())) {
        return points;
    }

    points.push_back(QVector2D(x, y));
    return points;
}

std::vector<QVector2D*> Geometry::intersectTriangles(QVector2D *triangle1, QVector2D *triangle2) {
    std::vector<QVector2D> intersectionPoints;
    for (int i = 0; i < 3; i++) {
        if (isPointInTriangle(triangle1[i], triangle2[0], triangle2[1], triangle2[2])) {
            intersectionPoints.push_back(triangle1[i]);
        }
        if (isPointInTriangle(triangle2[i], triangle1[0], triangle1[1], triangle1[2])) {
            intersectionPoints.push_back(triangle2[i]);
        }
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            QVector2D segment1[2] = {triangle1[i], triangle1[(i + 1) % 3]};
            QVector2D segment2[2] = {triangle2[j], triangle2[(j + 1) % 3]};
            std::vector<QVector2D> segmentsIntersectionPoints = Geometry::getSegmentsIntersectionPoints(segment1, segment2);
            intersectionPoints.insert(std::end(intersectionPoints),
                                      std::begin(segmentsIntersectionPoints), std::end(segmentsIntersectionPoints));
        }
    }

    std::vector<QVector2D*> trianglesInIntersection;

    if (intersectionPoints.size() < 3) {
        return trianglesInIntersection;
    }

    QVector2D leftPoint = intersectionPoints[0];
    for (QVector2D point : intersectionPoints) {
        if (point.x() < leftPoint.x() || (fabs(point.x() - leftPoint.x()) < 1e-4 && point.y() < leftPoint.y())) {
            leftPoint = point;
        }
    }

    std::sort(intersectionPoints.begin(), intersectionPoints.end(), [leftPoint](QVector2D a, QVector2D b) {
        QVector2D v1 = a - leftPoint;
        QVector2D v2 = b - leftPoint;
        float cp = QVector3D::crossProduct(v1, v2).z();
        if (fabs(cp) < 1e-5) {
            return QVector2D::dotProduct(v1, v1) < QVector2D::dotProduct(v2, v2);
        }
        return cp > 0;
    });

    for (int i = 1; i < intersectionPoints.size() - 1; i++) {
        QVector2D *points = new QVector2D[3] {leftPoint, intersectionPoints[i], intersectionPoints[i + 1]};
        trianglesInIntersection.push_back(points);
    }
    return trianglesInIntersection;
}
