#include "array_builder.h"

static void copy(char *src, char *dst, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        dst[i] = src[i];
    }
}

struct result_or_error new_array_builder(size_t element_size) {
    struct array_builder *builder = malloc(sizeof(struct array_builder));
    if (builder) {
        *builder = (struct array_builder) {
            .count = 0,
            .element_size = element_size,
            .first = NULL,
        };
        return RESULT(builder);
    }
    return ERROR(ARRAY_BUILDER_ALLOCATION_FAILED);
}

enum array_builder_error append(void *element, struct array_builder *builder) {
    size_t elt_size = builder->element_size;
    struct element_list *new_head = malloc(sizeof(struct element_list) + elt_size - 1);
    if (new_head) {
        new_head->tail = builder->first;
        copy(element, &new_head->head_contents, elt_size);
        builder->first = new_head;
        builder->count += 1;
        return ARRAY_BUILDER_OK;
    }
    return ARRAY_BUILDER_ALLOCATION_FAILED;
}

enum array_builder_error pop_last(struct array_builder *builder) {
    if (builder->first) {
        struct element_list *old_first = builder->first;
        builder->first = old_first->tail;
        builder->count -= 1;
        free(old_first);
        return ARRAY_BUILDER_OK;
    } else {
        return ARRAY_BUILDER_EMPTY;
    }
}

struct result_or_error build_array(struct array_builder *builder) {
    size_t elt_size = builder->element_size;
    size_t total_size = builder->count * elt_size;
    char *array = malloc(total_size);
    if (array) {
        size_t i = total_size - elt_size;
        struct element_list *list;
        list = builder->first;
        while (list) {
            copy(&list->head_contents, &array[i], elt_size);
            i -= elt_size;
            list = list->tail;
        }
        return RESULT(array);
    }
    return ERROR(ARRAY_BUILDER_ALLOCATION_FAILED);
}

static void free_element_list(struct element_list *list) {
    if (list) {
        free_element_list(list->tail);
        free(list);
    }
}

void free_builder(struct array_builder *builder) {
    free_element_list(builder->first);
    free(builder);
}
