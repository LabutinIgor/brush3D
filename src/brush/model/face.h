#ifndef FACE_H
#define FACE_H

#include "glm.hpp"

class Face {
public:
    Face(glm::vec3* coordinates, glm::vec2* uv);
    glm::vec2* getUvTriangle();
    glm::vec3* getCoordinatesTriangle();

private:
    glm::vec3 coordinates[3];
    glm::vec2 uv[3];
};

#endif // FACE_H
