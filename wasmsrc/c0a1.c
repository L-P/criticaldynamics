#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "lib/base.h"
#include "lib/vec.h"
#include "lib/native.h"
#include "lib/ent.h"
#include "lib/strings.h"

static const char* card_pickup_sound = "buttons/blip2.wav";

typedef enum {
    stage_init,
    stage_barney_is_out,
    stage_door_is_open,
} stage_t;
static stage_t stage = stage_init;

EXPORT void on_activate() {
    precache_sound(card_pickup_sound);
}

static void handle_barneys_death(const entity_t* barney) {
    const vec3_t cardSpawnOffset = {0, 0, 12};

    switch (stage) {
    // Barney is dead before he got out, we can't get his keycard, we're
    // softlocked.
    case stage_init:
        ent_fire("fail_asset", use_on, 0);
        return;

    // You monster.
    default:
        ent_movev("card", vec3_add(barney->origin, cardSpawnOffset));
        return;
    }
}

EXPORT int32_t on_fire(
    const entity_t* activator,
    const entity_t* caller,
    use_type_t use_type,
    float value
) {
    // Make Barney follow the player when his intro ends.
    if (ent_matches(caller, NULL, "barn_sentence_03")) {
        make_follow("barney");
        stage = stage_barney_is_out;
        return true;
    }

    if (ent_matches(caller, "monster_barney", "barney")) {
        handle_barneys_death(caller);
        return true;
    }

    // Barney opened the door, it'll stay open.
    if (ent_matches(caller, NULL, "card_door")) {
        stage = stage_door_is_open;
        return true;
    }

    if (ent_matches(caller, "item_security", "card")) {
        play_sound(card_pickup_sound, NULL, chan_item, 1, sound_att_norm);
        return true;
    }

    return false;
}
