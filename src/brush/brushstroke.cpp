#include "brushstroke.h"

BrushStroke::BrushStroke() {
}

BrushStroke::BrushStroke(const ColorChange& colorChange) {
    diff_.push_back(colorChange);
}

void BrushStroke::add(const ColorChange& diff) {
    diff_.push_back(diff);
}

void BrushStroke::addAll(const BrushStroke& stroke) {
    diff_.insert(std::end(diff_), std::begin(stroke.diff_), std::end(stroke.diff_));
}

const std::vector<ColorChange>& BrushStroke::getDiff() const {
    return diff_;
}
