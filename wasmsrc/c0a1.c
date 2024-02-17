#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "lib/base.h"
#include "lib/vec.h"
#include "lib/native.h"
#include "lib/ent.h"
#include "lib/format.h"

static const char* card_pickup_sound = "buttons/blip2.wav";

typedef struct {
    uint8_t barney_is_out;
    uint8_t barney_is_alive;
    uint8_t keycard_obtained;
} state_t;

// Persisted state.
static state_t state = {
    .barney_is_out = false,
    .barney_is_alive = true,
    .keycard_obtained = false,
};
IMPLEMENT_SAVERESTORE(state_t, state)

static void handle_barneys_death(const entity_t* barney) {
    const vec3_t cardSpawnOffset = {0, 0, 12};

    // You monster.
    if (state.barney_is_out) {
        ent_movev("card", vec3_add(barney->origin, cardSpawnOffset));
        return;
    }

    // Barney is dead before he got out, we can't get his keycard, we're
    // softlocked.
    ent_fire("fail_asset", use_on, 0);
}

EXPORT int32_t on_fire(
    const entity_t* activator,
    const entity_t* caller,
    use_type_t use_type,
    float value
) {
    // Make Barney follow the player when his intro ends.
    if (ent_matches(caller, NULL, "barn_sentence_03")) {
        state.barney_is_out = true;
        make_follow("barney");
        return true;
    }

    // If you anger Barney before he gets out he won't go through his sequence
    // and trigger barn_sentence_03 and we can't watch for his death _and_ his
    // anger because of TriggerCondition limitations, thus this supplementary
    // trigger.
    if (ent_matches(caller, "trigger_once", "barney_got_out")) {
        state.barney_is_out = true;
        return true;
    }

    if (ent_matches(caller, "monster_barney", "barney")) {
        state.barney_is_alive = false;
        handle_barneys_death(caller);
        return true;
    }

    if (ent_matches(caller, NULL, "card_door")) {
        // Ensure barney is following and try avoiding locking yourself in.
        if (state.barney_is_alive) {
            make_follow("barney");
        }
        return true;
    }

    if (ent_matches(caller, "item_security", "card")) {
        state.keycard_obtained = true;
        play_sound(card_pickup_sound, NULL, chan_item, 1, sound_att_norm);
        return true;
    }

    return false;
}

EXPORT int32_t on_master_check(const entity_t* activator, const entity_t* caller) {
    if (ent_matches(caller, NULL, "requires_keycard")) {
        return state.keycard_obtained;
    }

    if (ent_matches(caller, NULL, "requires_barney_out")) {
        return state.barney_is_out;
    }

    if (ent_matches(caller, NULL, "requires_barney")) {
        return state.barney_is_out && state.barney_is_alive;
    }

    if (ent_matches(caller, NULL, "card_door")) {
        return state.keycard_obtained;
    }

    console_log(log_error, "Unhandled master check for caller: ");
    ent_print(log_error, caller);

    return false;
}

EXPORT void on_activate() {
    precache_sound(card_pickup_sound);

    console_logf(
        log_debug,
        "state: barney_is_alive(%d), barney_is_out(%d), keycard_obtained(%d)\n",
        state.barney_is_alive,
        state.barney_is_out,
        state.keycard_obtained
    );
}
