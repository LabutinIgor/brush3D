#ifndef FACE_H
#define FACE_H

#include <vector>
#include <glm.hpp>

#include "objectmodel.h"

class Face {
public:
    Face(const ObjectModel& objectModel, size_t idFace);
    const glm::vec2& getUv(size_t id) const;
    const glm::vec3& getCoordinates(size_t id) const;
    const std::vector<glm::vec2>& getUvs() const;
    const std::vector<glm::vec3>& getCoordinates() const;

private:
    std::vector<glm::vec3> coordinates;
    std::vector<glm::vec2> uv;
};

#endif // FACE_H
