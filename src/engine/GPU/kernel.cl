#include "src/engine/GPU/types.h"
#include "src/engine/GPU/mathcl.h"

typedef unsigned int uint32_t;

void draw_line(
    int ax, int ay, 
    int bx, int by,
    const struct State* info,
    global int* C
) {
    int dx = bx - ax;
    int dy = by - ay;

    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    double Xinc = dx / (double) steps;
    double Yinc = dy / (double) steps;

    double X = ax;
    double Y = ay;

    for (int i = 0; i <= steps; i++) {
        if (X >= info->width_ || X < 0 || Y >= info->height_ || Y < 0) continue;
        C[(int) X + (int) Y * info->width_] = 0xffffff;
        X += Xinc;
        Y += Yinc;
    }
}

void drawWireframeTriangle(
    struct ScreenTriangle* tri,
    const struct State* info,
    global int* C
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
    const struct Triangle* in,
    struct ScreenTriangle* out,
    global const struct State* info
) {
    double fov = info->fov_;
    double dtp = ((double)info->width_ / 2.0) / tan((fov / 2.0) * (M_PI / 180.0)); 
    double h_height = (double)info->height_ / 2.0;
    double h_width = (double)info->width_ / 2.0;
    
    double t1 = (dtp - 0) / in->v1.z;
    double t2 = (dtp - 0) / in->v2.z;
    double t3 = (dtp - 0) / in->v3.z;

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
    out->t2 = in->t2;
    out->t3 = in->t3;
}

