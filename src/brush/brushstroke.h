#ifndef BRUSHSTROKE_H
#define BRUSHSTROKE_H

#include <vector>
#include <glm.hpp>

#include "colorchange.h"

class BrushStroke {
public:
    BrushStroke();
    BrushStroke(const ColorChange& colorChange);
    void add(const ColorChange& diff);
    void addAll(const BrushStroke& stroke);
    const std::vector<ColorChange>& getDiff() const;

private:
    std::vector<ColorChange> diff;
};

#endif // BRUSHSTROKE_H
