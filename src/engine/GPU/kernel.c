typedef unsigned int uint32_t;

/*
    This header only runs when open in IDE, __IACT_H__ is defined
    when compiling for GPU by iact.py.

    The CLion IDE does not recognise OpenCL code so a few keywords have to be defined
    to remove errors. eg 'global' is not recognised as a valid keyword.
*/
#ifndef __IACT_H__
#include "sys/types.h"
#include "types.h"
#include "mathcl.h"

#define global
#define tan
#define atan
#define acos
#define cos
#define sin
#define kernel
#define INFINITY 999
#define pow
#define min
#define bool short

#define true 1
#define false 0

#define get_global_id
#define get_global_size
#define get_global_offset

typedef struct float3 {float x, y, z;} float3;
typedef struct float2 {float x, y;} float2;

int floor () { return 0; }

#endif

int absi(const int in) {
    if (in < 0) {
        return in * -1;
    }

    return in;
}

float absf(const float in) {
    if (in < 0) {
        return in * -1;
    }

    return in;
}

void draw_line (
    int ax, int ay,
    const int bx, const int by,
    const State* info,
    global uint* C
) {
    const int dx = absi(bx - ax);
    const int dy = absi(by - ay);
    const int sx = (ax < bx) ? 1 : -1;
    const int sy = (ay < by) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        if (ax >= 0 && ax < info->width_ && ay >= 0 && ay < info->height_) {
            C[ax + ay * info->width_] = 0xffffff;
        }

        if (ax == bx && ay == by) break;

        const int e2 = 2 * err;

        if (e2 > -dy) {
            err -= dy;
            ax += sx;
        }

        if (e2 < dx) {
            err += dx;
            ay += sy;
        }
    }
}

void drawWireframeTriangle(
    const ScreenTriangle* tri,
    const State* info,
    global uint* C
) {
    draw_line(
        (int)tri->v1.x, (int)tri->v1.y,
        (int)tri->v2.x, (int)tri->v2.y,
        info,
        C
    );

    draw_line(
        (int)tri->v2.x, (int)tri->v2.y,
        (int)tri->v3.x, (int)tri->v3.y,
        info,
        C
    );

    draw_line(
        (int)tri->v3.x, (int)tri->v3.y,
        (int)tri->v1.x, (int)tri->v1.y,
        info,
        C
    );
}

void project (
    const Triangle* in,
    ScreenTriangle* out,
    global const State* info
) {
    const float fov = info->fov_;
    const float dtp = (info->width_ / 2.0f) / tan((fov / 2.0) * (M_PI / 180.0f)); // NOLINT (narrowing conversion)
    const float h_height = info->height_ / 2.0; // NOLINT (narrowing conversion)
    const float h_width = info->width_ / 2.0f; // NOLINT (narrowing conversion)

    const float t1 = (dtp - 0) / in->v1.z;
    const float t2 = (dtp - 0) / in->v2.z;
    const float t3 = (dtp - 0) / in->v3.z;

    out->v1.x = in->v1.x * t1 + h_width;
    out->v1.y = in->v1.y * t1 + h_height;
    out->v1.depth_info_ = in->v1.z;

    out->v2.x = in->v2.x * t2 + h_width;
    out->v2.y = in->v2.y * t2 + h_height;
    out->v2.depth_info_ = in->v2.z;

    out->v3.x = in->v3.x * t3 + h_width;
    out->v3.y = in->v3.y * t3 + h_height;
    out->v3.depth_info_ = in->v3.z;

    out->t1 = in->t1;
    out->t1.depth_info_ = in->v1.z;

    out->t2 = in->t2;
    out->t2.depth_info_ = in->v2.z;

    out->t3 = in->t3;
    out->t3.depth_info_ = in->v3.z;
}

void projectshadow (
    const Triangle* in,
    ScreenTriangle* out,
    const GPULight* info
) {
    const float fov = info->fov_;
    const float dtp = (info->shadow_map_width_ / 2.0) / tan((fov / 2.0) * (M_PI / 180.0)); // NOLINT (narrowing conversion)
    const float h_height = info->shadow_map_height_ / 2.0; // NOLINT (narrowing conversion)
    const float h_width = info->shadow_map_width_ / 2.0; // NOLINT (narrowing conversion)

    const float t1 = (dtp - 0) / in->v1.z;
    const float t2 = (dtp - 0) / in->v2.z;
    const float t3 = (dtp - 0) / in->v3.z;

    out->v1.x = in->v1.x * t1 + h_width;
    out->v1.y = in->v1.y * t1 + h_height;
    out->v1.depth_info_ = in->v1.z;

    out->v2.x = in->v2.x * t2 + h_width;
    out->v2.y = in->v2.y * t2 + h_height;
    out->v2.depth_info_ = in->v2.z;

    out->v3.x = in->v3.x * t3 + h_width;
    out->v3.y = in->v3.y * t3 + h_height;
    out->v3.depth_info_ = in->v3.z;
}

void projectvertex (
    const Vector3* in,
    Vector2* out,
    GPULight* info
) {
    const float fov = info->fov_;
    const float dtp = (info->shadow_map_width_ / 2.0) / tan((fov / 2.0) * (M_PI / 180.0)); // NOLINT (narrowing conversion)
    const float h_height = info->shadow_map_height_ / 2.0; // NOLINT (narrowing conversion)
    const float h_width = info->shadow_map_width_ / 2.0; // NOLINT (narrowing conversion)

    float t1 = (dtp - 0) / in->z;

    out->x = in->x * t1 + h_width;
    out->y = in->y * t1 + h_height;
    out->depth_info_ = in->z;
}