unsigned char clip(
    struct Triangle* tri,
    struct Plane* plane,
    struct Triangle* out_tris
) {
    struct Vector3 dir1 = tri->v2;
    struct Vector3 dir2 = tri->v3;
    struct Vector3 dir3 = tri->v1;

    Vec3Subtract(&dir1, &tri->v1, &dir1);
    Vec3Subtract(&dir2, &tri->v2, &dir2);
    Vec3Subtract(&dir3, &tri->v3, &dir3);
    
    struct Ray ray1 = {tri->v1, dir1};
    struct Ray ray2 = {tri->v2, dir2};
    struct Ray ray3 = {tri->v3, dir3};

    struct IntersectInfo info1;
    struct IntersectInfo info2;
    struct IntersectInfo info3;

    intersects(plane, &ray1, &info1);
    intersects(plane, &ray2, &info2);
    intersects(plane, &ray3, &info3);

    if (!info1.valid_ && !info2.valid_ && !info3.valid_) {
        bool inside = isInside(&plane->normal_, &plane->position_, tri->v1);

        if (inside) {
            out_tris[0] = *tri;
            return 1;
        } else {
            return 0;
        }
    }

    if (info1.valid_ && info2.valid_ && info3.valid_) {
        bool inside = isInside(&plane->normal_, &plane->position_, tri->v1);

        if (inside) {
            out_tris[0] = *tri;
            return 1;
        } else {
            return 0;
        }
    }

    if (
        !info1.valid_ && !info2.valid_ && info3.valid_,
        !info1.valid_ && info2.valid_ && !info3.valid_,
        info1.valid_ && !info2.valid_ && !info3.valid_
    ) {
        return 0;
    }

    if (info1.valid_ && info2.valid_) {
        bool inside = isInside(&plane->normal_, &plane->position_, tri->v2);

        if (inside) {
            out_tris[0].v1 = tri->v2;
            out_tris[0].v2 = info2.intersect_;
            out_tris[0].v3 = info1.intersect_;

            out_tris[0].t1 = tri->t2;
            calcUv(&tri->t2, &tri->t3, info2.t_, &out_tris[0].t2);
            calcUv(&tri->t1, &tri->t2, info1.t_, &out_tris[0].t3);

            return 1;
        } else {
            out_tris[0].v1 = tri->v1;
            out_tris[0].v2 = tri->v3;
            out_tris[0].v3 = info2.intersect_;

            out_tris[1].v1 = tri->v1;
            out_tris[1].v2 = info1.intersect_;
            out_tris[1].v3 = info2.intersect_;

            out_tris[0].t1 = tri->t1;
            out_tris[0].t2 = tri->t3;
            calcUv(&tri->t2, &tri->t3, info2.t_, &out_tris[0].t2);

            out_tris[1].t1 = tri->t1;
            calcUv(&tri->t1, &tri->t2, info1.t_, &out_tris[1].t2);
            calcUv(&tri->t2, &tri->t3, info2.t_, &out_tris[1].t3);

            return 2;
        }
    }

    if (info1.valid_ && info3.valid_) {
        bool inside = isInside(&plane->normal_, &plane->position_, tri->v1);

        if (inside) {
            out_tris[0].v1 = tri->v1;
            out_tris[0].v2 = info1.intersect_;
            out_tris[0].v3 = info3.intersect_;

            out_tris[0].t1 = tri->t1;
            calcUv(&tri->t1, &tri->t2, info1.t_, &out_tris[0].t2);
            calcUv(&tri->t3, &tri->t1, info3.t_, &out_tris[0].t3);

            return 1;
        } else {
            out_tris[0].v1 = tri->v2;
            out_tris[0].v2 = tri->v3;
            out_tris[0].v3 = info3.intersect_;

            out_tris[1].v1 = tri->v2;
            out_tris[1].v2 = info1.intersect_;
            out_tris[1].v3 = info3.intersect_;

            out_tris[0].t1 = tri->t2;
            out_tris[0].t2 = tri->t3;
            calcUv(&tri->t3, &tri->t1, info3.t_, &out_tris[0].t3);

            out_tris[1].t1 = tri->t2;
            calcUv(&tri->t1, &tri->t2, info1.t_, &out_tris[1].t2);
            calcUv(&tri->t3, &tri->t1, info3.t_, &out_tris[1].t3);

            return 2;
        }
    }

    if (info2.valid_ && info3.valid_) {
        bool inside = isInside(&plane->normal_, &plane->position_, tri->v3);

        if (inside) {
            out_tris[0].v1 = tri->v3;
            out_tris[0].v2 = info2.intersect_;
            out_tris[0].v3 = info3.intersect_;

            out_tris[0].t1 = tri->t3;
            calcUv(&tri->t2, &tri->t3, info2.t_, &out_tris[0].t2);
            calcUv(&tri->t3, &tri->t1, info3.t_, &out_tris[0].t3);

            return 1;
        } else {
            out_tris[0].v1 = tri->v1;
            out_tris[0].v2 = tri->v2;
            out_tris[0].v3 = info2.intersect_;

            out_tris[1].v1 = tri->v1;
            out_tris[1].v2 = info3.intersect_;
            out_tris[1].v3 = info2.intersect_;

            out_tris[0].t1 = tri->t1;
            out_tris[0].t2 = tri->t2;
            calcUv(&tri->t2, &tri->t3, info2.t_, &out_tris[0].t3);

            out_tris[1].t1 = tri->t1;
            calcUv(&tri->t3, &tri->t1, info3.t_, &out_tris[1].t2);
            calcUv(&tri->t2, &tri->t3, info2.t_, &out_tris[1].t3);

            return 2;
        }
    }

    return 0;
}

double interpolateZ(
    struct Vector2* p1,
    struct Vector2* p2,
    struct Vector2* p
) {
    if (p1->depth_info_ == p2->depth_info_) return p1->depth_info_;

    double denom = p2->x - p1->x;
    double t;

    if (denom == 0) {
        denom = p2->y - p1->y;

        if (denom == 0) {
            return p1->depth_info_;
        }

        t = (p->y - p1->y) / denom;
    } else {
        t = (p->x - p1->x) / denom;
    }

    return p1->depth_info_ + (p2->depth_info_ - p1->depth_info_) * t;
}

