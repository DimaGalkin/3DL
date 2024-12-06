#ifndef M_PI
#define M_PI (float)3.14159265358979323846
#endif

void Vec3Subtract(
    const Vector3* v1,
    const Vector3* v2,
    Vector3* out
) {
    out->x = v1->x - v2->x;
    out->y = v1->y - v2->y;
    out->z = v1->z - v2->z;
}

void Vec3Add(
    const Vector3* v1,
    const Vector3* v2,
    Vector3* out
) {
    out->x = v1->x + v2->x;
    out->y = v1->y + v2->y;
    out->z = v1->z + v2->z;
}

void Vec3Multiply(
    const Vector3* v1,
    const float v2,
    Vector3* out
) {
    out->x = v1->x * v2;
    out->y = v1->y * v2;
    out->z = v1->z * v2;
}

void Vec3MultiplyVec3(
    const Vector3* v1,
    const Vector3* v2,
    Vector3* out
) {
    out->x = v1->x * v2->x;
    out->y = v1->y * v2->y;
    out->z = v1->z * v2->z;
}

void powf(
    float* a,
    float b
) {
    *a = pow(*a, b);
}

void Vec3GammaCorrect(
    Vector3* v,
    Vector3* out
) {
    out->x = v->x;
    out->y = v->y;
    out->z = v->z;
}

void Vec3Divide(
    const Vector3* v1,
    const float v2,
    Vector3* out
) {
    out->x = v1->x / v2;
    out->y = v1->y / v2;
    out->z = v1->z / v2;
}

