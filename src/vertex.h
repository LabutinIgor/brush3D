#ifndef VERTEX_H
#define VERTEX_H

#include <QVector3D>
#include <QVector2D>

class Vertex {
public:
    Q_DECL_CONSTEXPR Vertex();
    Q_DECL_CONSTEXPR explicit Vertex(const QVector3D &position);
    Q_DECL_CONSTEXPR Vertex(const QVector3D &position, const QVector2D &uv, const QVector3D &id);

    Q_DECL_CONSTEXPR const QVector3D& position() const;

    Q_DECL_CONSTEXPR const QVector2D& uv() const;

    Q_DECL_CONSTEXPR const QVector3D& id() const;

    void setPosition(const QVector3D &position);

    void setUv(const QVector2D &uv);

    void setId(const QVector3D &id);

    static const int PositionTupleSize = 3;
    static const int uvTupleSize = 2;
    static const int idTupleSize = 3;

    static Q_DECL_CONSTEXPR int positionOffset();
    static Q_DECL_CONSTEXPR int uvOffset();
    static Q_DECL_CONSTEXPR int idOffset();
    static Q_DECL_CONSTEXPR int stride();

private:
    QVector3D m_position;
    QVector2D m_uv;
    QVector3D m_id;
};

/*******************************************************************************
 * Inline Implementation
 ******************************************************************************/

// Note: Q_MOVABLE_TYPE means it can be memcpy'd.
Q_DECLARE_TYPEINFO(Vertex, Q_MOVABLE_TYPE);

// Constructors
Q_DECL_CONSTEXPR inline Vertex::Vertex() {}

Q_DECL_CONSTEXPR inline Vertex::Vertex(const QVector3D &position) :
m_position(position) {}

Q_DECL_CONSTEXPR inline Vertex::Vertex(const QVector3D &position, const QVector2D &uv, const QVector3D &id) :
m_position (position), m_uv(uv), m_id(id) {}

Q_DECL_CONSTEXPR inline const QVector3D& Vertex::position() const { return m_position; }

Q_DECL_CONSTEXPR inline const QVector2D& Vertex::uv() const { return m_uv; }

Q_DECL_CONSTEXPR inline const QVector3D& Vertex::id() const { return m_id; }

void inline Vertex::setPosition(const QVector3D &position) { m_position = position; }

void inline Vertex::setUv(const QVector2D &uv) { m_uv = uv; }

void inline Vertex::setId(const QVector3D &id) { m_id = id; }

// OpenGL Helpers
Q_DECL_CONSTEXPR inline int Vertex::positionOffset() { return offsetof(Vertex, m_position); }

Q_DECL_CONSTEXPR inline int Vertex::uvOffset() { return offsetof(Vertex, m_uv); }

Q_DECL_CONSTEXPR inline int Vertex::idOffset() { return offsetof(Vertex, m_id); }

Q_DECL_CONSTEXPR inline int Vertex::stride() { return sizeof(Vertex); }

#endif // VERTEX_H
