#ifndef ARRAY_BUILDER_H
#define ARRAY_BUILDER_H

#include <stddef.h>
#include <errno.h>
#include <stdlib.h>

#include "res_or_err.h"

enum array_builder_error {
    ARRAY_BUILDER_OK,
    ARRAY_BUILDER_ALLOCATION_FAILED = ENOMEM,
    ARRAY_BUILDER_EMPTY,
};

struct array_builder;

// Allocate and initialize a new array builder for elements of size `element_size` (in bytes).
struct result_or_error new_array_builder(size_t element_size);
enum array_builder_error append(void *element, struct array_builder *builder);
enum array_builder_error pop_last(struct array_builder *builder);
// Allocate and return a single contiguous array containing all elements currently in the builder.
struct result_or_error build_array(struct array_builder *builder);
void free_builder(struct array_builder *builder);

#include "array_builder_internals.h"
#endif // !ARRAY_BUILDER_H
