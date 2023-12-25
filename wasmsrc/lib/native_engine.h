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
bool make_follow(const char* target);
float global_time(void);
