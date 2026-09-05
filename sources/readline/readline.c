#include "readline.h"

struct result_or_error readline(void) {
    struct result_or_error res;
    res = new_array_builder(sizeof(char));
    if (!res.ok) return res;
    struct array_builder *string_builder = res.result;
    int c; // note: int, not char, necessary for EOF (-1)
    enum array_builder_error status = ARRAY_BUILDER_OK;
    while ((c = fgetc(stdin)) != EOF && c != '\n' && status == ARRAY_BUILDER_OK) {
        if (c == '\b') {
            pop_last(string_builder);
        } else {
            status = append(&c, string_builder);
        }
    }
    if (status != ARRAY_BUILDER_OK) {
        res = ERROR(status);
    } else {
        if (ferror(stdin)) {
            res = ERROR(READLINE_INPUT_ERROR);
        } else {
            if (c == EOF) {
                res = ERROR(READLINE_END_OF_FILE);
            } else {
                c = 0;
                status = append(&c, string_builder);
                if (status == ARRAY_BUILDER_OK)
                    res = build_array(string_builder);
                else
                    res = ERROR(status);
            }
        }
    }
    free_builder(string_builder);
    return res;
}
