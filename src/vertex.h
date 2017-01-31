#ifndef VERTEX_H
#define VERTEX_H

#include <glm.hpp>

class Vertex {
public:
    Vertex();
    explicit Vertex(const glm::vec3 &position);
    Vertex(const glm::vec3 &position, const glm::vec2 &uv, const glm::vec3 &id);

    const glm::vec3& position() const;

    const glm::vec2& uv() const;

    const glm::vec3& id() const;

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

/*******************************************************************************
 * Inline Implementation
 ******************************************************************************/

// Note: Q_MOVABLE_TYPE means it can be memcpy'd.
Q_DECLARE_TYPEINFO(Vertex, Q_MOVABLE_TYPE);

// Constructors
inline Vertex::Vertex() {}

inline Vertex::Vertex(const glm::vec3 &position) :
m_position(position) {}

inline Vertex::Vertex(const glm::vec3 &position, const glm::vec2 &uv, const glm::vec3 &id) :
m_position (position), m_uv(uv), m_id(id) {}

inline const glm::vec3& Vertex::position() const { return m_position; }

inline const glm::vec2& Vertex::uv() const { return m_uv; }

inline const glm::vec3& Vertex::id() const { return m_id; }

void inline Vertex::setPosition(const glm::vec3 &position) { m_position = position; }

void inline Vertex::setUv(const glm::vec2 &uv) { m_uv = uv; }

void inline Vertex::setId(const glm::vec3 &id) { m_id = id; }

// OpenGL Helpers
inline int Vertex::positionOffset() { return offsetof(Vertex, m_position); }

inline int Vertex::uvOffset() { return offsetof(Vertex, m_uv); }

inline int Vertex::idOffset() { return offsetof(Vertex, m_id); }

inline int Vertex::stride() { return sizeof(Vertex); }

#endif // VERTEX_H
