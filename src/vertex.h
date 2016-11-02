#ifndef VERTEX_H
#define VERTEX_H

#include <QVector3D>

class Vertex {
public:
    Q_DECL_CONSTEXPR Vertex();
    Q_DECL_CONSTEXPR explicit Vertex(const QVector3D &position);
    Q_DECL_CONSTEXPR Vertex(const QVector3D &position, const QVector2D &uv);

    Q_DECL_CONSTEXPR const QVector3D& position() const;

    Q_DECL_CONSTEXPR const QVector2D& uv() const;

    void setPosition(const QVector3D &position);

    void setuv(const QVector2D &uv);

    static const int PositionTupleSize = 3;
    static const int uvTupleSize = 2;

    static Q_DECL_CONSTEXPR int positionOffset();
    static Q_DECL_CONSTEXPR int uvOffset();
    static Q_DECL_CONSTEXPR int stride();

private:
    QVector3D m_position;
    QVector2D m_uv;
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

Q_DECL_CONSTEXPR inline Vertex::Vertex(const QVector3D &position, const QVector2D &uv) :
m_position (position), m_uv(uv) {}


Q_DECL_CONSTEXPR inline const QVector3D& Vertex::position() const { return m_position; }

Q_DECL_CONSTEXPR inline const QVector2D& Vertex::uv() const { return m_uv; }

void inline Vertex::setPosition(const QVector3D &position) { m_position = position; }

void inline Vertex::setuv(const QVector2D &uv) { m_uv = uv; }

// OpenGL Helpers
Q_DECL_CONSTEXPR inline int Vertex::positionOffset() { return offsetof(Vertex, m_position); }

Q_DECL_CONSTEXPR inline int Vertex::uvOffset() { return offsetof(Vertex, m_uv); }

Q_DECL_CONSTEXPR inline int Vertex::stride() { return sizeof(Vertex); }

#endif // VERTEX_H
