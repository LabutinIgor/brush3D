#ifndef OBJECTMODEL_H
#define OBJECTMODEL_H

#include <vector>
#include <cstdint>
#include <glm.hpp>

namespace Brush {
    class ObjectModel {
    public:
        ObjectModel();

        ObjectModel(size_t verticesNumber, size_t facesNumber);

        glm::vec3 getPosition(size_t vertexId) const;

        glm::vec2 getUv(size_t vertexId) const;

        size_t getVerticesNumber() const;

        size_t getFacesNumber() const;

        void setVertex(size_t id, const glm::vec3 &vertexCoordinates, const glm::vec2 &vertexUv);

        bool areAdjacentFaces(size_t idFace1, size_t idFace2) const;

    private:
        size_t verticesNumber_;
        size_t facesNumber_;
        std::vector<glm::vec3> positions_;
        std::vector<glm::vec2> uv_;
    };
}

#endif // OBJECTMODEL_H
