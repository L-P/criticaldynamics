#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "lib/base.h"
#include "lib/vec.h"
#include "lib/ent.h"
#include "lib/native.h"

EXPORT int32_t on_fire(
    const entity_t* activator,
    const entity_t* caller,
    use_type_t use_type,
    float value
) {
    if (ent_matches(caller, NULL, "_wasm_make_follow") == 0) {
        make_follow("barney");
        console_log(log_debug, "Made Barney follow us.");
        return true;
    }

    return false;
}
