#include "brushhistory.h"

BrushHistory::BrushHistory() {
}

void BrushHistory::addStroke(const BrushStroke& stroke) {
    while (strokes_.size() > currentStroke_) {
        strokes_.pop_back();
    }
    strokes_.push_back(stroke);
    currentStroke_++;
}

void BrushHistory::undo(TextureStorage& currentTexture) {
    if (currentStroke_ == 0) {
        return;
    }
    currentStroke_--;

    auto diff = strokes_[currentStroke_].getDiff();
    for (int i = diff.size() - 1; i >= 0; i--) {
        currentTexture.setColor(diff[i].pixel, diff[i].previousColor);
    }
}

void BrushHistory::redo(TextureStorage& currentTexture) {
    if (currentStroke_ == strokes_.size()) {
        return;
    }

    auto diff = strokes_[currentStroke_].getDiff();
    for (int i = diff.size() - 1; i >= 0; i--) {
        currentTexture.setColor(diff[i].pixel, diff[i].previousColor);
    }

    currentStroke_++;
}
