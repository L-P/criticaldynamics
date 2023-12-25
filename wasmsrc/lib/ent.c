#include <stddef.h>

#include "base.h"
#include "vec.h"
#include "strings.h"

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