unsigned char clip(
    const Triangle* tri,
    Plane* plane,
    Triangle* out_tris
) {
    Vector3 dir1 = tri->v2;
    Vector3 dir2 = tri->v3;
    Vector3 dir3 = tri->v1;

    Vec3Subtract(&dir1, &tri->v1, &dir1);
    Vec3Subtract(&dir2, &tri->v2, &dir2);
    Vec3Subtract(&dir3, &tri->v3, &dir3);

    Ray ray1 = {tri->v1, dir1};
    Ray ray2 = {tri->v2, dir2};
    Ray ray3 = {tri->v3, dir3};

    IntersectInfo info1;
    IntersectInfo info2;
    IntersectInfo info3;

    intersects(plane, &ray1, &info1);
    intersects(plane, &ray2, &info2);
    intersects(plane, &ray3, &info3);

    if (!info1.valid_ && !info2.valid_ && !info3.valid_) {
        const bool inside = isInside(&plane->normal_, &plane->position_, tri->v1);

        if (inside) {
            out_tris[0] = *tri;
            return 1;
        }

        return 0;
    }

    if (info1.valid_ && info2.valid_ && info3.valid_) {
        return 0;
    }

    if (
        !info1.valid_ && !info2.valid_ && info3.valid_,
        !info1.valid_ && info2.valid_ && !info3.valid_,
        info1.valid_ && !info2.valid_ && !info3.valid_
    ) {
        return 0;
    }

    if (info1.valid_ && info2.valid_ && info3.valid_) {
        return 0;
    }

    if (info1.valid_ && info2.valid_) {
        const bool inside = isInside(&plane->normal_, &plane->position_, tri->v2);

        if (inside) {
            out_tris[0].v1 = tri->v2;
            out_tris[0].v2 = info1.intersect_;
            out_tris[0].v3 = info2.intersect_;

            out_tris[0].n1 = tri->n2;
            interpolateVec3AB(tri->n1, tri->n2, info1.t_, &out_tris[0].n2);
            interpolateVec3AB(tri->n2, tri->n3, info2.t_, &out_tris[0].n3);

            out_tris[0].t1 = tri->t2;
            interpolateAB(tri->t1, tri->t2, info1.t_, &out_tris[0].t2);
            interpolateAB(tri->t2, tri->t3, info2.t_, &out_tris[0].t3);

            out_tris[0].diffuse_color_ = tri->diffuse_color_;
            out_tris[0].specular_color_ = tri->specular_color_;

            return 1;
        }

        out_tris[0].v1 = tri->v1;
        out_tris[0].v2 = info1.intersect_;
        out_tris[0].v3 = tri->v3;

        out_tris[0].n1 = tri->n1;
        interpolateVec3AB(tri->n1, tri->n2, info1.t_, &out_tris[0].n2);
        out_tris[0].n3 = tri->n3;

        out_tris[0].t1 = tri->t1;
        interpolateAB(tri->t1, tri->t2, info1.t_, &out_tris[0].t2);
        out_tris[0].t3 = tri->t3;

        out_tris[0].diffuse_color_ = tri->diffuse_color_;
        out_tris[0].specular_color_ = tri->specular_color_;

        out_tris[1].v1 = tri->v3;
        out_tris[1].v2 = info2.intersect_;
        out_tris[1].v3 = info1.intersect_;

        out_tris[1].n1 = tri->n3;
        interpolateVec3AB(tri->n2, tri->n3, info2.t_, &out_tris[1].n2);
        interpolateVec3AB(tri->n1, tri->n2, info1.t_, &out_tris[1].n3);

        out_tris[1].t1 = tri->t3;
        interpolateAB(tri->t2, tri->t3, info2.t_, &out_tris[1].t2);
        interpolateAB(tri->t1, tri->t2, info1.t_, &out_tris[1].t3);

        out_tris[1].diffuse_color_ = tri->diffuse_color_;
        out_tris[1].specular_color_ = tri->specular_color_;

        return 2;
    }

    if (info1.valid_ && info3.valid_) {
        const bool inside = isInside(&plane->normal_, &plane->position_, tri->v1);

        if (inside) {
            out_tris[0].v1 = tri->v1;
            out_tris[0].v2 = info1.intersect_;
            out_tris[0].v3 = info3.intersect_;

            out_tris[0].n1 = tri->n1;
            interpolateVec3AB(tri->n1, tri->n2, info1.t_, &out_tris[0].n2);
            interpolateVec3AB(tri->n1, tri->n3, info3.t_, &out_tris[0].n3);

            out_tris[0].t1 = tri->t1;
            interpolateAB(tri->t1, tri->t2, info1.t_, &out_tris[0].t2);
            interpolateAB(tri->t1, tri->t3, info3.t_, &out_tris[0].t3);

            out_tris[0].diffuse_color_ = tri->diffuse_color_;
            out_tris[0].specular_color_ = tri->specular_color_;

            return 1;
        }

        out_tris[0].v1 = tri->v2;
        out_tris[0].v2 = info1.intersect_;
        out_tris[0].v3 = info3.intersect_;

        out_tris[0].n1 = tri->n2;
        interpolateVec3AB(tri->n1, tri->n2, info1.t_, &out_tris[0].n2);
        interpolateVec3AB(tri->n1, tri->n3, info3.t_, &out_tris[0].n3);

        out_tris[0].t1 = tri->t2;
        interpolateAB(tri->t1, tri->t2, info1.t_, &out_tris[0].t2);
        interpolateAB(tri->t1, tri->t3, info3.t_, &out_tris[0].t3);

        out_tris[0].diffuse_color_ = tri->diffuse_color_;
        out_tris[0].specular_color_ = tri->specular_color_;

        out_tris[1].v1 = tri->v2;
        out_tris[1].v2 = tri->v3;
        out_tris[1].v3 = info3.intersect_;

        out_tris[1].n1 = tri->n2;
        out_tris[1].n2 = tri->n3;
        interpolateVec3AB(tri->n1, tri->n3, info3.t_, &out_tris[1].n3);

        out_tris[1].t1 = tri->t2;
        out_tris[1].t2 = tri->t3;
        interpolateAB(tri->t1, tri->t3, info3.t_, &out_tris[1].t3);

        out_tris[1].diffuse_color_ = tri->diffuse_color_;
        out_tris[1].specular_color_ = tri->specular_color_;

        return 2;
    }

    if (info2.valid_ && info3.valid_) {
        const bool inside = isInside(&plane->normal_, &plane->position_, tri->v3);

        if (inside) {
            out_tris[0].v1 = tri->v3;
            out_tris[0].v2 = info3.intersect_;
            out_tris[0].v3 = info2.intersect_;

            out_tris[0].n1 = tri->n3;
            interpolateVec3AB(tri->n1, tri->n3, info3.t_, &out_tris[0].n2);
            interpolateVec3AB(tri->n2, tri->n3, info2.t_, &out_tris[0].n3);

            out_tris[0].t1 = tri->t3;
            interpolateAB(tri->t1, tri->t3, info3.t_, &out_tris[0].t2);
            interpolateAB(tri->t2, tri->t3, info2.t_, &out_tris[0].t3);

            out_tris[0].diffuse_color_ = tri->diffuse_color_;
            out_tris[0].specular_color_ = tri->specular_color_;

            return 1;
        }

        out_tris[0].v1 = tri->v1;
        out_tris[0].v2 = info3.intersect_;
        out_tris[0].v3 = tri->v2;

        out_tris[0].n1 = tri->n1;
        interpolateVec3AB(tri->n1, tri->n3, info3.t_, &out_tris[0].n2);
        out_tris[0].n3 = tri->n2;

        out_tris[0].t1 = tri->t1;
        interpolateAB(tri->t1, tri->t3, info3.t_, &out_tris[0].t2);
        out_tris[0].t3 = tri->t2;

        out_tris[0].diffuse_color_ = tri->diffuse_color_;
        out_tris[0].specular_color_ = tri->specular_color_;

        out_tris[1].v1 = tri->v2;
        out_tris[1].v2 = info3.intersect_;
        out_tris[1].v3 = info2.intersect_;

        out_tris[1].n1 = tri->n2;
        interpolateVec3AB(tri->n1, tri->n3, info3.t_, &out_tris[1].n2);
        interpolateVec3AB(tri->n2, tri->n3, info2.t_, &out_tris[1].n3);

        out_tris[1].t1 = tri->t2;
        interpolateAB(tri->t1, tri->t3, info3.t_, &out_tris[1].t2);
        interpolateAB(tri->t2, tri->t3, info2.t_, &out_tris[1].t3);

        out_tris[1].diffuse_color_ = tri->diffuse_color_;
        out_tris[1].specular_color_ = tri->specular_color_;

        return 2;
    }

    return 0;
}

