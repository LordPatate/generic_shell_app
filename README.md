# Shell App library

Tiny generic extensible shell application.

## Release contents

```
.
├─ binaries
│  └─ libShellApp.a
├─ headers
│  ├─ ...
│  └─ shell_app.h
└─ README.md  (this document)
```

## Basic usage

```c
#include "shell_app.h"

void main(void) {
    struct behavior app;
    enum shell_app_signal error = init_default_app(&app);
    if (error == SHELL_APP_OK) {
        // customize your app behavior by registering custom commands
        // or modifying the default app.on_start() and app.on_exit()
        run_shell_app(&app);
        free_app_internals(&app);
    }
}
```

## Registering custom commands

Commands callbacks must have the following signature:

```c
int (*callback)(struct behavior *this, char **args, size_t argc);
```

- `struct behavior *this` is the pointer that was given to `run_shell_app`.
- `char **args` is an array containing `argc` null-terminated strings,
  representing the arguments following the command name.
- If the function returns `SHELL_APP_EXIT`, the application will exit gracefully.

Callbacks should be registered using `register_command` before `run_shell_app` is called.

## Storing additional state

Since the first parameter of commands callbacks is just a pointer expected to point to a `struct behavior`, it can be used to store any additional information when interpreted as a different type.

```c
#include "shell_app.h"

struct extended_state {
    struct behavior app;
    char test_value;
};

int my_command(struct behavior *app, char **args, size_t argc) {
    struct extended_state *this = (void*) app;
    if (argc >= 1) {
        this->test_value = *args[0];
    } else {
        printf("%c\n", this->test_value);
    }
    return SHELL_APP_OK;
}

void main(void) {
    struct extended_state state;
    enum shell_app_signal error = init_default_app(&state.app);
    if (error == SHELL_APP_OK) {
        char *details = (
            "Used with an argument, store its first character.\n"
            "Without arguments, print the stored value."
        );
        register_command(&state.app, "my_command", "set or print character value", details, my_command);
        run_shell_app(&state.app);
        free_app_internals(&state.app);
    }
}
```
