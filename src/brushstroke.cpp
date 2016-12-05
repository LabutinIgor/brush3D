#include "brushstroke.h"

BrushStroke::BrushStroke() {
}

BrushStroke::BrushStroke(std::vector<std::pair<QPoint, std::pair<QColor, QColor>>> &diff) : diff(diff) {
}

void BrushStroke::add(std::vector<std::pair<QPoint, std::pair<QColor, QColor>>> &diff) {
    this->diff.insert(std::end(this->diff), std::begin(diff), std::end(diff));
}

std::vector<std::pair<QPoint, std::pair<QColor, QColor>>> BrushStroke::getDiff() {
    return diff;
}
