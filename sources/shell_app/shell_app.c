#include "shell_app.h"

enum shell_app_signal init_default_app(struct behavior *app) {
    app->_butler = empty_prefix_tree();
    if (!app->_butler) return SHELL_APP_ALLOCATION_FAILED;
    app->on_start = default_on_start;
    app->on_exit = default_on_exit;
    char *help_details = (
        "If called without arguments, list all possible commands and their summary.\n"
        "Otherwise, print a detailed help about the command specified as first argument."
    );
    int error_code;
    #define CHECK(REGISTER_CMD) \
        if ((error_code = REGISTER_CMD) != SHELL_APP_OK) { \
            free_ptree(app->_butler); \
            return error_code; \
        }
    CHECK(register_command(app, "help", "list available commands or help on one", help_details, default_help))
    CHECK(register_command(app, "exit", "exit shell app", "", shell_app_exit))
    #undef CHECK
    return SHELL_APP_OK;
}

enum shell_app_signal register_command(
    struct behavior *app,
    char *name,
    char *summary,
    char *help,
    int (*callback)(struct behavior *this, char **args, size_t argc)
) {
    struct shell_app_command *cmd = malloc(sizeof (struct shell_app_command));
    if (cmd == NULL) {
        return SHELL_APP_ALLOCATION_FAILED;
    }
    *cmd = (struct shell_app_command) {
        .name = name,
        .summary = summary,
        .help = help,
        .callback = callback,
    };
    enum ptree_error_code error_code = ptree_push(app->_butler, name, cmd);
    if (error_code != PTREE_OK) {
        free(cmd);
        return error_code;
    }
    return SHELL_APP_OK;
}

struct result_or_error lookup_command(struct behavior *app, char *cmd_name) {
    return ptree_search(app->_butler, cmd_name);
}

void run_shell_app(struct behavior *app) {
    int signal = SHELL_APP_OK;
    struct result_or_error res;
    struct command_line cmdline;
    struct shell_app_command *cmd;
    char *line;
    app->on_start(app);
    while (signal != SHELL_APP_EXIT) {
        fputs("> ", stderr);
        res = readline();
        if (!res.ok) {
            signal = (res.error == READLINE_END_OF_FILE) ? SHELL_APP_EXIT : res.error;
            display_error(signal);
            continue;
        }
        line = res.result;
        signal = parse_command_line(line, &cmdline);
        if (signal == ARRAY_BUILDER_OK) {
            if (cmdline.command[0]) {
                res = lookup_command(app, cmdline.command);
                if (res.ok) {
                    cmd = res.result;
                    if (cmd) {
                        signal = cmd->callback(app, cmdline.args, cmdline.argc);
                        if (signal != SHELL_APP_OK) {
                            display_error(signal);
                        }
                    } else {
                        display_error(SHELL_APP_COMMAND_NOT_FOUND);
                    }
                } else {
                    display_error(res.error);
                }
            }
            free(cmdline.args);
        } else {
            display_error(signal);
        }
        free(line);
    }
    app->on_exit(app);
}

void display_error(int error) {
    switch (error) {
    case SHELL_APP_ALLOCATION_FAILED:
        puts("Error: Allocation failed");
        break;
    case SHELL_APP_INPUT_ERROR:
        perror("Error reading input");
        break;
    case SHELL_APP_COMMAND_NOT_FOUND:
        puts("Error: Unkown command");
        break;
    case SHELL_APP_INVALID_CHARACTER:
        puts("Error: Invalid character");
        break;
    case SHELL_APP_EXIT:
        puts("Exiting...");
        break;
    case SHELL_APP_UNKOWN_ERROR:
    default:
        puts("Unkown error");
        break;
    }
}

void free_app_internals(struct behavior *app) {
    free_ptree(app->_butler);
}
