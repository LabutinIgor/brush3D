#include "face.h"

Face::Face(const ObjectModel& objectModel, size_t idFace) {
    for (int i = 0; i < 3; i++) {
        this->position_[i] = objectModel.getPositions(idFace * 3 + i);
        this->uv_[i] = objectModel.getUv(idFace * 3 + i);
    }
}

const glm::vec2& Face::getUv(size_t id) const {
    return uv_[id];
}

const glm::vec3& Face::getPositions(size_t id) const {
    return position_[id];
}

const std::vector<glm::vec2> Face::getUvs() const {
    std::vector<glm::vec2> uvs;
    for (int i = 0; i < 3; i++) {
        uvs.push_back(uv_[i]);
    }
    return uvs;
}

const std::vector<glm::vec3> Face::getPositions() const {
    std::vector<glm::vec3> positions;
    for (int i = 0; i < 3; i++) {
        positions.push_back(position_[i]);
    }
    return positions;
}
