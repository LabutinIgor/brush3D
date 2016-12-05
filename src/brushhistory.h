#ifndef BRUSHHISTORY_H
#define BRUSHHISTORY_H

#include <vector>
#include <iostream>
#include <QImage>

#include "brushstroke.h"

class BrushHistory {
public:
    BrushHistory();
    void addStroke(BrushStroke stroke);
    void undo(QImage *currentTexture);
    void redo(QImage *currentTexture);

private:
    std::vector<BrushStroke> strokes;
    int currentStroke = 0;
};

#endif // BRUSHHISTORY_H