float Vec3Dot(
    const Vector3* v1,
    const Vector3* v2
) {
    return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

void Vec3RotateX(
    const Vector3* v1,
    const float angle,
    Vector3* out
) {
    float c = cos(angle);
    float s = sin(angle);
    out->x = v1->x;
    out->y = v1->y * c - v1->z * s;
    out->z = v1->y * s + v1->z * c;
}

void Vec3RotateY(
    const Vector3* v1,
    const float angle,
    Vector3* out
) {
    float c = cos(angle);
    float s = sin(angle);
    out->x = v1->x * c + v1->z * s;
    out->y = v1->y;
    out->z = -v1->x * s + v1->z * c;
}

void Vec3RotateZ(
    const Vector3* v1,
    const float angle,
    Vector3* out
) {
    float c = cos(angle);
    float s = sin(angle);
    out->x = v1->x * c - v1->y * s;
    out->y = v1->x * s + v1->y * c;
    out->z = v1->z;
}

void Vec3Rotate(
    Vector3* a,
    Vector3* rotation
) {
    float x = a->x;
    float y = a->y;
    float z = a->z;

    float sinX = sin(rotation->x * (M_PI / 180.0));
    float cosX = cos(rotation->x * (M_PI / 180.0));
    float sinY = sin(rotation->y * (M_PI / 180.0));
    float cosY = cos(rotation->y * (M_PI / 180.0));
    float sinZ = sin(rotation->z * (M_PI / 180.0));
    float cosZ = cos(rotation->z * (M_PI / 180.0));

    float x1 = x * cosY + z * sinY;
    float y1 = x * sinX * sinY + y * cosX - z * sinX * cosY;
    float z1 = -x * cosX * sinY + y * sinX + z * cosX * cosY;

    float x2 = x1 * cosZ - y1 * sinZ;
    float y2 = x1 * sinZ + y1 * cosZ;
    float z2 = z1;

    a->x = x2;
    a->y = y2;
    a->z = z2;
}

void Vec2Subtract(
    const Vector2* v1,
    const Vector2* v2,
    Vector2* out
) {
    out->x = v1->x - v2->x;
    out->y = v1->y - v2->y;
    out->depth_info_ = v1->depth_info_ - v2->depth_info_;
}

void Vec2Add(
    const Vector2* v1,
    const Vector2* v2,
    Vector2* out
) {
    out->x = v1->x + v2->x;
    out->y = v1->y + v2->y;
    out->depth_info_ = v1->depth_info_ + v2->depth_info_;
}

void Vec2Multiply(
    const Vector2* v1,
    const float v2,
    Vector2* out
) {
    out->x = v1->x * v2;
    out->y = v1->y * v2;
    out->depth_info_ = v1->depth_info_ * v2;
}

void Vec2Divide(
    const Vector2* v1,
    const float v2,
    Vector2* out
) {
    out->x = v1->x / v2;
    out->y = v1->y / v2;
}

float Vec2Dot(
    const Vector2* v1,
    const Vector2* v2
) {
    return v1->x * v2->x + v1->y * v2->y;
}

void Vec2Rotate(
    const Vector2* v1,
    const float angle,
    Vector2* out
) {
    float c = cos(angle);
    float s = sin(angle);
    out->x = v1->x * c - v1->y * s;
    out->y = v1->x * s + v1->y * c;
}

void Vec3Cross(
    const Vector3* v1,
    const Vector3* v2,
    Vector3* out
) {
    out->x = v1->y * v2->z - v1->z * v2->y;
    out->y = v1->z * v2->x - v1->x * v2->z;
    out->z = v1->x * v2->y - v1->y * v2->x;
}

float Vec3Length(
    const Vector3* v
) {
    return sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
}

void normalize(
    Vector3* v,
    Vector3* out
) {
    float length = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
    out->x = v->x / length;
    out->y = v->y / length;
    out->z = v->z / length;
}

void copyUvs(
    const Triangle* v1,
    ScreenTriangle* v2
) {
    v2->t1 = v1->t1;
    v2->t2 = v1->t2;
    v2->t3 = v1->t3;
}

void calcUv(
    const Vector2* uv1,
    const Vector2* uv2,
    float t,
    Vector2* out
) {
    out->x = uv1->x + (uv2->x - uv1->x) * t;
    out->y = uv1->y + (uv2->y - uv1->y) * t;
}

float absd(
    float a
) {
    return a < 0 ? -a : a;
}

void intersects(
    Plane* plane,
    Ray* ray,
    IntersectInfo* out
) {
    float n = Vec3Dot(&plane->normal_, &plane->position_) - Vec3Dot(&plane->normal_, &ray->origin_);
    float d = Vec3Dot(&plane->normal_, &ray->direction_);

    if (absd(d) < 0.0001) {
        out->intersect_.x = 0;
        out->intersect_.y = 0;
        out->intersect_.z = 0;
        out->valid_ = false;
        return;
    }

    float t = n / d;
    Vector3 position = ray->direction_;

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

    Vector3 a = position;
    Vec3Subtract(&a, &ray->origin_, &a);
    out->t_ = Vec3Length(&a) / Vec3Length(&ray->direction_);
}

void interpolate(
    Vector2* start,
    Vector2* dir,
    float t,
    Vector2* out
) {
    out->x = start->x + dir->x * t;
    out->y = start->y + dir->y * t;
    out->depth_info_ = start->depth_info_ + dir->depth_info_ * t;
}

void interpolateAB(
    Vector2 a,
    Vector2 b,
    float t,
    Vector2* out
) {
    Vector2 dir = b;
    Vec2Subtract(&b, &a, &dir);
    interpolate(&a, &dir, t, out);
}

void interpolateVec3(
    Vector3* start,
    Vector3* dir,
    float t,
    Vector3* out
) {
    out->x = start->x + dir->x * t;
    out->y = start->y + dir->y * t;
    out->z = start->z + dir->z * t;
}

void interpolateVec3AB(
    Vector3 a,
    Vector3 b,
    float t,
    Vector3* out
) {
    Vector3 dir = b;
    Vec3Subtract(&b, &a, &dir);
    interpolateVec3(&a, &dir, t, out);
}

bool isInside(
    Vector3* plane_normal,
    Vector3* plane_pos,
    Vector3 vert
) {
    Vector3 vert2 = vert;

    Vec3Subtract(
        &vert2,
        plane_pos,
        &vert2
    );

    return Vec3Dot(
        plane_normal,
        &vert2
    ) > 0;
}

void copyColours(
    Triangle* v1,
    Triangle* v2
) {
    v2->diffuse_color_ = v1->diffuse_color_;
    v2->specular_color_ = v1->specular_color_;
}

void copyNormals(
    Triangle* v1,
    Triangle* v2
) {
    v2->n1 = v1->n1;
    v2->n2 = v1->n2;
    v2->n3 = v1->n3;
}

float maxf(
    float a,
    float b
) {
    return a > b ? a : b;
}