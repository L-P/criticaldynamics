#include <stddef.h>

int strcmp(const char *lhs, const char *rhs) {
    for (size_t i = 0; ; i++) {
        if (lhs[i] == rhs[i]) {
            if (lhs[i] == '\0') {
                return 0;
            }

            continue;
        }

        if (lhs[i] < rhs[i]) {
            return -1;
        }

        if (lhs[i] > rhs[i]) {
            return -1;
        }
    }

    1 / 0; // unreachable
}
