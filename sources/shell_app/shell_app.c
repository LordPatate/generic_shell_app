#include "shell_app.h"

#include "defaults.c"

enum shell_app_error_code init_default_app(struct behavior *app) {
    app->on_start = default_on_start;
    app->on_exit = default_on_exit;
    char *help_details = (
        "If called without arguments, list all possible commands and their summary.\n"
        "Otherwise, print a detailed help about the command specified as first argument."
    );
    #define CHECK(REGISTER_CMD) \
        if (REGISTER_CMD != SHELL_APP_OK) { \
            ptree_free(&app->_butler); \
            return SHELL_APP_REGISTER_FAILED; \
        }
    CHECK(register_command(app, "help", "list available commands or help on one", help_details, default_help))
    CHECK(register_command(app, "exit", "exit shell app", "", shell_app_exit))
    #undef CHECK
    return SHELL_APP_OK;
}

enum shell_app_error_code register_command(
    struct behavior *app,
    char *name,
    char *summary,
    char *help,
    enum app_signal (*callback)(struct behavior *this, char *args)
) {
    struct command *cmd = malloc(sizeof (struct command));
    if (cmd == NULL) {
        return SHELL_APP_REGISTER_FAILED;
    }
    cmd->name = name;
    cmd->summary = summary;
    cmd->help = help;
    cmd->callback = callback;
    if (ptree_push(&app->_butler, name, cmd) != PTREE_OK) {
        free(cmd);
        return SHELL_APP_REGISTER_FAILED;
    }
    return SHELL_APP_OK;
}

struct result_or_error lookup_command(struct behavior *app, char *cmd_name) {
    struct result_or_error search_res = ptree_search(&app->_butler, cmd_name);
    if (search_res.ok) {
        struct command *cmd = search_res.result;
        if (cmd != NULL)
            return RESULT(cmd);
        else
            return ERROR(SHELL_APP_COMMAND_NOT_FOUND);
    } else {
        switch (search_res.error) {
        case PTREE_INVALID_CHAR:
            return ERROR(SHELL_APP_INVALID_CHARACTER);
        case PTREE_KEY_NOT_FOUND:
            return ERROR(SHELL_APP_COMMAND_NOT_FOUND);
        default:
            return ERROR(SHELL_APP_UNKOWN_ERROR);
        }
    }
}

void display_lookup_error(enum shell_app_error_code error, char *cmd) {
    switch (error) {
    case SHELL_APP_COMMAND_NOT_FOUND:
        printf("Unkown command: %s\n", cmd);
        break;
    case SHELL_APP_INVALID_CHARACTER:
        printf("Error: Invalid character in %s\n", cmd);
        break;
    default:
        printf("Unkown error while looking up command %s\n", cmd);
        break;
    }
}

enum shell_app_error_code _readline(char *line_buf, struct _command_line *cmdline) {
    cmdline->command = line_buf;
    int i = 0;
    int c = fgetc(stdin);; // note: int, not char, required to handle EOF
    while (i < LINE_BUFFER_SIZE - 2 && c != EOF && c != ' ' && c != '\n') {
        line_buf[i++] = c;
        c = fgetc(stdin);
    }
    line_buf[i++] = 0;
    cmdline->args = line_buf + i;
    if (ferror(stdin))
        return SHELL_APP_INPUT_ERROR;
    while (c == ' ') {
        c = fgetc(stdin);
    }
    if (ferror(stdin))
        return SHELL_APP_INPUT_ERROR;
    while (i < LINE_BUFFER_SIZE - 1 && c != EOF && c != '\n') {
        line_buf[i++] = c;
        c = fgetc(stdin);
    }
    line_buf[i++] = 0;
    if (ferror(stdin))
        return SHELL_APP_INPUT_ERROR;
    return SHELL_APP_OK;
}

void run_shell_app(struct behavior *app) {
    enum app_signal signal = SHELL_APP_RUNNING;
    char line[LINE_BUFFER_SIZE];
    struct _command_line cmdline = {"", ""};
    app->on_start(app);
    while (signal != SHELL_APP_EXIT) {
        fputs("> ", stderr);
        enum shell_app_error_code read_err = _readline(line, &cmdline);
        if (read_err != SHELL_APP_OK) {
            switch (read_err) {
            case SHELL_APP_INPUT_ERROR:
                printf("Warning: error while reading: %s\n", strerror(errno));
                break;
            default:
                puts("Unkown error while reading.");
                continue;
            }
        }
        struct result_or_error lookup_res = lookup_command(app, cmdline.command);
        if (lookup_res.ok) {
            struct command *cmd = lookup_res.result;
            signal = cmd->callback(app, cmdline.args);
        } else {
            display_lookup_error(lookup_res.error, cmdline.command);
        }
    }
    app->on_exit(app);
}

void free_app(struct behavior *app) {
    ptree_free(&app->_butler);
}