float interpolateZ (
    const Vector2* p1,
    const Vector2* p2,
    const Vector2* p
) {
    if (p1->depth_info_ == p2->depth_info_) return p1->depth_info_;

    float denom = p2->x - p1->x;
    float t;

    if (absf(denom) < 0.001) {
        denom = p2->y - p1->y;

        if (absf(denom) < 0.001) {
            return p1->depth_info_;
        }

        t = (p->y - p1->y) / denom;
    } else {
        t = (p->x - p1->x) / denom;
    }

    return p1->depth_info_ + (p2->depth_info_ - p1->depth_info_) * t;
}

float intersectVec2(
    Vector2* a_pos,
    Vector2* a_dir,
    Vector2* b_pos,
    Vector2* b_dir,
    Vector2* out
) {
    const float t = (b_dir->x * (a_pos->y - b_pos->y) - b_dir->y * (a_pos->x - b_pos->x)) / (a_dir->x * b_dir->y - a_dir->y * b_dir->x);

    if (absf(t) < 0.0001) {
        out->x = a_pos->x;
        out->y = a_pos->y;
        return t;
    }

    Vector2 res = *a_dir;

    Vec2Multiply(&res, t, &res);
    Vec2Add(&res, a_pos, &res);

    out->x = res.x;
    out->y = res.y;

    return t;
}

void sortTri(
    ScreenTriangle* tri,
    Triangle* gsp_tri
) {
    if (tri->v1.y < tri->v2.y) {
        Vector2 temp = tri->v1;
        tri->v1 = tri->v2;
        tri->v2 = temp;

        temp = tri->t1;
        tri->t1 = tri->t2;
        tri->t2 = temp;

        Vector3 temp2 = gsp_tri->v1;
        gsp_tri->v1 = gsp_tri->v2;
        gsp_tri->v2 = temp2;

        temp2 = gsp_tri->n1;
        gsp_tri->n1 = gsp_tri->n2;
        gsp_tri->n2 = temp2;
    }

    if (tri->v2.y < tri->v3.y) {
        Vector2 temp = tri->v2;
        tri->v2 = tri->v3;
        tri->v3 = temp;

        temp = tri->t2;
        tri->t2 = tri->t3;
        tri->t3 = temp;

        Vector3 temp2 = gsp_tri->v2;
        gsp_tri->v2 = gsp_tri->v3;
        gsp_tri->v3 = temp2;

        temp2 = gsp_tri->n2;
        gsp_tri->n2 = gsp_tri->n3;
        gsp_tri->n3 = temp2;
    }

    if (tri->v1.y < tri->v2.y) {
        Vector2 temp = tri->v1;
        tri->v1 = tri->v2;
        tri->v2 = temp;

        temp = tri->t1;
        tri->t1 = tri->t2;
        tri->t2 = temp;

        Vector3 temp2 = gsp_tri->v1;
        gsp_tri->v1 = gsp_tri->v2;
        gsp_tri->v2 = temp2;

        temp2 = gsp_tri->n1;
        gsp_tri->n1 = gsp_tri->n2;
        gsp_tri->n2 = temp2;
    }
}

float3 asFloat3(const Vector2 in) {
    return (float3) {
        in.x,
        in.y,
        in.depth_info_
    };
}

float2 asFloat2(const Vector2 in) {
    return (float2) { in.x, in.y };
}

bool backfacecull(const Triangle* tr) {
    Vector3 normal;
    Vec3Cross(&tr->v2, &tr->v1, &normal);
    normalize(&normal, &normal);

    Vector3 view;
    Vec3Subtract(&tr->v1, &tr->v3, &view);
    normalize(&view, &view);

    return Vec3Dot(&normal, &view) < 0.0f;
}

