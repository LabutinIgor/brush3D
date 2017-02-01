#ifndef COLOR_H
#define COLOR_H

#include <inttypes.h>

class Color {
public:
    Color();
    Color(uint8_t r, uint8_t g, uint8_t b);

    uint8_t getR();
    uint8_t getG();
    uint8_t getB();
private:
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

#endif // COLOR_H
