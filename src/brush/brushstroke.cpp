#include "brushstroke.h"

BrushStroke::BrushStroke() {
}

BrushStroke::BrushStroke(std::vector<std::pair<glm::i32vec2, std::pair<Color, Color>>> &diff) : diff(diff) {
}

void BrushStroke::add(std::vector<std::pair<glm::i32vec2, std::pair<Color, Color>>> &diff) {
    this->diff.insert(std::end(this->diff), std::begin(diff), std::end(diff));
}

std::vector<std::pair<glm::i32vec2, std::pair<Color, Color>>> BrushStroke::getDiff() {
    return diff;
}
