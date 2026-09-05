#ifndef READLINE_H
#define READLINE_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "res_or_err.h"
#include "array_builder.h"

enum readline_errors {
    READLINE_OK,
    READLINE_ALLOCATION_FAILED = ENOMEM,
    READLINE_END_OF_FILE,
    READLINE_INPUT_ERROR,
};

// Read stdin until EOF or newline.
// Return an allocated char* pointer on success
// or an error code on failure.
// Possible failures:
// - READLINE_ALLOCATION_FAILED
// - READLINE_INPUT_ERROR
struct result_or_error readline(void);

#endif // !READLINE_H
