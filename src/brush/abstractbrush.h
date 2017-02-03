#ifndef ABSTRACTBRUSH_H
#define ABSTRACTBRUSH_H

#include <vector>
#include <glm.hpp>

#include "geometry.h"
#include "texturestorage.h"
#include "idsstorage.h"
#include "objectmodel.h"
#include "colorchanges.h"

class AbstractBrush {
public:
    AbstractBrush(ObjectModel* objectModel, TextureStorage* textureStorage);
    virtual ~AbstractBrush();
    virtual ColorChanges paint(glm::i32vec2 point, glm::mat4x4 matrixModelView, glm::mat4x4 projection, glm::i32vec2 screenSize) = 0;
    virtual ColorChanges paint(glm::i32vec2 previousPoint, glm::i32vec2 currentPoint,
                               glm::mat4x4 matrixModelView, glm::mat4x4 projection, glm::i32vec2 screenSize);
    void setRadius(double radius);
    void setColor(glm::u8vec3 color);
    void setIdsStorage(IdsStorage *idsStorage);
    TextureStorage *getTextureStorage();

protected:
    double radius = 10;
    glm::u8vec3 color = glm::u8vec3(255, 0, 0);
    ObjectModel* objectModel;
    TextureStorage *textureStorage;
    IdsStorage *idsStorage = 0;
};

#endif // ABSTRACTBRUSH_H
