#include <stdarg.h>

#include "stb_sprintf.h"
#include "native.h"

#include "format.h"

void console_logf(log_level_t level, const char * format, ...) {
    va_list args;
    char buf [512];

    va_start(args, format);
    stbsp_vsnprintf(buf, sizeof(buf), format, args);
    va_end(args);

    console_log(level, buf);
}
