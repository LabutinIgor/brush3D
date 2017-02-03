#include "face.h"

Face::Face(const ObjectModel& objectModel, size_t idFace) {
    this->coordinates_.resize(3);
    this->uv_.resize(3);
    for (int i = 0; i < 3; i++) {
        this->coordinates_[i] = objectModel.getCoordinates(idFace * 3 + i);
        this->uv_[i] = objectModel.getUv(idFace * 3 + i);
    }
}

const glm::vec2& Face::getUv(size_t id) const {
    return uv_[id];
}

const glm::vec3& Face::getCoordinates(size_t id) const {
    return coordinates_[id];
}

const std::vector<glm::vec2>& Face::getUvs() const {
    return uv_;
}

const std::vector<glm::vec3>& Face::getCoordinates() const {
    return coordinates_;
}
