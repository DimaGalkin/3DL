#pragma once

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
            explicit Vec3(const Vec3& other);
            Vec3();

            double x;
            double y;
            double z;

            ROTATEORDER order_;

            Vector3 asGPUType() const;

            void rotateX(double angle);
            void rotateY(double angle);
            void rotateZ(double angle);
            void rotate(double x, double y, double z);

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
            explicit Vec2(const Vec2& other);
            Vec2();

            double x;
            double y;

            Vector2 asGPUType() const;

            void rotate(double angle);

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
};