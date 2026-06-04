#include "shell_app.h"

void main(void) {
    struct behavior app;
    enum shell_app_error_code error = init_default_app(&app);
    if (error == SHELL_APP_OK) {
        run_shell_app(&app);
        free_app(&app);
    }
}
