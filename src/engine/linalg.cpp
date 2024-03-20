#include <stdexcept>
#include "linalg.hpp"

ThreeDL::Vec3::Vec3(double other_x, double other_y, double other_z)
    : x(other_x),
      y(other_y),
      z(other_z),
      order_(ROTATEORDER::YXZ)
{}

ThreeDL::Vec3::Vec3(const Vec3& other)
    : x(other.x),
      y(other.y),
      z(other.z),
      order_(other.order_)
{}

ThreeDL::Vec3::Vec3()
    : x(0),
      y(0),
      z(0),
      order_(ROTATEORDER::YXZ)
{}

Vector3 ThreeDL::Vec3::asGPUType() const {
    return Vector3 {x, y, z};
}

void ThreeDL::Vec3::rotateX(const double angle) {
    double radians = angle * M_PI / 180;
    double temp_y = y;
    y = y * cos(radians) - z * sin(radians);
    z = temp_y * sin(radians) + z * cos(radians);
}

void ThreeDL::Vec3::rotateY(const double angle) {
    double radians = angle * M_PI / 180;
    double temp_x = x;
    x = x * cos(radians) + z * sin(radians);
    z = -temp_x * sin(radians) + z * cos(radians);
}

void ThreeDL::Vec3::rotateZ(const double angle) {
    double radians = angle * M_PI / 180;
    double temp_x = x;
    x = x * cos(radians) - y * sin(radians);
    y = temp_x * sin(radians) + y * cos(radians);
}

void ThreeDL::Vec3::rotate(const double x, const double y, const double z) {
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

void ThreeDL::Vec3::normalize() {
    double length = sqrt(x * x + y * y + z * z);
    x /= length;
    y /= length;
    z /= length;
}

ThreeDL::Vec3 ThreeDL::Vec3::normalized() const {
    double length = sqrt(x * x + y * y + z * z);
    return Vec3 {x / length, y / length, z / length};
}

ThreeDL::Vec3 ThreeDL::Vec3::cross(const Vec3& other) const {
    return Vec3 {
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    };
}

double ThreeDL::Vec3::dot(const Vec3& other) const {
    return x * other.x + y * other.y + z * other.z;
}

double ThreeDL::Vec3::distance(const Vec3& other) const {
    return sqrt(pow(x - other.x, 2) + pow(y - other.y, 2) + pow(z - other.z, 2));
}

bool ThreeDL::Vec3::operator==(const Vec3& other) const {
    return x == other.x && y == other.y && z == other.z;
}

bool ThreeDL::Vec3::operator!=(const Vec3& other) const {
    return x != other.x || y != other.y || z != other.z;
}

ThreeDL::Vec3 ThreeDL::Vec3::operator+(const Vec3& other) const {
    return Vec3 {x + other.x, y + other.y, z + other.z};
}

ThreeDL::Vec3 ThreeDL::Vec3::operator-(const Vec3& other) const {
    return Vec3 {x - other.x, y - other.y, z - other.z};
}

ThreeDL::Vec3 ThreeDL::Vec3::operator*(const double other) const {
    return Vec3 {x * other, y * other, z * other};
}

ThreeDL::Vec3 ThreeDL::Vec3::operator/(const Vec3& other) const {
    return Vec3 {x / other.x, y / other.y, z / other.z};
}

void ThreeDL::Vec3::operator+=(const Vec3& other) {
    x += other.x;
    y += other.y;
    z += other.z;
}

void ThreeDL::Vec3::operator-=(const Vec3& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
}

void ThreeDL::Vec3::operator*=(const double other) {
    x *= other;
    y *= other;
    z *= other;
}

void ThreeDL::Vec3::operator/=(const Vec3& other) {
    x /= other.x;
    y /= other.y;
    z /= other.z;
}

ThreeDL::Vec2::Vec2(double other_x, double other_y)
    : x(other_x),
      y(other_y)
{}

ThreeDL::Vec2::Vec2(const Vec2& other)
    : x(other.x),
      y(other.y)
{}

ThreeDL::Vec2::Vec2()
    : x(0),
      y(0)
{}

Vector2 ThreeDL::Vec2::asGPUType() const {
    return Vector2 {x, y};
}

void ThreeDL::Vec2::rotate(const double angle) {
    double temp_x = x;
    x = x * cos(angle) - y * sin(angle);
    y = temp_x * sin(angle) + y * cos(angle);
}

void ThreeDL::Vec2::normalize() {
    double length = sqrt(x * x + y * y);
    x /= length;
    y /= length;
}

ThreeDL::Vec2 ThreeDL::Vec2::normalized() const {
    double length = sqrt(x * x + y * y);
    return Vec2 {x / length, y / length};
}

bool ThreeDL::Vec2::operator==(const Vec2& other) const {
    return x == other.x && y == other.y;
}

bool ThreeDL::Vec2::operator!=(const Vec2& other) const {
    return x != other.x || y != other.y;
}

ThreeDL::Vec2 ThreeDL::Vec2::operator+(const Vec2& other) const {
    return Vec2 {x + other.x, y + other.y};
}

ThreeDL::Vec2 ThreeDL::Vec2::operator-(const Vec2& other) const {
    return Vec2 {x - other.x, y - other.y};
}

ThreeDL::Vec2 ThreeDL::Vec2::operator*(const double other) const {
    return Vec2 {x * other, y * other};
}

ThreeDL::Vec2 ThreeDL::Vec2::operator/(const Vec2& other) const {
    return Vec2 {x / other.x, y / other.y};
}

void ThreeDL::Vec2::operator+=(const Vec2& other) {
    x += other.x;
    y += other.y;
}

void ThreeDL::Vec2::operator-=(const Vec2& other) {
    x -= other.x;
    y -= other.y;
}

void ThreeDL::Vec2::operator*=(const double other) {
    x *= other;
    y *= other;
}

void ThreeDL::Vec2::operator/=(const Vec2& other) {
    x /= other.x;
    y /= other.y;
}