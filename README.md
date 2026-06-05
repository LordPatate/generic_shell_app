# Shell App library

Tiny generic extensible shell application.

## Release contents

```
.
├─ binaries
│  └─ libshell_app.a
├─ headers
│  ├─ prefix_trees.h
│  ├─ res_or_err.h
│  └─ shell_app.h
└─ README.md  (this document)
```

## Basic usage

```c
#include "shell_app.h"

void main(void) {
    struct behavior app;
    enum shell_app_error_code error = init_default_app(&app);
    if (error == SHELL_APP_OK) {
        // customize your app behavior by registering custom commands
        // or modifying the default app.on_start() and app.on_exit()
        run_shell_app(&app);
        free_app(&app);
    }
}
```

## Registering custom commands

Commands callbacks must have the following signature:

```c
enum app_signal callback(struct behavior *this, char *args);
```

- `struct behavior *this` is the pointer that was given to `run_shell_app`.
- `char *args` is a null-terminated string containing the whole command line without the command name (the first word). All space characters (' ') immediately following the command name are skipped.
- If the function returns `SHELL_APP_EXIT`, the application will exit gracefully.

Callbacks should be registered using `register_command` before `run_shell_app` is called.

## Storing additional state

Since the first parameter of commands callbacks is just a pointer expected to point to a `struct behavior`, it can be used to store any additional information when interpreted as a different type.

```c
#include "shell_app.h"

struct extended_state {
    struct behavior app;
    int test_value;
};

enum app_signal my_command(struct behavior *app, char *args) {
    struct extended_state *this = (void*) app;
    if (*args) {
        this->test_value = *args;
    } else {
        printf("%d\n", this->test_value);
    }
    return SHELL_APP_RUNNING;
}

void main(void) {
    struct extended_state state;
    enum shell_app_error_code error = init_default_app(&state.app);
    if (error == SHELL_APP_OK) {
        char *details = (
            "Used with an argument, store its first character as an int.\n"
            "Without arguments, print the stored value."
        );
        register_command(&state.app, "my_command", "set or print character value", details, my_command);
        run_shell_app(&state.app);
        free_app(&state.app);
    }
}
```
