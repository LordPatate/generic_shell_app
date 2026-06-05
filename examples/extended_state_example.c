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
