#ifndef BRUSHHISTORY_H
#define BRUSHHISTORY_H

#include <vector>

#include "brushstroke.h"
#include "texturestorage.h"

class BrushHistory {
public:
    BrushHistory();
    void addStroke(BrushStroke stroke);
    void undo(TextureStorage *currentTexture);
    void redo(TextureStorage *currentTexture);

private:
    std::vector<BrushStroke> strokes;
    uint32_t currentStroke = 0;
};

#endif // BRUSHHISTORY_H
