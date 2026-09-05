#ifndef CMDLINE_PARSER_H
#define CMDLINE_PARSER_H

#include <stdlib.h>

#include "res_or_err.h"
#include "array_builder.h"

struct command_line {
    char *command;
    char **args;
    size_t argc;
};

#define WHITESPACE(c) ((c) == ' ' || (c) == '\t')

enum array_builder_error parse_command_line(char *line, struct command_line *result);

#endif // !CMDLINE_PARSER_H
