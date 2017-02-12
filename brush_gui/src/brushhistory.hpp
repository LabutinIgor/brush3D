#ifndef BRUSHHISTORY_H
#define BRUSHHISTORY_H

#include <vector>

#include "brushstroke.hpp"
#include "matrix.hpp"

class BrushHistory {
public:
    BrushHistory();
    void addStroke(const Brush::BrushStroke& stroke);
    void undo(Brush::TextureStorage& currentTexture);
    void redo(Brush::TextureStorage& currentTexture);

private:
    std::vector<Brush::BrushStroke> strokes_;
    uint32_t currentStroke_ = 0;
};

#endif // BRUSHHISTORY_H
