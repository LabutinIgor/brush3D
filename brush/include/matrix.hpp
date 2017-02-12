#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <glm/glm.hpp>

namespace Brush {
    template<class T>
    class Matrix {
    public:
        Matrix();
        Matrix(size_t width, size_t height);
        size_t getWidth() const;
        size_t getHeight() const;
        glm::i32vec2 getSize() const;
        T getValue(size_t row, size_t col) const;
        T getValue(const glm::u32vec2& point) const;
        void setValue(size_t row, size_t col, const T& value);
        void setValue(const glm::u32vec2& point, const T& value);

    private:
        size_t width_;
        size_t height_;
        std::vector<T> values_;
    };

    typedef uint32_t IdType;
    typedef Matrix<IdType> IdsStorage;
    typedef Matrix<glm::u8vec3> TextureStorage;


    template<class T>
    Matrix<T>::Matrix() : width_(0), height_(0) {
    }

    template<class T>
    Matrix<T>::Matrix(size_t width, size_t height) : width_(width), height_(height), values_(width * height) {
    }

    template<class T>
    size_t Matrix<T>::getWidth() const {
        return width_;
    }

    template<class T>
    size_t Matrix<T>::getHeight() const {
        return height_;
    }

    template<class T>
    glm::i32vec2 Matrix<T>::getSize() const {
        return glm::i32vec2(getWidth(), getHeight());
    }

    template<class T>
    T Matrix<T>::getValue(size_t row, size_t col) const {
        return values_[row * height_ + col];
    }

    template<class T>
    T Matrix<T>::getValue(const glm::u32vec2& point) const {
        return values_[point.x * height_ + point.y];
    }

    template<class T>
    void Matrix<T>::setValue(size_t row, size_t col, const T& value) {
        values_[row * height_ + col] = value;
    }

    template<class T>
    void Matrix<T>::setValue(const glm::u32vec2& point, const T& value) {
        values_[point.x * height_ + point.y] = value;
    }
}

#endif // MATRIX_H
