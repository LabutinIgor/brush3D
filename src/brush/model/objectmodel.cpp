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

bool ObjectModel::areAdjacentFaces(uint32_t idFace1, uint32_t idFace2) {
    if (idFace1 >= facesNumber || idFace2 >= facesNumber) {
        return false;
    }
    glm::vec3* face1 = getFace(idFace1).getCoordinatesTriangle();
    glm::vec3* face2 = getFace(idFace2).getCoordinatesTriangle();
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (glm::length(face1[i] - face2[j]) < 1e-5) {
                return true;
            }
        }
    }
    return false;
}
