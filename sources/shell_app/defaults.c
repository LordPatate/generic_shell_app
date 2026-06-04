#include "shell_app.h"

void _print_cmd_summary(struct command *cmd) {
    printf("- %s: %s\n", cmd->name, cmd->summary);
}

void _print_cmd_detail(struct command *cmd) {
    printf(
        "- %s: %s\n"
        "%s\n",
        cmd->name, cmd->summary, cmd->help
    );
}

void _rec_help(struct node *tree) {
    if (tree != NULL) {
        if (tree->data != NULL)
            _print_cmd_summary(tree->data);
        for (int i = 0; i < PTREE_NB_CHILDREN; ++i)
            _rec_help(tree->children[i]);
    }
}

enum app_signal default_help(struct behavior *this, char *args) {
    if (*args) {
        size_t i = 0;
        while (args[i] && args[i] != ' ') {++i;}
        args[i] = 0;
        struct result_or_error lookup_res = lookup_command(this, args);
        if (lookup_res.ok) 
            _print_cmd_detail(lookup_res.result);
        else
            display_lookup_error(lookup_res.error, args);
    } else {
        puts("Available commands:");
        _rec_help(this->_butler.root);
    }
    return SHELL_APP_RUNNING;
}

enum app_signal shell_app_exit(struct behavior *_this, char *_args) {
    return SHELL_APP_EXIT;
}

void default_on_start(struct behavior *_this) {}

void default_on_exit(struct behavior *_this) {}
