#ifndef MATHCL_H
#define MATHCL_H

#include "types.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void Vec3Subtract(
    const struct Vector3* v1,
    const struct Vector3* v2,
    struct Vector3* out
) {
    out->x = v1->x - v2->x;
    out->y = v1->y - v2->y;
    out->z = v1->z - v2->z;
}

void Vec3Add(
    const struct Vector3* v1,
    const struct Vector3* v2,
    struct Vector3* out
) {
    out->x = v1->x + v2->x;
    out->y = v1->y + v2->y;
    out->z = v1->z + v2->z;
}

void Vec3Multiply(
    const struct Vector3* v1,
    const double v2,
    struct Vector3* out
) {
    out->x = v1->x * v2;
    out->y = v1->y * v2;
    out->z = v1->z * v2;
}

void Vec3MultiplyVec3(
    const struct Vector3* v1,
    const struct Vector3* v2,
    struct Vector3* out
) {
    out->x = v1->x * v2->x;
    out->y = v1->y * v2->y;
    out->z = v1->z * v2->z;
}

void Vec3GammaCorrect(
    struct Vector3* v,
    struct Vector3* out
) {
    out->x = pow(v->x, 1.0 / 2.2);
    out->y = pow(v->y, 1.0 / 2.2);
    out->z = pow(v->z, 1.0 / 2.2);
}

void Vec3Divide(
    const struct Vector3* v1,
    const double v2,
    struct Vector3* out
) {
    out->x = v1->x / v2;
    out->y = v1->y / v2;
    out->z = v1->z / v2;
}

double Vec3Dot(
    const struct Vector3* v1,
    const struct Vector3* v2
) {
    return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

void Vec3RotateX(
    const struct Vector3* v1,
    const double angle,
    struct Vector3* out
) {
    double c = cos(angle);
    double s = sin(angle);
    out->x = v1->x;
    out->y = v1->y * c - v1->z * s;
    out->z = v1->y * s + v1->z * c;
}

void Vec3RotateY(
    const struct Vector3* v1,
    const double angle,
    struct Vector3* out
) {
    double c = cos(angle);
    double s = sin(angle);
    out->x = v1->x * c + v1->z * s;
    out->y = v1->y;
    out->z = -v1->x * s + v1->z * c;
}

void Vec3RotateZ(
    const struct Vector3* v1,
    const double angle,
    struct Vector3* out
) {
    double c = cos(angle);
    double s = sin(angle);
    out->x = v1->x * c - v1->y * s;
    out->y = v1->x * s + v1->y * c;
    out->z = v1->z;
}

void Vec3Rotate(
    struct Vector3* a,
    struct Vector3* rotation
) {
    double x = a->x;
    double y = a->y;
    double z = a->z;

    double sinX = sin(rotation->x * (M_PI / 180.0));
    double cosX = cos(rotation->x * (M_PI / 180.0));
    double sinY = sin(rotation->y * (M_PI / 180.0));
    double cosY = cos(rotation->y * (M_PI / 180.0));
    double sinZ = sin(rotation->z * (M_PI / 180.0));
    double cosZ = cos(rotation->z * (M_PI / 180.0));

    double x1 = x * cosY + z * sinY;
    double y1 = x * sinX * sinY + y * cosX - z * sinX * cosY;
    double z1 = -x * cosX * sinY + y * sinX + z * cosX * cosY;

    double x2 = x1 * cosZ - y1 * sinZ;
    double y2 = x1 * sinZ + y1 * cosZ;
    double z2 = z1;

    a->x = x2;
    a->y = y2;
    a->z = z2;
}

void Vec2Subtract(
    const struct Vector2* v1,
    const struct Vector2* v2,
    struct Vector2* out
) {
    out->x = v1->x - v2->x;
    out->y = v1->y - v2->y;
    out->texture_info_ = v1->texture_info_ - v2->texture_info_;
}

void Vec2Add(
    const struct Vector2* v1,
    const struct Vector2* v2,
    struct Vector2* out
) {
    out->x = v1->x + v2->x;
    out->y = v1->y + v2->y;
    out->texture_info_ = v1->texture_info_ + v2->texture_info_;
}

void Vec2Multiply(
    const struct Vector2* v1,
    const double v2,
    struct Vector2* out
) {
    out->x = v1->x * v2;
    out->y = v1->y * v2;
    out->texture_info_ = v1->texture_info_ * v2;
}

void Vec2Divide(
    const struct Vector2* v1,
    const double v2,
    struct Vector2* out
) {
    out->x = v1->x / v2;
    out->y = v1->y / v2;
}

double Vec2Dot(
    const struct Vector2* v1,
    const struct Vector2* v2
) {
    return v1->x * v2->x + v1->y * v2->y;
}

void Vec2Rotate(
    const struct Vector2* v1,
    const double angle,
    struct Vector2* out
) {
    double c = cos(angle);
    double s = sin(angle);
    out->x = v1->x * c - v1->y * s;
    out->y = v1->x * s + v1->y * c;
}

void Vec3Cross(
    const struct Vector3* v1,
    const struct Vector3* v2,
    struct Vector3* out
) {
    out->x = v1->y * v2->z - v1->z * v2->y;
    out->y = v1->z * v2->x - v1->x * v2->z;
    out->z = v1->x * v2->y - v1->y * v2->x;
}

double Vec3Length(
    const struct Vector3* v
) {
    return sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
}

void normalize(
    struct Vector3* v,
    struct Vector3* out
) {
    double length = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
    out->x = v->x / length;
    out->y = v->y / length;
    out->z = v->z / length;
}

void copyUvs(
    const struct Triangle* v1,
    struct ScreenTriangle* v2
) {
    v2->t1 = v1->t1;
    v2->t2 = v1->t2;
    v2->t3 = v1->t3;
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
    struct IntersectInfo* out
) {
    double n = Vec3Dot(&plane->normal_, &plane->position_) - Vec3Dot(&plane->normal_, &ray->origin_);
    double d = Vec3Dot(&plane->normal_, &ray->direction_);

    if (d == 0) {
        out->intersect_.x = 0;
        out->intersect_.y = 0;
        out->intersect_.z = 0;
        out->valid_ = false;
        return;
    }

    double t = n / d;
    struct Vector3 position = ray->direction_;

    Vec3Multiply(
        &position,
        t,
        &position
    );

    Vec3Add(
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
        out->intersect_.x = 0;
        out->intersect_.y = 0;
        out->intersect_.z = 0;
        out->valid_ = false;
        return;
    }

    out->intersect_ = position;
    out->valid_ = true;

    struct Vector3 a = position;
    Vec3Subtract(&a, &ray->origin_, &a);
    out->t_ = Vec3Length(&a) / Vec3Length(&ray->direction_);
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

    Vec3Subtract(
        &vert2,
        plane_pos,
        &vert2
    );

    return Vec3Dot(
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