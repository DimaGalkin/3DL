#ifndef GPU_TYPES
#define GPU_TYPES

enum LightType {
    AMBIENT,
    POINT
};

struct Vector2 {
    double x;
    double y;

    double depth_info_;
    double texture_info_;
};

struct Vector3 {
    double x;
    double y;
    double z;
};

struct State {
    struct Vector3 camera_position_;
    struct Vector3 camera_rotation_;
    double fov_;

    struct Vector3 model_position_;

    unsigned int width_;
    unsigned int height_;
    unsigned int texture_width_;
    unsigned int texture_height_;

    unsigned int num_lights_;
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

    unsigned int diffuse_color_;
    unsigned int specular_color_;
    double shininess_;
};

struct ScreenTriangle {
    struct Vector2 v1;
    struct Vector2 v2;
    struct Vector2 v3;
};

struct Intersection {
    struct Vector3 position_;
    bool valid_;
    double t_;
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

    unsigned int color_;

    double intensity_;
};

#endif