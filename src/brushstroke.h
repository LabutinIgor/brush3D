#ifndef BRUSHSTROKE_H
#define BRUSHSTROKE_H

#include <vector>
#include <glm.hpp>

#include "color.h"

class BrushStroke {
public:
    BrushStroke();
    BrushStroke(std::vector<std::pair<glm::i32vec2, std::pair<Color, Color>>> &diff);
    void add(std::vector<std::pair<glm::i32vec2, std::pair<Color, Color>>> &diff);
    std::vector<std::pair<glm::i32vec2, std::pair<Color, Color>>> getDiff();

private:
    std::vector<std::pair<glm::i32vec2, std::pair<Color, Color>>> diff;
};

#endif // BRUSHSTROKE_H
