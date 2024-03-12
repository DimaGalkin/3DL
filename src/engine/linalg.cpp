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

void ThreeDL::Vec3::rotateX(double angle) {
    double temp_y = y;
    y = y * cos(angle) - z * sin(angle);
    z = temp_y * sin(angle) + z * cos(angle);
}

void ThreeDL::Vec3::rotateY(double angle) {
    double temp_x = x;
    x = x * cos(angle) + z * sin(angle);
    z = -temp_x * sin(angle) + z * cos(angle);
}

void ThreeDL::Vec3::rotateZ(double angle) {
    double temp_x = x;
    x = x * cos(angle) - y * sin(angle);
    y = temp_x * sin(angle) + y * cos(angle);
}

void ThreeDL::Vec3::rotate(double x, double y, double z) {
    rotateY(y);
    rotateX(x);
    rotateZ(z);
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

void ThreeDL::Vec2::rotate(double angle) {
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