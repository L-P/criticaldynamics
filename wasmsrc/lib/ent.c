#include <stddef.h>
#include <stdbool.h>

#include "stb_sprintf.h"
#include "base.h"
#include "vec.h"
#include "strings.h"
#include "native.h"

#include "ent.h"

bool ent_matches(
    const entity_t* ent,
    const char* class,
    const char* name
) {
    if (ent == NULL) {
        return false;
    }

    if (class != NULL) {
        if (ent->class == NULL || strcmp(ent->class, class) != 0) {
            return false;
        }
    }

    if (name != NULL) {
        if (ent->name == NULL || strcmp(ent->name, name) != 0) {
            return false;
        }
    }

    return true;
}

void ent_print(log_level_t level, const entity_t* ent) {
    if (ent == NULL) {
        console_log(level, "NULL\n");
        return;
    }

    char buf[64];
    stbsp_snprintf(buf, sizeof(buf), "class: %s\n", ent->class);
    console_log(level, buf);
    stbsp_snprintf(buf, sizeof(buf), "name: %s\n", ent->name);
    console_log(level, buf);
    stbsp_snprintf(
        buf, sizeof(buf),
        "origin: %f %f %f\n",
        ent->origin.x,
        ent->origin.y,
        ent->origin.z
    );
    console_log(level, buf);
}
