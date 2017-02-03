#include "colorchange.h"

ColorChange::ColorChange(glm::u32vec2 pixel, glm::u8vec3 previousColor, glm::u8vec3 newColor) :
    pixel(pixel), previousColor(previousColor), newColor(newColor) {
}
