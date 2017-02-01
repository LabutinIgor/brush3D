#include "color.h"

Color::Color() {
}

Color::Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {
}

uint8_t Color::getR() {
    return r;
}

uint8_t Color::getG() {
    return g;
}

uint8_t Color::getB() {
    return b;
}
