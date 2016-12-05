#include "brushhistory.h"

BrushHistory::BrushHistory() {
}

void BrushHistory::addStroke(BrushStroke stroke) {
    while (strokes.size() > currentStroke) {
        strokes.pop_back();
    }
    strokes.push_back(stroke);
    currentStroke++;
}

void BrushHistory::undo(QImage *currentTexture) {
    if (currentStroke == 0) {
        return;
    }
    currentStroke--;

    auto diff = strokes[currentStroke].getDiff();
    for (int i = diff.size() - 1; i >= 0; i--) {
        currentTexture->setPixelColor(diff[i].first, diff[i].second.first);
    }
}

void BrushHistory::redo(QImage *currentTexture) {
    if (currentStroke == strokes.size()) {
        return;
    }

    auto diff = strokes[currentStroke].getDiff();
    for (int i = diff.size() - 1; i >= 0; i--) {
        currentTexture->setPixelColor(diff[i].first, diff[i].second.second);
    }

    currentStroke++;
}
