#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "lib/base.h"
#include "lib/vec.h"
#include "lib/native.h"
#include "lib/ent.h"
#include "lib/format.h"
#include "lib/sequence.h"

typedef enum {
    STEP_INTRO_VORTS = 0,
    STEP_INTRO_HUMANS,

    STEP_MAX,
} step_t;

typedef struct {
    sequence_t seq;
    step_t step;
} state_t;
static state_t state = {
    // .step = STEP_INTRO_HUMANS, // DEBUG
};
IMPLEMENT_SAVERESTORE(state_t, state)

static void intro_freeze(void*) {
    flags_add("!player", FL_FROZEN);
    ent_move("!player", 0, 0, -28);
}

static void intro_unfreeze(void*) {
    flags_remove("!player", FL_FROZEN);
}

static void intro_teleport(void*) {
    ent_copy_pos("!player", "intro_tpdest");
}

static sequence_event_t step_vorts_events[] = {
    // {{{ Set things in motion.
    {EVENT_CALLBACK, .1f, .callback = intro_freeze},
    {EVENT_FIRE, .0f,  .target = "intro_snd",         .use_type = use_on},
    {EVENT_FIRE, .0f,  .target = "intro_ray",         .use_type = use_on},
    {EVENT_FIRE, .1f,  .target = "intro_vort_a_anim", .use_type = use_on},
    {EVENT_FIRE, .15f, .target = "intro_vort_c_anim", .use_type = use_on},
    {EVENT_FIRE, .1f,  .target = "intro_vort_b_anim", .use_type = use_on},
    // }}}

    // {{{ Get back to sleep.
    {EVENT_FIRE, 9.f, .target = "intro_fadeout",   .use_type = use_on},
    {EVENT_FIRE, 1.f, .target = "intro_occluder",  .use_type = use_on},
    // Start moving after screen fadeout so we can have proper sound fade out,
    // not sure why ambient_generic won't work.
    {EVENT_FIRE, 0.f, .target = "intro_camera",  .use_type = use_on},
    {EVENT_CALLBACK, 4.f, .callback = intro_teleport},
    {EVENT_FIRE, 0.0f, .target = "intro_camera",  .use_type = use_off},
    // }}}

    // {{{ Cleanup before handing control over next sequence.
    {EVENT_FIRE, .0f, .target = "intro_snd", .use_type = use_off},
    {EVENT_KILL, .0f, .target = "intro_vort_a_anim"},
    {EVENT_KILL, .0f, .target = "intro_vort_c_anim"},
    {EVENT_KILL, .0f, .target = "intro_vort_b_anim"},
    {EVENT_KILL, .0f, .target = "intro_vort_a"},
    {EVENT_KILL, .0f, .target = "intro_vort_b"},
    {EVENT_KILL, .0f, .target = "intro_vort_c"},
    {EVENT_KILL, .0f, .target = "intro_vort_d"},
    {EVENT_KILL, .0f, .target = "intro_vort_e"},
    {EVENT_KILL, .0f, .target = "intro_snd"},
    {EVENT_KILL, .0f, .target = "intro_ray"},
    // }}}

    // {{{ Time to put on a little makeup.
    {EVENT_FIRE, 1.f,  .target = "intro_fadein", .use_type = use_on},
    {EVENT_KILL, .0f, .target = "intro_occluder"},
    {EVENT_CALLBACK, 1.f, .callback = intro_unfreeze},
    // }}}
};

static sequence_event_t step_humans_events[] = {
    {EVENT_PAUSE, -1.f},
};

static void init_seq_for_step(sequence_t *seq, const step_t step) {
#define init(evs) sequence_init(seq, evs, sizeof(evs) / sizeof(sequence_event_t))

    // Always cleanup before because sequence_init required a zeroed seq to work.
    memset(seq, 0, sizeof(sequence_t));

    switch (step) {
        case STEP_INTRO_VORTS:
            init(step_vorts_events);
            break;
        case STEP_INTRO_HUMANS:
            init(step_humans_events);
            break;
        case STEP_MAX:
        default:
            break;
    }
#undef init
}

EXPORT void on_activate() {
    console_logf(log_debug, "state:\n");
    init_seq_for_step(&state.seq, state.step);
}

EXPORT int32_t on_fire(
    const entity_t* activator,
    const entity_t* caller,
    use_type_t use_type,
    float value
) {
    return false;
}

EXPORT float on_think(float time) {
    if (state.step < STEP_MAX) {
        if (!sequence_think(&state.seq, global_time())) {
            state.step++;
            init_seq_for_step(&state.seq, state.step);
        }
        return .01f;
    }

    return .1f;
}
