#pragma once

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

    class Vec3 {
        public:
            Vec3(double other_x, double other_y, double other_z);
            Vec3(const Vec3& other);
            Vec3();

            double x;
            double y;
            double z;

            ROTATEORDER order_;

            Vector3 asGPUType() const;

            void rotateX(const double angle);
            void rotateY(const double angle);
            void rotateZ(const double angle);
            void rotate(const double x, const double y, const double z);

            void normalize();
            Vec3 normalized() const;

            Vec3 cross(const Vec3& other) const;
            double dot(const Vec3& other) const;
            double distance(const Vec3& other) const;

            bool operator==(const Vec3& other) const;
            bool operator!=(const Vec3& other) const;

            Vec3 operator+(const Vec3& other) const;
            Vec3 operator-(const Vec3& other) const;
            Vec3 operator*(const double other) const;
            Vec3 operator/(const Vec3& other) const;

            void operator+=(const Vec3& other);
            void operator-=(const Vec3& other);
            void operator*=(const double other);
            void operator/=(const Vec3& other);

            ~Vec3() = default;
    } typedef vec3;

    class Vec2 {
        public:
            Vec2(double other_x, double other_y);
            Vec2(const Vec2& other);
            Vec2();

            double x;
            double y;

            Vector2 asGPUType() const;

            void rotate(const double angle);

            void normalize();
            Vec2 normalized() const;

            bool operator==(const Vec2& other) const;
            bool operator!=(const Vec2& other) const;

            Vec2 operator+(const Vec2& other) const;
            Vec2 operator-(const Vec2& other) const;
            Vec2 operator*(const double other) const;
            Vec2 operator/(const Vec2& other) const;

            void operator+=(const Vec2& other);
            void operator-=(const Vec2& other);
            void operator*=(const double other);
            void operator/=(const Vec2& other);

            ~Vec2() = default;
    } typedef vec2;

    const vec3 zero_vec3 = vec3{0, 0, 0};
    const vec2 zero_vec2 = vec2{0, 0};
};