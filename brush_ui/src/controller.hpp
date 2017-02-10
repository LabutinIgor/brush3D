#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <vector>
#include <iostream>
#include <QImage>
#include <QPoint>
#include <QMatrix4x4>

#include "brush_ui/src/ui/vertexforbuffer.h"
#include "abstractbrush.hpp"
#include "pixelsfastbrush.hpp"
#include "objectmodel.hpp"
#include "brushstroke.hpp"
#include "brushhistory.hpp"

class Controller {
public:
    Controller();

    Controller(const Controller &other) = delete;

    void loadObj(const char *fileName);

    void setViewMatrixForObj();

    void loadTextureImage(const char *fileName);

    void initializeBrush();

    void setIdsStorage(QImage *idsBuffer);

    void beginBrushStroke(const QPoint &point);

    void continueBrushStroke(const QPoint &point);

    void endBrushStroke(const QPoint &point);

    void updateSize(int width, int height);

    void updateRotationMatrix();

    void updateTransformMatrix(const QPoint &mousePosition_);

    void updateProjectionMatrixToResize(int width, int height);

    void mouseMoved(const QPoint &position);

    void mousePressed(const QPoint &position);

    void mouseReleased(const QPoint &position);

    void keyPressed(int key);

    void keyReleased(int key);

    void wheelScrolled(int delta);

    bool getIsBrushUpdated();

    QMatrix4x4 getModelViewMatrix();

    const QMatrix4x4 &getProjectionMatrix();

    QImage *getTextureFromBrush();

    const std::vector<VertexForBuffer> &getVertices();

private:
    Brush::AbstractBrush *brush_ = 0;
    Brush::ObjectModel objectModel_;
    Brush::TextureStorage textureStorage_;
    Brush::IdsStorage idsStorage_;
    std::vector<VertexForBuffer> verticesForBuffer_;
    QImage textureImage_;
    QMatrix4x4 rotationMatrix_;
    QMatrix4x4 scaleMatrix_;
    QMatrix4x4 viewMatrix_;
    QMatrix4x4 projectionMatrix_;
    QPoint previousMousePosition_;
    QPoint mousePosition_;
    bool isMousePressed_ = false;
    bool isBrashActive_ = false;
    bool isBrushUpdated_ = false;
    double scaleCoefficient_ = 0;
    Brush::BrushStroke currentStroke_;
    QPoint lastPointOfStroke_;
    BrushHistory brushHistory_;

    glm::mat4x4 fromQMatrix(const QMatrix4x4 &qmat);

    VertexForBuffer vertexFromTinyobj(const std::vector<float> &vertices, const std::vector<float> &texcoords,
                                      uint32_t vId, uint32_t tId, uint32_t triangleId);
};

#endif // CONTROLLER_H
