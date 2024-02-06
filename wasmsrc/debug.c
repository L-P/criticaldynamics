#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "lib/base.h"
#include "lib/vec.h"
#include "lib/native.h"
#include "lib/ent.h"
#include "lib/format.h"

EXPORT void on_activate() {
    set_model_from("foodst", "foosrc");
    set_model_from("bardst", "barsrc");
    set_model_from("bazdst", "bazsrc");
}
