#ifndef BRUSHSTROKE_H
#define BRUSHSTROKE_H

#include <vector>
#include <QPoint>
#include <QColor>

class BrushStroke {
public:
    BrushStroke();
    BrushStroke(std::vector<std::pair<QPoint, std::pair<QColor, QColor>>> &diff);
    void add(std::vector<std::pair<QPoint, std::pair<QColor, QColor>>> &diff);
    std::vector<std::pair<QPoint, std::pair<QColor, QColor>>> getDiff();

private:
    std::vector<std::pair<QPoint, std::pair<QColor, QColor>>> diff;
};

#endif // BRUSHSTROKE_H
