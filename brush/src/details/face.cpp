#include "face.hpp"

namespace Brush {
    Face::Face(const ObjectModel& objectModel, size_t idFace) {
        for (size_t i = 0; i < getSize(); ++i) {
            this->position_[i] = objectModel.getPosition(idFace * getSize() + i);
            this->uv_[i] = objectModel.getUv(idFace * getSize() + i);
        }
    }

    size_t Face::getSize() const {
        return 3;
    }

    glm::vec2 Face::getUv(size_t id) const {
        return uv_[id];
    }

    glm::vec3 Face::getPosition(size_t id) const {
        return position_[id];
    }

    std::vector<glm::vec2> Face::getUvs() const {
        std::vector<glm::vec2> uvs;
        for (size_t i = 0; i < getSize(); ++i) {
            uvs.push_back(uv_[i]);
        }
        return uvs;
    }

    std::vector<glm::vec3> Face::getPositions() const {
        std::vector<glm::vec3> positions;
        for (size_t i = 0; i < getSize(); ++i) {
            positions.push_back(position_[i]);
        }
        return positions;
    }
}