#ifndef MATHCL_H
#define MATHCL_H

#include "types.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void SubVec3(
    const struct Vector3* a,
    const struct Vector3* b,
    struct Vector3* result
) {
    result->x = a->x - b->x;
    result->y = a->y - b->y;
    result->z = a->z - b->z;
}

void AddVec3(
    const struct Vector3* a,
    const struct Vector3* b,
    struct Vector3* result
) {
    result->x = a->x + b->x;
    result->y = a->y + b->y;
    result->z = a->z + b->z;
}

void MulVec3(
    const struct Vector3* a,
    const struct Vector3* b,
    struct Vector3* result
) {
    result->x = a->x * b->x;
    result->y = a->y * b->y;
    result->z = a->z * b->z;
}

void DivVec3(
    const struct Vector3* a,
    const struct Vector3* b,
    struct Vector3* result
) {
    result->x = a->x / b->x;
    result->y = a->y / b->y;
    result->z = a->z / b->z;
}

void AddVec3Scalar(
    const struct Vector3* a,
    const double b,
    struct Vector3* result
) {
    result->x = a->x + b;
    result->y = a->y + b;
    result->z = a->z + b;
}

void MulVec3Scalar(
    const struct Vector3* a,
    const double b,
    struct Vector3* result
) {
    result->x = a->x * b;
    result->y = a->y * b;
    result->z = a->z * b;
}

void DivVec3Scalar(
    const struct Vector3* a,
    const double b,
    struct Vector3* result
) {
    result->x = a->x / b;
    result->y = a->y / b;
    result->z = a->z / b;
}

void CrossVec3(
    const struct Vector3* a,
    const struct Vector3* b,
    struct Vector3* result
) {
    result->x = a->y * b->z - a->z * b->y;
    result->y = a->z * b->x - a->x * b->z;
    result->z = a->x * b->y - a->y * b->x;
}

double DotVec3(
    const struct Vector3* a,
    const struct Vector3* b
) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

double LengthVec3(
    const struct Vector3* a
) {
    return sqrt(a->x * a->x + a->y * a->y + a->z * a->z);
}

void NormalizeVec3(
    const struct Vector3* a,
    struct Vector3* result
) {
    double length = LengthVec3(a);
    result->x = a->x / length;
    result->y = a->y / length;
    result->z = a->z / length;
}

void RotateXVec3(
    const struct Vector3* a,
    const double angle,
    struct Vector3* result
) {
    result->x = a->x;
    result->y = a->y * cos(angle) - a->z * sin(angle);
    result->z = a->y * sin(angle) + a->z * cos(angle);
}

void RotateYVec3(
    const struct Vector3* a,
    const double angle,
    struct Vector3* result
) {
    result->x = a->x * cos(angle) + a->z * sin(angle);
    result->y = a->y;
    result->z = -a->x * sin(angle) + a->z * cos(angle);
}

void RotateZVec3(
    const struct Vector3* a,
    const double angle,
    struct Vector3* result
) {
    result->x = a->x * cos(angle) - a->y * sin(angle);
    result->y = a->x * sin(angle) + a->y * cos(angle);
    result->z = a->z;
}

void RotateVec3(
    const struct Vector3* a,
    const struct Vector3* angles,
    struct Vector3* result
) {
    RotateYVec3(a, angles->y, result);
    RotateXVec3(result, angles->x, result);
    RotateZVec3(result, angles->z, result);
}

void SubVec2(
    const struct Vector2* a,
    const struct Vector2* b,
    struct Vector2* result
) {
    result->x = a->x - b->x;
    result->y = a->y - b->y;
}

void AddVec2(
    const struct Vector2* a,
    const struct Vector2* b,
    struct Vector2* result
) {
    result->x = a->x + b->x;
    result->y = a->y + b->y;
}

void MulVec2(
    const struct Vector2* a,
    const struct Vector2* b,
    struct Vector2* result
) {
    result->x = a->x * b->x;
    result->y = a->y * b->y;
}

void DivVec2(
    const struct Vector2* a,
    const struct Vector2* b,
    struct Vector2* result
) {
    result->x = a->x / b->x;
    result->y = a->y / b->y;
}

void AddVec2Scalar(
    const struct Vector2* a,
    const double b,
    struct Vector2* result
) {
    result->x = a->x + b;
    result->y = a->y + b;
}

void MulVec2Scalar(
    const struct Vector2* a,
    const double b,
    struct Vector2* result
) {
    result->x = a->x * b;
    result->y = a->y * b;
}