void rasterise_shadow (
    ScreenTriangle* tri,
    const GPULight* light,
    global float* shadow_map
) {
    if (tri->v1.y < tri->v2.y) {
        const Vector2 temp = tri->v1;
        tri->v1 = tri->v2;
        tri->v2 = temp;
    }

    if (tri->v2.y < tri->v3.y) {
        const Vector2 temp = tri->v2;
        tri->v2 = tri->v3;
        tri->v3 = temp;
    }

    if (tri->v1.y < tri->v2.y) {
        const Vector2 temp = tri->v1;
        tri->v1 = tri->v2;
        tri->v2 = temp;
    }

    const Line line1 = {tri->v1, tri->v3};
    Line line2 = {tri->v2, tri->v1};

    for (int i = (int)tri->v1.y; i > (int)tri->v3.y; --i) {
        if (floor(tri->v2.y) == i) {
            line2 = (Line) { tri->v3, tri->v2 };
        }

        Vector2 intersect1;
        Vector2 intersect2;

        intersect1.depth_info_ = interpolateZ(&line1.a, &line1.b, &intersect1);
        intersect2.depth_info_ = interpolateZ(&line2.a, &line2.b, &intersect2);

        if (intersect1.x > intersect2.x) {
            const Vector2 temp = intersect1;
            intersect1 = intersect2;
            intersect2 = temp;
        }

        const float y = intersect1.y;

        for (int j = (int)intersect1.x; j < (int)intersect2.x; ++j) {

            Vector2 intersect3;
            intersect3.depth_info_ = interpolateZ(&intersect1, &intersect2, &intersect3);

            if (
                (int)y < light->shadow_map_height_ && (int)y >= 0 &&
                (int)j < light->shadow_map_width_  && (int)j >= 0 &&
                intersect3.depth_info_ > shadow_map[(int)floor(j + floor(y) * light->shadow_map_width_)])
            {
                shadow_map[(int)floor(j + floor(y) * light->shadow_map_width_)] = intersect3.depth_info_;
            }
        }
    }
}

void toLightSpace (
    const Vector3* in,
    Vector3* out,
    const GPULight* light
) {
    Vector3 light_dir = light->direction_;
    const Vector3 light_pos = light->position_;

    Vec3Subtract(in, &light_pos, out);
    Vec3Rotate(out, &light_dir);
}

void fromCameraSpace (
    const Vector3* in,
    Vector3* out,
    const State* info
) {
    const Vector3 camera_pos = info->camera_position_;
    Vector3 camera_rot = info->camera_rotation_;

    camera_rot.x *= -1;
    camera_rot.y *= -1;
    camera_rot.z *= -1;

    Vec3Add(in, &camera_pos, out);
    Vec3Rotate(out, &camera_rot);
}

void GetAnglesBetweenVectors (
    const Vector3* a,
    const Vector3* b,
    Vector3* angles
) {
    const Vector2 aYZ = {a->y, a->z};
    const Vector2 bYZ = {b->y, b->z};

    const Vector2 aXZ = {a->x, a->z};
    const Vector2 bXZ = {b->x, b->z};

    const Vector2 aXY = {a->x, a->y};
    const Vector2 bXY = {b->x, b->y};

    angles->x = acos(Vec2Dot(&aYZ, &bYZ));
    angles->y = acos(Vec2Dot(&aXZ, &bXZ));
    angles->z = acos(Vec2Dot(&aXY, &bXY));
}

void kernel gpu_shadow (
    global Triangle* tris,
    global float* shadow_map,
    global const GPULight* light_ptr
) {
    const int zero = get_global_id(0);
    const int one = get_global_id(1);
    const int two = get_global_id(2);

    const int tri_num = get_global_offset(0) + zero + get_global_size(0) * one + get_global_size(0) * get_global_size(1) * two;

    Triangle tri_cpy = tris[tri_num];

    Vector3 light_rotation = {90, 0, 0};

    GPULight light = *light_ptr;

    Vec3Subtract(&tri_cpy.v1, &light.position_, &tri_cpy.v1);
    Vec3Subtract(&tri_cpy.v2, &light.position_, &tri_cpy.v2);
    Vec3Subtract(&tri_cpy.v3, &light.position_, &tri_cpy.v3);

    Vec3Rotate(&tri_cpy.v1, &light_rotation);
    Vec3Rotate(&tri_cpy.v2, &light_rotation);
    Vec3Rotate(&tri_cpy.v3, &light_rotation);

    Vec3Rotate(&tri_cpy.n1, &light_rotation);
    Vec3Rotate(&tri_cpy.n2, &light_rotation);
    Vec3Rotate(&tri_cpy.n3, &light_rotation);

    if (backfacecull(&tri_cpy)) return;

    Triangle tri_near_clipped[2];
    Triangle tri_left_clipped[2];
    Triangle tri_right_clipped[2];
    Triangle tri_bottom_clipped[2];
    Triangle tri_top_clipped[2];

    Plane near = {
        {0, 0, -1},
        {0, 0, -1},
        {1, 0, 0}

    };

    Plane left = {
        {-1, 0, 0},
        {0, 0, 0},
        {0, 0, -1}
    };

    Plane right = {
        {1, 0, 0},
        {0, 0, 0},
        {0, 0, -1}
    };

    Plane bottom = {
        {0, 1, 0},
        {0, 0, 0},
        {0, 0, -1}
    };

    Plane top = {
        {0, -1, 0},
        {0, 0, 0},
        {0, 0, -1}
    };

    // Vec3Rotateate left and right planes as they are at the angle of fov/2
    Vector3 r = {0, -light_ptr->fov_/2, 0};
    Vec3Rotate(&left.direction_, &r);
    Vec3Rotate(&left.normal_, &r);
    r.y = r.y * -1;
    Vec3Rotate(&right.direction_, &r);
    Vec3Rotate(&right.normal_, &r);

    float half_width = light_ptr->shadow_map_width_ / 2.0f;
    float half_height = light_ptr->shadow_map_height_ / 2.0f;
    float half_fov = light_ptr->fov_ / 2;

    float horiz_fov = atan((half_height * tan(half_fov * (M_PI / 180.0)))/(half_width)) * (180.0 / M_PI);

    Vector3 r2 = {-horiz_fov, 0, 0};
    Vec3Rotate(&bottom.direction_, &r2);
    Vec3Rotate(&bottom.normal_, &r2);
    r2.x = r2.x * -1;
    Vec3Rotate(&top.direction_, &r2);
    Vec3Rotate(&top.normal_, &r2);

    unsigned char near_clipped = clip(&tri_cpy, &near, tri_near_clipped);

    if (near_clipped == 0) return;

    for (int i = 0; i < near_clipped; ++i) {
        unsigned char left_clipped = clip(&tri_near_clipped[i], &left, tri_left_clipped);

        if (left_clipped == 0) continue;

        for (int j = 0; j < left_clipped; ++j) {
            unsigned char right_clipped = clip(&tri_left_clipped[j], &right, tri_right_clipped);

            if (right_clipped == 0) continue;

            for (int k = 0; k < right_clipped; ++k) {
                unsigned char bottom_clipped = clip(&tri_right_clipped[k], &bottom, tri_bottom_clipped);
                if (bottom_clipped == 0) continue;

                for (int l = 0; l < bottom_clipped; ++l) {
                    unsigned char top_clipped = clip(&tri_bottom_clipped[l], &top, tri_top_clipped);

                    if (top_clipped == 0) continue;

                    for (int m = 0; m < top_clipped; ++m) {
                        ScreenTriangle tri_screen;
                        projectshadow(&tri_top_clipped[m], &tri_screen, &light);
                        rasterise_shadow(&tri_screen, &light, shadow_map);
                    }
                }
            }
        }
    }
}

