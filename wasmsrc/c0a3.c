#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "lib/base.h"
#include "lib/vec.h"
#include "lib/native.h"
#include "lib/ent.h"
#include "lib/format.h"

static const char* snd_garyyouhere = "c0a3/garyyouhere.wav";

typedef struct{
    float playGaryAfter;
} state_t;

static state_t state = {};

EXPORT int32_t on_fire(
    const entity_t* activator,
    const entity_t* caller,
    use_type_t use_type,
    float value
) {
    if (ent_matches(caller, "monster_human_grunt", "gary")) {
        state.playGaryAfter = global_time() + 2.f;
        return true;
    }

    return false;
}

EXPORT float on_think(float time) {
    if (state.playGaryAfter > 0.f && global_time() >= state.playGaryAfter) {
        // If Gary got gibbed nothing will play, but we can consider his radio
        // to be a fine mist covering his spread remains.
        play_sound(snd_garyyouhere, "gary", chan_voice, 1, sound_att_norm);
        state.playGaryAfter = -1;
    }

    return .1f;
}

EXPORT void on_activate() {
    precache_sound(snd_garyyouhere);
}

EXPORT void on_save(char* buf, size_t bufSize) {
    if (sizeof(state_t) > bufSize) {
        console_log(log_error, "sizeof(state_t) > bufSize\n");
        return;
    }

    memcpy(buf, &state, sizeof(state_t));
}

EXPORT void on_restore(const char* buf, size_t bufSize) {
    if (sizeof(state_t) > bufSize) {
        console_log(log_error, "sizeof(state_t) > bufSize\n");
        return;
    }

    memcpy(&state, buf, sizeof(state_t));
}
