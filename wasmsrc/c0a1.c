#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "lib/base.h"
#include "lib/native.h"
#include "lib/strings.h"

EXPORT int32_t on_fire(
    const char* activatorClass,
    const char* activatorName,
    const char* callerClass,
    const char* callerName,
    use_type_t use_type,
    float value
) {
    if (callerName && strcmp(callerName, "barn_sentence_03") == 0) {
        make_follow("barney");
        console_log(log_debug, "Made Barney follow us.");
        return true;
    }

    return false;
}
