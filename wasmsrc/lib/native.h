#include "native_engine.h"
#include "native_math.h"

/* {{{ Methods to export, all are optional:

// Called when the entity is fired (ie. ::Use() is called) if the game_wasm
// spawnflag SF_GAME_WASM_USE is set.
// Returns an arbitrary status code, 0 = success, other values are logged.
int32_t on_fire(
    const entity_t * activator,
    const entity_t * caller,
    use_type_t use_type,
    float value
);

// Called every time the entity thinks if the game_wasm spawnflag
// SF_GAME_WASM_THINK is set.
// Returns delay before next think, < 0 to error out and quit thinking.
// dt is the time delta since last think.
float on_think(float time);

// Called when the entity is activated, either right after spawn or after loading.
// Requires SF_GAME_WASM_ACTIVATE to be set.
void on_activate(void);

}}} */
