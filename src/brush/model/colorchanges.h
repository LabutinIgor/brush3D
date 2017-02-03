#ifndef COLORCHANGES_H
#define COLORCHANGES_H

#include <utility>
#include <vector>
#include <glm.hpp>

class ColorChanges {
public:
    ColorChanges();
    void addChange(glm::i32vec2 pixel, glm::u8vec3 oldColor, glm::u8vec3 newColor);
    void removeLast();
    uint32_t getSize();
    std::pair<glm::i32vec2, std::pair<glm::u8vec3, glm::u8vec3>> getChange(uint32_t id);
    void addAll(ColorChanges other);

private:
    std::vector<std::pair<glm::i32vec2, std::pair<glm::u8vec3, glm::u8vec3>>> changes;
};

#endif // COLORCHANGES_H
