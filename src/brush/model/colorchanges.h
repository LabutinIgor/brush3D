#ifndef COLORCHANGES_H
#define COLORCHANGES_H

#include <utility>
#include <vector>

#include "glm.hpp"
#include "color.h"

class ColorChanges {
public:
    ColorChanges();
    void addChange(glm::i32vec2 pixel, Color oldColor, Color newColor);
    void removeLast();
    uint32_t getSize();
    std::pair<glm::i32vec2, std::pair<Color, Color>> getChange(uint32_t id);
    void addAll(ColorChanges other);

private:
    std::vector<std::pair<glm::i32vec2, std::pair<Color, Color>>> changes;
};

#endif // COLORCHANGES_H
