#ifndef COLORCHANGE_H
#define COLORCHANGE_H

#include <glm/glm.hpp>

namespace Brush {
    struct ColorChange {
        ColorChange(const glm::u32vec2& pixel, const glm::u8vec3& previousColor, const glm::u8vec3& newColor);

        glm::u32vec2 pixel;
        glm::u8vec3 previousColor;
        glm::u8vec3 newColor;
    };
}

#endif // COLORCHANGE_H
