#include <stdint.h>
#include "native.h"

static int foo = 0;

int32_t on_fire(use_type_t use_type, float value)
{
    alert(con_console, "Inside wasm::on_fire.\n");
    return foo++;
}


float on_think(float dt) {
    alert(con_console, "Inside wasm::on_think.\n");

    return 1.f;
}
