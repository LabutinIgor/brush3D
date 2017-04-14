#include "include/objectmodel.hpp"

namespace Brush {
    ObjectModel::ObjectModel() : verticesNumber_(0), facesNumber_(0) {
    }

    ObjectModel::ObjectModel(size_t verticesNumber, size_t facesNumber) :
            verticesNumber_(verticesNumber), facesNumber_(facesNumber),
            positions_(verticesNumber), uv_(verticesNumber) {
    }

    size_t ObjectModel::getVerticesNumber() const {
        return verticesNumber_;
    }

    size_t ObjectModel::getFacesNumber() const {
        return facesNumber_;
    }

    glm::vec3 ObjectModel::getPosition(size_t vertexId) const {
        return positions_[vertexId];
    }

    glm::vec2 ObjectModel::getUv(size_t vertexId) const {
        return uv_[vertexId];
    }

    void ObjectModel::setVertex(size_t id, const glm::vec3& vertexPositions, const glm::vec2& vertexUv) {
        positions_[id] = vertexPositions;
        uv_[id] = vertexUv;
    }

    bool ObjectModel::areAdjacentFaces(size_t idFace1, size_t idFace2) const {
        if (idFace1 >= facesNumber_ || idFace2 >= facesNumber_) {
            return true;
        }

        // TODO improve
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (glm::length(getPosition(3 * idFace1 + i) - getPosition(3 * idFace2 + j)) < 1e-5) {
                    return true;
                }
            }
        }
        return false;
    }
}
