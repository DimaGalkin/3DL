#pragma once // include guard for templates

#include "linalg.hpp"

template <class vT_>
ThreeDL::Vec3<vT_>::Vec3(const vT_ other_x, const vT_ other_y, const vT_ other_z)
    : x(other_x),
      y(other_y),
      z(other_z),
      order_(ROTATEORDER::YXZ)
{}

template <class vT_>
ThreeDL::Vec3<vT_>::Vec3(const ThreeDL::Vec3<vT_>& other)
    : x(other.x),
      y(other.y),
      z(other.z),
      order_(other.order_)
{}

template <class vT_>
ThreeDL::Vec3<vT_>::Vec3()
    : x(0),
      y(0),
      z(0),
      order_(ROTATEORDER::YXZ)
{}

template <class vT_>
Vector3 ThreeDL::Vec3<vT_>::asGPUType() const {
    return Vector3 {
        static_cast<float>(x),
        static_cast<float>(y),
        static_cast<float>(z)
    };
}

template <class vT_>
void ThreeDL::Vec3<vT_>::rotateX(const float angle) {
    const float radians = static_cast<float>(angle * M_PI) / 180.0f;

    const vT_ y_t = static_cast<vT_>(y * cos(radians)) - static_cast<vT_>(z * sin(radians));
    const vT_ z_t = static_cast<vT_>(y * sin(radians)) + static_cast<vT_>(z * cos(radians));

    y = y_t;
    z = z_t;
}

template <class vT_>
void ThreeDL::Vec3<vT_>::rotateY(const float angle) {
    const float radians = static_cast<float>(angle * M_PI) / 180.0f;

    const vT_ x_t = static_cast<float>(x * cos(radians)) + static_cast<float>(z * sin(radians));
    const vT_ z_t = static_cast<float>(-x * sin(radians)) + static_cast<float>(z * cos(radians));

    x = x_t;
    z = z_t;
}

template <class vT_>
void ThreeDL::Vec3<vT_>::rotateZ(const float angle) {
    const float radians = static_cast<float>(angle * M_PI) / 180.0f;

    const vT_ x_t = static_cast<float>(x * cos(radians)) - static_cast<float>(y * sin(radians));
    const vT_ y_t = static_cast<float>(x * sin(radians)) + static_cast<float>(y * cos(radians));

    x = x_t;
    y = y_t;
}

template <class vT_>
void ThreeDL::Vec3<vT_>::rotate(const float x, const float y, const float z) {
    switch (order_) {
        case ROTATEORDER::XYZ:
            rotateX(x);
            rotateY(y);
            rotateZ(z);
            break;

        case ROTATEORDER::XZY:
            rotateX(x);
            rotateZ(z);
            rotateY(y);
            break;

        case ROTATEORDER::YXZ:
            rotateY(y);
            rotateX(x);
            rotateZ(z);
            break;

        case ROTATEORDER::YZX:
            rotateY(y);
            rotateZ(z);
            rotateX(x);
            break;

        case ROTATEORDER::ZXY:
            rotateZ(z);
            rotateX(x);
            rotateY(y);
            break;

        case ROTATEORDER::ZYX:
            rotateZ(z);
            rotateY(y);
            rotateX(x);
            break;

        default:
            throw std::runtime_error("Invalid rotation order");
    }
}

template <class vT_>
void ThreeDL::Vec3<vT_>::normalize() {
    float length = sqrt(x * x + y * y + z * z);
    x /= length;
    y /= length;
    z /= length;
}

template <class vT_>
ThreeDL::Vec3<vT_> ThreeDL::Vec3<vT_>::normalized() const {
    float length = sqrt(x * x + y * y + z * z);

    return Vec3<vT_> {
        static_cast<vT_>(x / length),
        static_cast<vT_>(y / length),
        static_cast<vT_>(z / length)
    };
}

template <class vT_>
ThreeDL::Vec3<vT_> ThreeDL::Vec3<vT_>::cross(const Vec3<vT_>& other) const {

    return Vec3<vT_> {
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    };
}

template <class vT_>
float ThreeDL::Vec3<vT_>::dot(const Vec3<vT_>& other) const {
    return x * other.x + y * other.y + z * other.z;
}

template <class vT_>
float ThreeDL::Vec3<vT_>::distance(const Vec3<vT_>& other) const {
    return sqrt(pow(x - other.x, 2) + pow(y - other.y, 2) + pow(z - other.z, 2));
}

template <class vT_>
bool ThreeDL::Vec3<vT_>::operator==(const Vec3<vT_>& other) const {
    return x == other.x && y == other.y && z == other.z;
}

template <class vT_>
bool ThreeDL::Vec3<vT_>::operator!=(const Vec3<vT_>& other) const {
    return x != other.x || y != other.y || z != other.z;
}

template <class vT_>
ThreeDL::Vec3<vT_> ThreeDL::Vec3<vT_>::operator+(const Vec3<vT_>& other) const {
    return Vec3<vT_> {x + other.x, y + other.y, z + other.z};
}

