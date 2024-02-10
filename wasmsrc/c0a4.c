#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "lib/base.h"
#include "lib/vec.h"
#include "lib/native.h"
#include "lib/ent.h"
#include "lib/format.h"

typedef struct{
    uint32_t generatorRunningCount;
    bool pressureIsUp;
    bool lightIsOn;
} state_t;

static state_t state = {};

EXPORT void on_activate() {
    console_logf(
        log_debug,
        "state: pressureIsUp(%d), generatorRunningCount(%d), lightIsOn(%d)\n",
        state.pressureIsUp,
        state.generatorRunningCount,
        state.lightIsOn
    );
}

bool generator_is_running() {
    return state.generatorRunningCount >= 2;
}

EXPORT int32_t on_fire(
    const entity_t* activator,
    const entity_t* caller,
    use_type_t use_type,
    float value
) {
    if (ent_matches(caller, NULL, "pressure_is_up")) {
        if (!state.pressureIsUp) {
            ent_fire("pressure_dials", use_on, 0);
            state.pressureIsUp = true;
            return true;
        }
    }

    if (   ent_matches(caller, NULL, "generator_button_a")
        || ent_matches(caller, NULL, "generator_button_b")
    ) {
        if (state.pressureIsUp) {
            state.generatorRunningCount += 1;

            if (ent_matches(caller, NULL, "generator_button_a")) {
                ent_fire("generator_sound_a", use_on, 0);
                ent_fire("pressure_dials_gen_a", use_on, 0);
            } else if (ent_matches(caller, NULL, "generator_button_b")) {
                ent_fire("generator_sound_b", use_on, 0);
                ent_fire("pressure_dials_gen_b", use_on, 0);
            }
        }

        if (generator_is_running()) {
            // Small red light above the breaker.
            ent_fire("biglight_ready", use_on, 0);
            ent_fire("generator_up_lights", use_on, 0);
        }
        return true;
    }

    if (ent_matches(caller, "func_button", "biglight_lever_button")) {
        ent_fire("biglight_lever", use_toggle, 0);

        if (generator_is_running() && !state.lightIsOn) {
            ent_fire("biglight", use_on, 0);
            ent_fire("biglight_ready", use_off, 0);
            ent_fire("biglight_clang", use_toggle, 0);
            state.lightIsOn = true;
        }

        return true;
    }

    return false;
}

EXPORT int32_t on_master_check(const entity_t* activator, const entity_t* caller) {
    if (   ent_matches(caller, NULL, "generator_button_a")
        || ent_matches(caller, NULL, "generator_button_b")
    ) {
        return state.pressureIsUp;
    }

    console_log(log_error, "Unhandled master check for caller: ");
    ent_print(log_error, caller);

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
