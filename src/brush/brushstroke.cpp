#include "brushstroke.h"

BrushStroke::BrushStroke() {
}

BrushStroke::BrushStroke(ColorChanges &diff) : diff(diff) {
}

void BrushStroke::add(ColorChanges &diff) {
    this->diff.addAll(diff);
}

ColorChanges BrushStroke::getDiff() {
    return diff;
}
