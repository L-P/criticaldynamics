// Mapped 1:1 to ALERT_TYPE in eiface.h.
typedef enum {
    log_debug = 2,
    log_info  = 1,
    log_warn  = 3,
    log_error = 4,
} log_level_t;

// Mapped 1:1 to USE_TYPE in cbase.h.
typedef enum
{
    use_off    = 0,
    use_on     = 1,
    use_set    = 2,
    use_toggle = 3,
} use_type_t;

void console_log(log_level_t log_level, const char* msg);
void ent_fire(const char* target, use_type_t use_type, float value);

float sin(float);
float cos(float);
float mod(float, float);

/*
 * Methods to export, all are optional:
 *
 * // Called when the entity is fired, ie. when ::Use() is called on the entity.
 * // Returns an arbitrary status code, 0 = success, other values are logged.
 * int32_t on_fire(use_type_t use_type, float value);
 *
 * // Called every time the entity thinks.
 * // Returns delay before next think, < 0 to error out and quit thinking.
 * // dt is the time delta since last think.
 * float on_think(float dt);
 */
