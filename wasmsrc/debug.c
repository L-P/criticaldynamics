#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "lib/base.h"
#include "lib/vec.h"
#include "lib/native.h"
#include "lib/ent.h"

EXPORT int32_t on_fire(
    const entity_t* activator,
    const entity_t* caller,
    use_type_t use_type,
    float value
) {
    console_log(log_debug, "activator:\n");
    ent_print(log_debug, activator);
    console_log(log_debug, "caller:\n");
    ent_print(log_debug, caller);

    if (ent_matches(activator, "monster_barney", "barney")) {
        console_log(log_debug, "Barney is activator.");
        return true;
    }

    if (ent_matches(caller, "monster_barney", "barney")) {
        console_log(log_debug, "Barney is caller.");
        return true;
    }

    return false;
}