bool vertexinfrustum(
    Plane* near,
    Plane* left,
    Plane* right,
    Plane* top,
    Plane* bottom,
    const Vector3* vertex
) {
    if (
        isInside(&near->normal_, &near->position_, *vertex) &&
        isInside(&left->normal_, &left->position_, *vertex) &&
        isInside(&right->normal_, &right->position_, *vertex) &&
        isInside(&top->normal_, &top->position_, *vertex) &&
        isInside(&bottom->normal_, &bottom->position_, *vertex))
    {
        return true;
    }

    return false;
}

uint maxhex(const uint a, const uint b) {
    return a > b ? a : b;
}

uint minhex(const uint a, const uint b) {
    return a < b ? a : b;
}

float Vec2Angle (
    const Vector2* a,
    const Vector2* b
) {
    return acos(Vec2Dot(a, b));
}

void rotateAboutAxis(
    const Vector3* in,
    const Vector3* axis,
    const float angle,
    Vector3* out
) {
    const float c = cos(angle);
    const float s = sin(angle);
    const float t = 1 - c;

    const float x = in->x;
    const float y = in->y;
    const float z = in->z;

    const float x2 = (t * axis->x * axis->x + c) * x + (t * axis->x * axis->y - s * axis->z) * y + (t * axis->x * axis->z + s * axis->y) * z;
    const float y2 = (t * axis->x * axis->y + s * axis->z) * x + (t * axis->y * axis->y + c) * y + (t * axis->y * axis->z - s * axis->x) * z;
    const float z2 = (t * axis->x * axis->z - s * axis->y) * x + (t * axis->y * axis->z + s * axis->x) * y + (t * axis->z * axis->z + c) * z;

    out->x = x2;
    out->y = y2;
    out->z = z2;
}

void toCameraSpace (
    Vector3* in,
    Vector3* out,
    global const State* info
) {
    const Vector3 camera_pos = info->camera_position_;
    Vector3 camera_rot = info->camera_rotation_;

    Vec3Subtract(in, &camera_pos, out);
    Vec3Rotate(in, &camera_rot);
}

bool vertexInCone (
    const Vector3* vertex,
    const float fov
) {
    const float rs = (fov * (M_PI / 180.0f)) / 2;

    Vector3 light_dir_norm = {0, 0, -1};
    normalize(&light_dir_norm, &light_dir_norm);

    Vector3 vertex_norm = *vertex;
    normalize(&vertex_norm, &vertex_norm);

    return Vec3Dot(&light_dir_norm, &vertex_norm) > cos(rs);
}

bool equaltowithinpercent (
    const float a,
    const float b,
    const float percent
) {
    float diff = a * percent;

    return (a - diff >= b && a + diff <= b) || (b - diff >= a && b + diff <= a);
}

