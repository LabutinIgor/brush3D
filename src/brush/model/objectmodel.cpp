#include "objectmodel.h"

ObjectModel::ObjectModel(uint32_t verticesNumber, uint32_t facesNumber, glm::vec3* coordinates, glm::vec2* uv) :
                verticesNumber(verticesNumber), facesNumber(facesNumber), coordinates(coordinates), uv(uv) {
}

uint32_t ObjectModel::getVerticesNumber() {
    return verticesNumber;
}

uint32_t ObjectModel::getFacesNumber() {
    return facesNumber;
}

glm::vec3 ObjectModel::getCoordinates(uint32_t vertexId) {
    return coordinates[vertexId];
}

glm::vec2 ObjectModel::getUv(uint32_t vertexId) {
    return uv[vertexId];
}

Face ObjectModel::getFace(uint32_t faceId) {
    return Face(coordinates + faceId * 3, uv + faceId * 3);
}
