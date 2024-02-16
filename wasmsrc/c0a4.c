#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "lib/base.h"
#include "lib/vec.h"
#include "lib/native.h"
#include "lib/ent.h"
#include "lib/format.h"

static const int numGenerators = 2;
static const float generatorStartDelay = 1.f;

typedef enum {
    WATER_NONE,
    WATER_RAISING,
    WATER_FILLED,
    WATER_PULL,
} waterstate_t;

typedef struct{
    // 1. Water pressure for turbine.
    bool pressureIsUp;

    // 2. Two generators to get running.
    uint32_t generatorRunningCount;
    float generatorRunningAfter;
    bool generatorIsRunning;

    // 3. Light breaker.
    bool lightIsOn;

    // 4. Water raises.
    waterstate_t waterState;
} state_t;

static state_t state = {};

EXPORT void on_activate() {
    console_logf(log_debug, "state:\n");
    console_logf(log_debug, "  generatorRunningCount: %d\n", state.generatorRunningCount);
    console_logf(log_debug, "  generatorRunningAfter: %f\n", state.generatorRunningAfter);
    console_logf(log_debug, "  generatorIsRunning: %d\n", state.generatorIsRunning);
    console_logf(log_debug, "  pressureIsUp: %d\n", state.pressureIsUp);
    console_logf(log_debug, "  lightIsOn: %d\n", state.lightIsOn);
    console_logf(log_debug, "  waterState: %d\n", state.waterState);
}

static int32_t handle_generator_setup(
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
        if (state.generatorIsRunning) {
            return true;
        }

        if (state.pressureIsUp) {
            state.generatorRunningCount += 1;
            if (state.generatorRunningCount >= numGenerators) {
                state.generatorRunningAfter = global_time() + generatorStartDelay;
            }

            if (ent_matches(caller, NULL, "generator_button_a")) {
                ent_fire("generator_sound_a", use_on, 0);
                ent_fire("pressure_dials_gen_a", use_on, 0);
            } else if (ent_matches(caller, NULL, "generator_button_b")) {
                ent_fire("generator_sound_b", use_on, 0);
                ent_fire("pressure_dials_gen_b", use_on, 0);
            }
        }

        return true;
    }

    if (ent_matches(caller, "func_button", "biglight_lever_button")) {
        ent_fire("biglight_lever", use_toggle, 0);

        if (state.generatorIsRunning && !state.lightIsOn) {
            ent_fire("biglight", use_on, 0);
            ent_fire("biglight_ready", use_off, 0);
            ent_fire("biglight_clang", use_toggle, 0);
            ent_fire("controller_spawn", use_toggle, 0);
            state.lightIsOn = true;
        }

        return true;
    }

    return false;
}


static int32_t handle_water_escape(
    const entity_t* activator,
    const entity_t* caller,
    use_type_t use_type,
    float value
) {
    if (ent_matches(caller, "func_button", "water_start_raise")) {
        if (!state.lightIsOn || state.waterState != WATER_NONE) {
            return true;
        }

        ent_fire("waa_mm", use_on, 0);
        ent_fire("closabledoor", use_off, 0);
        state.waterState = WATER_RAISING;
        return true;
    }

    if (ent_matches(caller, "func_water", "waa")) {
        if (state.waterState != WATER_RAISING) {
            console_log(log_error, "unreachable, waa should not be able to fire\n");
        }

        state.waterState = WATER_FILLED;
        ent_fire("waa_getmeout_mm", use_on, 0);
        return true;
    }

    if (ent_matches(caller, "multi_manager", "waa_getmeout_mm")) {
        state.waterState = WATER_PULL;
        return true;
    }

    return false;
}

EXPORT int32_t on_fire(
    const entity_t* activator,
    const entity_t* caller,
    use_type_t use_type,
    float value
) {
    if (handle_generator_setup(activator, caller, use_type, value)) {
        return true;
    }

    if (handle_water_escape(activator, caller, use_type, value)) {
        return true;
    }

    return false;
}

EXPORT int32_t on_master_check(const entity_t* activator, const entity_t* caller) {
    if (ent_matches(caller, "trigger_once", "falldamage")) {
		return state.waterState == WATER_NONE;
	}

    if (   ent_matches(caller, NULL, "generator_button_a")
        || ent_matches(caller, NULL, "generator_button_b")
    ) {
        return state.pressureIsUp;
    }

    if (ent_matches(caller, "func_door", NULL)) {
        return state.generatorIsRunning && state.lightIsOn && state.waterState == WATER_NONE;
    }

    if (ent_matches(caller, "trigger_once", "startescape")) {
        return state.waterState == WATER_PULL;
    }

    console_log(log_error, "Unhandled master check for caller: ");
    ent_print(log_error, caller);

    return false;
}

EXPORT float on_think(float time) {
    if (state.generatorRunningAfter > 0.f && global_time() >= state.generatorRunningAfter) {
        // Small red light above the breaker.
        ent_fire("biglight_ready", use_on, 0);
        ent_fire("generator_up_lights", use_on, 0);

        state.generatorIsRunning = true;
        state.generatorRunningAfter = 0.f;
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
