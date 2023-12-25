#include "vec.h"

vec2_t vec2_add(vec2_t a, vec2_t b) {
    return (vec2_t) {
        .x = a.x + b.x,
        .y = a.y + b.y,
    };
}

vec2_t vec2_sub(vec2_t a, vec2_t b) {
    return (vec2_t) {
        .x = a.x - b.x,
        .y = a.y - b.y,
    };
}

vec2_t vec2_mul(vec2_t a, float f) {
    return (vec2_t) {
        .x = a.x * f,
        .y = a.y * f,
    };
}
