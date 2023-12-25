typedef struct{
    float x;
    float y;
} vec2_t;

typedef struct{
    float x;
    float y;
    float z;
} vec3_t;

vec2_t vec2_add(vec2_t a, vec2_t b);
vec2_t vec2_sub(vec2_t a, vec2_t b);
vec2_t vec2_mul(vec2_t a, float f);