template <class vT_>
ThreeDL::Vec3<vT_> ThreeDL::Vec3<vT_>::operator-(const Vec3<vT_>& other) const {
    return Vec3<vT_> {x - other.x, y - other.y, z - other.z};
}

template <class vT_>
ThreeDL::Vec3<vT_> ThreeDL::Vec3<vT_>::operator*(const float other) const {
    return Vec3<vT_> {
        static_cast<vT_>(x * other),
        static_cast<vT_>(y * other),
        static_cast<vT_>(z * other)
    };
}

template <class vT_>
ThreeDL::Vec3<vT_> ThreeDL::Vec3<vT_>::operator/(const float other) const {
    return Vec3<vT_> {
        static_cast<vT_>(x / other),
        static_cast<vT_>(y / other),
        static_cast<vT_>(z / other)
    };
}

template <class vT_>
void ThreeDL::Vec3<vT_>::operator+=(const Vec3<vT_>& other) {
    x += other.x;
    y += other.y;
    z += other.z;
}

template <class vT_>
void ThreeDL::Vec3<vT_>::operator-=(const Vec3<vT_>& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
}

template <class vT_>
void ThreeDL::Vec3<vT_>::operator*=(const float other) {
    x = static_cast<vT_>(x * other);
    y = static_cast<vT_>(y * other);
    z = static_cast<vT_>(z * other);
}

template <class vT_>
void ThreeDL::Vec3<vT_>::operator/=(const float other) {
    x = static_cast<vT_>(x / other);
    y = static_cast<vT_>(y / other);
    z = static_cast<vT_>(z / other);
}

template <class vT_>
ThreeDL::Vec2<vT_>::Vec2(vT_ other_x, vT_ other_y)
    : x(other_x),
      y(other_y)
{}

template <class vT_>
ThreeDL::Vec2<vT_>::Vec2(const Vec2<vT_>& other)
    : x(other.x),
      y(other.y)
{}

template <class vT_>
ThreeDL::Vec2<vT_>::Vec2()
    : x(0),
      y(0)
{}

template <class vT_>
Vector2 ThreeDL::Vec2<vT_>::asGPUType() const {
    return Vector2 {
        static_cast<float>(x),
        static_cast<float>(y)
    };
}

template <class vT_>
void ThreeDL::Vec2<vT_>::rotate(const float angle) {
    const float radians = static_cast<float>(angle * M_PI) / 180.0f;

    const vT_ x_g = static_cast<vT_>(x * cos(radians)) - static_cast<vT_>(y * sin(radians));
    const vT_ y_g = static_cast<vT_>(x * sin(radians)) + static_cast<vT_>(y * cos(radians));

    x = x_g;
    y = y_g;
}

template <class vT_>
void ThreeDL::Vec2<vT_>::normalize() {
    const float length = sqrt(x * x + y * y);
    x /= length;
    y /= length;
}

template <class vT_>
ThreeDL::Vec2<vT_> ThreeDL::Vec2<vT_>::normalized() const {
    const float length = sqrt(x * x + y * y);
    return Vec2<vT_> {
        static_cast<vT_>(x / length),
        static_cast<vT_>(y / length)
    };
}

template <class vT_>
bool ThreeDL::Vec2<vT_>::operator==(const Vec2<vT_>& other) const {
    return x == other.x && y == other.y;
}

template <class vT_>
bool ThreeDL::Vec2<vT_>::operator!=(const Vec2<vT_>& other) const {
    return x != other.x || y != other.y;
}

template <class vT_>
ThreeDL::Vec2<vT_> ThreeDL::Vec2<vT_>::operator+(const Vec2<vT_>& other) const {
    return Vec2<vT_> {x + other.x, y + other.y};
}

template <class vT_>
ThreeDL::Vec2<vT_> ThreeDL::Vec2<vT_>::operator-(const Vec2<vT_>& other) const {
    return Vec2<vT_> {x - other.x, y - other.y};
}

template <class vT_>
ThreeDL::Vec2<vT_> ThreeDL::Vec2<vT_>::operator*(const float other) const {
    return Vec2<vT_> {
        static_cast<vT_>(x * other),
        static_cast<vT_>(y * other)
    };
}

template <class vT_>
ThreeDL::Vec2<vT_> ThreeDL::Vec2<vT_>::operator/(const float other) const {
    return Vec2<vT_> {
        static_cast<vT_>(x / other),
        static_cast<vT_>(y / other)
    };
}

template <class vT_>
void ThreeDL::Vec2<vT_>::operator+=(const Vec2<vT_>& other) {
    x += other.x;
    y += other.y;
}

template <class vT_>
void ThreeDL::Vec2<vT_>::operator-=(const Vec2<vT_>& other) {
    x -= other.x;
    y -= other.y;
}

template <class vT_>
void ThreeDL::Vec2<vT_>::operator*=(const float other) {
    x = static_cast<vT_>(x * other);
    y = static_cast<vT_>(y * other);
}

template <class vT_>
void ThreeDL::Vec2<vT_>::operator/=(const float other) {
    x = static_cast<vT_>(x / other);
    y = static_cast<vT_>(y / other);
}