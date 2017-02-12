#ifndef VERTEXFORBUFFER_H
#define VERTEXFORBUFFER_H

#include <glm/glm.hpp>

class VertexForBuffer {
public:
    VertexForBuffer();

    explicit VertexForBuffer(const glm::vec3 &position);

    VertexForBuffer(const glm::vec3 &position, const glm::vec2 &uv, const glm::vec3 &id);

    const glm::vec3 &position() const;

    const glm::vec2 &uv() const;

    const glm::vec3 &id() const;

    void setPosition(const glm::vec3 &position);

    void setUv(const glm::vec2 &uv);

    void setId(const glm::vec3 &id);

    static const int PositionTupleSize = 3;
    static const int uvTupleSize = 2;
    static const int idTupleSize = 3;

    static int positionOffset();

    static int uvOffset();

    static int idOffset();

    static int stride();

private:
    glm::vec3 m_position;
    glm::vec2 m_uv;
    glm::vec3 m_id;
};

inline VertexForBuffer::VertexForBuffer() {}

inline VertexForBuffer::VertexForBuffer(const glm::vec3 &position) :
        m_position(position) {}

inline VertexForBuffer::VertexForBuffer(const glm::vec3 &position, const glm::vec2 &uv, const glm::vec3 &id) :
        m_position(position), m_uv(uv), m_id(id) {}

inline const glm::vec3 &VertexForBuffer::position() const { return m_position; }

inline const glm::vec2 &VertexForBuffer::uv() const { return m_uv; }

inline const glm::vec3 &VertexForBuffer::id() const { return m_id; }

void inline VertexForBuffer::setPosition(const glm::vec3 &position) { m_position = position; }

void inline VertexForBuffer::setUv(const glm::vec2 &uv) { m_uv = uv; }

void inline VertexForBuffer::setId(const glm::vec3 &id) { m_id = id; }


inline int VertexForBuffer::positionOffset() { return offsetof(VertexForBuffer, m_position); }

inline int VertexForBuffer::uvOffset() { return offsetof(VertexForBuffer, m_uv); }

inline int VertexForBuffer::idOffset() { return offsetof(VertexForBuffer, m_id); }

inline int VertexForBuffer::stride() { return sizeof(VertexForBuffer); }

#endif // VERTEXFORBUFFER_H
