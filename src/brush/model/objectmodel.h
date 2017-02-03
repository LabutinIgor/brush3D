#ifndef OBJECTMODEL_H
#define OBJECTMODEL_H

#include <vector>
#include <cstdint>
#include <glm.hpp>

class ObjectModel {
public:
    ObjectModel();
    ObjectModel(uint32_t verticesNumber, uint32_t facesNumber);
    const glm::vec3& getCoordinates(uint32_t vertexId) const;
    const glm::vec2& getUv(uint32_t vertexId) const;
    uint32_t getVerticesNumber() const;
    uint32_t getFacesNumber() const;
    void setVertex(size_t id, glm::vec3 vertexCoordinates, glm::vec2 vertexUv);
    bool areAdjacentFaces(uint32_t idFace1, uint32_t idFace2) const;

private:
    uint32_t verticesNumber;
    uint32_t facesNumber;
    std::vector<glm::vec3> coordinates;
    std::vector<glm::vec2> uv;
};

#endif // OBJECTMODEL_H
