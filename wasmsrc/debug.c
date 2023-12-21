#include <stdint.h>
#include <stdbool.h>
#include "native.h"

#define SCREEN_WIDTH 10
#define SCREEN_HEIGHT 10
#define SCREEN_PIXELS (SCREEN_WIDTH*SCREEN_HEIGHT)

const float fps = 60;

static float time = .0f;
static bool enable = false;

static const char * screen[SCREEN_PIXELS] = {
    "screen_0_0", "screen_1_0", "screen_2_0", "screen_3_0", "screen_4_0", "screen_5_0", "screen_6_0", "screen_7_0", "screen_8_0", "screen_9_0",
    "screen_0_1", "screen_1_1", "screen_2_1", "screen_3_1", "screen_4_1", "screen_5_1", "screen_6_1", "screen_7_1", "screen_8_1", "screen_9_1",
    "screen_0_2", "screen_1_2", "screen_2_2", "screen_3_2", "screen_4_2", "screen_5_2", "screen_6_2", "screen_7_2", "screen_8_2", "screen_9_2",
    "screen_0_3", "screen_1_3", "screen_2_3", "screen_3_3", "screen_4_3", "screen_5_3", "screen_6_3", "screen_7_3", "screen_8_3", "screen_9_3",
    "screen_0_4", "screen_1_4", "screen_2_4", "screen_3_4", "screen_4_4", "screen_5_4", "screen_6_4", "screen_7_4", "screen_8_4", "screen_9_4",
    "screen_0_5", "screen_1_5", "screen_2_5", "screen_3_5", "screen_4_5", "screen_5_5", "screen_6_5", "screen_7_5", "screen_8_5", "screen_9_5",
    "screen_0_6", "screen_1_6", "screen_2_6", "screen_3_6", "screen_4_6", "screen_5_6", "screen_6_6", "screen_7_6", "screen_8_6", "screen_9_6",
    "screen_0_7", "screen_1_7", "screen_2_7", "screen_3_7", "screen_4_7", "screen_5_7", "screen_6_7", "screen_7_7", "screen_8_7", "screen_9_7",
    "screen_0_8", "screen_1_8", "screen_2_8", "screen_3_8", "screen_4_8", "screen_5_8", "screen_6_8", "screen_7_8", "screen_8_8", "screen_9_8",
    "screen_0_9", "screen_1_9", "screen_2_9", "screen_3_9", "screen_4_9", "screen_5_9", "screen_6_9", "screen_7_9", "screen_8_9", "screen_9_9",
};

static void pixeli(int i, bool state) {
    if (i < 0 || i >= SCREEN_PIXELS) {
        return;
    }

    if (state) {
        ent_fire(screen[i], use_on, 0);
    } else {
        ent_fire(screen[i], use_off, 0);
    }
}

static int pixelxy(int x, int y, bool state) {
    const int i = (y * SCREEN_WIDTH) + x;
    pixeli(i, state);
    return i;
}

static void fill_screen(bool on) {
    for (int i = 0; i < SCREEN_PIXELS; i++) {
        if (on) {
            ent_fire(screen[i], use_on, 0);
        } else {
            ent_fire(screen[i], use_off, 0);
        }
    }
}

typedef struct{
    float x;
    float y;
} vec_t;

static vec_t vec_add(vec_t a, vec_t b) {
    return (vec_t) {
        .x = a.x + b.x,
        .y = a.y + b.y,
    };
}

static int lastPixelIndex = 0;
static vec_t pos;
static vec_t vel;
static vec_t g = {0, .01f};

void draw(void)  {
    const int newPixelIndex = pixelxy((int) pos.x, (int) pos.y, true);
    if (newPixelIndex != lastPixelIndex) {
        pixeli(lastPixelIndex, false);
        lastPixelIndex = newPixelIndex;
    }
}

const float damp = .75f;
void update(void) {
    vel = vec_add(vel, g);
    pos = vec_add(pos, vel);

    if (pos.x <= 0) {
        pos.x = 0;
        vel.x = -vel.x * damp;
    }
    if (pos.x >= SCREEN_WIDTH) {
        pos.x = SCREEN_WIDTH - 1;
        vel.x = -vel.x * damp;
    }

    if (pos.y <= 0) {
        pos.y = 0;
        vel.y = -vel.y * damp;
    }
    if (pos.y >= SCREEN_HEIGHT) {
        pos.y = SCREEN_HEIGHT - 1;
        vel.y = -vel.y * damp;
    }
}

static void reset(void) {
    vel.x = .3f + mod(time, 8.f);
    vel.y = 0;
    pos.x = mod(time, 10.f);
    pos.y = 0;
}

float on_think(float dt) {
    if (!enable) {
        return .1f;
    }

    time += dt;

    update();
    draw();

    return 1 / fps;
}

int32_t on_fire(use_type_t use_type, float value)
{
    enable = true;
    reset();

    return 1;
}
