// Mapped 1:1 to ALERT_TYPE in eiface.h.
typedef enum {
    con_notice    = 0,
    con_console   = 1,
    con_aiconsole = 2,
    con_warning   = 3,
    con_error     = 4,
    con_logged    = 5,
} console_output_type_t;


// Mapped 1:1 to USE_TYPE in cbase.h.
typedef enum
{
    use_off    = 0,
    use_on     = 1,
    use_set    = 2,
    use_toggle = 3,
} use_type_t;

void alert(console_output_type_t dest, char* msg);

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
