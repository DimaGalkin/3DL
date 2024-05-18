#pragma once

#ifndef float3
typedef struct Vector3 float3;
#endif

#include <stdexcept>
#include <cmath>

#include "GPU/types.h"

namespace ThreeDL {
    enum class ROTATEORDER {
        XYZ,
        XZY,
        YXZ,
        YZX,
        ZXY,
        ZYX
    };

    template <class vT_>
    class Vec3 {
        public:
            Vec3<vT_>(const vT_ other_x, const vT_ other_y, const vT_ other_z)
                : x {other_x},
                  y {other_y},
                  z {other_z},
                  order_ {ROTATEORDER::YXZ}
            {};

            Vec3<vT_>(const Vec3<vT_>& other)
                : x {other.x},
                  y {other.y},
                  z {other.z},
                  order_ {other.order_}
            {}; // copy constructor not to be declared as explicit

            Vec3<vT_>()
                : x {0},
                  y {0},
                  z {0},
                  order_ {ROTATEORDER::YXZ}
            {};

            vT_ x;
            vT_ y;
            vT_ z;

            ROTATEORDER order_;

            Vector3 asGPUType() const {
                return Vector3 {
                    static_cast<float>(x),
                    static_cast<float>(y),
                    static_cast<float>(z)
                };
            };

            void rotateX(const float angle) {
                const float radians = static_cast<float>(angle * M_PI) / 180.0f;

                const vT_ y_t = static_cast<vT_>(y * cos(radians)) - static_cast<vT_>(z * sin(radians));
                const vT_ z_t = static_cast<vT_>(y * sin(radians)) + static_cast<vT_>(z * cos(radians));

                y = y_t;
                z = z_t;
            };

            void rotateY(const float angle) {
                const float radians = static_cast<float>(angle * M_PI) / 180.0f;

                const vT_ x_t = static_cast<float>(x * cos(radians)) + static_cast<float>(z * sin(radians));
                const vT_ z_t = static_cast<float>(-x * sin(radians)) + static_cast<float>(z * cos(radians));

                x = x_t;
                z = z_t;
            };

            void rotateZ(const float angle) {
                const float radians = static_cast<float>(angle * M_PI) / 180.0f;

                const vT_ x_t = static_cast<float>(x * cos(radians)) - static_cast<float>(y * sin(radians));
                const vT_ y_t = static_cast<float>(x * sin(radians)) + static_cast<float>(y * cos(radians));

                x = x_t;
                y = y_t;
            };

            void rotate(const float x, const float y, const float z) {
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
            };

            void normalize() {
                float length = sqrt(x * x + y * y + z * z);
                x /= length;
                y /= length;
                z /= length;
            };

            Vec3<vT_> normalized() const {
                float length = sqrt(x * x + y * y + z * z);

                return Vec3<vT_> {
                    static_cast<vT_>(x / length),
                    static_cast<vT_>(y / length),
                    static_cast<vT_>(z / length)
                };
            };

            Vec3<vT_> cross(const Vec3<vT_>& other) const {
                return Vec3<vT_> {
                    y * other.z - z * other.y,
                    z * other.x - x * other.z,
                    x * other.y - y * other.x
                };
            };

            float dot(const Vec3<vT_>& other) const {
                return x * other.x + y * other.y + z * other.z;
            };

            float distance(const Vec3<vT_>& other) const {
                return sqrt(pow(x - other.x, 2) + pow(y - other.y, 2) + pow(z - other.z, 2));
            };

            bool operator==(const Vec3<vT_>& other) const {
                return x == other.x && y == other.y && z == other.z;
            };

            bool operator!=(const Vec3<vT_>& other) const {
                return x != other.x || y != other.y || z != other.z;
            };

            Vec3<vT_> operator+(const Vec3<vT_>& other) const {
                return Vec3<vT_> {x + other.x, y + other.y, z + other.z};
            };

            Vec3<vT_> operator-(const Vec3<vT_>& other) const {
                return Vec3<vT_> {x - other.x, y - other.y, z - other.z};
            };

