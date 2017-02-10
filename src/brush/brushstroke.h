#ifndef BRUSHSTROKE_H
#define BRUSHSTROKE_H

#include <vector>
#include <glm.hpp>

#include "model/colorchange.h"
#include "model/matrix.h"

class BrushStroke {
public:
    BrushStroke();

    explicit BrushStroke(const ColorChange &colorChange);

    void add(const ColorChange &diff);

    void add(const BrushStroke &stroke);

    void apply(TextureStorage &currentTexture) const;

    void undo(TextureStorage &currentTexture) const;

private:
    std::vector<ColorChange> diff_;
};

#endif // BRUSHSTROKE_H
