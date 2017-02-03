#include "objectmodel.h"

ObjectModel::ObjectModel() {
}

ObjectModel::ObjectModel(uint32_t verticesNumber, uint32_t facesNumber) :
                verticesNumber_(verticesNumber), facesNumber_(facesNumber) {
    coordinates_.resize(verticesNumber);
    uv_.resize(verticesNumber);
}

uint32_t ObjectModel::getVerticesNumber() const {
    return verticesNumber_;
}

uint32_t ObjectModel::getFacesNumber() const {
    return facesNumber_;
}

const glm::vec3& ObjectModel::getCoordinates(uint32_t vertexId) const {
    return coordinates_[vertexId];
}

const glm::vec2& ObjectModel::getUv(uint32_t vertexId) const {
    return uv_[vertexId];
}

void ObjectModel::setVertex(size_t id, const glm::vec3& vertexCoordinates, const glm::vec2& vertexUv) {
    coordinates_[id] = vertexCoordinates;
    uv_[id] = vertexUv;
}

bool ObjectModel::areAdjacentFaces(uint32_t idFace1, uint32_t idFace2) const {
    if (idFace1 >= facesNumber_ || idFace2 >= facesNumber_) {
        return false;
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (glm::length(getCoordinates(3 * idFace1 + i) - getCoordinates(3 * idFace2 + j)) < 1e-5) {
                return true;
            }
        }
    }
    return false;
}
