#ifndef BRUSHHISTORY_H
#define BRUSHHISTORY_H

#include <vector>

#include "src/brush/include/brushstroke.h"
#include "src/brush/include/matrix.h"

class BrushHistory {
public:
    BrushHistory();

    void addStroke(const BrushStroke &stroke);

    void undo(TextureStorage &currentTexture);

    void redo(TextureStorage &currentTexture);

private:
    std::vector<BrushStroke> strokes_;
    uint32_t currentStroke_ = 0;
};

#endif // BRUSHHISTORY_H
