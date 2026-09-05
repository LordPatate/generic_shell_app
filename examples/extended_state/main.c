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