double intersectVec2(
    struct Vector2* a_pos,
    struct Vector2* a_dir,
    struct Vector2* b_pos,
    struct Vector2* b_dir,
    struct Vector2* out
) {
    double t = (b_dir->x * (a_pos->y - b_pos->y) - b_dir->y * (a_pos->x - b_pos->x)) / (a_dir->x * b_dir->y - a_dir->y * b_dir->x);
    struct Vector2 res = *a_dir;

    Vec2Multiply(&res, t, &res);
    Vec2Add(&res, a_pos, &res);

    out->x = res.x;
    out->y = res.y;

    return t;
}

int abs_m(int in) {
    if (in < 0) {
        return in * -1;
    }

    return in;
}

double abs_d(double in) {
    if (in < 0) {
        return in * -1;
    }

    return in;
}

void draw_texture(
    struct ScreenTriangle* tri,
    struct Triangle* gsp_tri,
    double* zbuffer,
    global const struct State* info,
    global uint* texture,
    global struct Vector3* normals,
    global struct Vector3* positions,
    global uint* diffuseColors,
    global uint* specularColors,
    global uint* C
) {
    if (tri->v1.y < tri->v2.y) {
        struct Vector2 temp = tri->v1;
        tri->v1 = tri->v2;
        tri->v2 = temp;

        temp = tri->t1;
        tri->t1 = tri->t2;
        tri->t2 = temp;

        struct Vector3 temp2 = gsp_tri->v1;
        gsp_tri->v1 = gsp_tri->v2;
        gsp_tri->v2 = temp2;

        temp2 = gsp_tri->n1;
        gsp_tri->n1 = gsp_tri->n2;
        gsp_tri->n2 = temp2;
    }

    if (tri->v2.y < tri->v3.y) {
        struct Vector2 temp = tri->v2;
        tri->v2 = tri->v3;
        tri->v3 = temp;

        temp = tri->t2;
        tri->t2 = tri->t3;
        tri->t3 = temp;

        struct Vector3 temp2 = gsp_tri->v2;
        gsp_tri->v2 = gsp_tri->v3;
        gsp_tri->v3 = temp2;

        temp2 = gsp_tri->n2;
        gsp_tri->n2 = gsp_tri->n3;
        gsp_tri->n3 = temp2;
    }

    if (tri->v1.y < tri->v2.y) {
        struct Vector2 temp = tri->v1;
        tri->v1 = tri->v2;
        tri->v2 = temp;

        temp = tri->t1;
        tri->t1 = tri->t2;
        tri->t2 = temp;

        struct Vector3 temp2 = gsp_tri->v1;
        gsp_tri->v1 = gsp_tri->v2;
        gsp_tri->v2 = temp2;

        temp2 = gsp_tri->n1;
        gsp_tri->n1 = gsp_tri->n2;
        gsp_tri->n2 = temp2;
    }

    double yMax = tri->v1.y;
    double yMid = tri->v2.y;
    double yMin = tri->v3.y;

    struct Vector2 horiz = {1, 0, 0};
    struct Vector2 line_one = tri->v1;
    struct Vector2 line_two = tri->v1;
    struct Vector2 on_line1 = tri->v3;
    struct Vector2 on_line2 = tri->v2;

    Vec2Subtract(&line_one, &tri->v3, &line_one);
    Vec2Subtract(&line_two, &tri->v2, &line_two);

    struct Vector2 uvOne = tri->t1;
    struct Vector2 uvTwo = tri->t1;
    struct Vector2 uv_on_line = tri->t3;
    struct Vector2 uv_on_line2 = tri->t2;

    Vec2Subtract(&uvOne, &tri->t3, &uvOne);
    Vec2Subtract(&uvTwo, &tri->t2, &uvTwo);

    struct Vector3 gsp_line_one = gsp_tri->v1;
    struct Vector3 gsp_line_two = gsp_tri->v1;
    struct Vector3 gsp_on_line1 = gsp_tri->v3;
    struct Vector3 gsp_on_line2 = gsp_tri->v2;

    Vec3Subtract(&gsp_line_one, &gsp_tri->v3, &gsp_line_one);
    Vec3Subtract(&gsp_line_two, &gsp_tri->v2, &gsp_line_two);

    struct Vector3 normal_line_one = gsp_tri->n1;
    struct Vector3 normal_line_two = gsp_tri->n1;
    struct Vector3 normal_on_line1 = gsp_tri->n3;
    struct Vector3 normal_on_line2 = gsp_tri->n2;

    Vec3Subtract(&normal_line_one, &gsp_tri->n3, &normal_line_one);
    Vec3Subtract(&normal_line_two, &gsp_tri->n2, &normal_line_two);

    struct Line line1 = {tri->v1, tri->v3};
    struct Line line2 = {tri->v2, tri->v1};

    for (int i = (int)yMax; i > yMin; --i) {
        if (floor(yMid) == i) {
            line_two = tri->v2;
            Vec2Subtract(&line_two, &tri->v3, &line_two);
            on_line2 = tri->v3;
            line2 = (struct Line){tri->v3, tri->v2};

            uv_on_line2 = tri->t3;
            uvTwo = tri->t2;
            Vec2Subtract(&uvTwo, &tri->t3, &uvTwo);

            gsp_line_two = gsp_tri->v2;
            Vec3Subtract(&gsp_line_two, &gsp_tri->v3, &gsp_line_two);
            gsp_on_line2 = gsp_tri->v3;

            normal_line_two = gsp_tri->n2;
            Vec3Subtract(&normal_line_two, &gsp_tri->n3, &normal_line_two);
            normal_on_line2 = gsp_tri->n3;
        }

        struct Vector2 intersect1;
        struct Vector2 intersect2;
        struct Vector2 vert = {0, i, 0};

        double l1t = intersectVec2(&on_line1, &line_one, &vert, &horiz, &intersect1);
        double l2t = intersectVec2(&on_line2, &line_two, &vert, &horiz, &intersect2);

        intersect1.depth_info_ = interpolateZ(&line1.a, &line1.b, &intersect1);
        intersect2.depth_info_ = interpolateZ(&line2.a, &line2.b, &intersect2);

        struct Vector2 uvIntersect1;
        struct Vector2 uvIntersect2;
        interpolate(&uv_on_line, &uvOne, l1t, &uvIntersect1);
        interpolate(&uv_on_line2, &uvTwo, l2t, &uvIntersect2);

        struct Vector3 gspIntersect1;
        struct Vector3 gspIntersect2;
        interpolateVec3(&gsp_on_line1, &gsp_line_one, l1t, &gspIntersect1);
        interpolateVec3(&gsp_on_line2, &gsp_line_two, l2t, &gspIntersect2);

        struct Vector3 normalIntersect1;
        struct Vector3 normalIntersect2;
        interpolateVec3(&normal_on_line1, &normal_line_one, l1t, &normalIntersect1);
        interpolateVec3(&normal_on_line2, &normal_line_two, l2t, &normalIntersect2);

        if (intersect1.x > intersect2.x) {
            struct Vector2 temp = intersect1;
            intersect1 = intersect2;
            intersect2 = temp;

            temp = uvIntersect1;
            uvIntersect1 = uvIntersect2;
            uvIntersect2 = temp;

            struct Vector3 temp2 = gspIntersect1;
            gspIntersect1 = gspIntersect2;
            gspIntersect2 = temp2;

            temp2 = normalIntersect1;
            normalIntersect1 = normalIntersect2;
            normalIntersect2 = temp2;
        }

        double xMin = (int)intersect1.x;
        double xMax = (int)intersect2.x;
        double y = (int)intersect1.y;

        double uvMax = (uvIntersect1.x > uvIntersect2.x) ? uvIntersect1.x : uvIntersect2.x;
        double uvMin = (uvIntersect1.x < uvIntersect2.x) ? uvIntersect1.x : uvIntersect2.x;

        for (int j = xMin; j < xMax; ++j) {
            double t = (double)(j - xMin) / (double)(xMax - xMin);
            if ((xMax - xMin) == 0) t = 0;

            struct Vector2 intersect3 = {j, y, 0};
            intersect3.depth_info_ = interpolateZ(&intersect1, &intersect2, &intersect3);

            struct Vector2 uvIntersect3;
            struct Vector2 isectDir = uvIntersect2;
            Vec2Subtract(&isectDir, &uvIntersect1, &isectDir);
            interpolate(&uvIntersect1, &isectDir, t, &uvIntersect3);

            struct Vector3 gspIntersect3;
            struct Vector3 gspIsectDir = gspIntersect2;
            Vec3Subtract(&gspIsectDir, &gspIntersect1, &gspIsectDir);
            interpolateVec3(&gspIntersect1, &gspIsectDir, t, &gspIntersect3);

            struct Vector3 normalIntersect3;
            struct Vector3 normalIsectDir = normalIntersect2;
            Vec3Subtract(&normalIsectDir, &normalIntersect1, &normalIsectDir);
            interpolateVec3(&normalIntersect1, &normalIsectDir, t, &normalIntersect3);

            uint32_t u = floor(uvIntersect3.x * info->texture_width_);
            uint32_t v = info->texture_height_ - floor(uvIntersect3.y * info->texture_height_);

            if ((int)y < info->height_ && (int)y >= 0 && j < info->width_ && j >= 0 && intersect3.depth_info_ > zbuffer[(int)(j + y * info->width_)]) {
                if (u > info->texture_width_ || u < 0 || v > info->texture_height_ || v < 0) {
                    C[(int)(j + y * info->width_)] |= 0x333333;
                    continue;
                };

                struct Vector3 normal = normalIntersect3;
                normal.x *= -1; normal.y *= -1; normal.z *= -1;

                if (info->texture_width_ == 1 || info->texture_height_ == 1) {
                    C[(int)(j + y * info->width_)] = texture[0];
                    continue;
                } else {
                    C[(int)(j + y * info->width_)] = texture[u + v * info->texture_width_];
                }

                zbuffer[(int)(j + y * info->width_)] = intersect3.depth_info_;
                normals[(int)(j + y * info->width_)] = normal;
                positions[(int)(j + y * info->width_)] = gspIntersect3;
                diffuseColors[(int)(j + y * info->width_)] = gsp_tri->diffuse_color_;
                specularColors[(int)(j + y * info->width_)] = gsp_tri->specular_color_;
            }
        }

        if (info->mode_ == WIREFRAME_OVERLAY) {
            C[(int)xMin + (int)y * info->width_] = 0xffffff;
            C[(int)xMax + (int)y * info->width_] = 0xffffff;
        }
    }
}

