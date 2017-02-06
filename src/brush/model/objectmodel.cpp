#include "objectmodel.h"

ObjectModel::ObjectModel() {
}

ObjectModel::ObjectModel(uint32_t verticesNumber, uint32_t facesNumber) :
                verticesNumber_(verticesNumber), facesNumber_(facesNumber) {
    positions_.resize(verticesNumber);
    uv_.resize(verticesNumber);
}

uint32_t ObjectModel::getVerticesNumber() const {
    return verticesNumber_;
}

uint32_t ObjectModel::getFacesNumber() const {
    return facesNumber_;
}

const glm::vec3& ObjectModel::getPositions(uint32_t vertexId) const {
    return positions_[vertexId];
}

const glm::vec2& ObjectModel::getUv(uint32_t vertexId) const {
    return uv_[vertexId];
}

void ObjectModel::setVertex(size_t id, const glm::vec3& vertexPositions, const glm::vec2& vertexUv) {
    positions_[id] = vertexPositions;
    uv_[id] = vertexUv;
}

bool ObjectModel::areAdjacentFaces(uint32_t idFace1, uint32_t idFace2) const {
    if (idFace1 >= facesNumber_ || idFace2 >= facesNumber_) {
        return true;
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (glm::length(getPositions(3 * idFace1 + i) - getPositions(3 * idFace2 + j)) < 1e-5) {
                return true;
            }
        }
    }
    return false;
}
