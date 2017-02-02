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

void BrushHistory::undo(TextureStorage *currentTexture) {
    if (currentStroke == 0) {
        return;
    }
    currentStroke--;

    auto diff = strokes[currentStroke].getDiff();
    for (int i = diff.getSize() - 1; i >= 0; i--) {
        currentTexture->setColor(diff.getChange(i).first.x, diff.getChange(i).first.y, diff.getChange(i).second.first);
    }
}

void BrushHistory::redo(TextureStorage *currentTexture) {
    if (currentStroke == strokes.size()) {
        return;
    }

    auto diff = strokes[currentStroke].getDiff();
    for (int i = diff.getSize() - 1; i >= 0; i--) {
        currentTexture->setColor(diff.getChange(i).first.x, diff.getChange(i).first.y, diff.getChange(i).second.second);
    }

    currentStroke++;
}