bool backfacecull(const struct Triangle* tr) {
    struct Vector3 normal;
    Vec3Cross(&tr->v2, &tr->v1, &normal);
    normalize(&normal, &normal);

    struct Vector3 view;
    Vec3Subtract(&tr->v1, &tr->v3, &view);
    normalize(&view, &view);

    double dp = Vec3Dot(&normal, &view);

    return dp > 0;
}

void kernel gpu_lighting (
    global uint* pixels,
    global const struct GPULight* lights,
    global const struct State* info,
    global const uint* diffuseColors,
    global const uint* specularColors,
    global struct Vector3* normals,
    global struct Vector3* positions,
    global const double* zbuffer
) {
    const int pixel_num = get_global_id(0);

    if (zbuffer[pixel_num] == -INFINITY) return;

    const uint pixel = pixels[pixel_num];

    double pixel_r = (double)((pixel & 0xff0000) >> 16) / 255.0;
    double pixel_g = (double)((pixel & 0xff00) >> 8) / 255.0;
    double pixel_b = (double)(pixel & 0xff) / 255.0;
    struct Vector3 pixel_color = {pixel_r, pixel_g, pixel_b};

    struct Vector3 normal = normals[pixel_num];
    struct Vector3 position = positions[pixel_num];
    const uint diffuseColorHex = diffuseColors[pixel_num];
    const uint specularColorHex = specularColors[pixel_num];

    double diffuse_r = (double)((diffuseColorHex & 0xff0000) >> 16) / 255.0;
    double diffuse_g = (double)((diffuseColorHex & 0xff00) >> 8) / 255.0;
    double diffuse_b = (double)(diffuseColorHex & 0xff) / 255.0;
    struct Vector3 diffuseColor = {diffuse_r, diffuse_g, diffuse_b};

    double specular_r = (double)((specularColorHex & 0xff0000) >> 16) / 255.0;
    double specular_g = (double)((specularColorHex & 0xff00) >> 8) / 255.0;
    double specular_b = (double)(specularColorHex & 0xff) / 255.0;
    struct Vector3 specularColor = {specular_r, specular_g, specular_b};

    struct Vector3 final_color = {0, 0, 0};

    for (int light_num = 0; light_num < info->num_lights_; ++light_num) {
        struct GPULight light = lights[light_num];

        double light_r = (double)((lights[light_num].color_ & 0xff0000) >> 16) / 255.0;
        double light_g = (double)((lights[light_num].color_ & 0xff00) >> 8) / 255.0;
        double light_b = (double)(lights[light_num].color_ & 0xff) / 255.0;

        struct Vector3 light_color = {light_r, light_g, light_b};

        if (light.type_ == AMBIENT) {
            Vec3Multiply(&light_color, light.intensity_, &light_color);
            Vec3Add(&final_color, &light_color, &final_color);
        } else if (light.type_ == POINT) {
            Vec3Subtract(&light.position_, &info->camera_position_, &light.position_);
            Vec3Rotate(&light.position_, &info->camera_rotation_);

            struct Vector3 light_dir;
            Vec3Subtract(&position, &light.position_, &light_dir);
            double distance = Vec3Length(&light_dir);
            distance = distance * distance;
            normalize(&light_dir, &light_dir);

            double lambert = Vec3Dot(&light_dir, &normal);
            double spec = 0.0;

            if (lambert > 0) {
                struct Vector3 view = position;
                normalize(&view, &view);

                struct Vector3 halfDir;
                Vec3Add(&light_dir, &view, &halfDir);
                normalize(&halfDir, &halfDir);

                double specAngle = Vec3Dot(&halfDir, &normal);
                specAngle = max(specAngle, 0.0);
                spec = pow(specAngle, 80);
            }

            struct Vector3 color;
            struct Vector3 diffuse;
            struct Vector3 specular;

            Vec3Multiply(&diffuseColor, lambert, &diffuse);
            Vec3MultiplyVec3(&diffuse, &light_color, &diffuse);
            Vec3Multiply(&diffuse, light.intensity_, &diffuse);
            Vec3Divide(&diffuse, distance, &diffuse);

            Vec3Multiply(&specularColor, spec, &specular);
            Vec3MultiplyVec3(&specular, &light_color, &specular);
            Vec3Multiply(&specular, light.intensity_, &specular);
            Vec3Divide(&specular, distance, &specular);

            diffuse.x = max(diffuse.x, 0.0);
            diffuse.y = max(diffuse.y, 0.0);
            diffuse.z = max(diffuse.z, 0.0);

            Vec3Add(&diffuse, &specular, &color);
            Vec3Add(&final_color, &color, &final_color);
        }
    }

    Vec3Divide(&final_color, info->num_lights_, &final_color);
    Vec3GammaCorrect(&final_color, &final_color);

    Vec3MultiplyVec3(&final_color, &pixel_color, &final_color);

    int r = (int)(final_color.x * 255);
    int g = (int)(final_color.y * 255);
    int b = (int)(final_color.z * 255);

    r = max(r, 0);
    g = max(g, 0);
    b = max(b, 0);

    r = min(r, 255);
    g = min(g, 255);
    b = min(b, 255);
    
    pixels[pixel_num] = (r << 16) | (g << 8) | b;
}

