#include "colorchange.h"

ColorChange::ColorChange(const glm::u32vec2 &pixel, const glm::u8vec3 &previousColor, const glm::u8vec3 &newColor) :
        pixel(pixel), previousColor(previousColor), newColor(newColor) {
}
