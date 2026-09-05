#include "cmdline_parser.h"

static char *parse_word(char *line, size_t *offset) {
    while (WHITESPACE(line[*offset])) {
        *offset += 1;
    }
    char *word_start = &line[*offset];
    while (line[*offset] && !WHITESPACE(line[*offset])) {
        *offset += 1;
    }
    if (line[*offset]) {
        line[*offset] = 0;
        *offset += 1;
    }
    return word_start;
}

enum array_builder_error parse_command_line(char *line, struct command_line *result) {
    struct result_or_error res;
    enum array_builder_error error = ARRAY_BUILDER_OK;
    size_t i = 0;
    result->command = parse_word(line, &i);
    res = new_array_builder(sizeof(char *));
    if (!res.ok) {
        error = res.error;
    } else {
        struct array_builder *args_builder = res.result;
        char *arg = "";
        while (line[i] && error == ARRAY_BUILDER_OK) {
            arg = parse_word(line, &i);
            if (*arg)
                error = append(&arg, args_builder);
        }
        if (error == ARRAY_BUILDER_OK) {
            res = build_array(args_builder);
            if (!res.ok) {
                error = res.error;
            } else {
                result->args = res.result;
                result->argc = args_builder->count;
            }
        }
        free_builder(args_builder);
    }
    return error;
}
