#ifndef VERTEX_H
#define VERTEX_H

//#include "tiny_obj_loader.h"

#include <QVector3D>
#include <QVector2D>

class Vertex {
public:
    Q_DECL_CONSTEXPR Vertex();
    Q_DECL_CONSTEXPR explicit Vertex(const QVector3D &position);
    Q_DECL_CONSTEXPR Vertex(const QVector3D &position, const QVector2D &uv);
    //Vertex(const tinyobj::attrib_t &attrib, const tinyobj::index_t &idx);

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

/*
Vertex::Vertex(const tinyobj::attrib_t &attrib, const tinyobj::index_t &idx) {
    float vx = attrib.vertices[3 * idx.vertex_index + 0];
    float vy = attrib.vertices[3 * idx.vertex_index + 1];
    float vz = attrib.vertices[3 * idx.vertex_index + 2];
    //float nx = attrib.normals[3 * idx.normal_index + 0];
    //float ny = attrib.normals[3 * idx.normal_index + 1];
    //float nz = attrib.normals[3 * idx.normal_index + 2];
    float tx = attrib.texcoords[2 * idx.texcoord_index + 0];
    float ty = attrib.texcoords[2 * idx.texcoord_index + 1];
    m_position = QVector3D(vx, vy, vz);
    m_uv = QVector2D(tx, ty);
}
*/

Q_DECL_CONSTEXPR inline const QVector3D& Vertex::position() const { return m_position; }

Q_DECL_CONSTEXPR inline const QVector2D& Vertex::uv() const { return m_uv; }

void inline Vertex::setPosition(const QVector3D &position) { m_position = position; }

void inline Vertex::setuv(const QVector2D &uv) { m_uv = uv; }

// OpenGL Helpers
Q_DECL_CONSTEXPR inline int Vertex::positionOffset() { return offsetof(Vertex, m_position); }

Q_DECL_CONSTEXPR inline int Vertex::uvOffset() { return offsetof(Vertex, m_uv); }

Q_DECL_CONSTEXPR inline int Vertex::stride() { return sizeof(Vertex); }

#endif // VERTEX_H
