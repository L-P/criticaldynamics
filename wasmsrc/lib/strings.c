#include <stddef.h>

size_t strlen(const char *str) {
    if (str == NULL) {
        return 0;
    }

    for (size_t i = 0; ; i++) {
        if (str[i] == '\0') {
            return i;
        }
    }
}

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
}
