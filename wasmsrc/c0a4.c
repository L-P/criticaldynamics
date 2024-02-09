#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "lib/base.h"
#include "lib/vec.h"
#include "lib/native.h"
#include "lib/ent.h"
#include "lib/format.h"

typedef struct{
    bool generatorIsRunning;
    bool lightIsOn;
} state_t;

static state_t state = {};

EXPORT void on_activate() {
    console_logf(
        log_debug,
        "state: generatorIsRunning(%d), lightIsOn(%d)\n",
        state.generatorIsRunning,
        state.lightIsOn
    );
}

EXPORT int32_t on_fire(
    const entity_t* activator,
    const entity_t* caller,
    use_type_t use_type,
    float value
) {
    if (ent_matches(caller, NULL, "generator_is_running")) {
        if (!state.generatorIsRunning) {
            // Small red light above the breaker.
            ent_fire("biglight_ready", use_on, 0);
        }

        state.generatorIsRunning = true;
        return true;
    }

    if (ent_matches(caller, "func_button", "biglight_lever_button")) {
        ent_fire("biglight_lever", use_toggle, 0);

        if (state.generatorIsRunning && !state.lightIsOn) {
            ent_fire("biglight", use_on, 0);
            ent_fire("biglight_ready", use_off, 0);
            ent_fire("biglight_clang", use_toggle, 0);
            state.lightIsOn = true;
        }

        return true;
    }

    return false;
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
