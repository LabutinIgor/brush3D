#include "face.h"

Face::Face(glm::vec3* coordinates, glm::vec2* uv) {
    for (int i = 0; i < 3; i++) {
        this->coordinates[i] = coordinates[i];
        this->uv[i] = uv[i];
    }
}

glm::vec2* Face::getUvTriangle() {
    return uv;
}

glm::vec3* Face::getCoordinatesTriangle() {
    return coordinates;
}
