#include "utils.hpp"

namespace Brush {
    namespace Utils {
        double_t getMinUvX(const Face &face) {
            double_t res = face.getUv(0).x;
            for (size_t i = 1; i < face.getSize(); ++i) {
                res = fmin(res, face.getUv(i).x);
            }
            return res;
        }

        double_t getMaxUvX(const Face &face) {
            double_t res = face.getUv(0).x;
            for (size_t i = 1; i < face.getSize(); ++i) {
                res = fmax(res, face.getUv(i).x);
            }
            return res;
        }

        double_t getMinY(const Face &face, double_t x) {
            double_t res = getMinIntersectionPoint(face.getUv(0), face.getUv(face.getSize() - 1), x);
            for (size_t i = 0; i < face.getSize() - 1; i++) {
                res = fmin(res, getMinIntersectionPoint(face.getUv(i), face.getUv(i + 1), x));
            }
            return res;
        }

        double_t getMaxY(const Face &face, double_t x) {
            double_t res = getMaxIntersectionPoint(face.getUv(0), face.getUv(face.getSize() - 1), x);
            for (size_t i = 0; i < face.getSize() - 1; i++) {
                res = fmax(res, getMaxIntersectionPoint(face.getUv(i), face.getUv(i + 1), x));
            }
            return res;
        }

        double_t getMinIntersectionPoint(const glm::vec2 &point1, const glm::vec2 &point2, double_t x) {
            if (fabs(point1.x - point2.x) < EPS) {
                if (fabs(point1.x - x) > EPS) {
                    return 1;
                } else {
                    return fmin(point1.y, point2.y);
                }
            } else {
                double y = getIntersectionPoint(point1, point2, x);
                if (y < fmin(point1.y, point2.y) || y > fmax(point1.y, point2.y)) {
                    return 1;
                } else {
                    return y;
                }
            }
        }

        double_t getMaxIntersectionPoint(const glm::vec2 &point1, const glm::vec2 &point2, double_t x) {
            if (fabs(point1.x - point2.x) < EPS) {
                if (fabs(point1.x - x) > EPS) {
                    return 0;
                } else {
                    return fmax(point1.y, point2.y);
                }
            } else {
                double y = getIntersectionPoint(point1, point2, x);
                if (y < fmin(point1.y, point2.y) || y > fmax(point1.y, point2.y)) {
                    return 0;
                } else {
                    return y;
                }
            }
        }

        double_t getIntersectionPoint(const glm::vec2 &point1, const glm::vec2 &point2, double_t x) {
            double intersectionY = point1.y + (x - point1.x) * (point2.y - point1.y) / (point2.x - point1.x);
            return fmax(0.0, fmin(1.0, intersectionY));
        }

        bool isInside(const glm::i32vec2 &point, const glm::i32vec2 &rectangle) {
            return point.x >= 0 && point.y >= 0 && point.x < rectangle.x && point.y < rectangle.y;
        }

        bool hasNeighbourWithId(const IdsStorage &idsStorage, const glm::i32vec2 &point, IdType id) {
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    glm::i32vec2 newPoint = glm::i32vec2(point.x + dx, point.y + dy);
                    if (isInside(newPoint, idsStorage.getSize()) && idsStorage.getValue(newPoint) == id) {
                        return true;
                    }
                }
            }
            return false;
        }



        glm::vec3
        getPointFromUVCoordinates(const std::vector<glm::vec2> &pointsUV,
                                  const std::vector<glm::vec3> &points,
                                  const glm::vec2 &pointUV) {
            glm::vec2 uvVector1 = pointsUV[1] - pointsUV[0];
            glm::vec2 uvVector2 = pointsUV[2] - pointsUV[0];
            glm::vec3 vector1 = points[1] - points[0];
            glm::vec3 vector2 = points[2] - points[0];
            glm::vec2 point = pointUV - pointsUV[0];

            float d = uvVector1.x * uvVector2.y - uvVector1.y * uvVector2.x;
            float c1 = (point.x * uvVector2.y - point.y * uvVector2.x) / d;
            float c2 = (uvVector1.x * point.y - uvVector1.y * point.x) / d;

            return points[0] + c1 * vector1 + c2 * vector2;
        }

        glm::i32vec2 toScreenCoordinates(const glm::vec3 &point, const glm::mat4x4 &matrixProjection,
                                         const glm::i32vec2 &screenSize) {
            glm::vec4 homogeneousCoordinates(matrixProjection * glm::vec4(point, 1.0));
            glm::vec3 projectedPoint(
                    glm::vec3(homogeneousCoordinates.x, homogeneousCoordinates.y, homogeneousCoordinates.z)
                    / homogeneousCoordinates.w);
            return glm::i32vec2(screenSize.x * (projectedPoint.x + 1) / 2,
                                screenSize.y * (1 - projectedPoint.y) / 2);
        }

        float_t calculateFaceAngleCos(const Face &face, const glm::mat4x4 &matrixModelView) {
            std::vector<glm::vec3> points(3);
            for (size_t i = 0; i < 3; ++i) {
                points[i] = glm::vec3(matrixModelView * glm::vec4(face.getPosition(i), 1.0));
            }
            glm::vec3 normal = glm::cross(points[1] - points[0], points[2] - points[0]);
            normal = glm::normalize(normal);

            glm::vec3 faceCenter((points[0] + points[1] + points[2]) / 3.0f);
            faceCenter = glm::normalize(faceCenter);

            return glm::dot(normal, faceCenter);
        }
    }
}