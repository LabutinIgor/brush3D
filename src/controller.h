#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "brush.h"
#include "slowraysbrush.h"
#include "abstractbrush.h"
#include "pixelspaintingbrush.h"
#include "pixelsfastbrush.h"
#include "vertex.h"
#include "brushstroke.h"
#include "brushhistory.h"

class Controller {
public:
    Controller();
    void loadObj(const char *fileName);
    void setViewMatrixForObj();
    void loadTextureImage(const char *fileName);
    void initializeBrush();
    void setIdsStorage(QImage *idsBuffer);
    void beginBrushStroke(QPoint point);
    void continueBrushStroke(QPoint point);
    void endBrushStroke(QPoint point);
    void updateSize(int width, int height);
    void updateRotationMatrix();
    void updateTransformMatrix(QPoint mousePosition);
    void updateProjectionMatrixToResize(int width, int height);
    void mouseMoved(QPoint position);
    void mousePressed(QPoint position);
    void mouseReleased(QPoint position);
    void keyPressed(int key);
    void keyReleased(int key);
    void wheelScrolled(int delta);
    bool getIsBrushUpdated();
    QMatrix4x4 getModelViewMatrix();
    QMatrix4x4 getProjectionMatrix();
    QImage *getTextureFromBrush();
    std::vector<Vertex> getVertices();

private:
    AbstractBrush *brush;
    std::vector<Vertex> vertices;
    QImage *textureImage = 0;
    QMatrix4x4 rotationMatrix;
    QMatrix4x4 scaleMatrix;
    QMatrix4x4 viewMatrix;
    QMatrix4x4 projectionMatrix;
    QPoint previousMousePosition;
    QPoint mousePosition;
    bool isMousePressed = false;
    bool isBrashActive = false;
    bool isBrushUpdated = false;
    double scaleCoefficient = 0;
    QPoint screenSize;
    BrushStroke currentStroke;
    QPoint lastPointOfStroke;
    BrushHistory brushHistory;

    glm::mat4x4 fromQMatrix(QMatrix4x4 qmat);
};

#endif // CONTROLLER_H
