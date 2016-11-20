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
