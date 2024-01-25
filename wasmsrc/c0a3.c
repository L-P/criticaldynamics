#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "lib/base.h"
#include "lib/vec.h"
#include "lib/native.h"
#include "lib/ent.h"
#include "lib/format.h"

typedef struct{
    bool garyIsDead;
    bool hasTalkie;
    float playGaryAfter;
} state_t;

static state_t state = {};

static void play_radio(const char *snd) {
    const char* src = NULL;
    if (!state.garyIsDead) {
        src = "gary";
    } else if (!state.hasTalkie) { 
        src = "talkie";
    }

    play_sound(snd, src, chan_voice, 1, sound_att_norm);
}

EXPORT void on_activate() {
    console_logf(
        log_debug,
        "state: hasTalkie(%d), playGaryAfter(%2.f)\n",
        state.hasTalkie,
        state.playGaryAfter
    );
}

EXPORT int32_t on_fire(
    const entity_t* activator,
    const entity_t* caller,
    use_type_t use_type,
    float value
) {
    if (ent_matches(caller, "monster_human_grunt", "gary")) {
        state.garyIsDead = true;
        state.playGaryAfter = global_time() + 2.f;

        // Move before setting model to avoid brush model position shenanigans.
        const vec3_t offset = {0, 0, 16};
        ent_movev("talkie", vec3_add(caller->origin, offset));
        set_model_from("talkie", "talkiemdl");
        return true;
    }

    if (ent_matches(caller, "item_security", "talkie")) {
        state.hasTalkie = true;
        return true;
    }

    return false;
}

EXPORT float on_think(float time) {
    if (state.playGaryAfter > 0.f && global_time() >= state.playGaryAfter) {
        // If Gary got gibbed nothing will play, but we can consider his radio
        // to be a fine mist covering his spread remains.
        play_radio("!C0A3_GARY");
        state.playGaryAfter = -1;
    }

    return .1f;
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
