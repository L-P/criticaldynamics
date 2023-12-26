#include <stddef.h>
#include <stdbool.h>

#include "stb_sprintf.h"
#include "base.h"
#include "vec.h"
#include "strings.h"
#include "native.h"
#include "format.h"

#include "ent.h"

bool ent_matches(
    const entity_t* ent,
    const char* class,
    const char* name
) {
    if (ent == NULL) {
        return false;
    }

    if (strlen(class) > 0) {
        if (ent->class == NULL || strcmp(ent->class, class) != 0) {
            return false;
        }
    }

    if (strlen(name) > 0) {
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

    if (ent->class != NULL) {
        console_logf(level, "class: '%s'\n", ent->class);
    } else {
        console_logf(level, "class: NULL\n");
    }

    if (ent->name != NULL) {
        console_logf(level, "name: '%s'\n", ent->name);
    } else {
        console_logf(level, "name: NULL\n");
    }

    console_logf(
        level,
        "origin: %f %f %f\n",
        ent->origin.x,
        ent->origin.y,
        ent->origin.z
    );
}

void ent_movev(const char* target, vec3_t pos) {
    ent_move(target, pos.x, pos.y, pos.z);
}
