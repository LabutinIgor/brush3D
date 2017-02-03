#ifndef BRUSHSTROKE_H
#define BRUSHSTROKE_H

#include <vector>
#include <glm.hpp>

#include "colorchanges.h"

class BrushStroke {
public:
    BrushStroke();
    BrushStroke(ColorChanges &diff);
    void add(ColorChanges &diff);
    ColorChanges getDiff();

private:
    ColorChanges diff;
};

#endif // BRUSHSTROKE_H