kernel void gpu_render (
    global const struct Triangle* A,
    global const struct GPULight* lights,
    global const struct State* info,
    global double* zbuffer,
    global uint* texture,
    global struct Vector3* normals,
    global struct Vector3* positions,
    global struct uint* diffuseColors,
    global struct uint* specularColors,
    global uint* C
){
    int gid = get_global_id(0);

    // make a copy of the triangle
    struct Triangle* tri_cpy = &A[gid];

    Vec3Add(&tri_cpy->v1, &info->model_position_, &tri_cpy->v1);
    Vec3Add(&tri_cpy->v2, &info->model_position_, &tri_cpy->v2);
    Vec3Add(&tri_cpy->v3, &info->model_position_, &tri_cpy->v3);

    Vec3Subtract(&tri_cpy->v1, &info->camera_position_, &tri_cpy->v1);
    Vec3Subtract(&tri_cpy->v2, &info->camera_position_, &tri_cpy->v2);
    Vec3Subtract(&tri_cpy->v3, &info->camera_position_, &tri_cpy->v3);

    Vec3Rotate(&tri_cpy->v1, &info->camera_rotation_);
    Vec3Rotate(&tri_cpy->v2, &info->camera_rotation_);
    Vec3Rotate(&tri_cpy->v3, &info->camera_rotation_);

    Vec3Rotate(&tri_cpy->n1, &info->camera_rotation_);
    Vec3Rotate(&tri_cpy->n2, &info->camera_rotation_);
    Vec3Rotate(&tri_cpy->n3, &info->camera_rotation_);

    if (backfacecull(tri_cpy)) return;

    struct Triangle tri_near_clipped[2];
    struct Triangle tri_left_clipped[2];
    struct Triangle tri_right_clipped[2];

    struct Plane near = {
        {0, 0, -1},
        {0, 0, -1},
        {1, 0, 0}
    
    };

    struct Plane left = {
        {-1, 0, 0},
        {0, 0, 0},
        {0, 0, -1}
    };

    struct Plane right = {
        {1, 0, 0},
        {0, 0, 0},
        {0, 0, -1}
    };

    // Vec3Rotateate left and right planes as they are at the angle of fov/2
    struct Vector3 r = {0, -info->fov_, 0};
    Vec3Rotate(&left.direction_, &r);
    Vec3Rotate(&left.normal_, &r);
    r.y = r.y * -1;
    Vec3Rotate(&right.direction_, &r);
    Vec3Rotate(&right.normal_, &r);

    unsigned char n_triangles = clip(tri_cpy, &near, &tri_near_clipped);
    struct ScreenTriangle tri_projected;

    if (n_triangles == 0) return;

    struct Vector3 light = {0, 0, 0};

    for (int i = 0; i < n_triangles; i++) {
        int n_triangles2 = clip(&tri_near_clipped[i], &left, &tri_left_clipped);

        if (n_triangles2 == 0) continue;

        for (int j = 0; j < n_triangles2; j++) {
            int n_triangles3 = clip(&tri_left_clipped[j], &right, &tri_right_clipped);

            if (n_triangles3 == 0) continue;

            for (int k = 0; k < n_triangles3; k++) {
                project(&tri_right_clipped[k], &tri_projected, info);

                // copyColours(tri_cpy, &tri_right_clipped[k]);
                // copyNormals(tri_cpy, &tri_right_clipped[k]);

                if (info->mode_ == WIREFRAME) {
                    drawWireframeTriangle(&tri_projected, info, C);
                    continue;
                }
                
                draw_texture (
                    &tri_projected,
                    &tri_right_clipped[k],
                    zbuffer,
                    info,
                    texture,
                    normals,
                    positions,
                    diffuseColors,
                    specularColors,
                    C
                );
            }
        }
    }
}