#ifndef OBJECTMODEL_H
#define OBJECTMODEL_H

#include <iostream>
#include <inttypes.h>
#include <glm.hpp>

#include "face.h"

class ObjectModel {
public:
    ObjectModel(uint32_t verticesNumber, uint32_t facesNumber, glm::vec3* coordinates, glm::vec2* uv);
    glm::vec3 getCoordinates(uint32_t vertexId);
    glm::vec2 getUv(uint32_t vertexId);
    Face getFace(uint32_t faceId);
    uint32_t getVerticesNumber();
    uint32_t getFacesNumber();
    bool areAdjacentFaces(uint32_t idFace1, uint32_t idFace2);

private:
    uint32_t verticesNumber;
    uint32_t facesNumber;
    glm::vec3* coordinates;
    glm::vec2* uv;

};

#endif // OBJECTMODEL_H
