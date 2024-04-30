#ifndef MATHCL
#define MATHCL

#ifndef uint32_t
typedef unsigned int uint32_t;
#endif

enum LightType {
    UNINITIALIZED,
    AMBIENT,
    POINT,
    DIRECTIONAL
};

enum RENDERMODE {
    WIREFRAME,
    WIREFRAME_OVERLAY,
    SHADED
};

struct Vector2 {
    float x;
    float y;

    float depth_info_;
    float texture_info_;
};

struct Vector3 {
    float x;
    float y;
    float z;
    float w;
};

struct State {
    enum RENDERMODE mode_;

    struct Vector3 camera_position_;
    struct Vector3 camera_rotation_;
    float fov_;

    struct Vector3 model_position_;

    uint32_t width_;
    uint32_t height_;
    uint32_t texture_width_;
    uint32_t texture_height_;

    uint32_t num_lights_;
};

struct TriangleStore {
    struct Vector2 v1;
    struct Vector2 v2;
    struct Vector2 v3;

    uint32_t texture_;

    struct Vector3 n1;
    struct Vector3 n2;
    struct Vector3 n3;

    struct Vector3 v1_;
    struct Vector3 v2_;
    struct Vector3 v3_;

    uint32_t diffuse_color_;
    uint32_t specular_color_;

    bool valid_;
    bool top_;
    bool swap_;
};

struct Triangle {
    struct Vector3 v1;
    struct Vector3 v2;
    struct Vector3 v3;

    struct Vector2 t1;
    struct Vector2 t2;
    struct Vector2 t3;

    struct Vector3 n1;
    struct Vector3 n2;
    struct Vector3 n3;

    uint32_t diffuse_color_;
    uint32_t specular_color_;

    float shininess_;
};

struct ScreenTriangle {
    struct Vector2 v1;
    struct Vector2 v2;
    struct Vector2 v3;

    struct Vector2 t1;
    struct Vector2 t2;
    struct Vector2 t3;
};

struct IntersectInfo {
    struct Vector3 intersect_;
    bool valid_;
    float t_;
};

struct Ray {
    struct Vector3 origin_;
    struct Vector3 direction_;
};

struct Plane {
    struct Vector3 normal_;
    struct Vector3 position_;
    struct Vector3 direction_;
};

struct Line {
    struct Vector2 a;
    struct Vector2 b;
};

struct GPULight {
    enum LightType type_;

    struct Vector3 position_;
    struct Vector3 direction_;

    bool shadows_enabled_;
    uint32_t shadow_map_width_;
    uint32_t shadow_map_height_;

    float fov_;

    uint32_t color_;

    bool first_;

    float intensity_;
};

#endif