void kernel gpu_lighting (
    global uint* pixels,
    global float* shadow_map,
    const global uint* C,
    global const GPULight* light_ptr,
    global const State* info,
    global const uint* diffuseColors,
    global const uint* specularColors,
    global Vector3* normals,
    global Vector3* positions,
    global const float* zbuffer
) {
    const int zero = get_global_id(0);
    const int one = get_global_id(1);
    const int two = get_global_id(2);

    const int pixel_num = get_global_offset(0) + zero + get_global_size(0) * one + get_global_size(0) * get_global_size(1) * two;

    if (zbuffer[pixel_num] == -INFINITY) return;

    const uint pixel = C[pixel_num];

    State inf = *info;

    float pixel_r = (float)((pixel & 0xff0000) >> 16) / 255.0f;
    float pixel_g = (float)((pixel & 0xff00) >> 8) / 255.0f;
    float pixel_b = (float)(pixel & 0xff) / 255.0f;
    Vector3 pixel_color = {pixel_r, pixel_g, pixel_b};

    Vector3 normal = normals[pixel_num];
    Vector3 position = positions[pixel_num];

    const uint diffuseColorHex = diffuseColors[pixel_num];
    const uint specularColorHex = specularColors[pixel_num];

    float diffuse_r = (float)((diffuseColorHex & 0xff0000) >> 16) / 255.0f;
    float diffuse_g = (float)((diffuseColorHex & 0xff00) >> 8) / 255.0f;
    float diffuse_b = (float)(diffuseColorHex & 0xff) / 255.0f;
    Vector3 diffuseColor = {diffuse_r, diffuse_g, diffuse_b};

    float specular_r = (float)((specularColorHex & 0xff0000) >> 16) / 255.0f;
    float specular_g = (float)((specularColorHex & 0xff00) >> 8) / 255.0f;
    float specular_b = (float)(specularColorHex & 0xff) / 255.0f;
    Vector3 specularColor = {specular_r, specular_g, specular_b};

    Vector3 final_color = {0, 0, 0};

    GPULight light = *light_ptr;

    float light_r = (float)((light.color_ & 0xff0000) >> 16) / 255.0f;
    float light_g = (float)((light.color_ & 0xff00) >> 8) / 255.0f;
    float light_b = (float)(light.color_ & 0xff) / 255.0f;

    Vector3 light_color = {light_r, light_g, light_b};

    if (light.type_ == AMBIENT) {
        Vec3Multiply(&light_color, light.intensity_, &light_color);
        Vec3Add(&final_color, &light_color, &final_color);
    } else if (light.type_ == POINT || light.type_ == DIRECTIONAL) {
        float distance;

        // shadow
        Vector3 light_dir = light.direction_;
        Vector3 light_pos = light.position_;
        Vector3 p = position;
        Vector2 projected;

        toCameraSpace(&position, &position, info);

        Vec3Subtract(&p, &light_pos, &p);

        Vector3 angles;
        Vector3 forward = {0, 0, -1};

        GetAnglesBetweenVectors(&light_dir, &forward, &angles);

        Vector3 axis;
        Vec3Cross(&light_dir, &forward, &axis);

        rotateAboutAxis(&p, &axis, angles.y, &p);
        rotateAboutAxis(&p, &axis, angles.x, &p);
        rotateAboutAxis(&p, &axis, angles.z, &p);

        projectvertex(&p, &projected, &light);

        if (!vertexInCone(&p, light.fov_)) { return; }

        int x = absi((int)projected.x);
        int y = absi((int)projected.y);

        if (x < 0 || x >= light.shadow_map_width_ || y < 0 || y >= light.shadow_map_height_) {
            return;
        } else {
           if (!equaltowithinpercent(shadow_map[x + y * light.shadow_map_width_], p.z, 1)) {
               return;
            }
        }

        Vec3Subtract(&light.position_, &inf.camera_position_, &light.position_);
        Vec3Rotate(&light.position_, &inf.camera_rotation_);

        Vec3Subtract(&position, &light.position_, &light_dir);
        distance = Vec3Length(&light_dir);
        distance = distance * distance;
        normalize(&light_dir, &light_dir);

        float lambert = Vec3Dot(&light_dir, &normal);
        float spec = 0.0f;

        if (lambert > 0) {
            Vector3 view = position;
            normalize(&view, &view);

            Vector3 halfDir;
            Vec3Add(&light_dir, &view, &halfDir);
            normalize(&halfDir, &halfDir);

            float specAngle = Vec3Dot(&halfDir, &normal);
            specAngle = maxf(specAngle, 0.0f);
            spec = pow(specAngle, 10);
        }

        Vector3 color;
        Vector3 diffuse;
        Vector3 specular;

        Vec3Multiply(&diffuseColor, lambert, &diffuse);
        Vec3MultiplyVec3(&diffuse, &light_color, &diffuse);
        Vec3Multiply(&diffuse, light.intensity_, &diffuse);
        Vec3Divide(&diffuse, distance, &diffuse);

        Vec3Multiply(&specularColor, spec, &specular);
        Vec3MultiplyVec3(&specular, &light_color, &specular);
        Vec3Multiply(&specular, light.intensity_, &specular);
        Vec3Divide(&specular, distance, &specular);

        diffuse.x = maxf(diffuse.x, 0.0);
        diffuse.y = maxf(diffuse.y, 0.0);
        diffuse.z = maxf(diffuse.z, 0.0);

        Vec3Add(&diffuse, &specular, &color);
        Vec3Add(&final_color, &color, &final_color);
    }

    Vec3GammaCorrect(&final_color, &final_color);

    Vec3Divide(&final_color, 2, &final_color);
    Vec3MultiplyVec3(&final_color, &pixel_color, &final_color);

    int r = (int)(final_color.x * 255);
    int g = (int)(final_color.y * 255);
    int b = (int)(final_color.z * 255);

    r = maxf(r, 0);
    g = maxf(g, 0);
    b = maxf(b, 0);

    r = min(r, 255);
    g = min(g, 255);
    b = min(b, 255);

    uint pixel_val = pixels[pixel_num] + ((r << 16) | (g << 8) | b);

    pixel_val = minhex(pixel_val, 0xffffff);
    pixel_val = maxhex(pixel_val, 0);

    pixels[pixel_num] = pixel_val;
}

void Vec2ConvertCoords(
    Vector2* vec
) {
    vec->x /= vec->depth_info_;
    vec->y /= vec->depth_info_;
    vec->depth_info_ = 1 / vec->depth_info_;
}

