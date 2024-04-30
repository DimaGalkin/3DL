#pragma once

#include <cmath>

#ifndef float3
typedef struct Vector3 float3;
#endif
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
            Vec3(float other_x, float other_y, float other_z);
            Vec3(const Vec3& other);
            Vec3();

            float x;
            float y;
            float z;

            ROTATEORDER order_;

            Vector3 asGPUType() const;

            void rotateX(const float angle);
            void rotateY(const float angle);
            void rotateZ(const float angle);
            void rotate(const float x, const float y, const float z);

            void normalize();
            Vec3 normalized() const;

            Vec3 cross(const Vec3& other) const;
            float dot(const Vec3& other) const;
            float distance(const Vec3& other) const;

            bool operator==(const Vec3& other) const;
            bool operator!=(const Vec3& other) const;

            Vec3 operator+(const Vec3& other) const;
            Vec3 operator-(const Vec3& other) const;
            Vec3 operator*(const float other) const;
            Vec3 operator/(const Vec3& other) const;

            void operator+=(const Vec3& other);
            void operator-=(const Vec3& other);
            void operator*=(const float other);
            void operator/=(const Vec3& other);

            ~Vec3() = default;
    } typedef vec3;

    class Vec2 {
        public:
            Vec2(float other_x, float other_y);
            Vec2(const Vec2& other);
            Vec2();

            float x;
            float y;

            Vector2 asGPUType() const;

            void rotate(const float angle);

            void normalize();
            Vec2 normalized() const;

            bool operator==(const Vec2& other) const;
            bool operator!=(const Vec2& other) const;

            Vec2 operator+(const Vec2& other) const;
            Vec2 operator-(const Vec2& other) const;
            Vec2 operator*(const float other) const;
            Vec2 operator/(const Vec2& other) const;

            void operator+=(const Vec2& other);
            void operator-=(const Vec2& other);
            void operator*=(const float other);
            void operator/=(const Vec2& other);

            ~Vec2() = default;
    } typedef vec2;

    const vec3 zero_vec3 = vec3{0, 0, 0};
    const vec2 zero_vec2 = vec2{0, 0};
};