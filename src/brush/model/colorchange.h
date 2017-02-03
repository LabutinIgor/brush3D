#ifndef COLORCHANGE_H
#define COLORCHANGE_H

#include <glm.hpp>

struct ColorChange {
    ColorChange(glm::u32vec2 pixel, glm::u8vec3 previousColor, glm::u8vec3 newColor);
    glm::u32vec2 pixel;
    glm::u8vec3 previousColor;
    glm::u8vec3 newColor;
};

#endif // COLORCHANGE_H