void DivVec2Scalar(
    const struct Vector2* a,
    const double b,
    struct Vector2* result
) {
    result->x = a->x / b;
    result->y = a->y / b;
}

void RotateVec2(
    const struct Vector2* a,
    const double angle,
    struct Vector2* result
) {
    result->x = a->x * cos(angle) - a->y * sin(angle);
    result->y = a->x * sin(angle) + a->y * cos(angle);
}

void NormalizeVec2(
    const struct Vector2* a,
    struct Vector2* result
) {
    double length = sqrt(a->x * a->x + a->y * a->y);
    result->x = a->x / length;
    result->y = a->y / length;
}

double LengthVec2(
    const struct Vector2* a
) {
    return sqrt(a->x * a->x + a->y * a->y);
}

double DotVec2(
    const struct Vector2* a,
    const struct Vector2* b
) {
    return a->x * b->x + a->y * b->y;
}

void Vec3GammaCorrect(
    const struct Vector3* a,
    struct Vector3* result
) {
    result->x = pow(a->x, 1.0 / 2.2);
    result->y = pow(a->y, 1.0 / 2.2);
    result->z = pow(a->z, 1.0 / 2.2);
}

void calcUv(
        const struct Vector2* uv1,
        const struct Vector2* uv2,
        double t,
        struct Vector2* out
) {
    out->x = uv1->x + (uv2->x - uv1->x) * t;
    out->y = uv1->y + (uv2->y - uv1->y) * t;
}

void intersects(
        struct Plane* plane,
        struct Ray* ray,
        struct Intersection* out
) {
    double n = DotVec3(&plane->normal_, &plane->position_) - DotVec3(&plane->normal_, &ray->origin_);
    double d = DotVec3(&plane->normal_, &ray->direction_);

    if (d == 0) {
        out->position_.x = 0;
        out->position_.y = 0;
        out->position_.z = 0;
        out->valid_ = false;
        return;
    }

    double t = n / d;
    struct Vector3 position = ray->direction_;

    MulVec3Scalar(
            &position,
            t,
            &position
    );

    AddVec3(
            &position,
            &ray->origin_,
            &position
    );

    if (
            (position.z > ray->origin_.z && position.z > ray->origin_.z + ray->direction_.z) ||
            (position.z < ray->origin_.z && position.z < ray->origin_.z + ray->direction_.z) ||
            (position.x > ray->origin_.x && position.x > ray->origin_.x + ray->direction_.x) ||
            (position.x < ray->origin_.x && position.x < ray->origin_.x + ray->direction_.x) ||
            (position.y > ray->origin_.y && position.y > ray->origin_.y + ray->direction_.y) ||
            (position.y < ray->origin_.y && position.y < ray->origin_.y + ray->direction_.y)
            ) {
        out->position_.x = 0;
        out->position_.y = 0;
        out->position_.z = 0;
        out->valid_ = false;
        return;
    }

    out->position_ = position;
    out->valid_ = true;

    struct Vector3 a = position;
    SubVec3(&a, &ray->origin_, &a);
    out->t_ = LengthVec3(&a) / LengthVec3(&ray->direction_);
}

void interpolate(
        struct Vector2* start,
        struct Vector2* dir,
        double t,
        struct Vector2* out
) {
    out->x = start->x + dir->x * t;
    out->y = start->y + dir->y * t;
    out->texture_info_ = start->texture_info_ + dir->texture_info_ * t;
}

void interpolateVec3(
        struct Vector3* start,
        struct Vector3* dir,
        double t,
        struct Vector3* out
) {
    out->x = start->x + dir->x * t;
    out->y = start->y + dir->y * t;
    out->z = start->z + dir->z * t;
}

bool isInside(
        struct Vector3* plane_normal,
        struct Vector3* plane_pos,
        struct Vector3 vert
) {
    struct Vector3 vert2 = vert;

    SubVec3(
            &vert2,
            plane_pos,
            &vert2
    );

    return DotVec3(
            plane_normal,
            &vert2
    ) >= 0;
}

void copyColours(
        struct Triangle* v1,
        struct Triangle* v2
) {
    v2->diffuse_color_ = v1->diffuse_color_;
    v2->specular_color_ = v1->specular_color_;
}

void copyNormals(
        struct Triangle* v1,
        struct Triangle* v2
) {
    v2->n1 = v1->n1;
    v2->n2 = v1->n2;
    v2->n3 = v1->n3;
}

#endif