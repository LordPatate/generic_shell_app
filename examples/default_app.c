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
