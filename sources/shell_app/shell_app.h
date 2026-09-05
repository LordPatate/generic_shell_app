#ifndef _SHELL_APP
#define _SHELL_APP

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "res_or_err.h"
#include "prefix_trees.h"
#include "readline.h"
#include "cmdline_parser.h"

enum shell_app_signal {
    SHELL_APP_OK,
    SHELL_APP_ALLOCATION_FAILED = ENOMEM,
    SHELL_APP_INVALID_CHARACTER = PTREE_INVALID_CHAR,
    SHELL_APP_COMMAND_NOT_FOUND = PTREE_KEY_NOT_FOUND,
    SHELL_APP_UNKOWN_ERROR,
    SHELL_APP_INPUT_ERROR,
    SHELL_APP_EXIT
};

enum sizes {
    LINE_BUFFER_SIZE = 128
};

struct behavior {
    // --- Public members ---

    // Called after start.
    void (*on_start)(struct behavior *this);
    // Called before exit.
    void (*on_exit)(struct behavior *this);

    // --- Internal members ---

    struct prefix_tree *_butler;
};

struct shell_app_command {
    char *name;
    char *summary;
    char *help;
    int (*callback)(struct behavior *this, char **args, size_t argc);
};

// Provide default on_start() and on_exit() that do nothing.
// Register predefined commands "help" and "exit".
// Return an error code in case of failure, SHELL_APP_OK otherwise.
// Delete all registered commands on failure.
// Possible errors:
// - SHELL_APP_ALLOCATION_FAILED
// - SHELL_APP_UNKOWN_ERROR
enum shell_app_signal init_default_app(struct behavior *empty_app);

// Register a function to call when a given command is typed.
// Return an error code in case of failure, SHELL_APP_OK otherwise.
// Possible errors:
// - SHELL_APP_ALLOCATION_FAILED
// - SHELL_APP_INVALID_CHAR if the command name contains non-ASCII characters
// - SHELL_APP_UNKOWN_ERROR
enum shell_app_signal register_command(
    struct behavior *app,
    char *name,     // The command itself.
    char *summary,  // A single-line description.
    char *help,     // A detailed explanation of its usage.
    int (*callback)(struct behavior *this, char **args, size_t argc)
);

// Search for the cmd_name in the registered commands.
// Return a struct shell_app_command* on success, an error otherwise.
// Possible errors:
// - SHELL_APP_COMMAND_NOT_FOUND
// - SHELL_APP_INVALID_CHARACTER if the cmd_name contains non-ASCII characters
// - SHELL_APP_UNKOWN_ERROR
struct result_or_error lookup_command(struct behavior *app, char *cmd_name);

// Print a suitable message on stdin for the error code.
void display_error(int error);

// Runs the app.
void run_shell_app(struct behavior *app);

// Free all resources associated with the behavior object
// (except the `app` pointer itself).
void free_app_internals(struct behavior *app);

#include "shell_app_defaults.h"

#endif  // _SHELL_APP
