#include "my_string.h"

size_t str_copy_sized(char *from, char *to, size_t size) {
    size_t i = 0;
    while (i < size - 1 && from[i]) {
        to[i] = from[i];
        ++i;
    }
    to[i] = 0;
    return i;
}
