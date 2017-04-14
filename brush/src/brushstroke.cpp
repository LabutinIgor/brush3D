#include "include/brushstroke.hpp"

namespace Brush {
    BrushStroke::BrushStroke() {
    }

    BrushStroke::BrushStroke(const ColorChange& colorChange) {
        diff_.push_back(colorChange);
    }

    void BrushStroke::add(const ColorChange& diff) {
        diff_.push_back(diff);
    }

    void BrushStroke::add(const BrushStroke& stroke) {
        diff_.insert(std::end(diff_), std::begin(stroke.diff_), std::end(stroke.diff_));
    }

    void BrushStroke::apply(TextureStorage& currentTexture) const {
        for (size_t i = 0; i < diff_.size(); ++i) {
            currentTexture.setValue(diff_[i].pixel, diff_[i].newColor);
        }
    }

    void BrushStroke::undo(TextureStorage& currentTexture) const {
        for (int32_t i = diff_.size() - 1; i >= 0; --i) {
            currentTexture.setValue(diff_[i].pixel, diff_[i].previousColor);
        }
    }
}
