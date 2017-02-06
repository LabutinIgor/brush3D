#ifndef FACE_H
#define FACE_H

#include <vector>
#include <glm.hpp>

#include "objectmodel.h"

class Face {
public:
    Face(const ObjectModel& objectModel, size_t idFace);
    glm::vec2 getUv(size_t id) const;
    glm::vec3 getPositions(size_t id) const;
    std::vector<glm::vec2> getUvs() const;
    std::vector<glm::vec3> getPositions() const;

private:
    glm::vec3 position_[3];
    glm::vec2 uv_[3];
};

#endif // FACE_H
