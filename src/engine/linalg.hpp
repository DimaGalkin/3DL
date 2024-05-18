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
            Vec3<vT_>(const vT_ other_x, const vT_ other_y, const vT_ other_z);
            Vec3<vT_>(const Vec3<vT_>& other); // copy constructor not to be declared as explicit
            Vec3<vT_>();

            vT_ x;
            vT_ y;
            vT_ z;

            ROTATEORDER order_;

            Vector3 asGPUType() const;

            void rotateX(const float angle);
            void rotateY(const float angle);
            void rotateZ(const float angle);
            void rotate(const float x, const float y, const float z);

            void normalize();
            Vec3<vT_> normalized() const;

            Vec3<vT_> cross(const Vec3<vT_>& other) const;
            float dot(const Vec3<vT_>& other) const;
            float distance(const Vec3<vT_>& other) const;

            bool operator==(const Vec3<vT_>& other) const;
            bool operator!=(const Vec3<vT_>& other) const;

            Vec3<vT_> operator+(const Vec3<vT_>& other) const;
            Vec3<vT_> operator-(const Vec3<vT_>& other) const;
            Vec3<vT_> operator*(const float other) const;
            Vec3<vT_> operator/(const float other) const;

            void operator+=(const Vec3<vT_>& other);
            void operator-=(const Vec3<vT_>& other);
            void operator*=(const float other);
            void operator/=(const float other);

            ~Vec3() = default;
    };

    typedef Vec3<float> vec3;
    typedef Vec3<double> vec3d;
    typedef Vec3<int> vec3i;

    template <class vT_>
    class Vec2 {
        public:
            Vec2<vT_>(vT_ other_x, vT_ other_y);
            Vec2<vT_>(const Vec2<vT_>& other); // copy constructor not to be declared as explicit
            Vec2<vT_>();

            vT_ x;
            vT_ y;

            Vector2 asGPUType() const;

            void rotate(const float angle);

            void normalize();
            Vec2 normalized() const;

            bool operator==(const Vec2<vT_>& other) const;
            bool operator!=(const Vec2<vT_>& other) const;

            Vec2<vT_> operator+(const Vec2<vT_>& other) const;
            Vec2<vT_> operator-(const Vec2<vT_>& other) const;
            Vec2<vT_> operator*(const float other) const;
            Vec2<vT_> operator/(const float other) const;

            void operator+=(const Vec2<vT_>& other);
            void operator-=(const Vec2<vT_>& other);
            void operator*=(const float other);
            void operator/=(const float other);

            ~Vec2() = default;
    };

    typedef Vec2<float> vec2;
    typedef Vec2<double> vec2d;
    typedef Vec2<int> vec2i;
};