void draw(
    ScreenTriangle* tri,
    Triangle* gsp_tri,
    global float* zbuffer,
    const global uint* texture,
    global TriangleStore* B,
    global Vector3* Ts,
    global uint* C,
    global const State* info
) {
    sortTri(tri, gsp_tri);

    float yMax = floor(tri->v1.y);
    float yMid = floor(tri->v2.y);
    float yMin = floor(tri->v3.y);

    float2 horiz = {1, 0};
    float2 line_one = asFloat2(tri->v1);
    float2 line_two = asFloat2(tri->v1);
    float2 on_line1 = asFloat2(tri->v3);
    float2 on_line2 = asFloat2(tri->v2);

    line_one -= asFloat2(tri->v3);
    line_two -= asFloat2(tri->v2);

    Line line1 = {tri->v1, tri->v3};
    Line line2 = {tri->v2, tri->v1};

    Vec2ConvertCoords(&tri->t1);
    Vec2ConvertCoords(&tri->t2);
    Vec2ConvertCoords(&tri->t3);

    float3 uvOne = asFloat3(tri->t1);
    float3 uvTwo = asFloat3(tri->t1);
    float3 uv_on_line = asFloat3(tri->t3);
    float3 uv_on_line2 = asFloat3(tri->t2);

    uvOne -= asFloat3(tri->t3);
    uvTwo -= asFloat3(tri->t2);

    bool top = true;
    bool swap = false;

    for (int i = (int)yMax; i > yMin; --i) {
        if (floor(yMid) == i) {
            line_two = asFloat2(tri->v2);
            line_two -= asFloat2(tri->v3);
            on_line2 = asFloat2(tri->v3);
            line2 = (Line){tri->v3, tri->v2};

            uv_on_line2 = asFloat3(tri->t3);
            uvTwo = asFloat3(tri->t2);
            uvTwo -= asFloat3(tri->t3);

            top = false;
        }

        Vector2 intersect1;
        Vector2 intersect2;
        Vector2 vert = {0, i};

        float l1t = intersectVec2(&on_line1, &line_one, &vert, &horiz, &intersect1);
        float l2t = intersectVec2(&on_line2, &line_two, &vert, &horiz, &intersect2);

        intersect1.depth_info_ = interpolateZ(&line1.a, &line1.b, &intersect1);
        intersect2.depth_info_ = interpolateZ(&line2.a, &line2.b, &intersect2);

        Vector2 uvIntersect1;
        Vector2 uvIntersect2;
        interpolate(&uv_on_line, &uvOne, l1t, &uvIntersect1);
        interpolate(&uv_on_line2, &uvTwo, l2t, &uvIntersect2);

        if (intersect1.x > intersect2.x) {
            Vector2 temp = intersect1;
            intersect1 = intersect2;
            intersect2 = temp;

            temp = uvIntersect1;
            uvIntersect1 = uvIntersect2;
            uvIntersect2 = temp;

            swap = true;
        } else {
            swap = false;
        }

        float xMin = floor(intersect1.x);
        float xMax = floor(intersect2.x);
        int y = floor(intersect1.y);

        int uv_min = (uvIntersect1.x < uvIntersect2.x) ? uvIntersect1.x : uvIntersect2.x;
        int uv_max = (uvIntersect1.x > uvIntersect2.x) ? uvIntersect1.x : uvIntersect2.x;

        for (int j = xMin; j < xMax; ++j) {
            float t = (float)(j - xMin) / (float)(xMax - xMin);

            Vector2 intersect3 = {j, y};
            intersect3.depth_info_ = interpolateZ(&intersect1, &intersect2, &intersect3);

            Vector2 uvIntersect3;
            Vector2 isectDir = uvIntersect2;
            Vec2Subtract(&isectDir, &uvIntersect1, &isectDir);
            interpolate(&uvIntersect1, &isectDir, t, &uvIntersect3);

            if ((int)y < info->height_ && (int)y >= 0 && j < info->width_ && j >= 0 && intersect3.depth_info_ > zbuffer[(int)(j + y * info->width_)]) {
                Vector3 aa = {l1t, l2t, t};
                Ts[(int)(j + y * info->width_)] = aa;

                uvIntersect3.depth_info_ = 1 / uvIntersect3.depth_info_;
                float uu = uvIntersect3.x * uvIntersect3.depth_info_;
                float vv = uvIntersect3.y * uvIntersect3.depth_info_;

                uint u = floor(uu * info->texture_width_);
                uint v = info->texture_height_ - floor(vv * info->texture_height_);

                uint pixel;

                if (u > info->texture_width_ || u < 0 || v > info->texture_height_ || v < 0) {
                    pixel = 0x22ffffff;
                } else {
                    pixel = texture[u + v * info->texture_width_];
                }

                if (info->texture_width_ == 1 && info->texture_height_ == 1) {
                    pixel = texture[0];
                }

                TriangleStore bb = {
                    tri->v1, tri->v2, tri->v3,
                    pixel,
                    gsp_tri->n1, gsp_tri->n2, gsp_tri->n3,
                    gsp_tri->v1, gsp_tri->v2, gsp_tri->v3,
                    gsp_tri->diffuse_color_, gsp_tri->specular_color_,
                    true, top, swap
                };
                B[(int)(j + y * info->width_)] = bb;

                zbuffer[(int)(j + y * info->width_)] = intersect3.depth_info_;
            }
        }
    }
}

void interpolateFragment(
    Vector3* a,
    Vector3* b,
    Vector3* c,
    Vector3* t,
    bool top_half,
    bool swap,
    Vector3* out
) {
    Vector3 position1 = *a;
    Vec3Subtract(&position1, c, &position1);
    interpolateVec3(c, &position1, t->x, &position1);

    Vector3 position2;
    if (top_half) {
        position2 = *a;
        Vec3Subtract(&position2, b, &position2);
        interpolateVec3(b, &position2, t->y, &position2);
    } else {
        position2 = *b;
        Vec3Subtract(&position2, c, &position2);
        interpolateVec3(c, &position2, t->y, &position2);
    }

    if (swap) {
        Vector3 temp = position1;
        position1 = position2;
        position2 = temp;
    }

    Vector3 position = position2;
    Vec3Subtract(&position, &position1, &position);
    interpolateVec3(&position1, &position, t->z, out);
}

kernel void gpu_fragment(
    global const TriangleStore* B,
    global const Vector3* Ts,
    global uint* texture,
    global uint* pixels,
    global Vector3* normals,
    global Vector3* positions,
    global uint* diffuseColors,
    global uint* specularColors,
    global const State* info
) {
    // get id of the current triangle in 3 dimensions
    const int zero = get_global_id(0);
    const int one = get_global_id(1);
    const int two = get_global_id(2);

    const int pixel_num = get_global_offset(0) + zero + get_global_size(0) * one + get_global_size(0) * get_global_size(1) * two;

    const TriangleStore Blocal = B[pixel_num];
    TriangleStore* tri = &Blocal;

    if (!tri->valid_) return;
    Vector3 ts = Ts[pixel_num];

    bool top_half = tri->top_;
    bool swap = tri->swap_;

    Vector3 normal;
    interpolateFragment(&tri->n1, &tri->n2, &tri->n3, &ts, top_half, swap, &normal);
    normal.x *= -1; normal.y *= -1; normal.z *= -1;
    normalize(&normal, &normal);

    Vector3 position;
    interpolateFragment(&tri->v1_, &tri->v2_, &tri->v3_, &ts, top_half, swap, &position);

    pixels[pixel_num] = tri->texture_;
    normals[pixel_num] = normal;
    positions[pixel_num] = position;
    diffuseColors[pixel_num] = tri->diffuse_color_;
    specularColors[pixel_num] = tri->specular_color_;
}

