#include "geometry.hpp"
#include <iostream>

namespace Brush {
    namespace Geometry {
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

        glm::vec3 fromScreenCoordinates(const glm::vec2 &point, const glm::mat4x4 &matrixProjection) {
            return glm::vec3(point.x / matrixProjection[0][0],
                             point.y / matrixProjection[1][1],
                             -1.0);
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