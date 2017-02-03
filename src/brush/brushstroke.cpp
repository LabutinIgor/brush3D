#include "brushstroke.h"

BrushStroke::BrushStroke() {
}

BrushStroke::BrushStroke(const ColorChange& colorChange) {
    diff.push_back(colorChange);
}

void BrushStroke::add(const ColorChange& diff) {
    this->diff.push_back(diff);
}

void BrushStroke::addAll(const BrushStroke& stroke) {
    diff.insert(std::end(diff), std::begin(stroke.diff), std::end(stroke.diff));
}

const std::vector<ColorChange>& BrushStroke::getDiff() const {
    return diff;
}