kernel void gpu_render (
    global const Triangle* A,
    global TriangleStore* B,
    global Vector3* Ts,
    global const GPULight* lights,
    global const State* info,
    global float* zbuffer,
    global uint* texture,
    global Vector3* normals,
    global Vector3* positions,
    global uint* diffuseColors,
    global uint* specularColors,
    global uint* C,
    global uint* pixels
) {
    // get id of the current triangle in 3 dimensions
    const int zero = get_global_id(0);
    const int one = get_global_id(1);
    const int two = get_global_id(2);

    const int gid = get_global_offset(0) + zero + get_global_size(0) * one + get_global_size(0) * get_global_size(1) * two;
    // make a copy of the triangle
    Triangle tri_cpy = A[gid];

    Vector3 model_pos = info->model_position_;
    //model_pos.x *= -1;

    Vec3Add(&tri_cpy.v1, &model_pos, &tri_cpy.v1);
    Vec3Add(&tri_cpy.v2, &model_pos, &tri_cpy.v2);
    Vec3Add(&tri_cpy.v3, &model_pos, &tri_cpy.v3);

    Vector3 posv1 = tri_cpy.v1;
    Vector3 posv2 = tri_cpy.v2;
    Vector3 posv3 = tri_cpy.v3;

    State inf = *info;

    Vec3Subtract(&tri_cpy.v1, &inf.camera_position_, &tri_cpy.v1);
    Vec3Subtract(&tri_cpy.v2, &inf.camera_position_, &tri_cpy.v2);
    Vec3Subtract(&tri_cpy.v3, &inf.camera_position_, &tri_cpy.v3);

    Vec3Rotate(&tri_cpy.v1, &inf.camera_rotation_);
    Vec3Rotate(&tri_cpy.v2, &inf.camera_rotation_);
    Vec3Rotate(&tri_cpy.v3, &inf.camera_rotation_);

    Vec3Rotate(&tri_cpy.n1, &inf.camera_rotation_);
    Vec3Rotate(&tri_cpy.n2, &inf.camera_rotation_);
    Vec3Rotate(&tri_cpy.n3, &inf.camera_rotation_);

    if (backfacecull(&tri_cpy)) return;

    Triangle tri_near_clipped[2];
    Triangle tri_left_clipped[2];
    Triangle tri_right_clipped[2];
    Triangle tri_bottom_clipped[2];
    Triangle tri_top_clipped[2];

    Plane near = {
        {0, 0, -1},
        {0, 0, -1},
        {1, 0, 0}

    };

    Plane left = {
        {-1, 0, 0},
        {0, 0, 0},
        {0, 0, -1}
    };

    Plane right = {
        {1, 0, 0},
        {0, 0, 0},
        {0, 0, -1}
    };

    Plane bottom = {
        {0, 1, 0},
        {0, 0, 0},
        {0, 0, -1}
    };

    Plane top = {
        {0, -1, 0},
        {0, 0, 0},
        {0, 0, -1}
    };

    // Vec3Rotateate left and right planes as they are at the angle of fov/2
    Vector3 r = {0, -info->fov_/2, 0};
    Vec3Rotate(&left.direction_, &r);
    Vec3Rotate(&left.normal_, &r);
    r.y = r.y * -1;
    Vec3Rotate(&right.direction_, &r);
    Vec3Rotate(&right.normal_, &r);

    float half_width = info->width_ / 2;
    float half_height = info->height_ / 2;
    float half_fov = info->fov_ / 2;

    float horiz_fov = atan((half_height * tan(half_fov * (M_PI / 180.0)))/(half_width)) * (180.0 / M_PI);

    Vector3 r2 = {-horiz_fov, 0, 0};
    Vec3Rotate(&bottom.direction_, &r2);
    Vec3Rotate(&bottom.normal_, &r2);
    r2.x = r2.x * -1;
    Vec3Rotate(&top.direction_, &r2);
    Vec3Rotate(&top.normal_, &r2);

    unsigned char n_triangles = clip(&tri_cpy, &near, &tri_near_clipped);
    ScreenTriangle tri_projected;

    if (n_triangles == 0) return;

    Vector3 light = {0, 0, 0};

    for (int i = 0; i < n_triangles; i++) {
        int n_triangles2 = clip(&tri_near_clipped[i], &left, &tri_left_clipped);

        if (n_triangles2 == 0) continue;

        for (int j = 0; j < n_triangles2; j++) {
            int n_triangles3 = clip(&tri_left_clipped[j], &right, &tri_right_clipped);

            if (n_triangles3 == 0) continue;

            for (int k = 0; k < n_triangles3; k++) {
                int n_triangles4 = clip(&tri_right_clipped[k], &bottom, &tri_bottom_clipped);

                if (n_triangles4 == 0) continue;

                for (int l = 0; l < n_triangles4; l++) {
                    int n_triangles5 = clip(&tri_bottom_clipped[l], &top, &tri_top_clipped);

                    if (n_triangles5 == 0) continue;

                    for (int m = 0; m < n_triangles5; m++) {
                        project(&tri_top_clipped[m], &tri_projected, info);

                        if (info->mode_ == WIREFRAME) {
                            drawWireframeTriangle(&tri_projected, &inf, pixels);
                            continue;
                        }

                        tri_top_clipped[m].v1 = posv1;
                        tri_top_clipped[m].v2 = posv2;
                        tri_top_clipped[m].v3 = posv3;
                        tri_bottom_clipped[m].specular_color_ = tri_cpy.specular_color_;
                        tri_bottom_clipped[m].diffuse_color_ = tri_cpy.diffuse_color_;

                        draw(
                           &tri_projected,
                           &tri_top_clipped[m],
                           zbuffer,
                           texture,
                           B,
                           Ts,
                           pixels,
                           info
                        );
                    }
                }
            }
        }
    }
}