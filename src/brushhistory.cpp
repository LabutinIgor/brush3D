#include "brushhistory.hpp"

BrushHistory::BrushHistory() {
}

void BrushHistory::addStroke(const BrushStroke &stroke) {
    while (strokes_.size() > currentStroke_) {
        strokes_.pop_back();
    }
    strokes_.push_back(stroke);
    ++currentStroke_;
}

void BrushHistory::undo(TextureStorage &currentTexture) {
    if (currentStroke_ == 0) {
        return;
    }
    --currentStroke_;
    strokes_[currentStroke_].undo(currentTexture);
}

void BrushHistory::redo(TextureStorage &currentTexture) {
    if (currentStroke_ == strokes_.size()) {
        return;
    }
    strokes_[currentStroke_].apply(currentTexture);
    ++currentStroke_;
}
