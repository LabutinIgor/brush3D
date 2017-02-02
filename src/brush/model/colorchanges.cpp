#include "colorchanges.h"

ColorChanges::ColorChanges() {
}

void ColorChanges::addChange(glm::i32vec2 pixel, Color oldColor, Color newColor) {
    changes.push_back({pixel, {oldColor, newColor}});
}

void ColorChanges::removeLast() {
    changes.pop_back();
}

uint32_t ColorChanges::getSize() {
    return changes.size();
}

std::pair<glm::i32vec2, std::pair<Color, Color>> ColorChanges::getChange(uint32_t id) {
    return changes[id];
}

void ColorChanges::addAll(ColorChanges other) {
    changes.insert(std::end(changes), std::begin(other.changes), std::end(other.changes));
}
