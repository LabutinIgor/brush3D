#include "face.h"

Face::Face(const ObjectModel& objectModel, size_t idFace) {
    this->coordinates.resize(3);
    this->uv.resize(3);
    for (int i = 0; i < 3; i++) {
        this->coordinates[i] = objectModel.getCoordinates(idFace * 3 + i);
        this->uv[i] = objectModel.getUv(idFace * 3 + i);
    }
}

const glm::vec2& Face::getUv(size_t id) const {
    return uv[id];
}

const glm::vec3& Face::getCoordinates(size_t id) const {
    return coordinates[id];
}

const std::vector<glm::vec2>& Face::getUvs() const {
    return uv;
}

const std::vector<glm::vec3>& Face::getCoordinates() const {
    return coordinates;
}
