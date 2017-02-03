#include "objectmodel.h"

ObjectModel::ObjectModel() {
}

ObjectModel::ObjectModel(uint32_t verticesNumber, uint32_t facesNumber) :
                verticesNumber(verticesNumber), facesNumber(facesNumber) {
    coordinates.resize(verticesNumber);
    uv.resize(verticesNumber);
}

uint32_t ObjectModel::getVerticesNumber() const {
    return verticesNumber;
}

uint32_t ObjectModel::getFacesNumber() const {
    return facesNumber;
}

const glm::vec3& ObjectModel::getCoordinates(uint32_t vertexId) const {
    return coordinates[vertexId];
}

const glm::vec2& ObjectModel::getUv(uint32_t vertexId) const {
    return uv[vertexId];
}

void ObjectModel::setVertex(size_t id, glm::vec3 vertexCoordinates, glm::vec2 vertexUv) {
    coordinates[id] = vertexCoordinates;
    uv[id] = vertexUv;
}

bool ObjectModel::areAdjacentFaces(uint32_t idFace1, uint32_t idFace2) const {
    if (idFace1 >= facesNumber || idFace2 >= facesNumber) {
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
