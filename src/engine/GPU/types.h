#ifndef MATHCL
#define MATHCL

#ifdef __cplusplus
#include <cstdint>
#endif

typedef enum LightType {
    UNINITIALIZED,
    AMBIENT,
    POINT,
    DIRECTIONAL
} LightType;

typedef enum RENDERMODE {
    WIREFRAME,
    WIREFRAME_OVERLAY,
    SHADED
} RENDERMODE;

typedef struct Vector2 {
    float x;
    float y;

    float depth_info_;
} Vector2;

typedef struct Vector3 {
    float x;
    float y;
    float z;
    float w;
} Vector3;

typedef struct State {
    RENDERMODE mode_;

    Vector3 camera_position_;
    Vector3 camera_rotation_;
    float fov_;

    Vector3 model_position_;

    unsigned int width_;
    unsigned int height_;
    unsigned int texture_width_;
    unsigned int texture_height_;

    unsigned int num_lights_;
} State;

typedef struct TriangleStore {
    Vector2 v1;
    Vector2 v2;
    Vector2 v3;

    unsigned int texture_;

    Vector3 n1;
    Vector3 n2;
    Vector3 n3;

    Vector3 v1_;
    Vector3 v2_;
    Vector3 v3_;

    unsigned int diffuse_color_;
    unsigned int specular_color_;

    bool valid_;
    bool top_;
    bool swap_;
} TriangleStore;

typedef struct Triangle {
    Vector3 v1;
    Vector3 v2;
    Vector3 v3;

    Vector2 t1;
    Vector2 t2;
    Vector2 t3;

    Vector3 n1;
    Vector3 n2;
    Vector3 n3;

    unsigned int diffuse_color_;
    unsigned int specular_color_;

    float shininess_;
} Triangle;

typedef struct ScreenTriangle {
    Vector2 v1;
    Vector2 v2;
    Vector2 v3;

    Vector2 t1;
    Vector2 t2;
    Vector2 t3;
} ScreenTriangle;

typedef struct IntersectInfo {
    Vector3 intersect_;
    bool valid_;
    float t_;
} IntersectInfo;

typedef struct Ray {
    Vector3 origin_;
    Vector3 direction_;
} Ray;

typedef struct Plane {
    Vector3 normal_;
    Vector3 position_;
    Vector3 direction_;
} Plane;

typedef struct Line {
    Vector2 a;
    Vector2 b;
} Line;

typedef struct GPULight {
    LightType type_;

    Vector3 position_;
    Vector3 direction_;

    bool shadows_enabled_;
    unsigned int shadow_map_width_;
    unsigned int shadow_map_height_;

    float fov_;

    unsigned int color_;

    float intensity_;
} GPULight;

#endif