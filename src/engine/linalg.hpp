#pragma once

#ifndef float3
typedef struct Vector3 float3;
#endif

#include <type_traits>
#include <stdexcept>
#include <cmath>

#include "GPU/types.h"

#define VECTOR_T_MSG "Vec3 can only be used with float, double or int types."

namespace ThreeDL {
    enum class ROTATEORDER {
        XYZ,
        XZY,
        YXZ,
        YZX,
        ZXY,
        ZYX
    };

    template <class _dT>
    class Vec3 {
        // static_assert(std::is_base_of<double, _dT>::value, VECTOR_T_MSG);
        // static_assert(std::is_base_of<float, _dT>::value, VECTOR_T_MSG);
        // static_assert(std::is_base_of<int, _dT>::value, VECTOR_T_MSG);

        public:
            Vec3<_dT>(const _dT other_x, const _dT other_y, const _dT other_z);
            Vec3<_dT>(const Vec3<_dT>& other); // copy constructor not to be declared as explicit
            Vec3<_dT>();

            _dT x;
            _dT y;
            _dT z;

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
            Vec3 operator/(const float other) const;

            void operator+=(const Vec3& other);
            void operator-=(const Vec3& other);
            void operator*=(const float other);
            void operator/=(const float other);

            ~Vec3() = default;
    };

    typedef Vec3<float> vec3;
    typedef Vec3<double> vec3d;
    typedef Vec3<int> vec3i;

    template <class _dT>
    class Vec2 {
        public:
            Vec2<_dT>(_dT other_x, _dT other_y);
            Vec2<_dT>(const Vec2<_dT>& other); // copy constructor not to be declared as explicit
            Vec2<_dT>();

            _dT x;
            _dT y;

            Vector2 asGPUType() const;

            void rotate(const float angle);

            void normalize();
            Vec2 normalized() const;

            bool operator==(const Vec2<_dT>& other) const;
            bool operator!=(const Vec2<_dT>& other) const;

            Vec2<_dT> operator+(const Vec2<_dT>& other) const;
            Vec2<_dT> operator-(const Vec2<_dT>& other) const;
            Vec2<_dT> operator*(const float other) const;
            Vec2<_dT> operator/(const float other) const;

            void operator+=(const Vec2<_dT>& other);
            void operator-=(const Vec2<_dT>& other);
            void operator*=(const float other);
            void operator/=(const float other);

            ~Vec2() = default;
    };

    typedef Vec2<float> vec2;
    typedef Vec2<double> vec2d;
    typedef Vec2<int> vec2i;
};