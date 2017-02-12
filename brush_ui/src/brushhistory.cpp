#include "brushhistory.hpp"

BrushHistory::BrushHistory() {
}

void BrushHistory::addStroke(const Brush::BrushStroke& stroke) {
    while (strokes_.size() > currentStroke_) {
        strokes_.pop_back();
    }
    strokes_.push_back(stroke);
    ++currentStroke_;
}

void BrushHistory::undo(Brush::TextureStorage& currentTexture) {
    if (currentStroke_ == 0) {
        return;
    }
    --currentStroke_;
    strokes_[currentStroke_].undo(currentTexture);
}

void BrushHistory::redo(Brush::TextureStorage& currentTexture) {
    if (currentStroke_ == strokes_.size()) {
        return;
    }
    strokes_[currentStroke_].apply(currentTexture);
    ++currentStroke_;
}
