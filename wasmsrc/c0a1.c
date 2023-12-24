#include <stdint.h>
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
        // TODO Make follow.
        return 1;
    }

    return 0;
}
