#include "shell_app.h"

static void _print_cmd_summary(struct shell_app_command *cmd) {
    printf("- %s: %s\n", cmd->name, cmd->summary);
}

static void _print_cmd_detail(struct shell_app_command *cmd) {
    printf(
        "- %s: %s\n"
        "%s\n",
        cmd->name, cmd->summary, cmd->help
    );
}

static void _rec_help(struct node *tree) {
    if (tree != NULL) {
        if (tree->data != NULL)
            _print_cmd_summary(tree->data);
        for (int i = 0; i < PTREE_NB_CHILDREN; ++i)
            _rec_help(tree->children[i]);
    }
}

int default_help(struct behavior *this, char **args, size_t argc) {
    if (argc > 0) {
        for (size_t i = 0; i < argc; ++i) {
            struct result_or_error lookup_res = lookup_command(this, args[i]);
            if (lookup_res.ok) {
                if (lookup_res.result) {
                    _print_cmd_detail(lookup_res.result);
                } else {
                    display_error(SHELL_APP_COMMAND_NOT_FOUND);
                }
            } else {
                display_error(lookup_res.error);
            }
        }
    } else {
        puts("Available commands:");
        _rec_help(this->_butler->root);
    }
    return SHELL_APP_OK;
}

int shell_app_exit(struct behavior *_this, char **_args, size_t _argc) {
    return SHELL_APP_EXIT;
}

void default_on_start(struct behavior *_this) {}

void default_on_exit(struct behavior *_this) {}
