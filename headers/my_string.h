#ifndef _MY_STRING
#define _MY_STRING

#include <stddef.h>

// Copy up to `size - 1` characters of the null-terminated string
// pointed by `from` to the buffer pointed by `to`.
// If `from` is too long, truncate the copy.
// Append a nul character at the end of the copy.
// Return the number of characters copied.
size_t str_copy_sized(char *from, char *to, size_t size);

#endif  // _MY_STRING
