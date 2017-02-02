#ifndef ABSTRACTBRUSH_H
#define ABSTRACTBRUSH_H

#include <vector>

#include "geometry.h"
#include "color.h"
#include "texturestorage.h"
#include "idsstorage.h"
#include "objectmodel.h"
#include "glm.hpp"

class AbstractBrush {
public:
    AbstractBrush(ObjectModel* objectModel, TextureStorage* textureStorage);
    virtual ~AbstractBrush();
    virtual std::vector<std::pair<glm::i32vec2, std::pair<Color, Color>>>
        paint(glm::i32vec2 point, glm::mat4x4 matrixModelView, glm::mat4x4 projection, glm::i32vec2 screenSize) = 0;
    virtual std::vector<std::pair<glm::i32vec2, std::pair<Color, Color>>>
            paint(glm::i32vec2 previousPoint, glm::i32vec2 currentPoint,
                  glm::mat4x4 matrixModelView, glm::mat4x4 projection, glm::i32vec2 screenSize);
    void setRadius(double radius);
    void setColor(Color color);
    void setIdsStorage(IdsStorage *idsStorage);
    TextureStorage *getTextureStorage();

protected:
    double radius = 10;
    Color color = Color(255, 0, 0);
    ObjectModel* objectModel;
    TextureStorage *textureStorage;
    IdsStorage *idsStorage = 0;
};

#endif // ABSTRACTBRUSH_H