            Vec3<vT_> operator*(const float other) const {
                return Vec3<vT_> {
                    static_cast<vT_>(x * other),
                    static_cast<vT_>(y * other),
                    static_cast<vT_>(z * other)
                };
            };

            Vec3<vT_> operator/(const float other) const {
                return Vec3<vT_> {
                    static_cast<vT_>(x / other),
                    static_cast<vT_>(y / other),
                    static_cast<vT_>(z / other)
                };
            };

            void operator+=(const Vec3<vT_>& other) {
                x += other.x;
                y += other.y;
                z += other.z;
            };

            void operator-=(const Vec3<vT_>& other) {
                x -= other.x;
                y -= other.y;
                z -= other.z;
            };

            void operator*=(const float other) {
                x = static_cast<vT_>(x * other);
                y = static_cast<vT_>(y * other);
                z = static_cast<vT_>(z * other);
            };

            void operator/=(const float other) {
                x = static_cast<vT_>(x / other);
                y = static_cast<vT_>(y / other);
                z = static_cast<vT_>(z / other);
            };

            ~Vec3() = default;
    };

    typedef Vec3<float> vec3;
    typedef Vec3<double> vec3d;
    typedef Vec3<int> vec3i;

    template <class vT_>
    class Vec2 {
        public:
            Vec2<vT_>(vT_ other_x, vT_ other_y)
                : x {other_x},
                  y {other_y}
            {};

            Vec2<vT_>(const Vec2<vT_>& other)
                : x {other.x},
                  y {other.y}
            {}; // copy constructor not to be declared as explicit

            Vec2<vT_>()
                : x {0},
                  y {0}
            {};

            vT_ x;
            vT_ y;

            Vector2 asGPUType() const {
                return Vector2 {
                    static_cast<float>(x),
                    static_cast<float>(y)
                };
            };

            void rotate(const float angle) {
                const float radians = static_cast<float>(angle * M_PI) / 180.0f;

                const vT_ x_g = static_cast<vT_>(x * cos(radians)) - static_cast<vT_>(y * sin(radians));
                const vT_ y_g = static_cast<vT_>(x * sin(radians)) + static_cast<vT_>(y * cos(radians));

                x = x_g;
                y = y_g;
            };

            void normalize() {
                const float length = sqrt(x * x + y * y);
                x /= length;
                y /= length;
            };

            Vec2 normalized() const {
                const float length = sqrt(x * x + y * y);
                return Vec2<vT_> {
                    static_cast<vT_>(x / length),
                    static_cast<vT_>(y / length)
                };
            };

            float distance(const Vec2<vT_>& other) const {
                return sqrt(pow(x - other.x, 2) + pow(y - other.y, 2));
            };

            bool operator==(const Vec2<vT_>& other) const {
                return x == other.x && y == other.y;
            };

            bool operator!=(const Vec2<vT_>& other) const {
                return x != other.x || y != other.y;
            };

            Vec2<vT_> operator+(const Vec2<vT_>& other) const {
                return Vec2<vT_> {x + other.x, y + other.y};
            };

            Vec2<vT_> operator-(const Vec2<vT_>& other) const {
                return Vec2<vT_> {x - other.x, y - other.y};
            };

            Vec2<vT_> operator*(const float other) const {
                return Vec2<vT_> {
                    static_cast<vT_>(x * other),
                    static_cast<vT_>(y * other)
                };
            };

            Vec2<vT_> operator/(const float other) const {
                return Vec2<vT_> {
                    static_cast<vT_>(x / other),
                    static_cast<vT_>(y / other)
                };
            };

            void operator+=(const Vec2<vT_>& other) {
                x += other.x;
                y += other.y;
            };

            void operator-=(const Vec2<vT_>& other) {
                x -= other.x;
                y -= other.y;
            };

            void operator*=(const float other) {
                x = static_cast<vT_>(x * other);
                y = static_cast<vT_>(y * other);
            };

            void operator/=(const float other) {
                x = static_cast<vT_>(x / other);
                y = static_cast<vT_>(y / other);
            };

            ~Vec2() = default;
    };

    typedef Vec2<float> vec2;
    typedef Vec2<double> vec2d;
    typedef Vec2<int> vec2i;
};