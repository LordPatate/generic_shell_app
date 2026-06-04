#ifndef _SHELL_APP
#define _SHELL_APP

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "my_string.h"
#include "prefix_trees.h"
#include "res_or_err.h"

enum shell_app_error_code {
    SHELL_APP_OK,
    SHELL_APP_UNKOWN_ERROR,
    SHELL_APP_REGISTER_FAILED,
    SHELL_APP_INPUT_ERROR,
    SHELL_APP_COMMAND_NOT_FOUND,
    SHELL_APP_INVALID_CHARACTER,
};

enum app_signal {
    SHELL_APP_RUNNING,
    SHELL_APP_EXIT
};

enum sizes {
    PROMPT_BUFFER_SIZE = 32,
    LINE_BUFFER_SIZE = 128
};

struct behavior {
    // --- Public members ---

    // Called after start.
    // The returned pointer will be forwarded as argument to all
    // callbacks expecting a state.
    void (*on_start)(struct behavior *this);
    // Called before exit.
    // Best time to free the state pointer.
    void (*on_exit)(struct behavior *this);

    // --- Internal members ---

    struct prefix_tree _butler;
};

struct command {
    char *name;
    char *summary;
    char *help;
    enum app_signal (*callback)(struct behavior *this, char *args);
};

// Initialize an app with predefined commands "help" and "exit".
// Return an error code in case of failure, 0 otherwise.
enum shell_app_error_code init_default_app(struct behavior *empty_app);

// Register a function to call when a given command is typed.
// Return an error code in case of failure, 0 otherwise.
enum shell_app_error_code register_command(
    struct behavior *app,
    char *name,
    char *summary,
    char *help,
    enum app_signal (*callback)(struct behavior *this, char *args)
);

// Search for the cmd_name in the registered commands.
// Return a struct command* on success or COMMAND_NOT_FOUND otherwise.
struct result_or_error lookup_command(struct behavior *app, char *cmd_name);

// Print a suitable message on stdin for the error code and the command looked up.
void display_lookup_error(enum shell_app_error_code error, char *cmd);

// Runs the app.
void run_shell_app(struct behavior *app);

// Free all resources associated with the behavior object
// (but not itself).
void free_app(struct behavior *app);

// --------------
// Internal stuff
// --------------

struct _command_line {
    char *command;
    char *args;
};

#endif  // _SHELL_APP
